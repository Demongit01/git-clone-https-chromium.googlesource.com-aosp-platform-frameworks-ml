// Copyright 2022 The ChromiumOS Authors.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef ML_NN_CHROMEOS_NNAPI_WRAPPERS_H_
#define ML_NN_CHROMEOS_NNAPI_WRAPPERS_H_

#include <base/memory/weak_ptr.h>
#include <mojo/public/cpp/bindings/pending_receiver.h>
#include <mojo/public/cpp/bindings/receiver.h>
#include <mojo/public/cpp/bindings/receiver_set.h>

#include "SharedMemory.h"
#include "aosp/frameworks/ml/chromeos/mojo_driver_canonical/mojom/IPreparedModel.mojom.h"
#include "nnapi/IPreparedModel.h"

// These classes are stub implementations of the NNAPI HAL
// interfaces which wrap around a Mojo remote. They are needed
// when passing a Mojo remote into an NNAPI method.

namespace android {
namespace nn {

class HasSequencedTaskRunner {
 public:
  HasSequencedTaskRunner()
      : task_runner_{::base::SequencedTaskRunnerHandle::Get()} {}

 protected:
  scoped_refptr<::base::SequencedTaskRunner> task_runner_;
};

class PreparedModelStub : public IPreparedModel, public HasSequencedTaskRunner {
 public:
  PreparedModelStub(
      mojo::PendingRemote<chromeos::nnapi::canonical::mojom::IPreparedModel> pm)
      : pending_remote_(std::move(pm)) {}

  ExecutionResult<std::pair<std::vector<OutputShape>, Timing>> execute(
      const Request& request,
      MeasureTiming measure,
      const OptionalTimePoint& deadline,
      const OptionalDuration& loopTimeoutDuration,
      const std::vector<TokenValuePair>& hints,
      const std::vector<ExtensionNameAndPrefix>& extensionNameToPrefix)
      const override;

  ExecutionResult<std::pair<std::vector<OutputShape>, Timing>> executeInternal(
      const Request& request,
      MeasureTiming measure,
      const OptionalTimePoint& deadline,
      const OptionalDuration& loopTimeoutDuration,
      const std::vector<nn::TokenValuePair>& hints,
      const std::vector<nn::ExtensionNameAndPrefix>& extensionNameToPrefix,
      const RequestRelocation& relocation) const;

  GeneralResult<std::pair<SyncFence, ExecuteFencedInfoCallback>> executeFenced(
      const Request& request,
      const std::vector<SyncFence>& waitFor,
      MeasureTiming measure,
      const OptionalTimePoint& deadline,
      const OptionalDuration& loopTimeoutDuration,
      const OptionalDuration& timeoutDurationAfterFence,
      const std::vector<nn::TokenValuePair>& hints,
      const std::vector<nn::ExtensionNameAndPrefix>& extensionNameToPrefix)
      const override;

  GeneralResult<SharedExecution> createReusableExecution(
      const Request& request,
      MeasureTiming measure,
      const OptionalDuration& loopTimeoutDuration,
      const std::vector<nn::TokenValuePair>& hints,
      const std::vector<nn::ExtensionNameAndPrefix>& extensionNameToPrefix)
      const override;

  GeneralResult<SharedBurst> configureExecutionBurst() const override;

  std::any getUnderlyingResource() const override;

 private:
  mutable mojo::PendingRemote<chromeos::nnapi::canonical::mojom::IPreparedModel>
      pending_remote_;
};

}  // namespace nn
}  // namespace android

#endif  // ML_NN_CHROMEOS_NNAPI_WRAPPERS_H_
