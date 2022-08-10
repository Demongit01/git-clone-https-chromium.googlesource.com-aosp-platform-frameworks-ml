// Copyright 2022 The ChromiumOS Authors.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#include "prepared_model_stub.h"

#include <mojo/public/cpp/bindings/self_owned_receiver.h>

#include "handle_error_canonical.h"
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
  std::optional<Request> maybeRequestInShared;
  RequestRelocation relocation;
  const Request& requestInShared = NN_TRY(convertRequestFromPointerToShared(
      &request, kDefaultRequestMemoryAlignment, kDefaultRequestMemoryPadding,
      &maybeRequestInShared, &relocation));
  return executeFencedInternal(requestInShared, waitFor, measure, deadline,
                               loopTimeoutDuration, timeoutDurationAfterFence,
                               hints, extensionNameToPrefix, relocation);
}

GeneralResult<std::pair<SyncFence, ExecuteFencedInfoCallback>>
PreparedModelStub::executeFencedInternal(
    const Request& request,
    const std::vector<SyncFence>& waitFor,
    MeasureTiming measure,
    const OptionalTimePoint& deadline,
    const OptionalDuration& loopTimeoutDuration,
    const OptionalDuration& timeoutDurationAfterFence,
    const std::vector<TokenValuePair>& hints,
    const std::vector<ExtensionNameAndPrefix>& extensionNameToPrefix,
    const RequestRelocation& relocation) const {
  if (relocation.input) {
    relocation.input->flush();
  }
  auto remote = mojo::Remote<mojom::IPreparedModel>(std::move(pending_remote_));
  android::nn::GeneralError status;
  android::nn::SyncFence syncFence;
  ::mojo::PendingRemote<
      chromeos::nnapi::canonical::mojom::IExecuteFencedInfoCallback>
      callback;
  HANDLE_REMOTE_CALL_FAILURE(
      remote->executeFenced(request, waitFor, measure, deadline,
                            loopTimeoutDuration, timeoutDurationAfterFence,
                            hints, extensionNameToPrefix, &status, &syncFence,
                            &callback),
      ErrorStatus::DEVICE_UNAVAILABLE);
  pending_remote_ = remote.Unbind();
  if (relocation.output) {
    relocation.output->flush();
  }
  if (!IS_OK(status.code)) {
    return base::unexpected{status};
  }
  ExecuteFencedInfoCallback callback_stub =
      [&callback]() -> GeneralResult<std::pair<Timing, Timing>> {
    auto remote = mojo::Remote<
        chromeos::nnapi::canonical::mojom::IExecuteFencedInfoCallback>(
        std::move(callback));
    android::nn::GeneralError status;
    android::nn::Timing timingLaunched;
    android::nn::Timing timingFenced;
    HANDLE_REMOTE_CALL_FAILURE(
        remote->getExecuteFencedInfo(&status, &timingLaunched, &timingFenced),
        ErrorStatus::DEVICE_UNAVAILABLE);
    if (!IS_OK(status.code)) {
      return base::unexpected{status};
    }
    return std::pair<Timing, Timing>{timingLaunched, timingFenced};
  };
  return std::pair<SyncFence, ExecuteFencedInfoCallback>{syncFence,
                                                         callback_stub};
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
