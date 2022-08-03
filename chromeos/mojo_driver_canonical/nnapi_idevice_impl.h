// Copyright 2022 The ChromiumOS Authors.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef ML_NN_CHROMEOS_NNAPI_IDEVICE_IMPL_H_
#define ML_NN_CHROMEOS_NNAPI_IDEVICE_IMPL_H_

#include "aosp/frameworks/ml/chromeos/mojo_driver_canonical/mojom/idevice.mojom.h"

#include <base/task/task_runner.h>
#include <mojo/public/cpp/bindings/pending_receiver.h>
#include <mojo/public/cpp/bindings/receiver.h>
#include <mojo/public/cpp/bindings/receiver_set.h>

// These classes will wrap the "real" HAL driver in the worker process
// and receive the IPC calls from the client IPC driver in the
// main application / NNAPI process. These are the implementation for
// all of the 'remote' interfaces used in the system.

namespace android {
namespace nn {

class IDeviceImpl : public chromeos::nnapi::canonical::mojom::IDevice {
 public:
  IDeviceImpl(SharedDevice underlying_driver)
      : wrapped_driver_(underlying_driver) {}

 private:
  void getCapabilities(getCapabilitiesCallback callback) override;
  void getVersionString(getVersionStringCallback callback) override;
  void getFeatureLevel(getFeatureLevelCallback callback) override;
  void getType(getTypeCallback callback) override;
  void getSupportedExtensions(getSupportedExtensionsCallback callback) override;
  void getNumberOfCacheFilesNeeded(
      getNumberOfCacheFilesNeededCallback callback) override;
  void wait(waitCallback callback) override;

  SharedDevice wrapped_driver_;
};

}  // namespace nn
}  // namespace android

#endif  // ML_NN_CHROMEOS_NNAPI_IDEVICE_IMPL_H_
