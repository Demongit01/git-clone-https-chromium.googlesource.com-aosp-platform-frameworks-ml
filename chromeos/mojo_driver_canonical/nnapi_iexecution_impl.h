// Copyright 2022 The ChromiumOS Authors.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef ML_NN_CHROMEOS_NNAPI_IEXECUTION_IMPL_H_
#define ML_NN_CHROMEOS_NNAPI_IEXECUTION_IMPL_H_

#include "aosp/frameworks/ml/chromeos/mojo_driver_canonical/mojom/IExecution.mojom.h"

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

class IExecutionImpl : public chromeos::nnapi::canonical::mojom::IExecution {
 public:
  IExecutionImpl(SharedExecution execution) : wrapped_execution_(execution) {}

  void compute(absl::optional<TimePoint> deadline,
               computeCallback callback) override;
  void computeFenced(std::vector<SyncFence> waitFor,
                     absl::optional<TimePoint> deadline,
                     absl::optional<Duration> timeoutDurationAfterFence,
                     computeFencedCallback callback) override;

 private:
  SharedExecution wrapped_execution_;
};

}  // namespace nn
}  // namespace android

#endif  // ML_NN_CHROMEOS_NNAPI_IEXECUTION_IMPL_H_
