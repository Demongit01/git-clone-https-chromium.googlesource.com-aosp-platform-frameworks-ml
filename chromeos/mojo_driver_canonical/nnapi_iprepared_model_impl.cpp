// Copyright 2022 The ChromiumOS Authors.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#include "nnapi_iprepared_model_impl.h"

#include <mojo/public/cpp/bindings/self_owned_receiver.h>

#include "handle_error_canonical.h"
#include "logger.h"
#include "nnapi_iburst_impl.h"

namespace android {
namespace nn {

using namespace chromeos::nnapi::canonical;

const ExecutionError kExecutionErrorNone =
    ExecutionError{"", ErrorStatus::NONE};
const GeneralError kGeneralErrorNone = GeneralError{"", ErrorStatus::NONE};

void IPreparedModelImpl::execute(
    Request request,
    MeasureTiming measure,
    absl::optional<TimePoint> deadline,
    absl::optional<Duration> loopTimeoutDuration,
    const std::vector<TokenValuePair>& hints,
    const std::vector<ExtensionNameAndPrefix>& extensionNameToPrefix,
    executeCallback callback) {
  VLOG(ML_NN_CHROMEOS_VLOG_LEVEL) << "IPreparedModelImpl::execute";
  auto result =
      wrapped_model_->execute(request, measure, deadline, loopTimeoutDuration,
                              hints, extensionNameToPrefix);
  HANDLE_ERROR_RESULT(result, callback, {}, {});
  std::move(callback).Run(kExecutionErrorNone, result->first, result->second);
}

void IPreparedModelImpl::executeFenced(
    Request request,
    std::vector<SyncFence> waitFor,
    MeasureTiming measure,
    absl::optional<TimePoint> deadline,
    absl::optional<Duration> loopTimeoutDuration,
    absl::optional<Duration> timeoutDurationAfterFence,
    const std::vector<TokenValuePair>& hints,
    const std::vector<ExtensionNameAndPrefix>& extensionNameToPrefix,
    executeFencedCallback callback) {
  VLOG(ML_NN_CHROMEOS_VLOG_LEVEL) << "IPreparedModelImpl::executeFenced";
  auto result = wrapped_model_->executeFenced(
      request, waitFor, measure, deadline, loopTimeoutDuration,
      timeoutDurationAfterFence, hints, extensionNameToPrefix);
  HANDLE_ERROR_RESULT(result, callback, {}, {});
  mojo::PendingRemote<mojom::IExecuteFencedInfoCallback> pm_remote;
  mojo::MakeSelfOwnedReceiver(
      std::make_unique<IExecuteFencedInfoCallbackImpl>(result.value().second),
      pm_remote.InitWithNewPipeAndPassReceiver());
  std::move(callback).Run(kGeneralErrorNone, result.value().first,
                          std::move(pm_remote));
}

void IPreparedModelImpl::configureExecutionBurst(
    configureExecutionBurstCallback callback) {
  VLOG(ML_NN_CHROMEOS_VLOG_LEVEL)
      << "IPreparedModelImpl::configureExecutionBurst";
  auto result = wrapped_model_->configureExecutionBurst();
  HANDLE_ERROR_RESULT(result, callback, {});
  mojo::PendingRemote<mojom::IBurst> pm_remote;
  mojo::MakeSelfOwnedReceiver(std::make_unique<IBurstImpl>(result.value()),
                              pm_remote.InitWithNewPipeAndPassReceiver());
  std::move(callback).Run(kGeneralErrorNone, std::move(pm_remote));
}

void IExecuteFencedInfoCallbackImpl::getExecuteFencedInfo(
    getExecuteFencedInfoCallback callback) {
  VLOG(ML_NN_CHROMEOS_VLOG_LEVEL)
      << "IExecuteFencedInfoCallbackImpl::getExecuteFencedInfo";
  auto result = wrapped_callback_();
  HANDLE_ERROR_RESULT(result, callback, {}, {});
  std::move(callback).Run(kGeneralErrorNone, result->first, result->second);
}

}  // namespace nn
}  // namespace android
