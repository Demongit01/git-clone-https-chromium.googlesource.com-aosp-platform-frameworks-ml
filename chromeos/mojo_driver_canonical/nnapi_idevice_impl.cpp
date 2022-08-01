// Copyright 2022 The ChromiumOS Authors.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#include "nnapi_idevice_impl.h"

#include <mojo/public/cpp/bindings/self_owned_receiver.h>

#include "logger.h"

namespace android {
namespace nn {

using namespace chromeos::nnapi::canonical;

void IDeviceImpl::getCapabilities(getCapabilitiesCallback callback) {
  VLOG(ML_NN_CHROMEOS_VLOG_LEVEL) << "IDeviceImpl::getCapabilities";
  auto capabilities = wrapped_driver_->getCapabilities();
  std::move(callback).Run(capabilities);
}

}  // namespace nn
}  // namespace android
