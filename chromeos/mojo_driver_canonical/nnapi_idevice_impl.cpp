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

void IDeviceImpl::getVersionString(getVersionStringCallback callback) {
  VLOG(ML_NN_CHROMEOS_VLOG_LEVEL) << "IDeviceImpl::getVersionString";
  auto versionString = wrapped_driver_->getVersionString();
  std::move(callback).Run(versionString);
}

void IDeviceImpl::getFeatureLevel(getFeatureLevelCallback callback) {
  VLOG(ML_NN_CHROMEOS_VLOG_LEVEL) << "IDeviceImpl::getFeatureLevel";
  auto version = wrapped_driver_->getFeatureLevel();
  std::move(callback).Run(version);
}

void IDeviceImpl::getType(getTypeCallback callback) {
  VLOG(ML_NN_CHROMEOS_VLOG_LEVEL) << "IDeviceImpl::getType";
  auto type = wrapped_driver_->getType();
  std::move(callback).Run(type);
}

void IDeviceImpl::getSupportedExtensions(
    getSupportedExtensionsCallback callback) {
  VLOG(ML_NN_CHROMEOS_VLOG_LEVEL) << "IDeviceImpl::getSupportedExtensions";
  auto extensions = wrapped_driver_->getSupportedExtensions();
  std::move(callback).Run(extensions);
}

void IDeviceImpl::getNumberOfCacheFilesNeeded(
    getNumberOfCacheFilesNeededCallback callback) {
  VLOG(ML_NN_CHROMEOS_VLOG_LEVEL) << "IDeviceImpl::getNumberOfCacheFilesNeeded";
  auto nums = wrapped_driver_->getNumberOfCacheFilesNeeded();
  std::move(callback).Run(nums.first, nums.second);
}

void IDeviceImpl::wait(waitCallback callback) {
  VLOG(ML_NN_CHROMEOS_VLOG_LEVEL) << "IDeviceImpl::wait";
  auto result = wrapped_driver_->wait();
  if (result.ok()) {
    std::move(callback).Run(
        GeneralError{"wait successfully", ErrorStatus::NONE});
  } else {
    std::move(callback).Run(result.error());
  }
}

}  // namespace nn
}  // namespace android
