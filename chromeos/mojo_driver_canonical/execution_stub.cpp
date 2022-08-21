// Copyright 2022 The ChromiumOS Authors.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#include "execution_stub.h"

#include <mojo/public/cpp/bindings/self_owned_receiver.h>

#include "handle_error_canonical.h"
#include "logger.h"

namespace android {
namespace nn {

using namespace chromeos::nnapi::canonical;

ExecutionResult<std::pair<std::vector<OutputShape>, Timing>>
ExecutionStub::compute(const OptionalTimePoint& deadline) const {
  VLOG(ML_NN_CHROMEOS_VLOG_LEVEL) << "ExecutionStub::compute";
  auto remote = mojo::Remote<mojom::IExecution>(std::move(pending_remote_));
  android::nn::ExecutionError status;
  std::vector<android::nn::OutputShape> outputShape;
  android::nn::Timing timing;
  HANDLE_REMOTE_CALL_FAILURE(
      remote->compute(deadline, &status, &outputShape, &timing),
      ErrorStatus::DEVICE_UNAVAILABLE);
  pending_remote_ = remote.Unbind();
  if (!IS_OK(status.code)) {
    return base::unexpected{status};
  }
  return std::pair<std::vector<OutputShape>, Timing>{outputShape, timing};
}

GeneralResult<std::pair<SyncFence, ExecuteFencedInfoCallback>>
ExecutionStub::computeFenced(
    const std::vector<SyncFence>& waitFor,
    const OptionalTimePoint& deadline,
    const OptionalDuration& timeoutDurationAfterFence) const {
  VLOG(ML_NN_CHROMEOS_VLOG_LEVEL) << "ExecutionStub::computeFenced";
  auto remote = mojo::Remote<mojom::IExecution>(std::move(pending_remote_));
  android::nn::GeneralError status;
  android::nn::SyncFence syncFence;
  ::mojo::PendingRemote<
      chromeos::nnapi::canonical::mojom::IExecuteFencedInfoCallback>
      callback;
  HANDLE_REMOTE_CALL_FAILURE(
      remote->computeFenced(waitFor, deadline, timeoutDurationAfterFence,
                            &status, &syncFence, &callback),
      ErrorStatus::DEVICE_UNAVAILABLE);
  pending_remote_ = remote.Unbind();
  if (!IS_OK(status.code)) {
    return base::unexpected{status};
  }
  ExecuteFencedInfoCallback callback_stub =
      [&callback]() -> GeneralResult<std::pair<Timing, Timing>> {
    auto remote = mojo::Remote<
        chromeos::nnapi::canonical::mojom::IExecuteFencedInfoCallback>(
        std::move(callback));
    android::nn::GeneralError status;
    android::nn::Timing timingLaunched;
    android::nn::Timing timingFenced;
    HANDLE_REMOTE_CALL_FAILURE(
        remote->getExecuteFencedInfo(&status, &timingLaunched, &timingFenced),
        ErrorStatus::DEVICE_UNAVAILABLE);
    if (!IS_OK(status.code)) {
      return base::unexpected{status};
    }
    return std::pair<Timing, Timing>{timingLaunched, timingFenced};
  };
  return std::pair<SyncFence, ExecuteFencedInfoCallback>{syncFence,
                                                         callback_stub};
}

}  // namespace nn
}  // namespace android
