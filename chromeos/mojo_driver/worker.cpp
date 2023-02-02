// Copyright 2022 The Chromium OS Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#include <base/logging.h>
#include <base/task/single_thread_task_runner.h>
#include <base/threading/thread.h>
#include <brillo/message_loops/base_message_loop.h>
#include <mojo/core/embedder/embedder.h>
#include <mojo/core/embedder/scoped_ipc_support.h>
#include <mojo/public/cpp/bindings/remote.h>
#include <mojo/public/cpp/platform/platform_channel.h>
#include <mojo/public/cpp/system/invitation.h>

#include "nnapi_hal_impl.h"

using namespace android::nn;

using IDeviceReceiver = mojo::Receiver<chromeos::nnapi::mojom::IDevice>;

namespace chromeos {

void nnapi_worker_process(int mojo_bootstrap_fd,
                          const char* service_name,
                          const char* pipe_name) {
  LOG(INFO) << "Worker started.";

  brillo::BaseMessageLoop message_loop;
  message_loop.SetAsCurrent();
  mojo::core::Init();
  mojo::core::ScopedIPCSupport ipc_support(base::SingleThreadTaskRunner::GetCurrentDefault(),
                                           mojo::core::ScopedIPCSupport::ShutdownPolicy::FAST);

  mojo::IncomingInvitation invitation;
  invitation = mojo::IncomingInvitation::Accept(mojo::PlatformChannelEndpoint(
      mojo::PlatformHandle(base::ScopedFD(mojo_bootstrap_fd))));
  mojo::ScopedMessagePipeHandle pipe = invitation.ExtractMessagePipe(pipe_name);

  // Use hidl getService until we switch to canonical ipc driver
  auto driver = V1_3::IDevice::getService(service_name);

  IDeviceImpl idevice_impl(driver);

  std::unique_ptr<IDeviceReceiver> receiver;

  message_loop.PostTask(
      FROM_HERE,
      base::BindOnce(
          [](IDeviceImpl* idevice, mojo::ScopedMessagePipeHandle pipe,
             std::unique_ptr<IDeviceReceiver>& receiver,
             brillo::BaseMessageLoop& loop) {
            receiver = std::make_unique<IDeviceReceiver>(
                idevice, mojo::PendingReceiver<chromeos::nnapi::mojom::IDevice>(
                             std::move(pipe)));
            receiver->set_disconnect_handler(base::BindOnce(
                [](brillo::BaseMessageLoop& loop) { loop.BreakLoop(); },
                std::ref(loop)));
          },
          &idevice_impl, std::move(pipe), std::ref(receiver),
          std::ref(message_loop)));

  LOG(INFO) << "Worker message loop.";

  message_loop.Run();

  LOG(INFO) << "Worker exit.";
}

}  // namespace chromeos
