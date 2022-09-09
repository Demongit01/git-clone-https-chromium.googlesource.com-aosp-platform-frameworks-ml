// Copyright 2022 The ChromiumOS Authors.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#include "execution_stub.h"

#include <mojo/public/cpp/bindings/self_owned_receiver.h>

#include "callbacks_stub.h"
#include "handle_error_canonical.h"
#include "logger.h"

namespace android {
namespace nn {

using namespace chromeos::nnapi::canonical;

ExecutionResult<std::pair<std::vector<OutputShape>, Timing>>
ExecutionStub::compute(const OptionalTimePoint& deadline) const {
  VLOG(ML_NN_CHROMEOS_VLOG_LEVEL) << "ExecutionStub::compute";
  ExecutionError status;
  std::vector<OutputShape> outputShape;
  Timing timing;
  auto remote_call = [&deadline](mojo::Remote<mojom::IExecution>& remote,
                                 mojom::IExecution::computeCallback cb) {
    remote->compute(deadline, std::move(cb));
  };
  auto callback = FullOutputCallback<ExecutionError, std::vector<OutputShape>,
                                     Timing, ExecutionError,
                                     const std::vector<OutputShape>&, Timing>;
  HANDLE_REMOTE_CALL_FAILURE(
      CallRemote(task_runner_, remote_, std::move(remote_call),
                 std::move(callback), std::ref(status), std::ref(outputShape),
                 std::ref(timing)),
      ErrorStatus::DEVICE_UNAVAILABLE);
  if (relocation_.output) {
    relocation_.output->flush();
  }
  return IS_OK(status.code)
             ? ExecutionResult<
                   std::pair<std::vector<OutputShape>, Timing>>{{outputShape,
                                                                 timing}}
             : base::unexpected{status};
}

GeneralResult<std::pair<SyncFence, ExecuteFencedInfoCallback>>
ExecutionStub::computeFenced(
    const std::vector<SyncFence>& waitFor,
    const OptionalTimePoint& deadline,
    const OptionalDuration& timeoutDurationAfterFence) const {
  VLOG(ML_NN_CHROMEOS_VLOG_LEVEL) << "ExecutionStub::computeFenced";

  GeneralError status;
  absl::optional<SyncFence> opt_fence;
  ::mojo::PendingRemote<mojom::IExecuteFencedInfoCallback> fenced_info;
  auto remote_call = [&waitFor, &deadline, &timeoutDurationAfterFence](
                         mojo::Remote<mojom::IExecution>& remote,
                         mojom::IExecution::computeFencedCallback cb) {
    remote->computeFenced(waitFor, deadline, timeoutDurationAfterFence,
                          std::move(cb));
  };
  auto callback = DefaultOutputCallback<
      GeneralError, absl::optional<SyncFence>,
      ::mojo::PendingRemote<mojom::IExecuteFencedInfoCallback>>;
  HANDLE_REMOTE_CALL_FAILURE(
      CallRemote(task_runner_, remote_, std::move(remote_call),
                 std::move(callback), std::ref(status), std::ref(opt_fence),
                 std::ref(fenced_info)),
      ErrorStatus::DEVICE_UNAVAILABLE);
  if (relocation_.output) {
    relocation_.output->flush();
  }

  if (!IS_OK(status.code)) {
    return base::unexpected{status};
  }
  auto callback_stub = std::make_shared<ExecuteFencedInfoCallbackStub>(
      std::move(fenced_info), task_runner_);
  ExecuteFencedInfoCallback cb = [callback_stub]() {
    return callback_stub->getExecuteFencedInfo();
  };
  return std::pair<SyncFence, ExecuteFencedInfoCallback>{
      opt_fence.has_value() ? opt_fence.value() : SyncFence{}, cb};
}

}  // namespace nn
}  // namespace android
