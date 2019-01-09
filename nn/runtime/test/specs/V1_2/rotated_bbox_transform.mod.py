#
# Copyright (C) 2018 The Android Open Source Project
#
# Licensed under the Apache License, Version 2.0 (the "License");
# you may not use this file except in compliance with the License.
# You may obtain a copy of the License at
#
#      http://www.apache.org/licenses/LICENSE-2.0
#
# Unless required by applicable law or agreed to in writing, software
# distributed under the License is distributed on an "AS IS" BASIS,
# WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
# See the License for the specific language governing permissions and
# limitations under the License.
#

r1 = Input("roi", "TENSOR_FLOAT32", "{8, 6}")
d1 = Input("bboxDeltas", "TENSOR_FLOAT32", "{8, 10}")
i1 = Input("imageInfo", "TENSOR_FLOAT32", "{5, 3}")
o1 = Output("out", "TENSOR_FLOAT32", "{8, 10}")
b1 = Output("batchSplit", "TENSOR_INT32", "{5}")

# TEST 1: ROTATED_BBOX_TRANSFORM
# weight = [2, 2, .5, .5], applyScale = false, angleBound = [-90, 90], clipAngleThreshold = 1.0
model1 = Model().Operation("ROTATED_BBOX_TRANSFORM", r1, d1, i1, [2., 2., 0.5, 0.5], False, True, -90, 90, 1.0).To([o1, b1])

# TEST 2: ROTATED_BBOX_TRANSFORM_2
# weight = [2, 2, .5, .5], applyScale = true,  angleBound = off,       clipAngleThreshold = 1.0
model2 = Model().Operation("ROTATED_BBOX_TRANSFORM", r1, d1, i1, [2., 2., 0.5, 0.5], True, False, -90, 90, 1.0).To([o1, b1])

# TEST 3: ROTATED_BBOX_TRANSFORM_3
# weight = [2, 2, .5, .5], applyScale = true,  angleBound = [-90, 90], clipAngleThreshold = -1.0
model3 = Model().Operation("ROTATED_BBOX_TRANSFORM", r1, d1, i1, [2., 2., 0.5, 0.5], True, True, -90, 90, -1.0).To([o1, b1])

inputs = {r1: # input 0
          [0, 250, 290, 300, 280, 0.1,
           1, 121, 60.5, 2, 0.5, -200,
           2, 15, 35, 10, 30, -58,
           2, 15, 35, 10, 30, -58,
           2, 100, 135, 50, 130, 58,
           2, 100, 135, 50, 130, -1.05,
           3, 700, 1050, 600, 1900, 185,
           3, 700, 1050, 600, 1900, -185],
          d1: # input 1
          [0.2, 0.2, 0.1, 0.1, 0.01,
           0.3, -0.1, -0.2, 0.1, -0.01,
           -0.5, 0.2, 0.2, -0.5, -0.1,
           -0.1, -0.1, 2.5, 3, 1,
           -0.5, -0.5, 1, 1, 1,
           0.5, 0.5, -1.5, -1.2, 1.7,
           -0.5, -0.5, 1, 1, 0.5,
           0.5, 0.5, -1.5, -1.2, -1.7,
           0.2, 0.2, -3, -4, -1,
           1, -0.5, 0.3, 0.5, -1,
           0.2, 0.2, -3, -4, -1.1,
           1, -0.5, 0.3, 0.5, -1.1,
           0.3, -0.2, 1.1, -0.8, -0.1,
           0.1, 0.05, -0.5, -0.5, 0.1,
           0.3, -0.2, 1.1, -0.8, -0.1,
           0.1, 0.05, -0.5, -0.5, 0.1],
          i1: # input 2
          [512, 512, 1,
           128, 256, 0.5,
           256, 256, 0.7,
           1024, 512, 2,
           1024, 512, 2]}

Example((inputs, {
    o1: # output 0
    [280.000000, 318.000000, 366.420837, 341.992798, 0.672958,
     295.000000, 276.000000, 201.096008, 341.992798, -0.472958,
     241.000000, 121.099998, 5.967299, 0.367879, -25.729584,
     241.800003, 120.949997, 250.000015, 62.500004, 37.295776,
     35.068058, 98.561310, 71.136116, 198.122620, -0.704224,
     25.000000, 60.714287, 0.711244, 3.887912, 39.402824,
     17.857143, 39.285713, 105.557945, 316.673828, -29.352112,
     25.000000, 60.714287, 0.711244, 3.887912, 24.597168,
     150.000000, 211.428574, 0.177063, 0.062286, 0.704224,
     178.571442, 182.500000, 130.151367, 366.000000, 0.704224,
     150.000000, 211.428574, 0.177054, 0.062300, -64.075356,
     178.571442, 146.428574, 130.151352, 504.823792, -64.075356,
     127.500000, 422.799561, 256.000000, 177.400848, -0.729584,
     365.000000, 548.750000, 110.363838, 349.485474, 10.729584,
     395.000000, 430.000000, 2707.504150, 191.801697, -10.729584,
     255.000000, 442.753632, 1.000000, 137.492737, 0.729584],
    b1: # batch split
    [1, 1, 4, 2, 0]
}), model=model1).AddVariations("relaxed", "float16")

