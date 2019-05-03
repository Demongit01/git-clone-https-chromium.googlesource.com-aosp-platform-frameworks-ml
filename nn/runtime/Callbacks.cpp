/*
 * Copyright (C) 2017 The Android Open Source Project
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *      http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#include "Callbacks.h"
#include <android-base/logging.h>

namespace android {
namespace hardware {
namespace neuralnetworks {
namespace V1_2 {
namespace implementation {

CallbackBase::CallbackBase() : mNotified(false) {}

CallbackBase::~CallbackBase() {
    // Note that we cannot call CallbackBase::join_thread from here:
    // CallbackBase is intended to be reference counted, and it is possible that
    // the reference count drops to zero in the bound thread, causing the
    // bound thread to call this destructor. If a thread tries to join
    // itself, it throws an exception, producing a message like the
    // following:
    //
    //     terminating with uncaught exception of type std::__1::system_error:
    //     thread::join failed: Resource deadlock would occur
}

void CallbackBase::wait() {
    std::unique_lock<std::mutex> lock(mMutex);
    mCondition.wait(lock, [this]{return mNotified;});
    join_thread_locked();
}

bool CallbackBase::on_finish(std::function<void()> post_work) {
    std::lock_guard<std::mutex> lock(mMutex);
    if (mPostWork != nullptr) {
        LOG(ERROR) << "CallbackBase::on_finish -- a post-work function has already been bound to "
                   "this callback object";
        return false;
    }
    if (post_work == nullptr) {
        LOG(ERROR) << "CallbackBase::on_finish -- the new post-work function is invalid";
        return false;
    }
    mPostWork = std::move(post_work);
    return true;
}

bool CallbackBase::bind_thread(std::thread&& asyncThread) {
    std::lock_guard<std::mutex> lock(mMutex);
    if (mThread.joinable()) {
        LOG(ERROR) << "CallbackBase::bind_thread -- a thread has already been bound to this "
                   "callback object";
        return false;
    }
    if (!asyncThread.joinable()) {
        LOG(ERROR) << "CallbackBase::bind_thread -- the new thread is not joinable";
        return false;
    }
    mThread = std::move(asyncThread);
    return true;
}

void CallbackBase::notify() {
    {
        std::lock_guard<std::mutex> lock(mMutex);
        mNotified = true;
        if (mPostWork) {
            mPostWork();
        }
    }
    mCondition.notify_all();
}

void CallbackBase::join_thread_locked() {
    if (mThread.joinable()) {
        mThread.join();
    }
}

Return<void> PreparedModelCallback::notify(ErrorStatus errorStatus,
                                           const sp<V1_0::IPreparedModel>& preparedModel) {
    {
        std::lock_guard<std::mutex> hold(mMutex);

        // quick-return if object has already been notified
        if (mNotified) {
            return Void();
        }

        // store results and mark as notified
        mErrorStatus = errorStatus;
        mPreparedModel = preparedModel;
        mNotified = true;
    }

    mCondition.notify_all();
    return Void();
}

Return<void> PreparedModelCallback::notify_1_2(ErrorStatus errorStatus,
                                               const sp<V1_2::IPreparedModel>& preparedModel) {
    return notify(errorStatus, preparedModel);
}

void PreparedModelCallback::wait() const {
    std::unique_lock<std::mutex> lock(mMutex);
    mCondition.wait(lock, [this] { return mNotified; });
}

ErrorStatus PreparedModelCallback::getStatus() const {
    wait();
    return mErrorStatus;
}

sp<V1_0::IPreparedModel> PreparedModelCallback::getPreparedModel() const {
    wait();
    return mPreparedModel;
}

ExecutionCallback::ExecutionCallback()
    : mErrorStatus(ErrorStatus::GENERAL_FAILURE), mOnFinish(nullptr) {
    on_finish([this] {
        if (mOnFinish != nullptr) {
            ErrorStatus status = mOnFinish(mErrorStatus, mOutputShapes);
            if (status != ErrorStatus::NONE) {
                mErrorStatus = status;
            }
        }
    });
}

ExecutionCallback::~ExecutionCallback() {}

Return<void> ExecutionCallback::notify(ErrorStatus errorStatus) {
    mErrorStatus = errorStatus;
    mOutputShapes = {};
    mTiming = {.timeOnDevice = UINT64_MAX, .timeInDriver = UINT64_MAX};
    CallbackBase::notify();
    return Void();
}

Return<void> ExecutionCallback::notify_1_2(ErrorStatus errorStatus,
                                           const hidl_vec<OutputShape>& outputShapes,
                                           const Timing& timing) {
    mErrorStatus = errorStatus;
    mOutputShapes = outputShapes;
    mTiming = timing;
    if (mErrorStatus == ErrorStatus::OUTPUT_INSUFFICIENT_SIZE) {
        // mOutputShapes must not be empty if OUTPUT_INSUFFICIENT_SIZE.
        if (mOutputShapes.size() == 0) {
            LOG(ERROR) << "Notified with empty output shape vector when OUTPUT_INSUFFICIENT_SIZE";
            mErrorStatus = ErrorStatus::GENERAL_FAILURE;
            mOutputShapes = {};
            mTiming = {.timeOnDevice = UINT64_MAX, .timeInDriver = UINT64_MAX};
        }
    } else if (mErrorStatus != ErrorStatus::NONE) {
        // mOutputShapes must be empty if mErrorStatus is neither NONE nor OUTPUT_INSUFFICIENT_SIZE.
        if (mOutputShapes.size() != 0) {
            LOG(ERROR) << "Notified with non-empty output shape vector when error status is "
                          "neither NONE nor OUTPUT_INSUFFICIENT_SIZE";
            mErrorStatus = ErrorStatus::GENERAL_FAILURE;
            mOutputShapes = {};
            mTiming = {.timeOnDevice = UINT64_MAX, .timeInDriver = UINT64_MAX};
        }
    }
    CallbackBase::notify();
    return Void();
}

ErrorStatus ExecutionCallback::getStatus() {
    wait();
    return mErrorStatus;
}

const std::vector<OutputShape>& ExecutionCallback::getOutputShapes() {
    wait();
    return mOutputShapes;
}

Timing ExecutionCallback::getTiming() {
    wait();
    return mTiming;
}

}  // namespace implementation
}  // namespace V1_2
}  // namespace neuralnetworks
}  // namespace hardware
}  // namespace android