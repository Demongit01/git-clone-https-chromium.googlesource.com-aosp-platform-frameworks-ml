// Copyright 2022 The ChromiumOS Authors.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#include "burst_stub.h"

#include <mojo/public/cpp/bindings/self_owned_receiver.h>

#include "execution_stub.h"
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
    const OptionalTimePoint& deadline,
    const OptionalDuration& loopTimeoutDuration,
    const std::vector<TokenValuePair>& hints,
    const std::vector<ExtensionNameAndPrefix>& extensionNameToPrefix) const {
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
  ExecutionError status;
  std::vector<OutputShape> output_shapes;
  Timing timing;
  auto remote_call = [&request, &measure, &deadline, &loopTimeoutDuration,
                      &hints, &extensionNameToPrefix](
                         mojo::Remote<mojom::IBurst>& remote,
                         mojom::IBurst::executeCallback cb) {
    remote->execute(request, measure, deadline, loopTimeoutDuration, hints,
                    extensionNameToPrefix, std::move(cb));
  };
  auto callback = FullOutputCallback<ExecutionError, std::vector<OutputShape>,
                                     Timing, ExecutionError,
                                     const std::vector<OutputShape>&, Timing>;
  HANDLE_REMOTE_CALL_FAILURE(
      CallRemote(task_runner_, remote_, std::move(remote_call),
                 std::move(callback), std::ref(status), std::ref(output_shapes),
                 std::ref(timing)),
      ErrorStatus::DEVICE_UNAVAILABLE);
  if (relocation.output) {
    relocation.output->flush();
  }
  return IS_OK(status.code)
             ? ExecutionResult<
                   std::pair<std::vector<OutputShape>, Timing>>{{output_shapes,
                                                                 timing}}
             : base::unexpected{status};
}

GeneralResult<SharedExecution> BurstStub::createReusableExecution(
    const Request& request,
    MeasureTiming measure,
    const OptionalDuration& loopTimeoutDuration,
    const std::vector<TokenValuePair>& hints,
    const std::vector<ExtensionNameAndPrefix>& extensionNameToPrefix) const {
  VLOG(ML_NN_CHROMEOS_VLOG_LEVEL) << "BurstStub::createReusableExecution";
  std::optional<Request> maybeRequestInShared;
  RequestRelocation relocation;
  const Request& requestInShared = NN_TRY(convertRequestFromPointerToShared(
      &request, kDefaultRequestMemoryAlignment, kDefaultRequestMemoryPadding,
      &maybeRequestInShared, &relocation));
  if (relocation.input) {
    relocation.input->flush();
  }
  GeneralError status;
  ::mojo::PendingRemote<mojom::IExecution> execution;
  auto remote_call = [&requestInShared, &measure, &loopTimeoutDuration, &hints,
                      &extensionNameToPrefix](
                         mojo::Remote<mojom::IBurst>& remote,
                         mojom::IBurst::createReusableExecutionCallback cb) {
    remote->createReusableExecution(requestInShared, measure,
                                    loopTimeoutDuration, hints,
                                    extensionNameToPrefix, std::move(cb));
  };
  auto callback =
      DefaultOutputCallback<GeneralError,
                            ::mojo::PendingRemote<mojom::IExecution>>;
  HANDLE_REMOTE_CALL_FAILURE(
      CallRemote(task_runner_, remote_, std::move(remote_call),
                 std::move(callback), std::ref(status), std::ref(execution)),
      ErrorStatus::DEVICE_UNAVAILABLE);
  VLOG(ML_NN_CHROMEOS_VLOG_LEVEL)
      << "BurstStub::createReusableExecution finished.";
  return IS_OK(status.code)
             ? GeneralResult<SharedExecution>{new ExecutionStub(
                   std::move(execution), task_runner_, std::move(relocation))}
             : base::unexpected{status};
}

}  // namespace nn
}  // namespace android
