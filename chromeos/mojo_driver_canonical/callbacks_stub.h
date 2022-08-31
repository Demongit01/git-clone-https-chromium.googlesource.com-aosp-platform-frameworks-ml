// Copyright 2022 The ChromiumOS Authors.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef ML_NN_CHROMEOS_NNAPI_CALLBACKS_STUB_H_
#define ML_NN_CHROMEOS_NNAPI_CALLBACKS_STUB_H_

#include <base/memory/weak_ptr.h>
#include <mojo/public/cpp/bindings/pending_receiver.h>
#include <mojo/public/cpp/bindings/receiver.h>
#include <mojo/public/cpp/bindings/receiver_set.h>

#include "aosp/frameworks/ml/chromeos/mojo_driver_canonical/mojom/Types.mojom.h"
#include "remote_call.h"

// These classes are stub implementations of the NNAPI HAL
// interfaces which wrap around a Mojo remote. They are needed
// when passing a Mojo remote into an NNAPI method.

namespace android {
namespace nn {

class ExecuteFencedInfoCallbackStub
    : public HasRemote<
          chromeos::nnapi::canonical::mojom::IExecuteFencedInfoCallback> {
 public:
  ExecuteFencedInfoCallbackStub(
      mojo::PendingRemote<
          chromeos::nnapi::canonical::mojom::IExecuteFencedInfoCallback>
          pending_remote,
      scoped_refptr<::base::SequencedTaskRunner>& task_runner)
      : HasRemote{std::move(pending_remote), task_runner} {};

  GeneralResult<std::pair<Timing, Timing>> getExecuteFencedInfo();
};

}  // namespace nn
}  // namespace android

#endif  // ML_NN_CHROMEOS_NNAPI_CALLBACKS_STUB_H_
