// Copyright 2022 The ChromiumOS Authors.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#include "burst_stub.h"

#include <mojo/public/cpp/bindings/self_owned_receiver.h>

#include "handle_error_canonical.h"
#include "logger.h"

namespace android {
namespace nn {

using namespace chromeos::nnapi::canonical;

IBurst::OptionalCacheHold BurstStub::cacheMemory(
    const SharedMemory& memory) const {
  return nullptr;
}

ExecutionResult<std::pair<std::vector<OutputShape>, Timing>> BurstStub::execute(
    const Request& request,
    MeasureTiming measure,
    const nn::OptionalTimePoint& deadline,
    const nn::OptionalDuration& loopTimeoutDuration,
    const std::vector<nn::TokenValuePair>& hints,
    const std::vector<nn::ExtensionNameAndPrefix>& extensionNameToPrefix)
    const {
  VLOG(ML_NN_CHROMEOS_VLOG_LEVEL) << "BurstStub::execute";
  // Ensure that request is ready for IPC.
  std::optional<Request> maybeRequestInShared;
  RequestRelocation relocation;
  const Request& requestInShared = NN_TRY(convertRequestFromPointerToShared(
      &request, kDefaultRequestMemoryAlignment, kDefaultRequestMemoryPadding,
      &maybeRequestInShared, &relocation));
  return executeInternal(requestInShared, measure, deadline,
                         loopTimeoutDuration, hints, extensionNameToPrefix,
                         relocation);
}

ExecutionResult<std::pair<std::vector<OutputShape>, Timing>>
BurstStub::executeInternal(
    const Request& request,
    MeasureTiming measure,
    const OptionalTimePoint& deadline,
    const OptionalDuration& loopTimeoutDuration,
    const std::vector<TokenValuePair>& hints,
    const std::vector<ExtensionNameAndPrefix>& extensionNameToPrefix,
    const RequestRelocation& relocation) const {
  if (relocation.input) {
    relocation.input->flush();
  }
  auto remote = mojo::Remote<mojom::IBurst>(std::move(pending_remote_));
  android::nn::ExecutionError status;
  std::vector<android::nn::OutputShape> outputShape;
  android::nn::Timing timing;
  HANDLE_REMOTE_CALL_FAILURE(
      remote->execute(request, measure, deadline, loopTimeoutDuration, hints,
                      extensionNameToPrefix, &status, &outputShape, &timing),
      ErrorStatus::DEVICE_UNAVAILABLE);
  pending_remote_ = remote.Unbind();
  if (relocation.output) {
    relocation.output->flush();
  }
  if (!IS_OK(status.code)) {
    return base::unexpected{status};
  }
  return std::pair<std::vector<OutputShape>, Timing>{outputShape, timing};
}

GeneralResult<SharedExecution> BurstStub::createReusableExecution(
    const Request& request,
    MeasureTiming measure,
    const nn::OptionalDuration& loopTimeoutDuration,
    const std::vector<nn::TokenValuePair>& hints,
    const std::vector<nn::ExtensionNameAndPrefix>& extensionNameToPrefix)
    const {
  return NN_ERROR() << "IBurst::createReusableExecution is not supported!";
}

}  // namespace nn
}  // namespace android
