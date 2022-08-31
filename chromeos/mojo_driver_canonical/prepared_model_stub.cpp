// Copyright 2022 The ChromiumOS Authors.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#include "prepared_model_stub.h"

#include <mojo/public/cpp/bindings/self_owned_receiver.h>

#include "burst_stub.h"
#include "callbacks_stub.h"
#include "execution_stub.h"
#include "handle_error_canonical.h"
#include "logger.h"

namespace android {
namespace nn {

using namespace chromeos::nnapi::canonical;

ExecutionResult<std::pair<std::vector<OutputShape>, Timing>>
PreparedModelStub::execute(
    const Request& request,
    MeasureTiming measure,
    const OptionalTimePoint& deadline,
    const OptionalDuration& loopTimeoutDuration,
    const std::vector<TokenValuePair>& hints,
    const std::vector<ExtensionNameAndPrefix>& extensionNameToPrefix) const {
  VLOG(ML_NN_CHROMEOS_VLOG_LEVEL) << "PreparedModelStub::execute";
  // Ensure that request is ready for IPC.
  std::optional<Request> maybeRequestInShared;
  RequestRelocation relocation;
  const Request& requestInShared = NN_TRY(convertRequestFromPointerToShared(
      &request, kDefaultRequestMemoryAlignment, kDefaultRequestMemoryPadding,
      &maybeRequestInShared, &relocation));
  return executeInternal(requestInShared, measure, deadline,
                         loopTimeoutDuration, hints, extensionNameToPrefix,
                         relocation);
}

ExecutionResult<std::pair<std::vector<OutputShape>, Timing>>
PreparedModelStub::executeInternal(
    const Request& request,
    MeasureTiming measure,
    const OptionalTimePoint& deadline,
    const OptionalDuration& loopTimeoutDuration,
    const std::vector<TokenValuePair>& hints,
    const std::vector<ExtensionNameAndPrefix>& extensionNameToPrefix,
    const RequestRelocation& relocation) const {
  if (relocation.input) {
    relocation.input->flush();
  }
  ExecutionError status;
  std::vector<OutputShape> output_shapes;
  Timing timing;
  auto remote_call = [&request, &measure, &deadline, &loopTimeoutDuration,
                      &hints, &extensionNameToPrefix](
                         mojo::Remote<mojom::IPreparedModel>& remote,
                         mojom::IPreparedModel::executeCallback cb) {
    remote->execute(request, measure, deadline, loopTimeoutDuration, hints,
                    extensionNameToPrefix, std::move(cb));
  };
  auto callback = FullOutputCallback<ExecutionError, std::vector<OutputShape>,
                                     Timing, ExecutionError,
                                     const std::vector<OutputShape>&, Timing>;
  HANDLE_REMOTE_CALL_FAILURE(
      CallRemote(task_runner_, remote_, std::move(remote_call),
                 std::move(callback), std::ref(status), std::ref(output_shapes),
                 std::ref(timing)),
      ErrorStatus::DEVICE_UNAVAILABLE);
  if (relocation.output) {
    relocation.output->flush();
  }
  VLOG(ML_NN_CHROMEOS_VLOG_LEVEL)
      << "PreparedModelStub::executeInternal finished";
  return IS_OK(status.code)
             ? ExecutionResult<
                   std::pair<std::vector<OutputShape>, Timing>>{{output_shapes,
                                                                 timing}}
             : base::unexpected{status};
}

GeneralResult<std::pair<SyncFence, ExecuteFencedInfoCallback>>
PreparedModelStub::executeFenced(
    const Request& request,
    const std::vector<SyncFence>& waitFor,
    MeasureTiming measure,
    const OptionalTimePoint& deadline,
    const OptionalDuration& loopTimeoutDuration,
    const OptionalDuration& timeoutDurationAfterFence,
    const std::vector<TokenValuePair>& hints,
    const std::vector<ExtensionNameAndPrefix>& extensionNameToPrefix) const {
  VLOG(ML_NN_CHROMEOS_VLOG_LEVEL) << "PreparedModelStub::executeFenced";
  std::optional<Request> maybeRequestInShared;
  RequestRelocation relocation;
  const Request& requestInShared = NN_TRY(convertRequestFromPointerToShared(
      &request, kDefaultRequestMemoryAlignment, kDefaultRequestMemoryPadding,
      &maybeRequestInShared, &relocation));
  return executeFencedInternal(requestInShared, waitFor, measure, deadline,
                               loopTimeoutDuration, timeoutDurationAfterFence,
                               hints, extensionNameToPrefix, relocation);
}

GeneralResult<std::pair<SyncFence, ExecuteFencedInfoCallback>>
PreparedModelStub::executeFencedInternal(
    const Request& request,
    const std::vector<SyncFence>& waitFor,
    MeasureTiming measure,
    const OptionalTimePoint& deadline,
    const OptionalDuration& loopTimeoutDuration,
    const OptionalDuration& timeoutDurationAfterFence,
    const std::vector<TokenValuePair>& hints,
    const std::vector<ExtensionNameAndPrefix>& extensionNameToPrefix,
    const RequestRelocation& relocation) const {
  if (relocation.input) {
    relocation.input->flush();
  }
  GeneralError status;
  absl::optional<SyncFence> opt_fence;
  ::mojo::PendingRemote<mojom::IExecuteFencedInfoCallback> fenced_info;
  auto remote_call = [&request, &waitFor, &measure, &deadline,
                      &loopTimeoutDuration, &timeoutDurationAfterFence, &hints,
                      &extensionNameToPrefix](
                         mojo::Remote<mojom::IPreparedModel>& remote,
                         mojom::IPreparedModel::executeFencedCallback cb) {
    remote->executeFenced(request, waitFor, measure, deadline,
                          loopTimeoutDuration, timeoutDurationAfterFence, hints,
                          extensionNameToPrefix, std::move(cb));
  };
  auto callback = DefaultOutputCallback<
      GeneralError, absl::optional<SyncFence>,
      ::mojo::PendingRemote<mojom::IExecuteFencedInfoCallback>>;
  HANDLE_REMOTE_CALL_FAILURE(
      CallRemote(task_runner_, remote_, std::move(remote_call),
                 std::move(callback), std::ref(status), std::ref(opt_fence),
                 std::ref(fenced_info)),
      ErrorStatus::DEVICE_UNAVAILABLE);
  if (relocation.output) {
    relocation.output->flush();
  }
  if (!IS_OK(status.code)) {
    return base::unexpected{status};
  }
  auto callback_stub = std::make_shared<ExecuteFencedInfoCallbackStub>(
      std::move(fenced_info), task_runner_);
  ExecuteFencedInfoCallback cb = [callback_stub]() {
    return callback_stub->getExecuteFencedInfo();
  };
  VLOG(ML_NN_CHROMEOS_VLOG_LEVEL)
      << "PreparedModelStub::executeFencedInternal finished.";
  return std::pair<SyncFence, ExecuteFencedInfoCallback>{
      opt_fence.has_value() ? opt_fence.value() : SyncFence{}, cb};
}

GeneralResult<SharedExecution> PreparedModelStub::createReusableExecution(
    const Request& request,
    MeasureTiming measure,
    const OptionalDuration& loopTimeoutDuration,
    const std::vector<TokenValuePair>& hints,
    const std::vector<ExtensionNameAndPrefix>& extensionNameToPrefix) const {
  VLOG(ML_NN_CHROMEOS_VLOG_LEVEL)
      << "PreparedModelStub::createReusableExecution";
  GeneralError status;
  ::mojo::PendingRemote<mojom::IExecution> execution;
  auto remote_call =
      [&request, &measure, &loopTimeoutDuration, &hints,
       &extensionNameToPrefix](
          mojo::Remote<mojom::IPreparedModel>& remote,
          mojom::IPreparedModel::createReusableExecutionCallback cb) {
        remote->createReusableExecution(request, measure, loopTimeoutDuration,
                                        hints, extensionNameToPrefix,
                                        std::move(cb));
      };
  auto callback =
      DefaultOutputCallback<GeneralError,
                            ::mojo::PendingRemote<mojom::IExecution>>;
  HANDLE_REMOTE_CALL_FAILURE(
      CallRemote(task_runner_, remote_, std::move(remote_call),
                 std::move(callback), std::ref(status), std::ref(execution)),
      ErrorStatus::DEVICE_UNAVAILABLE);
  VLOG(ML_NN_CHROMEOS_VLOG_LEVEL)
      << "PreparedModelStub::createReusableExecution finished.";
  return IS_OK(status.code) ? GeneralResult<SharedExecution>{new ExecutionStub(
                                  std::move(execution), task_runner_)}
                            : base::unexpected{status};
}

GeneralResult<SharedBurst> PreparedModelStub::configureExecutionBurst() const {
  VLOG(ML_NN_CHROMEOS_VLOG_LEVEL)
      << "PreparedModelStub::configureExecutionBurst";
  GeneralError status;
  ::mojo::PendingRemote<mojom::IBurst> burst;
  auto remote_call =
      [](mojo::Remote<mojom::IPreparedModel>& remote,
         mojom::IPreparedModel::configureExecutionBurstCallback cb) {
        remote->configureExecutionBurst(std::move(cb));
      };
  auto callback =
      DefaultOutputCallback<GeneralError, ::mojo::PendingRemote<mojom::IBurst>>;
  HANDLE_REMOTE_CALL_FAILURE(
      CallRemote(task_runner_, remote_, std::move(remote_call),
                 std::move(callback), std::ref(status), std::ref(burst)),
      ErrorStatus::DEVICE_UNAVAILABLE);
  VLOG(ML_NN_CHROMEOS_VLOG_LEVEL)
      << "PreparedModelStub::configureExecutionBurst finished.";
  return IS_OK(status.code) ? GeneralResult<SharedBurst>{new BurstStub(
                                  std::move(burst), task_runner_)}
                            : base::unexpected{status};
}

std::any PreparedModelStub::getUnderlyingResource() const {
  LOG(FATAL) << "PreparedModelStub::getUnderlyingResource should not be called "
                "since IDevice::allocate is not supported by ipc driver.";
  return {};
}

}  // namespace nn
}  // namespace android
