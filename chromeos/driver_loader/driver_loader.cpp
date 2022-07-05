// Copyright 2022 The ChromiumOS Authors.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#include "driver_loader.h"

#include <Utils.h>

#include "android-base/strings.h"
#include "driver_loader_utils.h"

namespace android {
namespace nn {

static const std::string kSandboxSuffix = "-sandbox";

std::vector<SharedDevice> getDevices() {
  std::vector<std::string> serviceNames = getServiceNames();

  std::vector<SharedDevice> devices;

  for (auto serviceName : serviceNames) {
    LOG(INFO) << "Loading service " << serviceName;
    GeneralResult<SharedDevice> result;
    if (base::EndsWith(serviceName, kSandboxSuffix)) {
      result = android::nn::ipc::getService(
          serviceName.substr(0, serviceName.size() - kSandboxSuffix.size()));
    } else {
      result = android::nn::getService(serviceName);
    }
    if (!result.has_value()) {
      LOG(ERROR) << "Failed to create Device (" << result.error().code
                 << "): " << result.error().message;
    } else {
      LOG(INFO) << "Creating SharedDevice for " << serviceName;
      devices.push_back(std::move(result).value());
    }
  }

  return devices;
}

}  // namespace nn
}  // namespace android