Example((inputs, {
    o1: # output 0
    [280.000000, 318.000000, 366.420837, 341.992798, 0.672958,
     295.000000, 276.000000, 201.096008, 341.992798, -0.472958,
     120.500000, 60.549999, 2.983649, 0.183940, -205.729584,
     120.900002, 60.474998, 125.000008, 31.250002, -142.704224,
     24.547640, 68.992912, 49.795280, 138.685837, -0.704224,
     17.500000, 42.500000, 0.497871, 2.721538, 39.402824,
     12.500000, 27.499998, 73.890564, 221.671677, -29.352112,
     17.500000, 42.500000, 0.497871, 2.721538, -155.402832,
     105.000000, 148.000000, 0.123944, 0.043600, 0.704224,
     125.000008, 127.750000, 91.105957, 256.199982, 0.704224,
     105.000000, 148.000000, 0.123938, 0.043610, -64.075356,
     125.000008, 102.500000, 91.105942, 353.376648, -64.075356,
     790.000000, 860.000000, 5415.008301, 383.603394, 179.270416,
     730.000000, 1097.500000, 220.727676, 698.970947, 190.729584,
     790.000000, 860.000000, 5415.008301, 383.603394, -190.729584,
     730.000000, 1097.500000, 220.727676, 698.970947, -179.270416],
    b1: # batch split
    [1, 1, 4, 2, 0]
}), model=model2).AddVariations("relaxed", "float16")

Example((inputs, {
    o1: # output 0
    [280.000000, 318.000000, 366.420837, 341.992767, 0.672958,
     295.000000, 276.000000, 201.096008, 341.992767, -0.472958,
     120.500000, 60.549999, 2.983649, 0.183940, -25.729584,
     120.900002, 60.474998, 125.000008, 31.250002, 37.295776,
     12.500000, 27.499998, 73.890564, 221.671677, -0.704224,
     17.500000, 42.500000, 0.497871, 2.721538, 39.402824,
     12.500000, 27.499998, 73.890564, 221.671677, -29.352112,
     17.500000, 42.500000, 0.497871, 2.721538, 24.597168,
     105.000000, 148.000000, 0.123938, 0.043610, 0.704224,
     125.000008, 102.500000, 91.105942, 353.376648, 0.704224,
     105.000000, 148.000000, 0.123938, 0.043610, -64.075356,
     125.000008, 102.500000, 91.105942, 353.376648, -64.075356,
     790.000000, 860.000000, 5415.008301, 383.603394, -0.729584,
     730.000000, 1097.500000, 220.727676, 698.970947, 10.729584,
     790.000000, 860.000000, 5415.008301, 383.603394, -10.729584,
     730.000000, 1097.500000, 220.727676, 698.970947, 0.729584],
    b1: # batch split
    [1, 1, 4, 2, 0]
}), model=model3).AddVariations("relaxed", "float16")


# TEST 4: ROTATED_BBOX_TRANSFORM_SINGLE_BATCH
# weight = [2, 2, .5, .5], applyScale = true,  angleBound = [-90, 90], clipAngleThreshold = -1.0
r4 = Input("roi", "TENSOR_FLOAT32", "{4, 5}")
d4 = Input("bboxDeltas", "TENSOR_FLOAT32", "{4, 10}")
i4 = Input("imageInfo", "TENSOR_FLOAT32", "{1, 3}")
o4 = Output("out", "TENSOR_FLOAT32", "{4, 10}")
b4 = Output("batchSplit", "TENSOR_INT32", "{1}")
Model("single_batch").Operation("ROTATED_BBOX_TRANSFORM", r4, d4, i4, [2., 2., 0.5, 0.5], True, True, -90, 90, -1.0).To([o4, b4])

Example({
    r4: [15, 35, 10, 30, -58,
         15, 35, 10, 30, -58,
         100, 135, 50, 130, 58,
         100, 135, 50, 130, -1.05],
    d4: [-0.5, -0.5, 1, 1, 1,
         0.5, 0.5, -1.5, -1.2, 1.7,
         -0.5, -0.5, 1, 1, 0.5,
         0.5, 0.5, -1.5, -1.2, -1.7,
         0.2, 0.2, -3, -4, -1,
         1, -0.5, 0.3, 0.5, -1,
         0.2, 0.2, -3, -4, -1.1,
         1, -0.5, 0.3, 0.5, -1.1],
    i4: [256, 256, 0.7],
    o4: [12.500000, 27.499998, 73.890564, 221.671677, -0.704224,
         17.500000, 42.500000, 0.497871, 2.721538, 39.402824,
         12.500000, 27.499998, 73.890564, 221.671677, -29.352112,
         17.500000, 42.500000, 0.497871, 2.721538, 24.597168,
         105.000000, 148.000000, 0.123938, 0.043610, 0.704224,
         125.000008, 102.500000, 91.105942, 353.376648, 0.704224,
         105.000000, 148.000000, 0.123938, 0.043610, -64.075356,
         125.000008, 102.500000, 91.105942, 353.376648, -64.075356],
    b4: [4]
}).AddVariations("relaxed", "float16")
