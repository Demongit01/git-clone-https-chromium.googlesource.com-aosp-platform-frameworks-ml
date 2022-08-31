// Copyright 2022 The ChromiumOS Authors.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#include "ipc_driver_canonical.h"

#include <base/no_destructor.h>
#include <mojo/core/embedder/embedder.h>
#include "mojo_controller_canonical.h"

namespace android {
namespace nn {

class MojoThread {
 public:
  // This MojoThread should be used as singleton, the reason for this
  // constructor being public is the base::NoDestructor<> needs a public
  // constructor.
  MojoThread();

  MojoThread(MojoThread const&) = delete;
  void operator=(MojoThread const&) = delete;

  static MojoThread& Get();

  scoped_refptr<::base::SingleThreadTaskRunner>& GetIpcTaskRunner();

 private:
  std::unique_ptr<::base::Thread> ipc_thread_;
  std::unique_ptr<mojo::core::ScopedIPCSupport> ipc_support_;
};

MojoThread::MojoThread() {
  mojo::core::Init();
  ipc_thread_ = std::make_unique<::base::Thread>("IpcThread");
  ipc_thread_->StartWithOptions(
      ::base::Thread::Options(::base::MessagePumpType::IO, 0));
  ipc_support_ = std::make_unique<mojo::core::ScopedIPCSupport>(
      ipc_thread_->task_runner(),
      mojo::core::ScopedIPCSupport::ShutdownPolicy::CLEAN);
}

scoped_refptr<::base::SingleThreadTaskRunner>& MojoThread::GetIpcTaskRunner() {
  static auto task_runner = ipc_thread_->task_runner();
  return task_runner;
}

// static
MojoThread& MojoThread::Get() {
  static ::base::NoDestructor<MojoThread> kMojoThread;
  return *kMojoThread;
}

IPCDriverCanonical::IPCDriverCanonical(const std::string name,
                                       SharedDevice delegate)
    : name_{name}, delegate_(delegate) {
  // We need to get task runner from MojoThread which also initializes mojo
  // core, creates ipc thread, provides mojo ipc support, etc.
  mojo_ = std::make_unique<MojoControllerCanonical>(
      MojoThread::Get().GetIpcTaskRunner(), name);
}

const std::string& IPCDriverCanonical::getName() const {
  return name_;
}

const std::string& IPCDriverCanonical::getVersionString() const {
  const static std::string version = mojo_->getVersionString();
  return version;
}

Version IPCDriverCanonical::getFeatureLevel() const {
  return mojo_->getFeatureLevel();
}

DeviceType IPCDriverCanonical::getType() const {
  return mojo_->getType();
}

const std::vector<Extension>& IPCDriverCanonical::getSupportedExtensions()
    const {
  const static std::vector<Extension> extensions =
      mojo_->getSupportedExtensions();
  return extensions;
}

const Capabilities& IPCDriverCanonical::getCapabilities() const {
  const static Capabilities capabilities = mojo_->getCapabilities();
  return capabilities;
}

std::pair<uint32_t, uint32_t> IPCDriverCanonical::getNumberOfCacheFilesNeeded()
    const {
  return mojo_->getNumberOfCacheFilesNeeded();
}

GeneralResult<void> IPCDriverCanonical::wait() const {
  return mojo_->wait();
}

GeneralResult<std::vector<bool>> IPCDriverCanonical::getSupportedOperations(
    const Model& model) const {
  return mojo_->getSupportedOperations(model);
}

GeneralResult<SharedPreparedModel> IPCDriverCanonical::prepareModel(
    const Model& model,
    ExecutionPreference preference,
    Priority priority,
    OptionalTimePoint deadline,
    const std::vector<SharedHandle>& modelCache,
    const std::vector<SharedHandle>& dataCache,
    const CacheToken& token,
    const std::vector<nn::TokenValuePair>& hints,
    const std::vector<nn::ExtensionNameAndPrefix>& extensionNameToPrefix)
    const {
  return mojo_->prepareModel(model, preference, priority, deadline,
                                 modelCache, dataCache, token, hints,
                                 extensionNameToPrefix);
}

GeneralResult<SharedPreparedModel> IPCDriverCanonical::prepareModelFromCache(
    OptionalTimePoint deadline,
    const std::vector<SharedHandle>& modelCache,
    const std::vector<SharedHandle>& dataCache,
    const CacheToken& token) const {
  return mojo_->prepareModelFromCache(deadline, modelCache, dataCache,
                                          token);
}

GeneralResult<SharedBuffer> IPCDriverCanonical::allocate(
    const BufferDesc& desc,
    const std::vector<SharedPreparedModel>& preparedModels,
    const std::vector<BufferRole>& inputRoles,
    const std::vector<BufferRole>& outputRoles) const {
  return NN_ERROR() << "IDevice::allocate is not supported.";
}

}  // namespace nn
}  // namespace android
