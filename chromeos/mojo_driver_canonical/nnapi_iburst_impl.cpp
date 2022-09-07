// Copyright 2022 The ChromiumOS Authors.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#include "nnapi_iburst_impl.h"

#include <mojo/public/cpp/bindings/self_owned_receiver.h>

#include "handle_error_canonical.h"
#include "logger.h"
#include "nnapi_iexecution_impl.h"

namespace android {
namespace nn {

using namespace chromeos::nnapi::canonical;

const ExecutionError kExecutionErrorNone =
    ExecutionError{"", ErrorStatus::NONE};
const GeneralError kGeneralErrorNone = GeneralError{"", ErrorStatus::NONE};

void IBurstImpl::execute(
    Request request,
    MeasureTiming measure,
    absl::optional<TimePoint> deadline,
    absl::optional<Duration> loopTimeoutDuration,
    const std::vector<TokenValuePair>& hints,
    const std::vector<ExtensionNameAndPrefix>& extensionNameToPrefix,
    executeCallback callback) {
  VLOG(ML_NN_CHROMEOS_VLOG_LEVEL) << "IBurstImpl::execute";
  auto result =
      wrapped_burst_->execute(request, measure, deadline, loopTimeoutDuration,
                              hints, extensionNameToPrefix);
  HANDLE_ERROR_RESULT(result, callback, {}, {});
  std::move(callback).Run(kExecutionErrorNone, result->first, result->second);
}

void IBurstImpl::createReusableExecution(
    Request request,
    MeasureTiming measure,
    absl::optional<Duration> loopTimeoutDuration,
    const std::vector<TokenValuePair>& hints,
    const std::vector<ExtensionNameAndPrefix>& extensionNameToPrefix,
    createReusableExecutionCallback callback) {
  VLOG(ML_NN_CHROMEOS_VLOG_LEVEL) << "IBurstImpl::createReusableExecution";
  auto result = wrapped_burst_->createReusableExecution(
      request, measure, loopTimeoutDuration, hints, extensionNameToPrefix);
  mojo::PendingRemote<mojom::IExecution> pending_remote;
  mojo::MakeSelfOwnedReceiver(
      std::make_unique<IExecutionImpl>(result.ok() ? result.value() : nullptr),
      pending_remote.InitWithNewPipeAndPassReceiver());
  std::move(callback).Run(result.ok() ? kGeneralErrorNone : result.error(),
                          std::move(pending_remote));
  VLOG(ML_NN_CHROMEOS_VLOG_LEVEL)
      << "IBurstImpl::createReusableExecution finished";
}

}  // namespace nn
}  // namespace android
