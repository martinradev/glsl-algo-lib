#include "glsl_algo/shader_utils.h"

const char *const GLSL_ALGO_SET_MEMORY_TO_ZERO_SRC = 
SHADER(450,
layout(local_size_x=BLOCK_SIZE) in;\n

layout(location=0) uniform uint ElementsPerThread;\n
layout(location=1) uniform uint ArraySize;\n

layout(binding = 0) buffer OutputArray\n
{\n
	restrict writeonly coherent TYPE outputArray[];\n
};\n

void main()\n
{\n
   uint threadId = gl_WorkGroupID.x * gl_WorkGroupSize.x * ElementsPerThread + gl_LocalInvocationID.x;\n
   for (uint i = 0; i < ElementsPerThread; ++i)\n
   {\n
      if (threadId >= ArraySize) return;\n
      outputArray[threadId] = TYPE(0);\n
      threadId += gl_WorkGroupSize.x;\n
   }\n
}\n
);

const char *const GLSL_ALGO_COPY_MEMORY_SRC = 
SHADER(450,
layout(local_size_x=BLOCK_SIZE) in;\n

layout(location=0) uniform uint ElementsPerThread;\n
layout(location=1) uniform uint ArraySize;\n

layout(binding = 0) buffer InputArray\n
{\n
	restrict readonly coherent TYPE inputArray[];\n
};\n

layout(binding = 1) buffer OutputArray\n
{\n
	restrict writeonly coherent TYPE outputArray[];\n
};\n

void main()\n
{\n
   uint threadId = gl_WorkGroupID.x * gl_WorkGroupSize.x * ElementsPerThread + gl_LocalInvocationID.x;\n
   for (uint i = 0; i < ElementsPerThread; ++i)\n
   {\n
      if (threadId >= ArraySize) return;\n
      outputArray[threadId] = inputArray[threadId];\n
      threadId += gl_WorkGroupSize.x;\n
   }\n
}\n
);