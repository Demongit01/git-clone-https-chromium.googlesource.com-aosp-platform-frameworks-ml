// Copyright 2020 The ChromiumOS Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#include <android-base/logging.h>
#include <cros_config/cros_config.h>
#include <sstream>
#include <string>
#include <unordered_map>
#include <utility>
#include <vector>

#include "HalInterfaces.h"
#include "chromeos_config_portal.h"
#include "sample_driver_provider_util.h"

// type of function pointer
typedef void* (*get_driver_func)();

std::unordered_map<std::string, std::string> driverToLibFallback = {
    {"default", "libfull-driver.so"},
    {"cros-default",
     "libmojo-driver.so"}  // 'cros-default' is used in VTS tests
};
std::unordered_map<std::string, std::string> driverToLib;
std::string funcName = "get_driver";

template <typename T>
T getDriverInstance(std::string serviceName) {
  auto configs = getDriverConfigs(std::make_unique<brillo::CrosConfig>());

  for (auto config : configs) {
    driverToLib.insert(std::pair<std::string, std::string>(
        config.name, config.shared_library));
  }

  // will use default config if no config provided
  if (!driverToLib.size()) {
    LOG(ERROR) << "No cros-config found. Will use fallback config";
    driverToLib = driverToLibFallback;
  }

  if (driverToLib.find(serviceName) == driverToLib.end()) {
    LOG(ERROR) << "Cannot find " << serviceName << " in available driver list.";
    return nullptr;
  }

  void* driverHandle = GetFunctionFrom(driverToLib[serviceName], funcName);

  LOG(INFO) << "Loading " << serviceName << " from "
            << driverToLib[serviceName];
  if (driverHandle == nullptr) {
    LOG(ERROR) << driverToLib[serviceName] << " can not be loaded!";
    return nullptr;
  }
  auto getDriverFunc = reinterpret_cast<get_driver_func>(driverHandle);

  auto sampleDriverInstance = static_cast<T>(getDriverFunc());
  if (sampleDriverInstance == nullptr) {
    LOG(FATAL) << "Cannot create driver from " << driverToLib[serviceName];
  }

  return sampleDriverInstance;
}

namespace android {
namespace hardware {
namespace neuralnetworks {

// We're currently getting our driver implementation in the runtime from
// V1_0::IDevice::getService, but if the same call is being made from another
// version (such as in VTS), we can cast it up. This is outside of
// sampledriver.cpp so that it will work with a vendor hal.

namespace V1_1 {
::android::sp<IDevice> IDevice::getService(const std::string& serviceName,
                                           bool getStub) {
  return IDevice::castFrom(V1_0::IDevice::getService(serviceName, getStub));
}
}  // namespace V1_1

namespace V1_2 {
::android::sp<IDevice> IDevice::getService(const std::string& serviceName,
                                           bool getStub) {
  return IDevice::castFrom(V1_0::IDevice::getService(serviceName, getStub));
}
}  // namespace V1_2

namespace V1_3 {
::android::sp<IDevice> IDevice::getService(const std::string& serviceName,
                                           bool getStub) {
  return IDevice::castFrom(V1_0::IDevice::getService(serviceName, getStub));
}
}  // namespace V1_3

namespace V1_0 {
// static
// This registers the SampleDriverFull into the DeviceManager.
::android::sp<IDevice> IDevice::getService(const std::string& serviceName,
                                           bool /*dummy*/) {
  LOG(INFO) << "Creating " << serviceName << " driver";

  auto sampleDriverInstance = getDriverInstance<IDevice*>(serviceName);

  return sampleDriverInstance;
}
}  // namespace V1_0
}  // namespace neuralnetworks
}  // namespace hardware
}  // namespace android
