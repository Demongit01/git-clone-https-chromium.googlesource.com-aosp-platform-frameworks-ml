// clang-format off
// Generated file (from: mul_broadcast_quant8.mod.py). Do not edit
#include "../../TestGenerated.h"

namespace mul_broadcast_quant8 {
// Generated mul_broadcast_quant8 test
#include "generated/examples/mul_broadcast_quant8.example.cpp"
// Generated model constructor
#include "generated/models/mul_broadcast_quant8.model.cpp"
} // namespace mul_broadcast_quant8

TEST_F(GeneratedTests, mul_broadcast_quant8) {
    execute(mul_broadcast_quant8::CreateModel,
            mul_broadcast_quant8::is_ignored,
            mul_broadcast_quant8::get_examples());
}

TEST_F(DynamicOutputShapeTest, mul_broadcast_quant8_dynamic_output_shape) {
    execute(mul_broadcast_quant8::CreateModel_dynamic_output_shape,
            mul_broadcast_quant8::is_ignored_dynamic_output_shape,
            mul_broadcast_quant8::get_examples_dynamic_output_shape());
}
