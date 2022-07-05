// Copyright 2022 The ChromiumOS Authors.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef ML_NN_CHROMEOS_DRIVER_LOADER_H_
#define ML_NN_CHROMEOS_DRIVER_LOADER_H_

#include <nnapi/IDevice.h>

namespace android {
namespace nn {

GeneralResult<SharedDevice> getService(const std::string& serviceName);

}  // namespace nn
}  // namespace android

#endif  // ML_NN_CHROMEOS_DRIVER_LOADER_H_
