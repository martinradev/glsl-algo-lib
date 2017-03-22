#include "../shader_utils.h"

const char *const GLSL_ALGO_LOCAL_SCAN_SHADER_SRC = 
SHADER(450,
layout(local_size_x=BLOCK_SIZE) in;\n

layout(location=0) uniform uint ElementsPerThread;\n
layout(location=1) uniform uint ArraySize;\n
layout(location=2) uniform bool IsInclusive;\n
layout(location=3) uniform bool AddBlockOffset;\n

layout(binding = 0) buffer InputArray\n
{\n
	readonly TYPE inputArray[];\n
};\n

layout(binding = 1) buffer OutputArray\n
{
	coherent writeonly TYPE outputArray[];\n
};\n

layout(binding = 2) buffer BlockArray\n
{
	readonly SCALAR_TYPE blockArray[];\n
};\n

shared SCALAR_TYPE sharedMem[BLOCK_SIZE];\n
shared SCALAR_TYPE blockWarpScan[WARP_SIZE];\n

SCALAR_TYPE warpScanInclusive(in SCALAR_TYPE value, in uint localId, in uint laneIndex, in uint warpSize)\n
{\n
	sharedMem[localId] = value;\n
	uint off = 1;\n
	while (off < warpSize)\n
	{\n
		uint prev = localId-off;\n
		if (off <= laneIndex)\n
		{\n
			value += sharedMem[prev];\n
			sharedMem[localId] = value;\n
		}\n
		off<<=1;\n
	}\n
	return value;\n
}\n

SCALAR_TYPE warpScanExclusive(in SCALAR_TYPE value, in uint localId, in uint laneIndex, in uint warpSize)\n
{\n
	sharedMem[localId] = value;\n
	uint off = 1;\n
	while (off < warpSize)\n
	{\n
		uint prev = localId-off;\n
		if (off <= laneIndex)\n
		{\n
			value += sharedMem[prev];\n
			sharedMem[localId] = value;\n
		}\n
		off<<=1;\n
	}\n
	return laneIndex == 0 ? 0 : sharedMem[localId-1];\n
}\n

void main()\n
{\n
	uint threadId = gl_WorkGroupID.x * gl_WorkGroupSize.x * ElementsPerThread + gl_LocalInvocationID.x;\n
	uint localId = gl_LocalInvocationID.x;\n
	uint laneId = GET_LANE_ID(localId);\n
	uint warpId = GET_WARP_ID(localId);\n
	SCALAR_TYPE offset = AddBlockOffset ? blockArray[gl_WorkGroupID.x] : SCALAR_TYPE(0);\n
	for (uint i = 0; i < ElementsPerThread && threadId < ArraySize; ++i)\n
	{\n
			TYPE item = inputArray[threadId];\n
			SCALAR_TYPE val = SUM(item);\n
			SCALAR_TYPE valueInWarp = warpScanInclusive(val, localId, laneId, WARP_SIZE);\n
			
			if (laneId == WARP_SIZE-1)\n
			{\n
				blockWarpScan[warpId] = valueInWarp;\n
			}\n
			
			SCALAR_TYPE prevSharedMem = laneId == 0 ? SCALAR_TYPE(0) : sharedMem[localId-1];
			
			memoryBarrierShared();\n
		  barrier();\n

			if (warpId == 0)\n
			{\n
				val = blockWarpScan[laneId];\n
				blockWarpScan[laneId] = warpScanExclusive(val, laneId, laneId, gl_WorkGroupSize.x / WARP_SIZE);\n
			}\n
			
			memoryBarrierShared();\n
			barrier();\n
			
			SCAN(itemScan, item);\n
			if (IsInclusive)\n
			{\n
					TO_INCLUSIVE(itemScan, item);\n
			}\n
			outputArray[threadId] = TYPE(prevSharedMem+blockWarpScan[warpId]+offset)+itemScan;\n
			offset += blockWarpScan[gl_WorkGroupSize.x/WARP_SIZE-1]+sharedMem[gl_WorkGroupSize.x-1];\n
			barrier();\n
			threadId += gl_WorkGroupSize.x;\n
	}\n
}\n
);