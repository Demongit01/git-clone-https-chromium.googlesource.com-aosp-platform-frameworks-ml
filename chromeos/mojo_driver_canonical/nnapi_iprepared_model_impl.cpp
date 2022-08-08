// Copyright 2022 The ChromiumOS Authors.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#include "nnapi_iprepared_model_impl.h"

#include <mojo/public/cpp/bindings/self_owned_receiver.h>

#include "logger.h"

namespace android {
namespace nn {

using namespace chromeos::nnapi::canonical;

void IPreparedModelImpl::execute(
    Request request,
    MeasureTiming measure,
    absl::optional<TimePoint> deadline,
    absl::optional<Duration> loopTimeoutDuration,
    const std::vector<TokenValuePair>& hints,
    const std::vector<ExtensionNameAndPrefix>& extensionNameToPrefix,
    executeCallback callback) {
  LOG(INFO) << "IPreparedModelImpl::execute";
  auto result =
      wrapped_model_->execute(request, measure, deadline, loopTimeoutDuration,
                              hints, extensionNameToPrefix);
  if (result.ok()) {
    std::move(callback).Run(
        ExecutionError{"preparedModel executed successfully",
                       ErrorStatus::NONE},
        result->first, result->second);
    LOG(INFO) << "IPreparedModelImpl::execute: okay";
  } else {
    std::move(callback).Run(result.error(), {}, {});
    LOG(INFO) << "IPreparedModelImpl::execute:"
              << static_cast<int>(result.error().code) << ":"
              << result.error().message;
  }
}

}  // namespace nn
}  // namespace android
