// Copyright 2022 The ChromiumOS Authors.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#include "nnapi_iexecution_impl.h"

#include <mojo/public/cpp/bindings/self_owned_receiver.h>

#include "handle_error_canonical.h"
#include "logger.h"
#include "nnapi_iprepared_model_impl.h"

namespace android {
namespace nn {

using namespace chromeos::nnapi::canonical;

const ExecutionError kExecutionErrorNone =
    ExecutionError{"", ErrorStatus::NONE};
const GeneralError kGeneralErrorNone = GeneralError{"", ErrorStatus::NONE};

void IExecutionImpl::compute(absl::optional<TimePoint> deadline,
                             computeCallback callback) {
  VLOG(ML_NN_CHROMEOS_VLOG_LEVEL) << "IExecutionImpl::compute";
  auto result = wrapped_execution_->compute(deadline);
  HANDLE_ERROR_RESULT(result, callback, {}, {});
  std::move(callback).Run(kExecutionErrorNone, result->first, result->second);
}
void IExecutionImpl::computeFenced(
    std::vector<SyncFence> waitFor,
    absl::optional<TimePoint> deadline,
    absl::optional<Duration> timeoutDurationAfterFence,
    computeFencedCallback callback) {
  VLOG(ML_NN_CHROMEOS_VLOG_LEVEL) << "IExecutionImpl::compute";
  auto result = wrapped_execution_->computeFenced(waitFor, deadline,
                                                  timeoutDurationAfterFence);
  mojo::PendingRemote<mojom::IExecuteFencedInfoCallback> pending_remote;
  mojo::MakeSelfOwnedReceiver(
      std::make_unique<IExecuteFencedInfoCallbackImpl>(
          result.ok() ? result.value().second : nullptr),
      pending_remote.InitWithNewPipeAndPassReceiver());
  absl::optional<SyncFence> opt_fence =
      result.ok() && result.value().first.hasFd()
          ? absl::make_optional(result.value().first)
          : absl::nullopt;
  std::move(callback).Run(result.ok() ? kGeneralErrorNone : result.error(),
                          opt_fence, std::move(pending_remote));
}

}  // namespace nn
}  // namespace android
