// Copyright 2022 The ChromiumOS Authors.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#include <android-base/logging.h>
#include <nnapi/IDevice.h>

#include <Utils.h>
#include <nnapi/hal/1.3/Device.h>

#include "SampleDriverFull.h"

namespace android {
namespace nn {

GeneralResult<SharedDevice> getService(const std::string& serviceName) {
  LOG(INFO) << "Creating " << serviceName << " driver";
  auto driver = new android::nn::sample_driver::SampleDriverFull(
      "ChromeSampleDriverFull",
      // Lower is better, this will force the driver to be selected by the
      // ExecutionPlan
      {.execTime = .9f, .powerUsage = .9f});
  GeneralResult<SharedDevice> result =
      V1_3::utils::Device::create(serviceName, std::move(driver));
  return result;
}

}  // namespace nn
}  // namespace android
