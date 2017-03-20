#include "../shader_utils.h"

const char *const GLSL_ALGO_RADIX_SORT_GATHER_SRC = 
SHADER(450,
layout(local_size_x=BLOCK_SIZE) in;\n

layout(location=0) uniform uint ElementsPerThread;\n
layout(location=1) uniform uint ArraySize;\n
layout(location=2) uniform uint RadixOffset;\n

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

SCALAR_TYPE warpReduce(in SCALAR_TYPE value, in uint localId, in uint laneIndex)\n
{\n
	sharedMem[localId] = value;\n
	uint off = 1;\n
	while (off < WARP_SIZE)\n
	{\n
		uint prev = localId-off;\n
		value += sharedMem[prev];
		sharedMem[localId] = value;\n
		off<<=1;\n
	}\n
	return value;\n
}\n

TYPE decode(in TYPE item, in uint radixOffset)\n
{\n
    return (item >> radixOffset) & TYPE((1<<RADIX_SIZE)-1);\n
}\n

void reduceRadix(in uint localId, in uint laneId, in uint warpId, inout int value, in uint offset)\n
{\n
	SCALAR_TYPE warpSum = warpReduce(value, localId, laneId);\n
	if (laneId == WARP_SIZE-1)\n
	{\n
		blockWarpScan[warpId] = warpSum;\n
	}\n
	memoryBarrierShared();\n
	barrier();\n
	if (warpId == 0)\n
	{\n
		value = blockWarpScan[laneId];\n
		value = warpReduce(value, laneId, laneId);\n
		if (localId == WARP_SIZE-1)\n
		{\n
			outputArray[gl_WorkGroupID.x+offset] = value;\n
		}\n
	}\n
}\n

void main()\n
{\n
   uint threadId = gl_WorkGroupID.x * gl_WorkGroupSize.x * ElementsPerThread + gl_LocalInvocationID.x;\n
   uint localId = gl_LocalInvocationID.x;\n
   uint laneId = GET_LANE_ID(localId);\n
   uint warpId = GET_WARP_ID(localId);\n
	 if (warpId == 0)\n
   {\n
		 		blockWarpScan[laneId] = SCALAR_TYPE(0);\n
	 }\n
	 memoryBarrierShared();\n
   barrier();\n
   int val[1<<RADIX_SIZE];\n
	 for (int i = 0; i < (1<<RADIX_SIZE); ++i) val[i] = 0;\n
	 uint i = 0;\n
   while(threadId < ArraySize && i < ElementsPerThread)\n
   {\n
      TYPE item = inputArray[threadId];\n
      threadId += gl_WorkGroupSize.x;\n
      TYPE qq = decode(item, RadixOffset);\n
			for (uint j = 0; j < ELEMENTS_PER_THREAD; ++j)\n
			{\n
					val[READ_INDEX(qq,j)] += 1;\n
			}\n
			++i;\n
   }\n
	 
	 uint offset = 0u;
	 for (int i = 0; i < (1<<RADIX_SIZE)-1; ++i)
	 {
		 	reduceRadix(localId, laneId, warpId, val[i], offset);\n
			offset += gl_NumWorkGroups.x;
			barrier();\n
	 }
	 reduceRadix(localId, laneId, warpId, val[(1<<RADIX_SIZE)-1], offset);\n
}\n
);