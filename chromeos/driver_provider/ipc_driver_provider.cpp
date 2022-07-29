// Copyright 2022 The ChromiumOS Authors.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#include <Utils.h>
#include <android-base/logging.h>
#include <nnapi/IDevice.h>

#include "chromeos_config_portal.h"
#include "driver_loader.h"
#include "ipc_driver_canonical.h"

namespace android {
namespace nn {
namespace ipc {

GeneralResult<SharedDevice> getService(const std::string& serviceName) {
  LOG(INFO) << "Creating " << serviceName << " mojo driver";

  // This is the temporary local driver only used during development phase
  // which will be replaced by remote driver later.
  GeneralResult<SharedDevice> result = android::nn::getService(serviceName);
  if (!result.has_value()) {
    LOG(ERROR) << "Failed to create Device (" << result.error().code
               << "): " << result.error().message;
    return result;
  }
  auto ipcDriver = new android::nn::IPCDriverCanonical(
      serviceName + kSandboxSuffix, std::move(result).value());
  return GeneralResult<SharedDevice>{ipcDriver};
}

}  // namespace ipc
}  // namespace nn
}  // namespace android
