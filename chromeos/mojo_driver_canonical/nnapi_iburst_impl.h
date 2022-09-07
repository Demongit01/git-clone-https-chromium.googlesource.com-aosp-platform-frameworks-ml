// Copyright 2022 The ChromiumOS Authors.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef ML_NN_CHROMEOS_NNAPI_IBURST_IMPL_H_
#define ML_NN_CHROMEOS_NNAPI_IBURST_IMPL_H_

#include "aosp/frameworks/ml/chromeos/mojo_driver_canonical/mojom/IBurst.mojom.h"

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

class IBurstImpl : public chromeos::nnapi::canonical::mojom::IBurst {
 public:
  IBurstImpl(SharedBurst burst) : wrapped_burst_(burst) {}

  void execute(Request request,
               MeasureTiming measure,
               absl::optional<TimePoint> deadline,
               absl::optional<Duration> loopTimeoutDuration,
               const std::vector<TokenValuePair>& hints,
               const std::vector<ExtensionNameAndPrefix>& extensionNameToPrefix,
               executeCallback callback) override;

  void createReusableExecution(
      Request request,
      MeasureTiming measure,
      absl::optional<Duration> loopTimeoutDuration,
      const std::vector<TokenValuePair>& hints,
      const std::vector<ExtensionNameAndPrefix>& extensionNameToPrefix,
      createReusableExecutionCallback callback) override;

 private:
  SharedBurst wrapped_burst_;
};

}  // namespace nn
}  // namespace android

#endif  // ML_NN_CHROMEOS_NNAPI_IBURST_IMPL_H_
