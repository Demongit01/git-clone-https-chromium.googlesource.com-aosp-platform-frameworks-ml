// Copyright 2022 The ChromiumOS Authors.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#include "prepared_model_stub.h"

#include <mojo/public/cpp/bindings/self_owned_receiver.h>

#include "logger.h"

namespace android {
namespace nn {

using namespace chromeos::nnapi::canonical;

ExecutionResult<std::pair<std::vector<OutputShape>, Timing>>
PreparedModelStub::execute(
    const Request& request,
    MeasureTiming measure,
    const OptionalTimePoint& deadline,
    const OptionalDuration& loopTimeoutDuration,
    const std::vector<TokenValuePair>& hints,
    const std::vector<ExtensionNameAndPrefix>& extensionNameToPrefix) const {
  VLOG(ML_NN_CHROMEOS_VLOG_LEVEL) << "PreparedModelStub::execute";
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
PreparedModelStub::executeInternal(
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

  auto remote = mojo::Remote<mojom::IPreparedModel>(std::move(pending_remote_));
  android::nn::ExecutionError executionError;
  std::vector<android::nn::OutputShape> outputShape;
  android::nn::Timing timing;
  ExecutionResult<std::pair<std::vector<OutputShape>, Timing>> result;
  remote->execute(request, measure, deadline, loopTimeoutDuration, hints,
                  extensionNameToPrefix, &executionError, &outputShape,
                  &timing);
  pending_remote_ = remote.Unbind();

  if (relocation.output) {
    relocation.output->flush();
  }

  if (ErrorStatus::NONE == executionError.code) {
    return std::pair<std::vector<OutputShape>, Timing>{outputShape, timing};
  }
  return base::unexpected{executionError};
}

GeneralResult<std::pair<SyncFence, ExecuteFencedInfoCallback>>
PreparedModelStub::executeFenced(
    const Request& request,
    const std::vector<SyncFence>& waitFor,
    MeasureTiming measure,
    const OptionalTimePoint& deadline,
    const OptionalDuration& loopTimeoutDuration,
    const OptionalDuration& timeoutDurationAfterFence,
    const std::vector<nn::TokenValuePair>& hints,
    const std::vector<nn::ExtensionNameAndPrefix>& extensionNameToPrefix)
    const {
  VLOG(ML_NN_CHROMEOS_VLOG_LEVEL) << "PreparedModelStub::executeFenced";
  GeneralResult<std::pair<SyncFence, ExecuteFencedInfoCallback>> result =
      base::unexpected{
          GeneralError{"Just for a test", ErrorStatus::GENERAL_FAILURE}};
  return result;
}

GeneralResult<SharedExecution> PreparedModelStub::createReusableExecution(
    const Request& request,
    MeasureTiming measure,
    const OptionalDuration& loopTimeoutDuration,
    const std::vector<nn::TokenValuePair>& hints,
    const std::vector<nn::ExtensionNameAndPrefix>& extensionNameToPrefix)
    const {
  VLOG(ML_NN_CHROMEOS_VLOG_LEVEL)
      << "PreparedModelStub::createReusableExecution";
  return {};
}

GeneralResult<SharedBurst> PreparedModelStub::configureExecutionBurst() const {
  VLOG(ML_NN_CHROMEOS_VLOG_LEVEL)
      << "PreparedModelStub::configureExecutionBurst";
  return {};
}

std::any PreparedModelStub::getUnderlyingResource() const {
  VLOG(ML_NN_CHROMEOS_VLOG_LEVEL) << "PreparedModelStub::getUnderlyingResource";
  return {};
}
}  // namespace nn
}  // namespace android
