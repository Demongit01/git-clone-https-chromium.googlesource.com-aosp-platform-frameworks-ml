// Copyright 2022 The ChromiumOS Authors.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef ML_NN_CHROMEOS_NNAPI_PREPARED_MODEL_STUB_H_
#define ML_NN_CHROMEOS_NNAPI_PREPARED_MODEL_STUB_H_

#include <base/memory/weak_ptr.h>
#include <mojo/public/cpp/bindings/pending_receiver.h>
#include <mojo/public/cpp/bindings/receiver.h>
#include <mojo/public/cpp/bindings/receiver_set.h>

#include "SharedMemory.h"
#include "aosp/frameworks/ml/chromeos/mojo_driver_canonical/mojom/IPreparedModel.mojom.h"
#include "nnapi/IPreparedModel.h"
#include "remote_call.h"

// These classes are stub implementations of the NNAPI HAL
// interfaces which wrap around a Mojo remote. They are needed
// when passing a Mojo remote into an NNAPI method.

namespace android {
namespace nn {

class PreparedModelStub
    : public IPreparedModel,
      public HasRemote<chromeos::nnapi::canonical::mojom::IPreparedModel> {
 public:
  PreparedModelStub(
      mojo::PendingRemote<chromeos::nnapi::canonical::mojom::IPreparedModel>
          pending_remote,
      scoped_refptr<::base::SequencedTaskRunner>& task_runner)
      : HasRemote(std::move(pending_remote), task_runner) {}

  ExecutionResult<std::pair<std::vector<OutputShape>, Timing>> execute(
      const Request& request,
      MeasureTiming measure,
      const OptionalTimePoint& deadline,
      const OptionalDuration& loopTimeoutDuration,
      const std::vector<TokenValuePair>& hints,
      const std::vector<ExtensionNameAndPrefix>& extensionNameToPrefix)
      const override;

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
  ExecutionResult<std::pair<std::vector<OutputShape>, Timing>> executeInternal(
      const Request& request,
      MeasureTiming measure,
      const OptionalTimePoint& deadline,
      const OptionalDuration& loopTimeoutDuration,
      const std::vector<nn::TokenValuePair>& hints,
      const std::vector<nn::ExtensionNameAndPrefix>& extensionNameToPrefix,
      const RequestRelocation& relocation) const;

  GeneralResult<std::pair<SyncFence, ExecuteFencedInfoCallback>>
  executeFencedInternal(
      const Request& request,
      const std::vector<SyncFence>& waitFor,
      MeasureTiming measure,
      const OptionalTimePoint& deadline,
      const OptionalDuration& loopTimeoutDuration,
      const OptionalDuration& timeoutDurationAfterFence,
      const std::vector<TokenValuePair>& hints,
      const std::vector<ExtensionNameAndPrefix>& extensionNameToPrefix,
      const RequestRelocation& relocation) const;
};

}  // namespace nn
}  // namespace android

#endif  // ML_NN_CHROMEOS_NNAPI_PREPARED_MODEL_STUB_H_
