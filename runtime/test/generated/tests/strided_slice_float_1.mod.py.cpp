// clang-format off
// Generated file (from: strided_slice_float_1.mod.py). Do not edit
#include "../../TestGenerated.h"

namespace strided_slice_float_1 {
// Generated strided_slice_float_1 test
#include "generated/examples/strided_slice_float_1.example.cpp"
// Generated model constructor
#include "generated/models/strided_slice_float_1.model.cpp"
} // namespace strided_slice_float_1

TEST_F(GeneratedTests, strided_slice_float_1) {
    execute(strided_slice_float_1::CreateModel,
            strided_slice_float_1::is_ignored,
            strided_slice_float_1::get_examples());
}

TEST_F(DynamicOutputShapeTest, strided_slice_float_1_dynamic_output_shape) {
    execute(strided_slice_float_1::CreateModel_dynamic_output_shape,
            strided_slice_float_1::is_ignored_dynamic_output_shape,
            strided_slice_float_1::get_examples_dynamic_output_shape());
}
