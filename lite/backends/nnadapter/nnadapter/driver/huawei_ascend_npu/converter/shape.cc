// Copyright (c) 2021 PaddlePaddle Authors. All Rights Reserved.
//
// Licensed under the Apache License, Version 2.0 (the "License");
// you may not use this file except in compliance with the License.
// You may obtain a copy of the License at
//
//     http://www.apache.org/licenses/LICENSE-2.0
//
// Unless required by applicable law or agreed to in writing, software
// distributed under the License is distributed on an "AS IS" BASIS,
// WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
// See the License for the specific language governing permissions and
// limitations under the License.

#include "core/operation/shape.h"
#include "driver/huawei_ascend_npu/converter.h"
#include "utility/debug.h"
#include "utility/logging.h"

namespace nnadapter {
namespace huawei_ascend_npu {

int Program::ConvertShape(hal::Operation* operation) {
  SHAPE_OPERATION_EXTRACT_INPUTS_OUTPUTS

  // Convert to GE operators
  auto input_operator = GetMappedOperator(input_operand);
  if (!input_operator) {
    input_operator = ConvertOperand(input_operand);
  }
  auto shape_name = GetOperatorName(output_operand);
  auto shape_op = std::make_shared<ge::op::Shape>(shape_name);
  switch (dtype) {
    case NNADAPTER_TENSOR_INT32:
      shape_op->set_attr_dtype(ge::DT_INT32);
      break;
    case NNADAPTER_TENSOR_INT64:
      shape_op->set_attr_dtype(ge::DT_INT64);
      break;
    default:
      NNADAPTER_LOG(ERROR) << "Unsupported output data type: "
                           << OperandPrecisionCodeToString(dtype);
  }
  SET_INPUT(shape_op, x, input_operator);
  MAP_OUTPUT(shape_op, y, output_operand);
  return NNADAPTER_NO_ERROR;
}

}  // namespace huawei_ascend_npu
}  // namespace nnadapter