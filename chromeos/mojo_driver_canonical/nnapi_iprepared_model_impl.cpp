// Copyright 2022 The ChromiumOS Authors.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#include "nnapi_iprepared_model_impl.h"

#include <mojo/public/cpp/bindings/self_owned_receiver.h>

#include "handle_error_canonical.h"
#include "logger.h"

namespace android {
namespace nn {

using namespace chromeos::nnapi::canonical;

const ExecutionError kExecutionErrorNone =
    ExecutionError{"", ErrorStatus::NONE};

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

}  // namespace nn
}  // namespace android
