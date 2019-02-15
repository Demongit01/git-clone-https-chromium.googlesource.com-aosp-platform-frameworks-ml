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

#include "CpuOperationUtils.h"
#include "Operations.h"

#include "tensorflow/lite/kernels/internal/optimized/legacy_optimized_ops.h"
#include "tensorflow/lite/kernels/internal/reference/reference_ops.h"

#include "Tracing.h"

namespace android {
namespace nn {

// executionMutex is used to protect concurrent access of non-threadsafe resources
// like gemmlowp::GemmContext.
// std::mutex is safe for pthreads on Android.
static std::mutex executionMutex;

bool fullyConnectedFloat16(const _Float16* inputData, const Shape& inputShape,
                           const _Float16* weightsData, const Shape& weightsShape,
                           const _Float16* biasData, const Shape& biasShape, int32_t activation,
                           _Float16* outputData, const Shape& outputShape) {
    NNTRACE_TRANS("fullyConnectedFloat16");
    std::vector<float> inputDataFloat32(getNumberOfElements(inputShape));
    convertFloat16ToFloat32(inputData, &inputDataFloat32);
    std::vector<float> weightsDataFloat32(getNumberOfElements(weightsShape));
    convertFloat16ToFloat32(weightsData, &weightsDataFloat32);
    std::vector<float> biasDataFloat32(getNumberOfElements(biasShape));
    convertFloat16ToFloat32(biasData, &biasDataFloat32);

    std::vector<float> outputDataFloat32(getNumberOfElements(outputShape));
    fullyConnectedFloat32(inputDataFloat32.data(), inputShape, weightsDataFloat32.data(),
                          weightsShape, biasDataFloat32.data(), biasShape, activation,
                          outputDataFloat32.data(), outputShape);
    convertFloat32ToFloat16(outputDataFloat32, outputData);

    return true;
}

bool fullyConnectedFloat32(const float* inputData, const Shape& inputShape,
                           const float* weightsData, const Shape& weightsShape,
                           const float* biasData, const Shape& biasShape, int32_t activation,
                           float* outputData, const Shape& outputShape) {
    NNTRACE_TRANS("fullyConnectedFloat32");
    float output_activation_min, output_activation_max;
    CalculateActivationRangeFloat(activation, &output_activation_min, &output_activation_max);

    // b/80425683, optimized implementation produces incorrect results when the
    // number of input elements is the squre of batch_size.
    uint32_t batch_size = getSizeOfDimension(outputShape, 0);
    uint32_t input_n_elements = getNumberOfElements(inputShape);
    if (batch_size * batch_size == input_n_elements) {
        NNTRACE_COMP_SWITCH("reference_ops::FullyConnected");
        tflite::reference_ops::FullyConnected(inputData, convertShapeToDims(inputShape),
                                              weightsData, convertShapeToDims(weightsShape),
                                              biasData, convertShapeToDims(biasShape),
                                              output_activation_min, output_activation_max,
                                              outputData, convertShapeToDims(outputShape));
    } else {
        NNTRACE_COMP_SWITCH("optimized_ops::FullyConnected");
        tflite::optimized_ops::FullyConnected(inputData, convertShapeToDims(inputShape),
                                              weightsData, convertShapeToDims(weightsShape),
                                              biasData, convertShapeToDims(biasShape),
                                              output_activation_min, output_activation_max,
                                              outputData, convertShapeToDims(outputShape));
    }
    return true;
}

bool fullyConnectedQuant8(const uint8_t* inputData, const Shape& inputShape,
                          const uint8_t* weightsData, const Shape& weightsShape,
                          const int32_t* biasData, const Shape& biasShape, int32_t activation,
                          uint8_t* outputData, const Shape& outputShape) {
    NNTRACE_TRANS("fullyConnectedQuant8");
    int32_t inputOffset = -inputShape.offset;
    int32_t weightsOffset = -weightsShape.offset;
    int32_t outputOffset = outputShape.offset;

    float real_multiplier = 0.0;
    int32_t output_multiplier = 0;
    int32_t output_shift = 0;
    int32_t output_activation_min = 0;
    int32_t output_activation_max = 0;

    if (!GetQuantizedConvolutionMultipler(inputShape, weightsShape, biasShape, outputShape,
                                          &real_multiplier) ||
        !QuantizeMultiplierSmallerThanOne(real_multiplier, &output_multiplier, &output_shift)) {
        return false;
    }
    CalculateActivationRangeUint8(activation, outputShape, &output_activation_min,
                                  &output_activation_max);

    static gemmlowp::GemmContext gemm_context;

    // Prevent concurrent executions that access gemm_context.
    std::unique_lock<std::mutex> lock(executionMutex);
    // Alow gemmlowp automatically decide how many threads to use.
    gemm_context.set_max_num_threads(0);

    NNTRACE_COMP_SWITCH("optimized_ops::FullyConnected");
    tflite::optimized_ops::FullyConnected(inputData, convertShapeToDims(inputShape), inputOffset,
                                          weightsData, convertShapeToDims(weightsShape),
                                          weightsOffset, biasData, convertShapeToDims(biasShape),
                                          outputOffset, output_multiplier, output_shift,
                                          output_activation_min, output_activation_max, outputData,
                                          convertShapeToDims(outputShape), &gemm_context);

    return true;
}
}  // namespace nn
}  // namespace android