// Copyright 2022 The ChromiumOS Authors.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef ML_NN_CHROMEOS_MOJO_DRIVER_HANDLE_ERROR_CANONICAL_H_
#define ML_NN_CHROMEOS_MOJO_DRIVER_HANDLE_ERROR_CANONICAL_H_

namespace chromeos {
namespace nnapi {

#define HANDLE_REMOTE_CALL_FAILURE(re, status)                     \
  if (!(re)) {                                                     \
    LOG(ERROR) << "Remote call to the mojo service failed.";       \
    NN_ERROR(status) << "Remote call to the mojo service failed."; \
  }

#define LOG_REMOTE_CALL_FAILURE(re)                          \
  if (!(re)) {                                               \
    LOG(ERROR) << "Remote call to the mojo service failed."; \
  }

#define IS_OK(status) ((status) == android::nn::ErrorStatus::NONE)

#define HANDLE_ERROR_RESULT(re, callback, ...)          \
  if (!(re).ok()) {                                     \
    std::move(callback).Run(re.error(), ##__VA_ARGS__); \
    return;                                             \
  }

}  // namespace nnapi
}  // namespace chromeos

#endif  // ML_NN_CHROMEOS_MOJO_DRIVER_HANDLE_ERROR_CANONICAL_H_
