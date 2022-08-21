// Copyright 2022 The ChromiumOS Authors.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef ML_NN_CHROMEOS_NNAPI_EXECUTION_STUB_H_
#define ML_NN_CHROMEOS_NNAPI_EXECUTION_STUB_H_

#include <base/memory/weak_ptr.h>
#include <mojo/public/cpp/bindings/pending_receiver.h>
#include <mojo/public/cpp/bindings/receiver.h>
#include <mojo/public/cpp/bindings/receiver_set.h>

#include "SharedMemory.h"
#include "aosp/frameworks/ml/chromeos/mojo_driver_canonical/mojom/IExecution.mojom.h"
#include "nnapi/IExecution.h"
#include "nnapi/Types.h"

// These classes are stub implementations of the NNAPI HAL
// interfaces which wrap around a Mojo remote. They are needed
// when passing a Mojo remote into an NNAPI method.

namespace android {
namespace nn {

class ExecutionStub : public IExecution {
 public:
  ExecutionStub(
      mojo::PendingRemote<chromeos::nnapi::canonical::mojom::IExecution> pm)
      : pending_remote_(std::move(pm)) {}

  ExecutionResult<std::pair<std::vector<OutputShape>, Timing>> compute(
      const OptionalTimePoint& deadline) const override;

  GeneralResult<std::pair<SyncFence, ExecuteFencedInfoCallback>> computeFenced(
      const std::vector<SyncFence>& waitFor,
      const OptionalTimePoint& deadline,
      const OptionalDuration& timeoutDurationAfterFence) const override;

 private:
  mutable mojo::PendingRemote<chromeos::nnapi::canonical::mojom::IExecution>
      pending_remote_;
};

}  // namespace nn
}  // namespace android

#endif  // ML_NN_CHROMEOS_NNAPI_EXECUTION_STUB_H_
