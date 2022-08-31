// Copyright 2022 The ChromiumOS Authors.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#include "callbacks_stub.h"

#include <mojo/public/cpp/bindings/self_owned_receiver.h>

#include "handle_error_canonical.h"
#include "logger.h"

namespace android {
namespace nn {

using namespace chromeos::nnapi::canonical;

GeneralResult<std::pair<Timing, Timing>>
ExecuteFencedInfoCallbackStub::getExecuteFencedInfo() {
  VLOG(ML_NN_CHROMEOS_VLOG_LEVEL) << "ExecuteFencedInfoCallbackStub started.";
  GeneralError status;
  Timing timingLaunched;
  Timing timingFenced;
  auto remote_call =
      [](mojo::Remote<mojom::IExecuteFencedInfoCallback>& remote,
         mojom::IExecuteFencedInfoCallback::getExecuteFencedInfoCallback cb) {
        remote->getExecuteFencedInfo(std::move(cb));
      };
  auto callback = DefaultOutputCallback<GeneralError, Timing, Timing>;
  HANDLE_REMOTE_CALL_FAILURE(
      CallRemote(task_runner_, remote_, std::move(remote_call),
                 std::move(callback), std::ref(status),
                 std::ref(timingLaunched), std::ref(timingFenced)),
      ErrorStatus::DEVICE_UNAVAILABLE);
  VLOG(ML_NN_CHROMEOS_VLOG_LEVEL) << "ExecuteFencedInfoCallbackStub finished.";
  return IS_OK(status.code)
             ? GeneralResult<std::pair<Timing, Timing>>{{timingLaunched,
                                                         timingFenced}}
             : base::unexpected{status};
}

}  // namespace nn
}  // namespace android
