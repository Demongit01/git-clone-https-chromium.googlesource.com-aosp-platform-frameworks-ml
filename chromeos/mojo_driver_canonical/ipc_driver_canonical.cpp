// Copyright 2022 The ChromiumOS Authors.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#include "ipc_driver_canonical.h"

#include "mojo_controller_canonical.h"

namespace android {
namespace nn {

IPCDriverCanonical::IPCDriverCanonical(const std::string name,
                                       SharedDevice delegate)
    : name_{name}, delegate_(delegate) {
  mojo_ = std::make_unique<MojoControllerCanonical>(name);
}

const std::string& IPCDriverCanonical::getName() const {
  return name_;
}

const std::string& IPCDriverCanonical::getVersionString() const {
  return delegate_->getVersionString();
}

Version IPCDriverCanonical::getFeatureLevel() const {
  return delegate_->getFeatureLevel();
}

DeviceType IPCDriverCanonical::getType() const {
  return delegate_->getType();
}

const std::vector<Extension>& IPCDriverCanonical::getSupportedExtensions()
    const {
  return delegate_->getSupportedExtensions();
}

const Capabilities& IPCDriverCanonical::getCapabilities() const {
  const static Capabilities capabilities = mojo_->getCapabilities();
  return capabilities;
}

std::pair<uint32_t, uint32_t> IPCDriverCanonical::getNumberOfCacheFilesNeeded()
    const {
  return delegate_->getNumberOfCacheFilesNeeded();
}

GeneralResult<void> IPCDriverCanonical::wait() const {
  return delegate_->wait();
}

GeneralResult<std::vector<bool>> IPCDriverCanonical::getSupportedOperations(
    const Model& model) const {
  return delegate_->getSupportedOperations(model);
}

GeneralResult<SharedPreparedModel> IPCDriverCanonical::prepareModel(
    const Model& model,
    ExecutionPreference preference,
    Priority priority,
    OptionalTimePoint deadline,
    const std::vector<SharedHandle>& modelCache,
    const std::vector<SharedHandle>& dataCache,
    const CacheToken& token,
    const std::vector<nn::TokenValuePair>& hints,
    const std::vector<nn::ExtensionNameAndPrefix>& extensionNameToPrefix)
    const {
  return delegate_->prepareModel(model, preference, priority, deadline,
                                 modelCache, dataCache, token, hints,
                                 extensionNameToPrefix);
}

GeneralResult<SharedPreparedModel> IPCDriverCanonical::prepareModelFromCache(
    OptionalTimePoint deadline,
    const std::vector<SharedHandle>& modelCache,
    const std::vector<SharedHandle>& dataCache,
    const CacheToken& token) const {
  return delegate_->prepareModelFromCache(deadline, modelCache, dataCache,
                                          token);
}

GeneralResult<SharedBuffer> IPCDriverCanonical::allocate(
    const BufferDesc& desc,
    const std::vector<SharedPreparedModel>& preparedModels,
    const std::vector<BufferRole>& inputRoles,
    const std::vector<BufferRole>& outputRoles) const {
  return delegate_->allocate(desc, preparedModels, inputRoles, outputRoles);
}

}  // namespace nn
}  // namespace android
