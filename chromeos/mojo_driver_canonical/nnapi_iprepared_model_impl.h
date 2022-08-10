// Copyright 2022 The ChromiumOS Authors.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef ML_NN_CHROMEOS_NNAPI_IPREPARED_MODEL_IMPL_H_
#define ML_NN_CHROMEOS_NNAPI_IPREPARED_MODEL_IMPL_H_

#include "aosp/frameworks/ml/chromeos/mojo_driver_canonical/mojom/IPreparedModel.mojom.h"

#include <base/task/task_runner.h>
#include <mojo/public/cpp/bindings/pending_receiver.h>
#include <mojo/public/cpp/bindings/receiver.h>
#include <mojo/public/cpp/bindings/receiver_set.h>

#include "nnapi/Types.h"
#include "utils/StrongPointer.h"

// These classes will wrap the "real" HAL driver in the worker process
// and receive the IPC calls from the client IPC driver in the
// main application / NNAPI process. These are the implementation for
// all of the 'remote' interfaces used in the system.

namespace android {
namespace nn {

class IPreparedModelImpl
    : public chromeos::nnapi::canonical::mojom::IPreparedModel {
 public:
  IPreparedModelImpl(SharedPreparedModel preparedModel)
      : wrapped_model_(preparedModel) {}

  void execute(Request request,
               MeasureTiming measure,
               absl::optional<TimePoint> deadline,
               absl::optional<Duration> loopTimeoutDuration,
               const std::vector<TokenValuePair>& hints,
               const std::vector<ExtensionNameAndPrefix>& extensionNameToPrefix,
               executeCallback callback) override;

  void executeFenced(
      Request request,
      std::vector<SyncFence> waitFor,
      MeasureTiming measure,
      absl::optional<TimePoint> deadline,
      absl::optional<Duration> loopTimeoutDuration,
      absl::optional<Duration> timeoutDurationAfterFence,
      const std::vector<TokenValuePair>& hints,
      const std::vector<ExtensionNameAndPrefix>& extensionNameToPrefix,
      executeFencedCallback callback) override;

 private:
  SharedPreparedModel wrapped_model_;
};

class IExecuteFencedInfoCallbackImpl
    : public chromeos::nnapi::canonical::mojom::IExecuteFencedInfoCallback {
 public:
  IExecuteFencedInfoCallbackImpl(ExecuteFencedInfoCallback callback)
      : wrapped_callback_(callback) {}

  void getExecuteFencedInfo(getExecuteFencedInfoCallback callback);

 private:
  ExecuteFencedInfoCallback wrapped_callback_;
};

}  // namespace nn
}  // namespace android

#endif  // ML_NN_CHROMEOS_NNAPI_IPREPARED_MODEL_IMPL_H_
