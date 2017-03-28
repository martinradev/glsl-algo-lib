#include "../shader_utils.h"

/*
		BEGIN DECODE-GATHER SHADER
*/
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
		value += sharedMem[prev];\n
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
	 
	 uint offset = 0u;\n
	 for (int i = 0; i < (1<<RADIX_SIZE)-1; ++i)\n
	 {\n
		 	reduceRadix(localId, laneId, warpId, val[i], offset);\n
			offset += gl_NumWorkGroups.x;\n
			barrier();\n
	 }\n
	 reduceRadix(localId, laneId, warpId, val[(1<<RADIX_SIZE)-1], offset);\n
}\n
);

/*
		BEGIN SCATTER SHADER
*/
const char *const GLSL_ALGO_RADIX_SORT_SCATTER_SRC = 
SHADER(450,
layout(local_size_x=BLOCK_SIZE) in;\n

layout(location=0) uniform uint ElementsPerThread;\n
layout(location=1) uniform uint ArraySize;\n
layout(location=2) uniform uint RadixOffset;\n

layout(binding = 0) buffer InputArray\n
{\n
	readonly TYPE inputArray[];\n
};\n

layout(binding = 1) buffer OutputArray\n
{
	coherent writeonly SCALAR_TYPE outputArray[];\n
};\n

layout(binding = 2) buffer BlockArray\n
{
	readonly SCALAR_TYPE blockArray[];\n
};\n

shared SCALAR_TYPE sharedMem[BLOCK_SIZE];\n
shared SCALAR_TYPE blockWarpScan[WARP_SIZE];\n
shared int sharedOffsets[1<<RADIX_SIZE];\n

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

TYPE decode(in TYPE item, in uint radixOffset)\n
{\n
    return (item >> radixOffset) & TYPE((1<<RADIX_SIZE)-1);\n
}\n

void blockScan(in int val, in uint localId, in uint laneId, in uint warpId, out int threadOffset, out int sum)\n
{\n
		SCALAR_TYPE valueInWarp = warpScanInclusive(val, localId, laneId, WARP_SIZE);\n
		if (laneId == WARP_SIZE-1)\n
		{\n
			blockWarpScan[warpId] = valueInWarp;\n
		}\n
		
		SCALAR_TYPE prevSharedMem = laneId == 0 ? SCALAR_TYPE(0) : sharedMem[localId-1];\n
		
		memoryBarrierShared();\n
		barrier();\n
		
		if (warpId == 0)\n
		{\n
			int tmp = blockWarpScan[laneId];\n
			blockWarpScan[laneId] = warpScanInclusive(tmp, laneId, laneId, gl_WorkGroupSize.x / WARP_SIZE);\n
		}\n
		
		memoryBarrierShared();\n
		barrier();\n

		threadOffset = prevSharedMem+(warpId == 0 ? 0 : blockWarpScan[warpId-1]);\n
		sum = blockWarpScan[gl_WorkGroupSize.x/WARP_SIZE-1];\n
}\n

void scatter(in uint threadOffset, in int radix, in TYPE item, in TYPE qq)\n
{\n
		uint cc = 0;\n
		for (uint j = 0; j < ELEMENTS_PER_THREAD; ++j)\n
		{\n
				if (READ_INDEX(qq,j) == radix)\n
				{\n
						uint idx = threadOffset+cc;\n
						outputArray[idx] = READ_INDEX(item,j);\n
						++cc;\n
				}\n
		}\n
}\n

void main()\n
{\n
		uint threadId = gl_WorkGroupID.x * gl_WorkGroupSize.x * ElementsPerThread + gl_LocalInvocationID.x;\n
		uint localId = gl_LocalInvocationID.x;\n
		uint laneId = GET_LANE_ID(localId);\n
		uint warpId = GET_WARP_ID(localId);\n
		
		if (localId < (1<<RADIX_SIZE))\n
		{\n
				uint index = max(0, gl_NumWorkGroups.x*localId + gl_WorkGroupID.x - 1);\n
				sharedOffsets[localId] = blockArray[index];\n
		}\n
		
		memoryBarrierShared();\n
		barrier();\n

		int val[1<<RADIX_SIZE];\n
		
		for (uint i = 0; i < ElementsPerThread && threadId < ArraySize; ++i)\n
		{\n
		 	 	for (int j = 0; j < (1<<RADIX_SIZE); ++j) val[j] = 0;\n

				TYPE item = inputArray[threadId];\n
				TYPE qq = decode(item, RadixOffset);\n
				for (uint j = 0; j < ELEMENTS_PER_THREAD; ++j)\n
				{\n
						val[READ_INDEX(qq,j)] += 1;\n
				}\n

				int threadOffset;\n
				int sumTmp;\n
				blockScan(val[0], localId, laneId, warpId, threadOffset, sumTmp);\n
				threadOffset += sharedOffsets[0];\n
				barrier();\n
				if (localId==0)\n
				{\n
						sharedOffsets[0] += sumTmp;\n
				}\n
				scatter(threadOffset, 0, item, qq);\n
				
				for (int j = 1; j < (1<<RADIX_SIZE); ++j)\n
				{\n
						barrier();\n
						blockScan(val[j], localId, laneId, warpId, threadOffset, sumTmp);\n
						threadOffset += sharedOffsets[j];\n
						barrier();\n
						if (localId==0)\n
						{\n
						sharedOffsets[j] += sumTmp;\n
						}\n
						scatter(threadOffset, j, item, qq);\n
				}\n
				threadId += gl_WorkGroupSize.x;\n
				barrier();\n
		}\n
}\n
);