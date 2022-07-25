// Copyright 2022 The Chromium OS Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef ML_NN_CHROMEOS_CHROMEOS_CONFIG_PORTAL_H_
#define ML_NN_CHROMEOS_CHROMEOS_CONFIG_PORTAL_H_

#include <cros_config/cros_config_interface.h>
#include <string>
#include <vector>

struct DriverConfig {
  std::string name;
  std::string shared_library;
};

std::vector<DriverConfig> getDriverConfigs(
    std::unique_ptr<brillo::CrosConfigInterface> cfg_portal);

extern const char kSandboxSuffix[];

#endif  // ML_NN_CHROMEOS_CHROMEOS_CONFIG_PORTAL_H_
