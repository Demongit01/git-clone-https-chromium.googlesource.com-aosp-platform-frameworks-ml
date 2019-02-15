// clang-format off
// Generated file (from: logistic_float16_2.mod.py). Do not edit
#include "../../TestGenerated.h"

namespace logistic_float16_2 {
// Generated logistic_float16_2 test
#include "generated/examples/logistic_float16_2.example.cpp"
// Generated model constructor
#include "generated/models/logistic_float16_2.model.cpp"
} // namespace logistic_float16_2

TEST_F(GeneratedTests, logistic_float16_2) {
    execute(logistic_float16_2::CreateModel,
            logistic_float16_2::is_ignored,
            logistic_float16_2::get_examples());
}

TEST_F(DynamicOutputShapeTest, logistic_float16_2_dynamic_output_shape) {
    execute(logistic_float16_2::CreateModel_dynamic_output_shape,
            logistic_float16_2::is_ignored_dynamic_output_shape,
            logistic_float16_2::get_examples_dynamic_output_shape());
}
