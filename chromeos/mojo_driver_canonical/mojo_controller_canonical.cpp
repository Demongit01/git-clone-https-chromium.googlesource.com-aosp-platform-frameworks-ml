// Copyright 2022 The ChromiumOS Authors.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#include "mojo_controller_canonical.h"

#include <base/logging.h>
#include <base/strings/stringprintf.h>
#include <base/unguessable_token.h>
#include <libminijail.h>
#include <mojo/public/cpp/bindings/remote.h>
#include <mojo/public/cpp/bindings/self_owned_receiver.h>
#include <mojo/public/cpp/system/invitation.h>
#include <scoped_minijail.h>
#include <signal.h>

#include "handle_error_canonical.h"
#include "logger.h"
#include "prepared_model_stub.h"
#include "remote_call.h"

namespace android {
namespace nn {

using namespace chromeos::nnapi::canonical;

MojoControllerCanonical::MojoControllerCanonical(
    scoped_refptr<::base::SequencedTaskRunner> task_runner,
    const std::string service_name)
    : task_runner_(task_runner), service_name_(service_name) {
  pid_t worker_pid;
  mojo::PlatformChannel channel;
  // Use unique ids to differentiate mojo message pipe names if there are
  // multiple ipc drivers
  auto pipe_name = "mojo_driver_" + ::base::UnguessableToken{}.ToString();
  if (!SpawnWorkerProcessAndGetPid(channel, pipe_name, &worker_pid)) {
    LOG(FATAL) << "Failed to spawn worker process";
  } else {
    LOG(INFO) << "Spawned worker process: " << worker_pid;
  }
  MojoControllerCanonical::SendMojoInvitationAndGetRemote(
      worker_pid, std::move(channel), pipe_name);
}

MojoControllerCanonical::~MojoControllerCanonical() {
  // Move remote into ipc thread to be destroyed within the ipc thread
  task_runner_->PostTask(
      FROM_HERE, ::base::BindOnce([](mojo::Remote<mojom::IDevice> remote) {},
                                  std::move(remote_)));
}

void MojoControllerCanonical::SendMojoInvitationAndGetRemote(
    pid_t child_pid,
    mojo::PlatformChannel channel,
    std::string pipe_name) {
  // Send the Mojo invitation to the worker process.
  mojo::OutgoingInvitation invitation;
  mojo::ScopedMessagePipeHandle pipe = invitation.AttachMessagePipe(pipe_name);
  mojo::OutgoingInvitation::Send(std::move(invitation), child_pid,
                                 channel.TakeLocalEndpoint());
  remote_ = mojo::Remote<mojom::IDevice>(
      mojo::PendingRemote<mojom::IDevice>(std::move(pipe), 0u /* version */),
      task_runner_);
  task_runner_->PostTask(
      FROM_HERE, ::base::BindOnce(
                     [](mojo::Remote<mojom::IDevice>& remote, pid_t child_pid) {
                       remote.set_disconnect_handler(::base::BindOnce(
                           [](pid_t child) {
                             LOG(ERROR) << "remote_.disconnect_handler";
                             kill(child, SIGKILL);
                           },
                           child_pid));
                     },
                     std::ref(remote_), child_pid));
}

bool MojoControllerCanonical::SpawnWorkerProcessAndGetPid(
    const mojo::PlatformChannel& channel,
    std::string pipe_name,
    pid_t* worker_pid) {
  LOG(INFO) << "Starting to spawn nnapi worker process...";
  DCHECK(worker_pid != nullptr);

  // Start the process.
  auto mojo_bootstrap_fd =
      channel.remote_endpoint().platform_handle().GetFD().get();

  ScopedMinijail jail(minijail_new());
#define NNAPI_HAL_IPC_DRIVER_IN_CHROOT
#ifdef NNAPI_HAL_IPC_DRIVER_IN_CHROOT
  // Skip in order to conveniently run ipc driver in chroot during dev phase.
#else
  minijail_namespace_ipc(jail.get());
  minijail_namespace_uts(jail.get());
  minijail_namespace_net(jail.get());
  minijail_namespace_cgroups(jail.get());
  minijail_namespace_pids(jail.get());
  minijail_namespace_vfs(jail.get());

  // Closes the unused FDs in the worker process.
  // We keep the standard FDs here (should all point to `/dev/null`).
  // Also we need to keep the FD used in bootstrapping the mojo connection.
  CHECK(minijail_preserve_fd(jail.get(), STDIN_FILENO, STDIN_FILENO) == 0);
  CHECK(minijail_preserve_fd(jail.get(), STDOUT_FILENO, STDOUT_FILENO) == 0);
  CHECK(minijail_preserve_fd(jail.get(), STDERR_FILENO, STDERR_FILENO) == 0);
  CHECK(minijail_preserve_fd(jail.get(), mojo_bootstrap_fd,
                             mojo_bootstrap_fd) == 0);
  minijail_close_open_fds(jail.get());
#endif

  std::string worker_path = "/usr/bin/nnapi_worker_canonical";
  std::string fd_argv = ::base::StringPrintf("%d", mojo_bootstrap_fd);
  std::string service_name_argv = service_name_;
#ifdef STRACE_NNAPI_HAL_IPC_DRIVER
  // We use strace to create the log file to generate seccomp policy only
  // for the nnapi_worker. When we use strace, we do not call
  // minijail_use_seccomp_filter to use seccomp policy in minijail.
  LOG(INFO) << "Running strace on nnapi_worker...";

  std::string strace_path = "/usr/local/bin/strace";
  std::string strace_arg_1 = "-f";
  std::string strace_arg_2 = "-o";
  std::string strace_arg_3 = "/tmp/nnapi_worker_strace.log";
  const char* argv[9] = {
      &strace_path[0],       &strace_arg_1[0], &strace_arg_2[0],
      &strace_arg_3[0],      &worker_path[0],  &fd_argv[0],
      &service_name_argv[0], &pipe_name[0],    nullptr};

  if (minijail_run_pid(jail.get(), &strace_path[0], argv, worker_pid) != 0) {
    LOG(FATAL) << "Failed to spawn worker process using minijail";
    return false;
  }
#else
  std::string seccomp_policy_path =
      "/usr/share/policy/nnapi-hal-driver-seccomp.policy";
  char* argv[5] = {&worker_path[0], &fd_argv[0], &service_name_argv[0],
                   &pipe_name[0], nullptr};

#ifdef NNAPI_HAL_IPC_DRIVER_IN_CHROOT
  // Skip in order to conveniently run ipc driver in chroot during dev phase.
#else
  minijail_parse_seccomp_filters(jail.get(), &seccomp_policy_path[0]);
  minijail_use_seccomp_filter(jail.get());
#endif

  if (minijail_run_pid(jail.get(), &worker_path[0], argv, worker_pid) != 0) {
    LOG(FATAL) << "Failed to spawn worker process using minijail";
    return false;
  }
#endif

  LOG(INFO) << "Worker process spawned successfully!";
  return true;
}

Capabilities MojoControllerCanonical::getCapabilities() {
  VLOG(ML_NN_CHROMEOS_VLOG_LEVEL) << "MojoControllerCanonical::getCapabilities";
  Capabilities capabilities;
  auto remote_call = [](mojo::Remote<mojom::IDevice>& remote,
                        mojom::IDevice::getCapabilitiesCallback cb) {
    remote->getCapabilities(std::move(cb));
  };
  auto callback = DefaultOutputCallback<Capabilities>;
  LOG_REMOTE_CALL_FAILURE(
      CallRemote(task_runner_, remote_, std::move(remote_call),
                 std::move(callback), std::ref(capabilities)));
  return capabilities;
}

std::string MojoControllerCanonical::getVersionString() {
  VLOG(ML_NN_CHROMEOS_VLOG_LEVEL)
      << "MojoControllerCanonical::getVersionString";
  std::string version;
  auto remote_call = [](mojo::Remote<mojom::IDevice>& remote,
                        mojom::IDevice::getVersionStringCallback cb) {
    remote->getVersionString(std::move(cb));
  };
  auto callback = FullOutputCallback<std::string, const std::string&>;
  LOG_REMOTE_CALL_FAILURE(CallRemote(task_runner_, remote_,
                                     std::move(remote_call),
                                     std::move(callback), std::ref(version)));
  return version;
}

Version MojoControllerCanonical::getFeatureLevel() {
  VLOG(ML_NN_CHROMEOS_VLOG_LEVEL) << "MojoControllerCanonical::getFeatureLevel";
  Version featureLevel;
  auto remote_call = [](mojo::Remote<mojom::IDevice>& remote,
                        mojom::IDevice::getFeatureLevelCallback cb) {
    remote->getFeatureLevel(std::move(cb));
  };
  auto callback = DefaultOutputCallback<Version>;
  LOG_REMOTE_CALL_FAILURE(
      CallRemote(task_runner_, remote_, std::move(remote_call),
                 std::move(callback), std::ref(featureLevel)));
  return featureLevel;
}

DeviceType MojoControllerCanonical::getType() {
  VLOG(ML_NN_CHROMEOS_VLOG_LEVEL) << "MojoControllerCanonical::getType";
  DeviceType type;
  auto remote_call = [](mojo::Remote<mojom::IDevice>& remote,
                        mojom::IDevice::getTypeCallback cb) {
    remote->getType(std::move(cb));
  };
  auto callback = DefaultOutputCallback<DeviceType>;
  LOG_REMOTE_CALL_FAILURE(CallRemote(task_runner_, remote_,
                                     std::move(remote_call),
                                     std::move(callback), std::ref(type)));
  return type;
}

std::vector<Extension> MojoControllerCanonical::getSupportedExtensions() {
  VLOG(ML_NN_CHROMEOS_VLOG_LEVEL)
      << "MojoControllerCanonical::getSupportedExtensions";
  std::vector<Extension> extensions;
  auto remote_call = [](mojo::Remote<mojom::IDevice>& remote,
                        mojom::IDevice::getSupportedExtensionsCallback cb) {
    remote->getSupportedExtensions(std::move(cb));
  };
  auto callback =
      FullOutputCallback<std::vector<Extension>, const std::vector<Extension>&>;

  LOG_REMOTE_CALL_FAILURE(
      CallRemote(task_runner_, remote_, std::move(remote_call),
                 std::move(callback), std::ref(extensions)));
  return extensions;
}

std::pair<uint32_t, uint32_t>
MojoControllerCanonical::getNumberOfCacheFilesNeeded() {
  VLOG(ML_NN_CHROMEOS_VLOG_LEVEL)
      << "MojoControllerCanonical::getNumberOfCacheFilesNeeded";
  uint32_t numModelCache;
  uint32_t numDataCache;
  auto remote_call =
      [](mojo::Remote<mojom::IDevice>& remote,
         mojom::IDevice::getNumberOfCacheFilesNeededCallback cb) {
        remote->getNumberOfCacheFilesNeeded(std::move(cb));
      };
  auto callback = DefaultOutputCallback<uint32_t, uint32_t>;
  LOG_REMOTE_CALL_FAILURE(CallRemote(
      task_runner_, remote_, std::move(remote_call), std::move(callback),
      std::ref(numModelCache), std::ref(numDataCache)));
  return std::pair<uint32_t, uint32_t>{numModelCache, numDataCache};
}

GeneralResult<void> MojoControllerCanonical::wait() {
  VLOG(ML_NN_CHROMEOS_VLOG_LEVEL) << "MojoControllerCanonical::wait";
  GeneralError status;
  auto remote_call = [](mojo::Remote<mojom::IDevice>& remote,
                        mojom::IDevice::waitCallback cb) {
    remote->wait(std::move(cb));
  };
  auto callback = DefaultOutputCallback<GeneralError>;
  HANDLE_REMOTE_CALL_FAILURE(
      CallRemote(task_runner_, remote_, std::move(remote_call),
                 std::move(callback), std::ref(status)),
      ErrorStatus::DEVICE_UNAVAILABLE);
  return IS_OK(status.code) ? GeneralResult<void>{} : base::unexpected{status};
}

GeneralResult<std::vector<bool>>
MojoControllerCanonical::getSupportedOperations(const Model& model) {
  VLOG(ML_NN_CHROMEOS_VLOG_LEVEL)
      << "MojoControllerCanonical::getSupportedOperations";
  GeneralError status;
  std::vector<bool> operations;
  auto remote_call = [&](mojo::Remote<mojom::IDevice>& remote,
                         mojom::IDevice::getSupportedOperationsCallback cb) {
    remote->getSupportedOperations(model, std::move(cb));
  };
  auto callback = FullOutputCallback<GeneralError, std::vector<bool>,
                                     GeneralError, const std::vector<bool>&>;
  HANDLE_REMOTE_CALL_FAILURE(
      CallRemote(task_runner_, remote_, std::move(remote_call),
                 std::move(callback), std::ref(status), std::ref(operations)),
      ErrorStatus::DEVICE_UNAVAILABLE);
  return IS_OK(status.code) ? GeneralResult<std::vector<bool>>{operations}
                            : base::unexpected{status};
}

GeneralResult<SharedPreparedModel> MojoControllerCanonical::prepareModel(
    const Model& model,
    ExecutionPreference preference,
    Priority priority,
    OptionalTimePoint deadline,
    const std::vector<SharedHandle>& modelCache,
    const std::vector<SharedHandle>& dataCache,
    const CacheToken& token,
    const std::vector<nn::TokenValuePair>& hints,
    const std::vector<nn::ExtensionNameAndPrefix>& extensionNameToPrefix) {
  VLOG(ML_NN_CHROMEOS_VLOG_LEVEL) << "MojoControllerCanonical::prepareModel";
  GeneralError status;
  ::mojo::PendingRemote<mojom::IPreparedModel> preparedModel;
  auto remote_call = [&](mojo::Remote<mojom::IDevice>& remote,
                         mojom::IDevice::prepareModelCallback cb) {
    remote->prepareModel(model, preference, priority, deadline, modelCache,
                         dataCache, token, hints, extensionNameToPrefix,
                         std::move(cb));
  };
  auto callback =
      DefaultOutputCallback<GeneralError,
                            ::mojo::PendingRemote<mojom::IPreparedModel>>;
  HANDLE_REMOTE_CALL_FAILURE(
      CallRemote(task_runner_, remote_, std::move(remote_call),
                 std::move(callback), std::ref(status),
                 std::ref(preparedModel)),
      ErrorStatus::DEVICE_UNAVAILABLE);
  return IS_OK(status.code)
             ? GeneralResult<
                   SharedPreparedModel>{new android::nn::PreparedModelStub(
                   std::move(preparedModel), task_runner_)}
             : base::unexpected{status};
}

GeneralResult<SharedPreparedModel>
MojoControllerCanonical::prepareModelFromCache(
    OptionalTimePoint deadline,
    const std::vector<SharedHandle>& modelCache,
    const std::vector<SharedHandle>& dataCache,
    const CacheToken& token) {
  VLOG(ML_NN_CHROMEOS_VLOG_LEVEL)
      << "MojoControllerCanonical::prepareModelFromCache";
  GeneralError status;
  ::mojo::PendingRemote<mojom::IPreparedModel> preparedModel;
  auto remote_call = [&](mojo::Remote<mojom::IDevice>& remote,
                         mojom::IDevice::prepareModelFromCacheCallback cb) {
    remote->prepareModelFromCache(deadline, modelCache, dataCache, token,
                                  std::move(cb));
  };
  auto callback =
      DefaultOutputCallback<GeneralError,
                            ::mojo::PendingRemote<mojom::IPreparedModel>>;
  HANDLE_REMOTE_CALL_FAILURE(
      CallRemote(task_runner_, remote_, std::move(remote_call),
                 std::move(callback), std::ref(status),
                 std::ref(preparedModel)),
      ErrorStatus::DEVICE_UNAVAILABLE);
  return IS_OK(status.code)
             ? GeneralResult<
                   SharedPreparedModel>{new android::nn::PreparedModelStub(
                   std::move(preparedModel), task_runner_)}
             : base::unexpected{status};
}

}  // namespace nn
}  // namespace android
