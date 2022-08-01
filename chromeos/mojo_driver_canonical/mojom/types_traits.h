// Copyright 2022 The ChromiumOS Authors.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#include "aosp/frameworks/ml/chromeos/mojo_driver_canonical/mojom/types.mojom.h"

namespace mojo {

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
struct StructTraits<chromeos::nnapi::canonical::mojom::PerformanceInfoDataView,
                    android::nn::Capabilities::PerformanceInfo> {
 public:
  static float execTime(const android::nn::Capabilities::PerformanceInfo& p) {
    return p.execTime;
  }
  static float powerUsage(const android::nn::Capabilities::PerformanceInfo& p) {
    return p.powerUsage;
  }

  static bool Read(
      chromeos::nnapi::canonical::mojom::PerformanceInfoDataView& input,
      android::nn::Capabilities::PerformanceInfo* out) {
    out->execTime = input.execTime();
    out->powerUsage = input.powerUsage();
    return true;
  }
};

template <>
struct StructTraits<
    chromeos::nnapi::canonical::mojom::OperandPerformanceDataView,
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
      chromeos::nnapi::canonical::mojom::OperandPerformanceDataView& input,
      android::nn::Capabilities::OperandPerformance* out) {
    bool result = true;
    result &= input.ReadType(&out->type);
    result &= input.ReadInfo(&out->info);
    return result;
  }
};

template <>
struct StructTraits<
    chromeos::nnapi::canonical::mojom::OperandPerformanceTableDataView,
    android::nn::Capabilities::OperandPerformanceTable> {
 public:
  static std::vector<android::nn::Capabilities::OperandPerformance>
  operandPerformances(
      const android::nn::Capabilities::OperandPerformanceTable& obj) {
    return obj.asVector();
  }

  static bool Read(
      chromeos::nnapi::canonical::mojom::OperandPerformanceTableDataView& input,
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
struct StructTraits<chromeos::nnapi::canonical::mojom::CapabilitiesDataView,
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

  static bool Read(chromeos::nnapi::canonical::mojom::CapabilitiesDataView& cap,
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

}  // namespace mojo
