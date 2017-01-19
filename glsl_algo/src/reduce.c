#include "glsl_algo/shader_utils.h"

const char *const GLSL_ALGO_LOCAL_REDUCE_SHADER_SRC = 
SHADER(450,
layout(local_size_x=BLOCK_SIZE) in;\n

layout(location=0) uniform uint ElementsPerThread;\n
layout(location=1) uniform uint ArraySize;\n

layout(binding = 0) buffer InputArray\n
{\n
	restrict readonly TYPE inputArray[];\n
};\n

layout(binding = 1) buffer OutputBlockArray\n
{
	restrict writeonly coherent SCALAR_TYPE outputArray[];\n
};\n

shared SCALAR_TYPE blockWarpScan[WARP_SIZE];\n
shared SCALAR_TYPE sharedMem[BLOCK_SIZE];\n

SCALAR_TYPE warpReduce(in SCALAR_TYPE value, in uint localId, in uint width)\n
{\n
	sharedMem[localId] = value;\n
	uint laneIndex = localId %% WARP_SIZE;\n
	uint off = 1;\n
	while (off < width)\n
	{\n
		uint prev = localId-off;\n
		sharedMem[localId] += sharedMem[prev];\n
		off<<=1;\n
	}\n
	return sharedMem[localId];\n
}\n

void main()\n
{\n
   uint threadId = gl_WorkGroupID.x * gl_WorkGroupSize.x * ElementsPerThread + gl_LocalInvocationID.x;\n
   uint localId = gl_LocalInvocationID.x;\n
   uint laneId = localId %% WARP_SIZE;\n
   uint warpId = localId/WARP_SIZE;\n
   SCALAR_TYPE val = SCALAR_TYPE(0);\n
   for (uint i = 0; i < ElementsPerThread; ++i)\n
   {\n
      TYPE item = threadId >= ArraySize ? TYPE(0) : inputArray[threadId];\n
      threadId += gl_WorkGroupSize.x;\n
      val += SUM(item);\n
   }\n
   SCALAR_TYPE warpSum = warpReduce(val, localId, WARP_SIZE);\n
   if (laneId == WARP_SIZE-1)\n
   {\n
     blockWarpScan[warpId] = warpSum;\n
   }\n
   memoryBarrierShared();\n
   barrier();\n
   if (warpId == 0)\n
   {\n
     val = laneId*WARP_SIZE >= BLOCK_SIZE ? SCALAR_TYPE(0) : blockWarpScan[laneId];\n
     blockWarpScan[laneId] = warpReduce(val, laneId, WARP_SIZE);\n
   }\n
   if (localId == 0)\n
   {\n
     outputArray[gl_WorkGroupID.x] = blockWarpScan[WARP_SIZE-1];\n
   }\n
}\n
);