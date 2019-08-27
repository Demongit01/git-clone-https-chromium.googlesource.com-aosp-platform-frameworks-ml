// Generated from l2_normalization.mod.py
// DO NOT EDIT
// clang-format off
#include "GeneratedTests.h"


namespace generated_tests::l2_normalization {

const ::test_helper::TestModel& get_test_model();

} // namespace generated_tests::l2_normalization

namespace android::hardware::neuralnetworks::V1_2::generated_tests::l2_normalization {

TEST_F(GeneratedTest, l2_normalization) {
    Execute(device, ::generated_tests::l2_normalization::get_test_model());
}

TEST_F(DynamicOutputShapeTest, l2_normalization) {
    Execute(device, ::generated_tests::l2_normalization::get_test_model(), true);
}

TEST_F(ValidationTest, l2_normalization) {
    const Model model = createModel(::generated_tests::l2_normalization::get_test_model());
    const Request request = createRequest(::generated_tests::l2_normalization::get_test_model());
    validateEverything(model, request);
}

} // namespace android::hardware::neuralnetworks::V1_2::generated_tests::l2_normalization


namespace generated_tests::l2_normalization {

const ::test_helper::TestModel& get_test_model_all_inputs_as_internal();

} // namespace generated_tests::l2_normalization

namespace android::hardware::neuralnetworks::V1_2::generated_tests::l2_normalization {

TEST_F(GeneratedTest, l2_normalization_all_inputs_as_internal) {
    Execute(device, ::generated_tests::l2_normalization::get_test_model_all_inputs_as_internal());
}

TEST_F(DynamicOutputShapeTest, l2_normalization_all_inputs_as_internal) {
    Execute(device, ::generated_tests::l2_normalization::get_test_model_all_inputs_as_internal(), true);
}

TEST_F(ValidationTest, l2_normalization_all_inputs_as_internal) {
    const Model model = createModel(::generated_tests::l2_normalization::get_test_model_all_inputs_as_internal());
    const Request request = createRequest(::generated_tests::l2_normalization::get_test_model_all_inputs_as_internal());
    validateEverything(model, request);
}

} // namespace android::hardware::neuralnetworks::V1_2::generated_tests::l2_normalization

