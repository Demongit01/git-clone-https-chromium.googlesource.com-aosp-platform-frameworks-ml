// Generated from bidirectional_sequence_lstm.mod.py
// DO NOT EDIT
// clang-format off
#include "TestGenerated.h"


namespace generated_tests::bidirectional_sequence_lstm {

const ::test_helper::TestModel& get_test_model_blackbox();

TEST_F(GeneratedTests, bidirectional_sequence_lstm_blackbox) { execute(get_test_model_blackbox()); }

TEST_F(DynamicOutputShapeTest, bidirectional_sequence_lstm_blackbox) { execute(get_test_model_blackbox()); }

} // namespace generated_tests::bidirectional_sequence_lstm

TEST_AVAILABLE_SINCE(V1_2, bidirectional_sequence_lstm_blackbox, generated_tests::bidirectional_sequence_lstm::get_test_model_blackbox());


namespace generated_tests::bidirectional_sequence_lstm {

const ::test_helper::TestModel& get_test_model_blackbox_all_inputs_as_internal();

TEST_F(GeneratedTests, bidirectional_sequence_lstm_blackbox_all_inputs_as_internal) { execute(get_test_model_blackbox_all_inputs_as_internal()); }

TEST_F(DynamicOutputShapeTest, bidirectional_sequence_lstm_blackbox_all_inputs_as_internal) { execute(get_test_model_blackbox_all_inputs_as_internal()); }

} // namespace generated_tests::bidirectional_sequence_lstm

TEST_AVAILABLE_SINCE(V1_2, bidirectional_sequence_lstm_blackbox_all_inputs_as_internal, generated_tests::bidirectional_sequence_lstm::get_test_model_blackbox_all_inputs_as_internal());

