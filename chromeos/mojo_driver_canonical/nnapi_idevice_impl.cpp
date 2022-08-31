// Copyright 2022 The ChromiumOS Authors.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#include "nnapi_idevice_impl.h"

#include <mojo/public/cpp/bindings/self_owned_receiver.h>

#include "handle_error_canonical.h"
#include "logger.h"
#include "nnapi_iprepared_model_impl.h"

namespace android {
namespace nn {

using namespace chromeos::nnapi::canonical;

const GeneralError kGeneralErrorNone = GeneralError{"", ErrorStatus::NONE};

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
  HANDLE_ERROR_RESULT(result, callback);
  std::move(callback).Run(kGeneralErrorNone);
}

void IDeviceImpl::getSupportedOperations(
    Model model,
    getSupportedOperationsCallback callback) {
  VLOG(ML_NN_CHROMEOS_VLOG_LEVEL) << "IDeviceImpl::getSupportedOperations";
  auto result = wrapped_driver_->getSupportedOperations(model);
  HANDLE_ERROR_RESULT(result, callback, {});
  std::move(callback).Run(kGeneralErrorNone, result.value());
}

void IDeviceImpl::prepareModel(
    Model model,
    ExecutionPreference preference,
    Priority priority,
    absl::optional<TimePoint> deadline,
    const std::vector<SharedHandle>& modelCache,
    const std::vector<SharedHandle>& dataCache,
    CacheToken token,
    const std::vector<TokenValuePair>& hints,
    const std::vector<ExtensionNameAndPrefix>& extensionNameToPrefix,
    prepareModelCallback callback) {
  VLOG(ML_NN_CHROMEOS_VLOG_LEVEL) << "IDeviceImpl::prepareModel";
  auto result = wrapped_driver_->prepareModel(
      model, preference, priority, deadline, modelCache, dataCache, token,
      hints, extensionNameToPrefix);
  mojo::PendingRemote<mojom::IPreparedModel> pm_remote;
  if (!result.ok()) {
    mojo::MakeSelfOwnedReceiver(std::make_unique<IPreparedModelImpl>(nullptr),
                                pm_remote.InitWithNewPipeAndPassReceiver());
    std::move(callback).Run(result.error(), std::move(pm_remote));
    return;
  }
  mojo::MakeSelfOwnedReceiver(
      std::make_unique<IPreparedModelImpl>(result.value()),
      pm_remote.InitWithNewPipeAndPassReceiver());
  std::move(callback).Run(kGeneralErrorNone, std::move(pm_remote));
  VLOG(ML_NN_CHROMEOS_VLOG_LEVEL) << "IDeviceImpl::prepareModel finished";
}

void IDeviceImpl::prepareModelFromCache(
    absl::optional<TimePoint> deadline,
    const std::vector<SharedHandle>& modelCache,
    const std::vector<SharedHandle>& dataCache,
    CacheToken token,
    prepareModelFromCacheCallback callback) {
  VLOG(ML_NN_CHROMEOS_VLOG_LEVEL) << "IDeviceImpl::prepareModelFromCache";
  auto result = wrapped_driver_->prepareModelFromCache(deadline, modelCache,
                                                       dataCache, token);
  mojo::PendingRemote<mojom::IPreparedModel> pm_remote;
  if (!result.ok()) {
    mojo::MakeSelfOwnedReceiver(std::make_unique<IPreparedModelImpl>(nullptr),
                                pm_remote.InitWithNewPipeAndPassReceiver());
    std::move(callback).Run(result.error(), std::move(pm_remote));
    return;
  }
  mojo::MakeSelfOwnedReceiver(
      std::make_unique<IPreparedModelImpl>(result.value()),
      pm_remote.InitWithNewPipeAndPassReceiver());
  std::move(callback).Run(kGeneralErrorNone, std::move(pm_remote));
  VLOG(ML_NN_CHROMEOS_VLOG_LEVEL) << "IDeviceImpl::prepareModelFromCache finished";
}

}  // namespace nn
}  // namespace android
