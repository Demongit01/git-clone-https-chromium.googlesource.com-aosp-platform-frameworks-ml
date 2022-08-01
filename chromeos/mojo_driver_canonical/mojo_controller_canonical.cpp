// Copyright 2022 The ChromiumOS Authors.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#include "mojo_controller_canonical.h"

#include <base/logging.h>
#include <base/no_destructor.h>
#include <base/strings/stringprintf.h>
#include <base/task/current_thread.h>
#include <base/task/task_traits.h>
#include <base/unguessable_token.h>
#include <brillo/message_loops/base_message_loop.h>
#include <libminijail.h>
#include <mojo/core/embedder/embedder.h>
#include <mojo/public/cpp/bindings/remote.h>
#include <mojo/public/cpp/bindings/self_owned_receiver.h>
#include <mojo/public/cpp/system/invitation.h>
#include <scoped_minijail.h>
#include <signal.h>

#include "logger.h"

namespace android {
namespace nn {

using namespace chromeos::nnapi;

class MojoThread {
 public:
  // This MojoThread should be used as singleton, the reason for this
  // constructor being public is the base::NoDestructor<> needs a public
  // constructor.
  MojoThread();

  MojoThread(MojoThread const&) = delete;
  void operator=(MojoThread const&) = delete;

  static MojoThread& Get();

  scoped_refptr<::base::SingleThreadTaskRunner>& GetIpcTaskRunner();

 private:
  std::unique_ptr<::base::Thread> ipc_thread_;
  std::unique_ptr<mojo::core::ScopedIPCSupport> ipc_support_;
};

MojoThread::MojoThread() {
  mojo::core::Init();
  ipc_thread_ = std::make_unique<::base::Thread>("IpcThread");
  ipc_thread_->StartWithOptions(
      ::base::Thread::Options(::base::MessagePumpType::IO, 0));
  ipc_support_ = std::make_unique<mojo::core::ScopedIPCSupport>(
      ipc_thread_->task_runner(),
      mojo::core::ScopedIPCSupport::ShutdownPolicy::CLEAN);
}

scoped_refptr<::base::SingleThreadTaskRunner>& MojoThread::GetIpcTaskRunner() {
  static auto task_runner = ipc_thread_->task_runner();
  return task_runner;
}

// static
MojoThread& MojoThread::Get() {
  static ::base::NoDestructor<MojoThread> kMojoThread;
  return *kMojoThread;
}

MojoControllerCanonical::MojoControllerCanonical(const std::string service_name)
    : service_name_(service_name) {
  // We need to get task runner from MojoThread which also initializes mojo
  // core, creates ipc thread, provides mojo ipc support, etc.
  ipc_task_runner_ = MojoThread::Get().GetIpcTaskRunner();

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

void MojoControllerCanonical::SendMojoInvitationAndGetRemote(
    pid_t child_pid,
    mojo::PlatformChannel channel,
    std::string pipe_name) {
  // Send the Mojo invitation to the worker process.
  mojo::OutgoingInvitation invitation;
  mojo::ScopedMessagePipeHandle pipe = invitation.AttachMessagePipe(pipe_name);

  remote_ = mojo::Remote<canonical::mojom::IDevice>(
      mojo::PendingRemote<canonical::mojom::IDevice>(std::move(pipe),
                                                     0u /* version */));

  mojo::OutgoingInvitation::Send(std::move(invitation), child_pid,
                                 channel.TakeLocalEndpoint());
  remote_.set_disconnect_handler(::base::BindOnce(
      [](pid_t child) {
        LOG(ERROR) << "remote_.disconnect_handler";
        kill(child, SIGKILL);
      },
      child_pid));
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
  android::nn::Capabilities out_capabilities;
  remote_->getCapabilities(&out_capabilities);
  return out_capabilities;
}

}  // namespace nn
}  // namespace android
