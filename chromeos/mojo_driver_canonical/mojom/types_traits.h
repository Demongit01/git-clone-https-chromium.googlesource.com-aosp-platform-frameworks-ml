// Copyright 2022 The ChromiumOS Authors.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#include "aosp/frameworks/ml/chromeos/mojo_driver_canonical/mojom/types.mojom.h"

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

}  // namespace mojo
