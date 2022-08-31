// Copyright 2022 The ChromiumOS Authors.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#include "nnapi_iprepared_model_impl.h"

#include <mojo/public/cpp/bindings/self_owned_receiver.h>

#include "handle_error_canonical.h"
#include "logger.h"
#include "nnapi_iburst_impl.h"
#include "nnapi_iexecution_impl.h"

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
  VLOG(ML_NN_CHROMEOS_VLOG_LEVEL) << "IPreparedModelImpl::execute finished";
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
  mojo::PendingRemote<mojom::IExecuteFencedInfoCallback> pm_remote;
  if (!result.ok()) {
    mojo::MakeSelfOwnedReceiver(
        std::make_unique<IExecuteFencedInfoCallbackImpl>(nullptr),
        pm_remote.InitWithNewPipeAndPassReceiver());
    std::move(callback).Run(result.error(), {}, std::move(pm_remote));
    return;
  }
  mojo::MakeSelfOwnedReceiver(
      std::make_unique<IExecuteFencedInfoCallbackImpl>(result.value().second),
      pm_remote.InitWithNewPipeAndPassReceiver());
  // PrepareModel implmentation might return a SyncFence with a nullptr handle
  absl::optional<SyncFence> opt_fence =
      result.value().first.hasFd() ? absl::make_optional(result.value().first)
                                   : absl::nullopt;
  std::move(callback).Run(kGeneralErrorNone, opt_fence, std::move(pm_remote));
  VLOG(ML_NN_CHROMEOS_VLOG_LEVEL)
      << "IPreparedModelImpl::executeFenced finished.";
}

void IPreparedModelImpl::createReusableExecution(
    Request request,
    MeasureTiming measure,
    absl::optional<Duration> loopTimeoutDuration,
    const std::vector<TokenValuePair>& hints,
    const std::vector<ExtensionNameAndPrefix>& extensionNameToPrefix,
    createReusableExecutionCallback callback) {
  VLOG(ML_NN_CHROMEOS_VLOG_LEVEL)
      << "IPreparedModelImpl::createReusableExecution";
  auto result = wrapped_model_->createReusableExecution(
      request, measure, loopTimeoutDuration, hints, extensionNameToPrefix);
  mojo::PendingRemote<mojom::IExecution> pm_remote;
  if (!result.ok()) {
    mojo::MakeSelfOwnedReceiver(std::make_unique<IExecutionImpl>(nullptr),
                                pm_remote.InitWithNewPipeAndPassReceiver());
    std::move(callback).Run(result.error(), std::move(pm_remote));
    return;
  }
  mojo::MakeSelfOwnedReceiver(std::make_unique<IExecutionImpl>(result.value()),
                              pm_remote.InitWithNewPipeAndPassReceiver());
  std::move(callback).Run(kGeneralErrorNone, std::move(pm_remote));
  VLOG(ML_NN_CHROMEOS_VLOG_LEVEL)
      << "IPreparedModelImpl::createReusableExecution finished";
}

void IPreparedModelImpl::configureExecutionBurst(
    configureExecutionBurstCallback callback) {
  VLOG(ML_NN_CHROMEOS_VLOG_LEVEL)
      << "IPreparedModelImpl::configureExecutionBurst";
  auto result = wrapped_model_->configureExecutionBurst();
  mojo::PendingRemote<mojom::IBurst> pm_remote;
  if (!result.ok()) {
    mojo::MakeSelfOwnedReceiver(std::make_unique<IBurstImpl>(nullptr),
                                pm_remote.InitWithNewPipeAndPassReceiver());
    std::move(callback).Run(result.error(), std::move(pm_remote));
    return;
  }
  mojo::MakeSelfOwnedReceiver(std::make_unique<IBurstImpl>(result.value()),
                              pm_remote.InitWithNewPipeAndPassReceiver());
  std::move(callback).Run(kGeneralErrorNone, std::move(pm_remote));
  VLOG(ML_NN_CHROMEOS_VLOG_LEVEL)
      << "IPreparedModelImpl::configureExecutionBurst finished";
}

void IExecuteFencedInfoCallbackImpl::getExecuteFencedInfo(
    getExecuteFencedInfoCallback callback) {
  VLOG(ML_NN_CHROMEOS_VLOG_LEVEL)
      << "IExecuteFencedInfoCallbackImpl::getExecuteFencedInfo";
  auto result = wrapped_callback_();
  HANDLE_ERROR_RESULT(result, callback, {}, {});
  std::move(callback).Run(kGeneralErrorNone, result->first, result->second);
  VLOG(ML_NN_CHROMEOS_VLOG_LEVEL)
      << "IExecuteFencedInfoCallbackImpl::getExecuteFencedInfo finished";
}

}  // namespace nn
}  // namespace android
