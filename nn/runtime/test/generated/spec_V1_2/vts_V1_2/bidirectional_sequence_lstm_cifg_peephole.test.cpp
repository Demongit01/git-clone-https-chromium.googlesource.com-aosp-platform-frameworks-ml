// Generated from bidirectional_sequence_lstm_cifg_peephole.mod.py
// DO NOT EDIT
// clang-format off
#include "GeneratedTests.h"


namespace generated_tests::bidirectional_sequence_lstm_cifg_peephole {

const ::test_helper::TestModel& get_test_model_blackbox();

} // namespace generated_tests::bidirectional_sequence_lstm_cifg_peephole

namespace android::hardware::neuralnetworks::V1_2::generated_tests::bidirectional_sequence_lstm_cifg_peephole {

TEST_F(GeneratedTest, bidirectional_sequence_lstm_cifg_peephole_blackbox) {
    Execute(device, ::generated_tests::bidirectional_sequence_lstm_cifg_peephole::get_test_model_blackbox());
}

TEST_F(DynamicOutputShapeTest, bidirectional_sequence_lstm_cifg_peephole_blackbox) {
    Execute(device, ::generated_tests::bidirectional_sequence_lstm_cifg_peephole::get_test_model_blackbox(), true);
}

TEST_F(ValidationTest, bidirectional_sequence_lstm_cifg_peephole_blackbox) {
    const Model model = createModel(::generated_tests::bidirectional_sequence_lstm_cifg_peephole::get_test_model_blackbox());
    const Request request = createRequest(::generated_tests::bidirectional_sequence_lstm_cifg_peephole::get_test_model_blackbox());
    validateEverything(model, request);
}

} // namespace android::hardware::neuralnetworks::V1_2::generated_tests::bidirectional_sequence_lstm_cifg_peephole


namespace generated_tests::bidirectional_sequence_lstm_cifg_peephole {

const ::test_helper::TestModel& get_test_model_blackbox_all_inputs_as_internal();

} // namespace generated_tests::bidirectional_sequence_lstm_cifg_peephole

namespace android::hardware::neuralnetworks::V1_2::generated_tests::bidirectional_sequence_lstm_cifg_peephole {

TEST_F(GeneratedTest, bidirectional_sequence_lstm_cifg_peephole_blackbox_all_inputs_as_internal) {
    Execute(device, ::generated_tests::bidirectional_sequence_lstm_cifg_peephole::get_test_model_blackbox_all_inputs_as_internal());
}

TEST_F(DynamicOutputShapeTest, bidirectional_sequence_lstm_cifg_peephole_blackbox_all_inputs_as_internal) {
    Execute(device, ::generated_tests::bidirectional_sequence_lstm_cifg_peephole::get_test_model_blackbox_all_inputs_as_internal(), true);
}

TEST_F(ValidationTest, bidirectional_sequence_lstm_cifg_peephole_blackbox_all_inputs_as_internal) {
    const Model model = createModel(::generated_tests::bidirectional_sequence_lstm_cifg_peephole::get_test_model_blackbox_all_inputs_as_internal());
    const Request request = createRequest(::generated_tests::bidirectional_sequence_lstm_cifg_peephole::get_test_model_blackbox_all_inputs_as_internal());
    validateEverything(model, request);
}

} // namespace android::hardware::neuralnetworks::V1_2::generated_tests::bidirectional_sequence_lstm_cifg_peephole

