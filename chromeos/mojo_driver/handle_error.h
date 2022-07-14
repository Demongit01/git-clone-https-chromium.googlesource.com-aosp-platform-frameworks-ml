// Copyright 2022 The ChromiumOS Authors.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef ML_NN_CHROMEOS_MOJO_DRIVER_HANDLE_ERROR_H_
#define ML_NN_CHROMEOS_MOJO_DRIVER_HANDLE_ERROR_H_

namespace chromeos {
namespace nnapi {

#define HANDLE_REMOTE_CALL_FAILURE(re, status, new_status)   \
  if (!(re)) {                                               \
    LOG(ERROR) << "Remote call to the mojo service failed."; \
    status = (new_status);                                   \
  }

}  // namespace nnapi
}  // namespace chromeos

#endif  // ML_NN_CHROMEOS_MOJO_DRIVER_HANDLE_ERROR_H_
