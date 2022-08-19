// Copyright 2022 The ChromiumOS Authors.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#include "aosp/frameworks/ml/chromeos/mojo_driver_canonical/mojom/Types.mojom.h"

namespace mojo {

template <>
struct EnumTraits<chromeos::nnapi::canonical::mojom::ErrorStatus,
                  android::nn::ErrorStatus> {
  static chromeos::nnapi::canonical::mojom::ErrorStatus ToMojom(
      android::nn::ErrorStatus input) {
    return static_cast<chromeos::nnapi::canonical::mojom::ErrorStatus>(input);
  }
  static bool FromMojom(chromeos::nnapi::canonical::mojom::ErrorStatus input,
                        android::nn::ErrorStatus* output) {
    *output = static_cast<android::nn::ErrorStatus>(input);
    return true;
  }
};

template <>
struct EnumTraits<chromeos::nnapi::canonical::mojom::OperandType,
                  android::nn::OperandType> {
  static chromeos::nnapi::canonical::mojom::OperandType ToMojom(
      android::nn::OperandType input) {
    return static_cast<chromeos::nnapi::canonical::mojom::OperandType>(input);
  }
  static bool FromMojom(chromeos::nnapi::canonical::mojom::OperandType input,
                        android::nn::OperandType* output) {
    *output = static_cast<android::nn::OperandType>(input);
    return true;
  }
};

template <>
struct EnumTraits<chromeos::nnapi::canonical::mojom::VersionLevel,
                  android::nn::Version::Level> {
  static chromeos::nnapi::canonical::mojom::VersionLevel ToMojom(
      android::nn::Version::Level input) {
    return static_cast<chromeos::nnapi::canonical::mojom::VersionLevel>(input);
  }
  static bool FromMojom(chromeos::nnapi::canonical::mojom::VersionLevel input,
                        android::nn::Version::Level* output) {
    *output = static_cast<android::nn::Version::Level>(input);
    return true;
  }
};

template <>
struct EnumTraits<chromeos::nnapi::canonical::mojom::DeviceType,
                  android::nn::DeviceType> {
  static chromeos::nnapi::canonical::mojom::DeviceType ToMojom(
      android::nn::DeviceType input) {
    return static_cast<chromeos::nnapi::canonical::mojom::DeviceType>(input);
  }
  static bool FromMojom(chromeos::nnapi::canonical::mojom::DeviceType input,
                        android::nn::DeviceType* output) {
    *output = static_cast<android::nn::DeviceType>(input);
    return true;
  }
};

template <>
struct EnumTraits<chromeos::nnapi::canonical::mojom::ExecutionPreference,
                  android::nn::ExecutionPreference> {
  static chromeos::nnapi::canonical::mojom::ExecutionPreference ToMojom(
      android::nn::ExecutionPreference input) {
    return static_cast<chromeos::nnapi::canonical::mojom::ExecutionPreference>(
        input);
  }
  static bool FromMojom(
      chromeos::nnapi::canonical::mojom::ExecutionPreference input,
      android::nn::ExecutionPreference* output) {
    *output = static_cast<android::nn::ExecutionPreference>(input);
    return true;
  }
};

template <>
struct EnumTraits<chromeos::nnapi::canonical::mojom::Priority,
                  android::nn::Priority> {
  static chromeos::nnapi::canonical::mojom::Priority ToMojom(
      android::nn::Priority input) {
    return static_cast<chromeos::nnapi::canonical::mojom::Priority>(input);
  }
  static bool FromMojom(chromeos::nnapi::canonical::mojom::Priority input,
                        android::nn::Priority* output) {
    *output = static_cast<android::nn::Priority>(input);
    return true;
  }
};

template <>
struct EnumTraits<chromeos::nnapi::canonical::mojom::OperandLifeTime,
                  android::nn::Operand::LifeTime> {
  static chromeos::nnapi::canonical::mojom::OperandLifeTime ToMojom(
      android::nn::Operand::LifeTime input) {
    return static_cast<chromeos::nnapi::canonical::mojom::OperandLifeTime>(
        input);
  }
  static bool FromMojom(
      chromeos::nnapi::canonical::mojom::OperandLifeTime input,
      android::nn::Operand::LifeTime* output) {
    *output = static_cast<android::nn::Operand::LifeTime>(input);
    return true;
  }
};

template <>
struct EnumTraits<chromeos::nnapi::canonical::mojom::OperationType,
                  android::nn::OperationType> {
  static chromeos::nnapi::canonical::mojom::OperationType ToMojom(
      android::nn::OperationType input) {
    return static_cast<chromeos::nnapi::canonical::mojom::OperationType>(input);
  }
  static bool FromMojom(chromeos::nnapi::canonical::mojom::OperationType input,
                        android::nn::OperationType* output) {
    *output = static_cast<android::nn::OperationType>(input);
    return true;
  }
};

template <>
struct EnumTraits<chromeos::nnapi::canonical::mojom::MeasureTiming,
                  android::nn::MeasureTiming> {
  static chromeos::nnapi::canonical::mojom::MeasureTiming ToMojom(
      android::nn::MeasureTiming input) {
    return static_cast<chromeos::nnapi::canonical::mojom::MeasureTiming>(input);
  }
  static bool FromMojom(chromeos::nnapi::canonical::mojom::MeasureTiming input,
                        android::nn::MeasureTiming* output) {
    *output = static_cast<android::nn::MeasureTiming>(input);
    return true;
  }
};

template <>
struct EnumTraits<chromeos::nnapi::canonical::mojom::RequestArgumentLifeTime,
                  android::nn::Request::Argument::LifeTime> {
  static chromeos::nnapi::canonical::mojom::RequestArgumentLifeTime ToMojom(
      android::nn::Request::Argument::LifeTime input) {
    return static_cast<
        chromeos::nnapi::canonical::mojom::RequestArgumentLifeTime>(input);
  }
  static bool FromMojom(
      chromeos::nnapi::canonical::mojom::RequestArgumentLifeTime input,
      android::nn::Request::Argument::LifeTime* output) {
    *output = static_cast<android::nn::Request::Argument::LifeTime>(input);
    return true;
  }
};

template <>
struct StructTraits<chromeos::nnapi::canonical::mojom::MonostateDataView,
                    std::monostate> {
 public:
  static bool Read(chromeos::nnapi::canonical::mojom::MonostateDataView data,
                   std::monostate* out) {
    return true;
  }
};

template <>
struct StructTraits<chromeos::nnapi::canonical::mojom::GeneralError::DataView,
                    android::nn::GeneralError> {
 public:
  static std::string message(const android::nn::GeneralError& obj) {
    return obj.message;
  }

  static android::nn::ErrorStatus code(const android::nn::GeneralError& obj) {
    return obj.code;
  }

  static bool Read(
      chromeos::nnapi::canonical::mojom::GeneralError::DataView input,
      android::nn::GeneralError* out) {
    bool result = true;
    result &= input.ReadMessage(&out->message);
    result &= input.ReadCode(&out->code);
    return result;
  }
};

template <>
struct StructTraits<
    chromeos::nnapi::canonical::mojom::PerformanceInfo::DataView,
    android::nn::Capabilities::PerformanceInfo> {
 public:
  static float execTime(const android::nn::Capabilities::PerformanceInfo& p) {
    return p.execTime;
  }

  static float powerUsage(const android::nn::Capabilities::PerformanceInfo& p) {
    return p.powerUsage;
  }

  static bool Read(
      chromeos::nnapi::canonical::mojom::PerformanceInfo::DataView& input,
      android::nn::Capabilities::PerformanceInfo* out) {
    out->execTime = input.execTime();
    out->powerUsage = input.powerUsage();
    return true;
  }
};

template <>
struct StructTraits<
    chromeos::nnapi::canonical::mojom::OperandPerformance::DataView,
    android::nn::Capabilities::OperandPerformance> {
 public:
  static android::nn::OperandType type(
      const android::nn::Capabilities::OperandPerformance& obj) {
    return obj.type;
  }

  static android::nn::Capabilities::PerformanceInfo info(
      const android::nn::Capabilities::OperandPerformance& obj) {
    return obj.info;
  }

  static bool Read(
      chromeos::nnapi::canonical::mojom::OperandPerformance::DataView& input,
      android::nn::Capabilities::OperandPerformance* out) {
    bool result = true;
    result &= input.ReadType(&out->type);
    result &= input.ReadInfo(&out->info);
    return result;
  }
};

template <>
struct StructTraits<
    chromeos::nnapi::canonical::mojom::OperandPerformanceTable::DataView,
    android::nn::Capabilities::OperandPerformanceTable> {
 public:
  static std::vector<android::nn::Capabilities::OperandPerformance>
  operandPerformances(
      const android::nn::Capabilities::OperandPerformanceTable& obj) {
    return obj.asVector();
  }

  static bool Read(
      chromeos::nnapi::canonical::mojom::OperandPerformanceTable::DataView&
          input,
      android::nn::Capabilities::OperandPerformanceTable* out) {
    bool result = true;
    std::vector<android::nn::Capabilities::OperandPerformance>
        operandPerformances;
    result &= input.ReadOperandPerformances(&operandPerformances);

    auto create_result =
        android::nn::Capabilities::OperandPerformanceTable::create(
            operandPerformances);
    result &= create_result.ok();
    if (create_result.ok()) {
      *out = create_result.value();
    }
    return result;
  }
};

template <>
struct StructTraits<chromeos::nnapi::canonical::mojom::Capabilities::DataView,
                    android::nn::Capabilities> {
 public:
  static android::nn::Capabilities::PerformanceInfo
  relaxedFloat32toFloat16PerformanceScalar(const android::nn::Capabilities& c) {
    return c.relaxedFloat32toFloat16PerformanceScalar;
  }

  static android::nn::Capabilities::PerformanceInfo
  relaxedFloat32toFloat16PerformanceTensor(const android::nn::Capabilities& c) {
    return c.relaxedFloat32toFloat16PerformanceTensor;
  }

  static android::nn::Capabilities::OperandPerformanceTable operandPerformance(
      const android::nn::Capabilities& c) {
    return c.operandPerformance;
  }

  static android::nn::Capabilities::PerformanceInfo ifPerformance(
      const android::nn::Capabilities& c) {
    return c.ifPerformance;
  }

  static android::nn::Capabilities::PerformanceInfo whilePerformance(
      const android::nn::Capabilities& c) {
    return c.whilePerformance;
  }

  static bool Read(
      chromeos::nnapi::canonical::mojom::Capabilities::DataView& cap,
      android::nn::Capabilities* out) {
    bool result = true;
    result &= cap.ReadRelaxedFloat32toFloat16PerformanceScalar(
        &out->relaxedFloat32toFloat16PerformanceScalar);
    result &= cap.ReadRelaxedFloat32toFloat16PerformanceTensor(
        &out->relaxedFloat32toFloat16PerformanceTensor);
    result &= cap.ReadIfPerformance(&out->ifPerformance);
    result &= cap.ReadWhilePerformance(&out->whilePerformance);
    result &= cap.ReadOperandPerformance(&out->operandPerformance);
    return result;
  }
};

template <>
struct StructTraits<chromeos::nnapi::canonical::mojom::Version::DataView,
                    android::nn::Version> {
 public:
  static android::nn::Version::Level level(const android::nn::Version& obj) {
    return obj.level;
  }

  static bool runtimeOnlyFeatures(const android::nn::Version& obj) {
    return obj.runtimeOnlyFeatures;
  }

  static bool Read(chromeos::nnapi::canonical::mojom::Version::DataView& input,
                   android::nn::Version* out) {
    bool result = true;
    result &= input.ReadLevel(&out->level);
    out->runtimeOnlyFeatures = input.runtimeOnlyFeatures();
    return result;
  }
};

template <>
struct StructTraits<chromeos::nnapi::canonical::mojom::
                        ExtensionOperandTypeInformation::DataView,
                    android::nn::Extension::OperandTypeInformation> {
 public:
  static uint16_t type(
      const android::nn::Extension::OperandTypeInformation& obj) {
    return obj.type;
  }

  static bool isTensor(
      const android::nn::Extension::OperandTypeInformation& obj) {
    return obj.isTensor;
  }

  static uint32_t byteSize(
      const android::nn::Extension::OperandTypeInformation& obj) {
    return obj.byteSize;
  }

  static bool Read(chromeos::nnapi::canonical::mojom::
                       ExtensionOperandTypeInformation::DataView input,
                   android::nn::Extension::OperandTypeInformation* out) {
    out->type = input.type();
    out->isTensor = input.isTensor();
    out->byteSize = input.byteSize();
    return true;
  }
};

template <>
struct StructTraits<chromeos::nnapi::canonical::mojom::Extension::DataView,
                    android::nn::Extension> {
 public:
  static std::string name(const android::nn::Extension& obj) {
    return obj.name;
  }

  static const std::vector<android::nn::Extension::OperandTypeInformation>&
  operandTypes(const android::nn::Extension& obj) {
    return obj.operandTypes;
  }

  static bool Read(chromeos::nnapi::canonical::mojom::Extension::DataView input,
                   android::nn::Extension* out) {
    bool result = true;
    result &= input.ReadName(&out->name);
    result &= input.ReadOperandTypes(&out->operandTypes);
    return result;
  }
};

template <>
struct StructTraits<chromeos::nnapi::canonical::mojom::TimePoint::DataView,
                    android::nn::TimePoint> {
 public:
  static uint64_t nanosecondsSinceEpoch(const android::nn::TimePoint& obj) {
    return obj.time_since_epoch().count();
  }

  static bool Read(chromeos::nnapi::canonical::mojom::TimePoint::DataView& data,
                   android::nn::TimePoint* out) {
    *out = android::nn::TimePoint{
        std::chrono::nanoseconds{data.nanosecondsSinceEpoch()}};
    return true;
  }
};

template <>
struct StructTraits<chromeos::nnapi::canonical::mojom::NnHandle::DataView,
                    android::nn::SharedHandle> {
 public:
  static mojo::PlatformHandle fd(const android::nn::SharedHandle& obj) {
    return mojo::PlatformHandle(::base::ScopedFD(dup(obj->get())));
  }

  static bool Read(chromeos::nnapi::canonical::mojom::NnHandle::DataView input,
                   android::nn::SharedHandle* out) {
    out->reset(new android::nn::Handle{input.TakeFd().ReleaseFD()});
    return true;
  }
};

template <>
struct StructTraits<chromeos::nnapi::canonical::mojom::CacheToken::DataView,
                    android::nn::CacheToken> {
 public:
  static const std::vector<uint8_t> token(const android::nn::CacheToken& obj) {
    std::vector<uint8_t> v_token{
        obj.data(), obj.data() + sizeof(obj) / sizeof(*obj.data())};
    return v_token;
  }

  static bool Read(
      chromeos::nnapi::canonical::mojom::CacheToken::DataView& data,
      android::nn::CacheToken* out) {
    bool result = true;
    std::vector<uint8_t> v_token{};
    result &= data.ReadToken(&v_token);
    std::copy(v_token.begin(), v_token.end(), out->data());
    return result;
  }
};

template <>
struct StructTraits<
    typename chromeos::nnapi::canonical::mojom::TokenValuePair::DataView,
    typename android::nn::TokenValuePair> {
  static int32_t token(const android::nn::TokenValuePair& obj) {
    return obj.token;
  }
  static std::vector<uint8_t> value(const android::nn::TokenValuePair& obj) {
    return obj.value;
  }

  static bool Read(
      chromeos::nnapi::canonical::mojom::TokenValuePair::DataView& input,
      android::nn::TokenValuePair* output) {
    bool result = true;
    output->token = input.token();
    result &= input.ReadValue(&output->value);
    return result;
  }
};

template <>
struct StructTraits<typename chromeos::nnapi::canonical::mojom::
                        ExtensionNameAndPrefix::DataView,
                    typename android::nn::ExtensionNameAndPrefix> {
  static const std::string& name(
      const android::nn::ExtensionNameAndPrefix& obj) {
    return obj.name;
  }
  static uint16_t prefix(const android::nn::ExtensionNameAndPrefix& obj) {
    return obj.prefix;
  }

  static bool Read(
      chromeos::nnapi::canonical::mojom::ExtensionNameAndPrefix::DataView&
          input,
      android::nn::ExtensionNameAndPrefix* output) {
    bool result = true;
    result &= input.ReadName(&output->name);
    output->prefix = input.prefix();
    return result;
  }
};

template <>
struct StructTraits<chromeos::nnapi::canonical::mojom::OperandValues::DataView,
                    android::nn::Model::OperandValues> {
 public:
  static const std::vector<uint8_t> mData(
      const android::nn::Model::OperandValues& obj) {
    std::vector<uint8_t> values{obj.data(), obj.data() + obj.size()};
    return values;
  }

  static bool Read(
      chromeos::nnapi::canonical::mojom::OperandValues::DataView& input,
      android::nn::Model::OperandValues* out) {
    bool result = true;
    std::vector<uint8_t> values{};
    result &= input.ReadMData(&values);
    *out = android::nn::Model::OperandValues{values.data(), values.size()};
    return result;
  }
};

template <>
struct StructTraits<
    chromeos::nnapi::canonical::mojom::SymmPerChannelQuantParams::DataView,
    android::nn::Operand::SymmPerChannelQuantParams> {
 public:
  static const std::vector<float>& scales(
      const android::nn::Operand::SymmPerChannelQuantParams& obj) {
    return obj.scales;
  }
  static uint32_t channelDim(
      const android::nn::Operand::SymmPerChannelQuantParams& obj) {
    return obj.channelDim;
  }

  static bool Read(
      chromeos::nnapi::canonical::mojom::SymmPerChannelQuantParamsDataView data,
      android::nn::Operand::SymmPerChannelQuantParams* out) {
    bool result = true;
    result &= data.ReadScales(&out->scales);
    out->channelDim = data.channelDim();
    return result;
  }
};

template <>
struct UnionTraits<chromeos::nnapi::canonical::mojom::ExtraParams::DataView,
                   android::nn::Operand::ExtraParams> {
 public:
  static const std::monostate& none(
      const android::nn::Operand::ExtraParams& obj) {
    return std::get<0>(obj);
  }
  static android::nn::Operand::SymmPerChannelQuantParams channelQuant(
      const android::nn::Operand::ExtraParams& obj) {
    return std::get<1>(obj);
  }
  static const std::vector<uint8_t>& extension(
      const android::nn::Operand::ExtraParams& obj) {
    return std::get<2>(obj);
  }
  static chromeos::nnapi::canonical::mojom::ExtraParams::DataView::Tag GetTag(
      const android::nn::Operand::ExtraParams& obj) {
    return static_cast<
        chromeos::nnapi::canonical::mojom::ExtraParams::DataView::Tag>(
        obj.index());
  }

  static bool Read(
      chromeos::nnapi::canonical::mojom::ExtraParams::DataView input,
      android::nn::Operand::ExtraParams* out) {
    bool result = true;
    switch (input.tag()) {
      case chromeos::nnapi::canonical::mojom::ExtraParams::Tag::kNone:
        *out = std::monostate();
        break;
      case chromeos::nnapi::canonical::mojom::ExtraParams::Tag::kChannelQuant: {
        android::nn::Operand::SymmPerChannelQuantParams params;
        result &= input.ReadChannelQuant(&params);
        *out = std::move(params);
        break;
      }
      case chromeos::nnapi::canonical::mojom::ExtraParams::Tag::kExtension: {
        std::vector<uint8_t> extension;
        result &= input.ReadExtension(&extension);
        *out = std::move(extension);
        break;
      }
    }
    return result;
  }
};

template <>
struct StructTraits<chromeos::nnapi::canonical::mojom::DataLocation::DataView,
                    android::nn::DataLocation> {
 public:
  static const uint32_t& poolIndex(const android::nn::DataLocation& d) {
    return d.poolIndex;
  }
  static const uint32_t& offset(const android::nn::DataLocation& d) {
    return d.offset;
  }
  static const uint32_t& length(const android::nn::DataLocation& d) {
    return d.length;
  }
  static const uint32_t& padding(const android::nn::DataLocation& d) {
    return d.padding;
  }

  static bool Read(
      chromeos::nnapi::canonical::mojom::DataLocation::DataView input,
      android::nn::DataLocation* out) {
    out->poolIndex = input.poolIndex();
    out->offset = input.offset();
    out->length = input.length();
    out->padding = input.padding();
    return true;
  }
};

template <>
struct StructTraits<chromeos::nnapi::canonical::mojom::Operand::DataView,
                    android::nn::Operand> {
 public:
  static const android::nn::OperandType& type(const android::nn::Operand& obj) {
    return obj.type;
  }
  static const std::vector<uint32_t>& dimensions(
      const android::nn::Operand& obj) {
    return obj.dimensions;
  }
  static const float& scale(const android::nn::Operand& obj) {
    return obj.scale;
  }
  static const int32_t& zeroPoint(const android::nn::Operand& obj) {
    return obj.zeroPoint;
  }
  static const android::nn::Operand::LifeTime& lifetime(
      const android::nn::Operand& obj) {
    return obj.lifetime;
  }
  static const android::nn::DataLocation& location(
      const android::nn::Operand& obj) {
    return obj.location;
  }
  static const android::nn::Operand::ExtraParams& extraParams(
      const android::nn::Operand& obj) {
    return obj.extraParams;
  }

  static bool Read(chromeos::nnapi::canonical::mojom::Operand::DataView input,
                   android::nn::Operand* out) {
    bool result = true;
    EnumTraits<chromeos::nnapi::canonical::mojom::OperandType,
               android::nn::OperandType>::FromMojom(input.type(), &out->type);
    result &= input.ReadDimensions(&out->dimensions);
    out->scale = input.scale();
    out->zeroPoint = input.zeroPoint();
    EnumTraits<chromeos::nnapi::canonical::mojom::OperandLifeTime,
               android::nn::Operand::LifeTime>::FromMojom(input.lifetime(),
                                                          &out->lifetime);
    result &= input.ReadLocation(&out->location);
    result &= input.ReadExtraParams(&out->extraParams);
    return result;
  }
};

template <>
struct StructTraits<chromeos::nnapi::canonical::mojom::Operation::DataView,
                    android::nn::Operation> {
 public:
  static android::nn::OperationType type(const android::nn::Operation& obj) {
    return obj.type;
  }
  static const std::vector<uint32_t>& inputs(
      const android::nn::Operation& obj) {
    return obj.inputs;
  }
  static const std::vector<uint32_t>& outputs(
      const android::nn::Operation& obj) {
    return obj.outputs;
  }

  static bool Read(
      chromeos::nnapi::canonical::mojom::Operation::DataView operation,
      android::nn::Operation* out) {
    bool result = true;
    EnumTraits<chromeos::nnapi::canonical::mojom::OperationType,
               android::nn::OperationType>::FromMojom(operation.type(),
                                                      &out->type);
    result &= operation.ReadInputs(&out->inputs);
    result &= operation.ReadOutputs(&out->outputs);
    return result;
  }
};

template <>
struct StructTraits<chromeos::nnapi::canonical::mojom::ModelSubgraph::DataView,
                    android::nn::Model::Subgraph> {
 public:
  static const std::vector<android::nn::Operand>& operands(
      const android::nn::Model::Subgraph& obj) {
    return obj.operands;
  }
  static const std::vector<android::nn::Operation>& operations(
      const android::nn::Model::Subgraph& obj) {
    return obj.operations;
  }
  static const std::vector<uint32_t>& inputIndexes(
      const android::nn::Model::Subgraph& obj) {
    return obj.inputIndexes;
  }
  static const std::vector<uint32_t>& outputIndexes(
      const android::nn::Model::Subgraph& obj) {
    return obj.outputIndexes;
  }

  static bool Read(
      chromeos::nnapi::canonical::mojom::ModelSubgraph::DataView input,
      android::nn::Model::Subgraph* out) {
    bool result = true;
    result &= input.ReadOperands(&out->operands);
    result &= input.ReadOperations(&out->operations);
    result &= input.ReadInputIndexes(&out->inputIndexes);
    result &= input.ReadOutputIndexes(&out->outputIndexes);
    return result;
  }
};

template <>
struct StructTraits<chromeos::nnapi::canonical::mojom::Ashmem::DataView,
                    android::nn::Memory::Ashmem> {
 public:
  static mojo::PlatformHandle fd(const android::nn::Memory::Ashmem& obj) {
    return mojo::PlatformHandle(::base::ScopedFD(dup(obj.fd.get())));
  }
  static int32_t size(const android::nn::Memory::Ashmem& obj) {
    return obj.size;
  }

  static bool Read(chromeos::nnapi::canonical::mojom::Ashmem::DataView input,
                   android::nn::Memory::Ashmem* out) {
    out->fd.reset(input.TakeFd().ReleaseFD());
    out->size = input.size();
    return true;
  }
};

template <>
struct StructTraits<chromeos::nnapi::canonical::mojom::Fd::DataView,
                    android::nn::Memory::Fd> {
 public:
  static int32_t size(const android::nn::Memory::Fd& obj) { return obj.size; }
  static int32_t prot(const android::nn::Memory::Fd& obj) { return obj.prot; }
  static mojo::PlatformHandle fd(const android::nn::Memory::Fd& obj) {
    return mojo::PlatformHandle(::base::ScopedFD(dup(obj.fd.get())));
  }
  static int32_t offset(const android::nn::Memory::Fd& obj) {
    return obj.offset;
  }

  static bool Read(chromeos::nnapi::canonical::mojom::Fd::DataView input,
                   android::nn::Memory::Fd* out) {
    out->size = input.size();
    out->prot = input.prot();
    out->fd.reset(input.TakeFd().ReleaseFD());
    out->offset = input.offset();
    return true;
  }
};

template <>
struct StructTraits<chromeos::nnapi::canonical::mojom::HardwareBuffer::DataView,
                    android::nn::Memory::HardwareBuffer> {
 public:
  static bool Read(chromeos::nnapi::canonical::mojom::HardwareBuffer input,
                   android::nn::Memory::HardwareBuffer* out) {
    return true;
  }
};

template <>
struct StructTraits<chromeos::nnapi::canonical::mojom::MemoryUnknown::DataView,
                    android::nn::Memory::Unknown> {
 public:
  static std::vector<mojo::PlatformHandle> fds(
      const android::nn::Memory::Unknown& obj) {
    std::vector<mojo::PlatformHandle> handles;
    for (auto& fd : obj.handle.fds) {
      handles.push_back(mojo::PlatformHandle(::base::ScopedFD(dup(fd.get()))));
    }
    return handles;
  }
  static const std::vector<int>& ints(const android::nn::Memory::Unknown& obj) {
    return obj.handle.ints;
  }
  static uint32_t size(const android::nn::Memory::Unknown& obj) {
    return obj.size;
  }
  static const std::string& name(const android::nn::Memory::Unknown& obj) {
    return obj.name;
  }

  static bool Read(
      chromeos::nnapi::canonical::mojom::MemoryUnknown::DataView& input,
      android::nn::Memory::Unknown* out) {
    bool result = true;
    result &= input.ReadInts(&out->handle.ints);
    result &= input.ReadName(&out->name);
    mojo::ArrayDataView<mojo::PlatformHandle> fds;
    input.GetFdsDataView(&fds);
    for (int i = 0; i < fds.size(); i++) {
      out->handle.fds.push_back(
          android::base::unique_fd{fds.Take(i).ReleaseFD()});
    }
    return result;
  }
};

template <>
struct UnionTraits<chromeos::nnapi::canonical::mojom::NnMemory::DataView,
                   android::nn::SharedMemory> {
 public:
  static const android::nn::Memory::Ashmem& ashmemHandle(
      const android::nn::SharedMemory& obj) {
    return std::get<0>(obj->handle);
  }
  static const android::nn::Memory::Fd& fdHandle(
      const android::nn::SharedMemory& obj) {
    return std::get<1>(obj->handle);
  }
  static const android::nn::Memory::HardwareBuffer& hardwareBufferHandle(
      const android::nn::SharedMemory& obj) {
    return std::get<2>(obj->handle);
  }
  static const android::nn::Memory::Unknown& unknownHandle(
      const android::nn::SharedMemory& obj) {
    return std::get<3>(obj->handle);
  }
  static chromeos::nnapi::canonical::mojom::NnMemory::DataView::Tag GetTag(
      const android::nn::SharedMemory& obj) {
    return static_cast<
        chromeos::nnapi::canonical::mojom::NnMemory::DataView::Tag>(
        obj->handle.index());
  }

  static bool Read(chromeos::nnapi::canonical::mojom::NnMemory::DataView input,
                   android::nn::SharedMemory* out) {
    bool result = true;
    auto memory = new android::nn::Memory{};
    switch (input.tag()) {
      case chromeos::nnapi::canonical::mojom::NnMemory::Tag::kAshmemHandle: {
        android::nn::Memory::Ashmem ashmem;
        result &= input.ReadAshmemHandle(&ashmem);
        memory->handle = std::move(ashmem);
        break;
      }
      case chromeos::nnapi::canonical::mojom::NnMemory::Tag::kFdHandle: {
        android::nn::Memory::Fd fd;
        result &= input.ReadFdHandle(&fd);
        memory->handle = std::move(fd);
        break;
      }
      case chromeos::nnapi::canonical::mojom::NnMemory::Tag::
          kHardwareBufferHandle:
        break;
      case chromeos::nnapi::canonical::mojom::NnMemory::Tag::kUnknownHandle: {
        android::nn::Memory::Unknown unknown;
        result &= input.ReadUnknownHandle(&unknown);
        memory->handle = std::move(unknown);
        break;
      }
    }
    out->reset(memory);
    return result;
  }
};

template <>
struct StructTraits<chromeos::nnapi::canonical::mojom::Model::DataView,
                    android::nn::Model> {
 public:
  static const android::nn::Model::Subgraph& main(const android::nn::Model& m) {
    return m.main;
  }
  static const std::vector<android::nn::Model::Subgraph>& referenced(
      const android::nn::Model& m) {
    return m.referenced;
  }
  static const android::nn::Model::OperandValues& operandValues(
      const android::nn::Model& m) {
    return m.operandValues;
  }
  static const std::vector<android::nn::SharedMemory> pools(
      const android::nn::Model& m) {
    return m.pools;
  }
  static bool relaxComputationFloat32toFloat16(const android::nn::Model& m) {
    return m.relaxComputationFloat32toFloat16;
  }
  static const std::vector<android::nn::ExtensionNameAndPrefix>&
  extensionNameToPrefix(const android::nn::Model& m) {
    return m.extensionNameToPrefix;
  }

  static bool Read(chromeos::nnapi::canonical::mojom::Model::DataView& model,
                   android::nn::Model* out) {
    bool result = true;
    result &= model.ReadMain(&out->main);
    result &= model.ReadReferenced(&out->referenced);
    result &= model.ReadOperandValues(&out->operandValues);
    result &= model.ReadPools(&out->pools);
    out->relaxComputationFloat32toFloat16 =
        model.relaxComputationFloat32toFloat16();
    result &= model.ReadExtensionNameToPrefix(&out->extensionNameToPrefix);
    return result;
  }
};

template <>
struct StructTraits<chromeos::nnapi::canonical::mojom::ExecutionError::DataView,
                    android::nn::ExecutionError> {
 public:
  static const std::string& message(const android::nn::ExecutionError& obj) {
    return obj.message;
  }
  static android::nn::ErrorStatus code(const android::nn::ExecutionError& obj) {
    return obj.code;
  }
  static const std::vector<android::nn::OutputShape>& outputShapes(
      const android::nn::ExecutionError& obj) {
    return obj.outputShapes;
  }

  static bool Read(
      chromeos::nnapi::canonical::mojom::ExecutionError::DataView input,
      android::nn::ExecutionError* out) {
    bool result = true;
    EnumTraits<chromeos::nnapi::canonical::mojom::ErrorStatus,
               android::nn::ErrorStatus>::FromMojom(input.code(), &out->code);
    result &= input.ReadMessage(&out->message);
    result &= input.ReadOutputShapes(&out->outputShapes);
    return result;
  }
};

template <>
struct StructTraits<chromeos::nnapi::canonical::mojom::OutputShape::DataView,
                    android::nn::OutputShape> {
 public:
  static const std::vector<uint32_t>& dimensions(
      const android::nn::OutputShape& obj) {
    return obj.dimensions;
  }
  static bool isSufficient(const android::nn::OutputShape& obj) {
    return obj.isSufficient;
  }

  static bool Read(
      chromeos::nnapi::canonical::mojom::OutputShape::DataView input,
      android::nn::OutputShape* out) {
    bool result = true;
    result &= input.ReadDimensions(&out->dimensions);
    out->isSufficient = input.isSufficient();
    return result;
  }
};

template <>
struct StructTraits<chromeos::nnapi::canonical::mojom::Timing::DataView,
                    android::nn::Timing> {
 public:
  static absl::optional<android::nn::Duration> timeOnDevice(
      const android::nn::Timing& obj) {
    return obj.timeOnDevice;
  }
  static absl::optional<android::nn::Duration> timeInDriver(
      const android::nn::Timing& obj) {
    return obj.timeInDriver;
  }

  static bool Read(chromeos::nnapi::canonical::mojom::Timing::DataView& data,
                   android::nn::Timing* out) {
    bool result = true;
    result &= data.ReadTimeInDriver(&out->timeOnDevice);
    result &= data.ReadTimeInDriver(&out->timeInDriver);
    return result;
  }
};

template <>
struct StructTraits<chromeos::nnapi::canonical::mojom::Duration::DataView,
                    android::nn::Duration> {
 public:
  static uint64_t nanoseconds(const android::nn::Duration& obj) {
    return obj.count();
  }

  static bool Read(chromeos::nnapi::canonical::mojom::Duration::DataView& data,
                   android::nn::Duration* out) {
    *out = android::nn::Duration{std::chrono::nanoseconds{data.nanoseconds()}};
    return true;
  }
};

template <>
struct StructTraits<
    chromeos::nnapi::canonical::mojom::RequestArgument::DataView,
    android::nn::Request::Argument> {
 public:
  static android::nn::Request::Argument::LifeTime lifetime(
      const android::nn::Request::Argument& obj) {
    return obj.lifetime;
  }
  static const android::nn::DataLocation& location(
      const android::nn::Request::Argument& obj) {
    return obj.location;
  }
  static const std::vector<uint32_t>& dimensions(
      const android::nn::Request::Argument& obj) {
    return obj.dimensions;
  }

  static bool Read(
      chromeos::nnapi::canonical::mojom::RequestArgument::DataView input,
      android::nn::Request::Argument* out) {
    bool result = true;
    EnumTraits<
        chromeos::nnapi::canonical::mojom::RequestArgumentLifeTime,
        android::nn::Request::Argument::LifeTime>::FromMojom(input.lifetime(),
                                                             &out->lifetime);
    result &= input.ReadDimensions(&out->dimensions);
    result &= input.ReadLocation(&out->location);
    return result;
  }
};

template <>
struct StructTraits<
    chromeos::nnapi::canonical::mojom::RequestMemoryPool::DataView,
    android::nn::Request::MemoryPool> {
 public:
  static const android::nn::SharedMemory& memory(
      const android::nn::Request::MemoryPool& m) {
    return std::get<0>(m);
  }

  static bool Read(
      chromeos::nnapi::canonical::mojom::RequestMemoryPool::DataView& model,
      android::nn::Request::MemoryPool* out) {
    bool result = true;
    android::nn::SharedMemory memory;
    result &= model.ReadMemory(&memory);
    *out = memory;
    return result;
  }
};

template <>
struct StructTraits<chromeos::nnapi::canonical::mojom::Request::DataView,
                    android::nn::Request> {
 public:
  static const std::vector<android::nn::Request::MemoryPool> pools(
      const android::nn::Request& m) {
    return m.pools;
  }
  static const std::vector<android::nn::Request::Argument>& inputs(
      const android::nn::Request& m) {
    return m.inputs;
  }
  static const std::vector<android::nn::Request::Argument>& outputs(
      const android::nn::Request& m) {
    return m.outputs;
  }

  static bool Read(chromeos::nnapi::canonical::mojom::Request::DataView& model,
                   android::nn::Request* out) {
    bool result = true;
    result &= model.ReadInputs(&out->inputs);
    result &= model.ReadOutputs(&out->outputs);
    result &= model.ReadPools(&out->pools);
    return result;
  }
};

}  // namespace mojo
