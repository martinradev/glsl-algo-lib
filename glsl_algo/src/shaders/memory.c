#include "glsl_algo/shader_utils.h"

const char *const GLSL_ALGO_WRITE_TO_MEMORY_SRC = 
SHADER(450,
layout(local_size_x=BLOCK_SIZE) in;\n

layout(location=0) uniform uint ElementsPerThread;\n
layout(location=1) uniform uint ArraySize;\n

layout(binding = 0) buffer InputArray\n
{\n
	restrict readonly TYPE inputArray[];\n
};\n

DEFINE_MACRO COPY_FROM_BUFFER %u\n
IF_MACRO COPY_FROM_BUFFER == 1\n
layout(binding = 1) buffer OutputArray\n
{
	restrict writeonly coherent SCALAR_TYPE outputArray[];\n
};\n
ELSE_MACRO\n
  layout(location=2) uniform SCALAR_TYPE Value;\n
ENDIF_MACRO

void main()\n
{\n
   uint threadId = gl_WorkGroupID.x * gl_WorkGroupSize.x * ElementsPerThread + gl_LocalInvocationID.x;\n
   for (uint i = 0; i < ElementsPerThread; ++i)\n
   {\n
      if (threadId >= ArraySize) return;\n
      IF_MACRO COPY_FROM_BUFFER == 1\n
        outputArray[threadId] = inputArray[threadId];\n
      ELSE_MACRO\n
        outputArray[threadId] = TYPE(Value);\n
      ENDIF_MACRO\n
      TYPE item = inputArray[threadId];\n
      threadId += gl_WorkGroupSize.x;\n
   }\n
}\n
);