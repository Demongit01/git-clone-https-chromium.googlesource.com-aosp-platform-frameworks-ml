// Copyright 2022 The ChromiumOS Authors.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef ML_NN_CHROMEOS_IPC_DRIVER_CANONICAL_H_
#define ML_NN_CHROMEOS_IPC_DRIVER_CANONICAL_H_

#include <nnapi/IDevice.h>

namespace android {
namespace nn {

class IPCDriverCanonical : public IDevice {
 public:
  IPCDriverCanonical(const std::string name, SharedDevice delegate);

  const std::string& getName() const override;
  const std::string& getVersionString() const;
  Version getFeatureLevel() const;
  DeviceType getType() const;
  const std::vector<Extension>& getSupportedExtensions() const;
  const Capabilities& getCapabilities() const;
  std::pair<uint32_t, uint32_t> getNumberOfCacheFilesNeeded() const;
  GeneralResult<void> wait() const;
  GeneralResult<std::vector<bool>> getSupportedOperations(
      const Model& model) const;
  GeneralResult<SharedPreparedModel> prepareModel(
      const Model& model,
      ExecutionPreference preference,
      Priority priority,
      OptionalTimePoint deadline,
      const std::vector<SharedHandle>& modelCache,
      const std::vector<SharedHandle>& dataCache,
      const CacheToken& token,
      const std::vector<nn::TokenValuePair>& hints,
      const std::vector<nn::ExtensionNameAndPrefix>& extensionNameToPrefix)
      const;
  GeneralResult<SharedPreparedModel> prepareModelFromCache(
      OptionalTimePoint deadline,
      const std::vector<SharedHandle>& modelCache,
      const std::vector<SharedHandle>& dataCache,
      const CacheToken& token) const;
  GeneralResult<SharedBuffer> allocate(
      const BufferDesc& desc,
      const std::vector<SharedPreparedModel>& preparedModels,
      const std::vector<BufferRole>& inputRoles,
      const std::vector<BufferRole>& outputRoles) const;

 private:
  const std::string name_;
  SharedDevice delegate_;
};

}  // namespace nn
}  // namespace android

#endif  // ML_NN_CHROMEOS_IPC_DRIVER_CANONICAL_H_
