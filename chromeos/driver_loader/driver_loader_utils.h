// Copyright 2022 The Chromium OS Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef ML_NN_CHROMEOS_DRIVER_LOADER_UTILS_H_
#define ML_NN_CHROMEOS_DRIVER_LOADER_UTILS_H_

#include <string>
#include <vector>

namespace android {
namespace nn {

std::vector<std::string> getServiceNames();

}  // namespace nn
}  // namespace android

#endif  // ML_NN_CHROMEOS_DRIVER_LOADER_UTILS_H_
