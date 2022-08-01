// Copyright 2022 The ChromiumOS Authors.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef ML_NN_CHROMEOS_MOJO_CONTROLLER_CANONICAL_H_
#define ML_NN_CHROMEOS_MOJO_CONTROLLER_CANONICAL_H_

#include <base/threading/thread.h>
#include <mojo/core/embedder/scoped_ipc_support.h>
#include <mojo/public/cpp/bindings/remote.h>
#include <mojo/public/cpp/platform/platform_channel.h>
#include <nnapi/IDevice.h>

#include "aosp/frameworks/ml/chromeos/mojo_driver_canonical/mojom/idevice.mojom.h"

namespace android {
namespace nn {

class MojoThread;

// This class contains the Mojo implementations
// of the V1_3::IDevice interface. Ideally we would fold
// this into the IPCDriver class, but this isn't
// possible due to the incompatability of ChromeOS
// and Android headers. This class is also responsible
// for spawning the remote HAL driver process, which is
// done upon construction.
class MojoControllerCanonical {
 public:
  MojoControllerCanonical(const std::string service_name = "default");

  Capabilities getCapabilities();

 private:
  void SendMojoInvitationAndGetRemote(pid_t child_pid,
                                      mojo::PlatformChannel channel,
                                      std::string pipe_name);
  bool SpawnWorkerProcessAndGetPid(const mojo::PlatformChannel& channel,
                                   std::string pipe_name,
                                   pid_t* worker_pid);

  const std::string service_name_;
  scoped_refptr<::base::SequencedTaskRunner> ipc_task_runner_;
  mojo::Remote<chromeos::nnapi::canonical::mojom::IDevice> remote_;
};

}  // namespace nn
}  // namespace android

#endif  // ML_NN_CHROMEOS_MOJO_CONTROLLER_CANONICAL_H_
