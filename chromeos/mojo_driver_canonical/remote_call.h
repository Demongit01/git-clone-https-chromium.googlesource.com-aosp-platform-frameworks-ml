// Copyright 2022 The ChromiumOS Authors.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef ML_NN_CHROMEOS_IPC_DRIVER_CANONICAL_COMMON_H_
#define ML_NN_CHROMEOS_IPC_DRIVER_CANONICAL_COMMON_H_

#include <base/logging.h>
#include <brillo/message_loops/base_message_loop.h>
#include <mojo/public/cpp/bindings/remote.h>

#include "base/time/time.h"

inline constexpr ::base::TimeDelta kRemoteCallTimeoutMilliSec =
    base::Milliseconds(100000);

template <typename T>
void DefaultOutputCallback(::base::WaitableEvent& event, T& l, T r) {
  l = std::move(r);
  event.Signal();
}

template <typename T1, typename T2, typename... Args>
void DefaultOutputCallback(::base::WaitableEvent& event,
                           T1& l1,
                           T2& l2,
                           Args&... ls,
                           T1 r1,
                           T2 r2,
                           Args... rs) {
  l1 = std::move(r1);
  DefaultOutputCallback(event, l2, ls..., std::forward<T2>(r2),
                        std::forward<Args&&>(rs)...);
}

// Some output parameter U is not the same as T, e.g. it can be `const T&`, so
// we need to explicitly specify typename U
template <typename T, typename U>
void FullOutputCallback(::base::WaitableEvent& event, T& t, U u) {
  t = std::move(u);
  event.Signal();
}

template <typename T1, typename T2, typename U1, typename U2>
void FullOutputCallback(::base::WaitableEvent& event,
                        T1& t1,
                        T2& t2,
                        U1 u1,
                        U2 u2) {
  t1 = std::move(u1);
  FullOutputCallback(event, t2, std::forward<U2>(u2));
}

template <typename T1,
          typename T2,
          typename T3,
          typename U1,
          typename U2,
          typename U3>
void FullOutputCallback(::base::WaitableEvent& event,
                        T1& t1,
                        T2& t2,
                        T3& t3,
                        U1 u1,
                        U2 u2,
                        U3 u3) {
  t1 = std::move(u1);
  FullOutputCallback(event, t2, t3, std::forward<U2>(u2), std::forward<U3>(u3));
}

template <typename RC, typename CB, typename P, typename... Args>
bool CallRemote(scoped_refptr<::base::SequencedTaskRunner>& task_runner,
                mojo::Remote<P>& remote,
                RC remote_call,
                CB callback,
                Args... refs) {
  ::base::WaitableEvent event;
  auto fn = [](::base::WaitableEvent& event, mojo::Remote<P>& remote,
               RC remote_call, CB callback, Args... refs) {
    remote_call(remote, ::base::BindOnce(callback, std::ref(event), refs...));
  };
  task_runner->PostTask(
      FROM_HERE,
      ::base::BindOnce(fn, std::ref(event), std::ref(remote),
                       std::ref(remote_call), std::ref(callback), refs...));
  return event.TimedWait(kRemoteCallTimeoutMilliSec);
}

class HasSequencedTaskRunner {
 public:
  HasSequencedTaskRunner(
      scoped_refptr<::base::SequencedTaskRunner>& task_runner)
      : task_runner_{task_runner} {}

 protected:
  mutable scoped_refptr<::base::SequencedTaskRunner> task_runner_;
};

template <typename P>
class HasRemote : public HasSequencedTaskRunner {
 public:
  HasRemote(mojo::PendingRemote<P> pending_remote,
            scoped_refptr<::base::SequencedTaskRunner>& task_runner)
      : HasSequencedTaskRunner{task_runner} {
    remote_ = mojo::Remote<P>(std::move(pending_remote), task_runner);
  }
  ~HasRemote() {
    // Move remote into ipc thread to be destroyed within the ipc thread
    task_runner_->PostTask(
        FROM_HERE,
        base::BindOnce([](mojo::Remote<P> remote) {}, std::move(remote_)));
  }

 protected:
  mutable mojo::Remote<P> remote_;
};

#endif  // ML_NN_CHROMEOS_IPC_DRIVER_CANONICAL_COMMON_H_
