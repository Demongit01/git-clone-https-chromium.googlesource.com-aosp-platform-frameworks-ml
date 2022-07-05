// Copyright 2022 The ChromiumOS Authors.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#include <android-base/logging.h>
#include <nnapi/IDevice.h>

#include <Utils.h>
#include <nnapi/hal/1.3/Device.h>

#include "SampleDriverFull.h"
#include "ipc_driver.h"

namespace android {
namespace nn {
namespace ipc {

GeneralResult<SharedDevice> getService(const std::string& serviceName) {
  LOG(INFO) << "Creating " << serviceName << " mojo driver";
  auto tempDriver = new android::nn::sample_driver::SampleDriverFull(
      "ChromeSampleDriverFull", {.execTime = .9f, .powerUsage = .9f});
  auto driver = new android::nn::IPCDriver(serviceName.c_str(), tempDriver);
  GeneralResult<SharedDevice> result =
      V1_3::utils::Device::create(serviceName, std::move(driver));
  return result;
}

}  // namespace ipc
}  // namespace nn
}  // namespace android
