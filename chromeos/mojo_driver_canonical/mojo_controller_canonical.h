// Copyright 2022 The ChromiumOS Authors.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef ML_NN_CHROMEOS_MOJO_CONTROLLER_CANONICAL_H_
#define ML_NN_CHROMEOS_MOJO_CONTROLLER_CANONICAL_H_

#include <base/threading/thread.h>
#include <mojo/core/embedder/scoped_ipc_support.h>
#include <mojo/public/cpp/bindings/remote.h>
#include <mojo/public/cpp/platform/platform_channel.h>
#include <nnapi/IDevice.h>

#include "aosp/frameworks/ml/chromeos/mojo_driver_canonical/mojom/IDevice.mojom.h"

namespace android {
namespace nn {

class MojoThread;

// This class contains the Mojo implementations
// of the V1_3::IDevice interface. Ideally we would fold
// this into the IPCDriver class, but this isn't
// possible due to the incompatability of ChromeOS
// and Android headers. This class is also responsible
// for spawning the remote HAL driver process, which is
// done upon construction.
class MojoControllerCanonical {
 public:
  MojoControllerCanonical(
      scoped_refptr<::base::SequencedTaskRunner> task_runner,
      const std::string service_name = "default");
  ~MojoControllerCanonical();

  Capabilities getCapabilities();
  std::string getVersionString();
  Version getFeatureLevel();
  DeviceType getType();
  std::vector<Extension> getSupportedExtensions();
  std::pair<uint32_t, uint32_t> getNumberOfCacheFilesNeeded();
  GeneralResult<void> wait();
  GeneralResult<std::vector<bool>> getSupportedOperations(const Model& model);
  GeneralResult<SharedPreparedModel> prepareModel(
      const Model& model,
      ExecutionPreference preference,
      Priority priority,
      OptionalTimePoint deadline,
      const std::vector<SharedHandle>& modelCache,
      const std::vector<SharedHandle>& dataCache,
      const CacheToken& token,
      const std::vector<nn::TokenValuePair>& hints,
      const std::vector<nn::ExtensionNameAndPrefix>& extensionNameToPrefix);
  GeneralResult<SharedPreparedModel> prepareModelFromCache(
      OptionalTimePoint deadline,
      const std::vector<SharedHandle>& modelCache,
      const std::vector<SharedHandle>& dataCache,
      const CacheToken& token);

 private:
  void SendMojoInvitationAndGetRemote(pid_t child_pid,
                                      mojo::PlatformChannel channel,
                                      std::string pipe_name);
  bool SpawnWorkerProcessAndGetPid(const mojo::PlatformChannel& channel,
                                   std::string pipe_name,
                                   pid_t* worker_pid);

  scoped_refptr<::base::SequencedTaskRunner> task_runner_;
  const std::string service_name_;
  mojo::Remote<chromeos::nnapi::canonical::mojom::IDevice> remote_;
};

}  // namespace nn
}  // namespace android

#endif  // ML_NN_CHROMEOS_MOJO_CONTROLLER_CANONICAL_H_
