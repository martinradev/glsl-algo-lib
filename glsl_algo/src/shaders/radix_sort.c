#include "../shader_utils.h"

/*
		BEGIN DECODE-GATHER SHADER
*/
const char *const GLSL_ALGO_RADIX_SORT_GATHER_SRC = 
SHADER(450,
layout(local_size_x=BLOCK_SIZE) in;\n

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

const uint DSIZE = 1<<RADIX_SIZE;
const uint C_DSIZE = DSIZE>>1;
const uint BIT_OFFSET = 16;

shared uint blockWarpScan[C_DSIZE][WARP_SIZE];\n
shared uint sharedMem[C_DSIZE][BLOCK_SIZE];\n

void warpReduce(inout uint value[C_DSIZE], in uint localId, in uint laneIndex)\n
{\n
	for (uint i = 0; i < C_DSIZE; ++i) sharedMem[i][localId] = value[i];\n
	uint off = 1;\n
	while (off < WARP_SIZE)\n
	{\n
		uint prev = localId-off;\n
		for (uint i = 0; i < C_DSIZE; ++i)\n
		{\n
			value[i] += sharedMem[i][prev];\n
			sharedMem[i][localId] = value[i];\n
		}\n
		off<<=1;\n
	}\n
}\n

TYPE decode(in TYPE item, in uint radixOffset)\n
{\n
    return (item >> radixOffset) & TYPE(DSIZE-1);\n
}\n

void main()\n
{\n
   uint threadId = gl_WorkGroupID.x * gl_WorkGroupSize.x * RADIX_SORT_NUM_PASSES + gl_LocalInvocationID.x;\n
   uint localId = gl_LocalInvocationID.x;\n
   uint laneId = GET_LANE_ID(localId);\n
   uint warpId = GET_WARP_ID(localId);\n
   if (warpId == 0)\n
   {\n
		for (uint i = 0; i < C_DSIZE; ++i) blockWarpScan[i][laneId] = 0;\n
   }\n
   memoryBarrierShared();\n
   barrier();\n
   uint val[C_DSIZE];\n
   for (uint i = 0; i < C_DSIZE; ++i) val[i] = 0;\n
   uint i = 0;\n
   while(threadId < ArraySize && i < RADIX_SORT_NUM_PASSES)\n
   {\n
      TYPE item = inputArray[threadId];\n
      threadId += gl_WorkGroupSize.x;\n
      TYPE qq = decode(item, RadixOffset);\n
	  for (uint j = 0; j < ELEMENTS_PER_THREAD; ++j)\n
	  {\n
		uint idx = READ_INDEX(qq,j);\n
		val[idx>>1] += 1<<((idx&1)*BIT_OFFSET);\n
	  }\n
	  ++i;\n
   }\n
   
   warpReduce(val, localId, laneId);\n
   if (laneId == WARP_SIZE-1)\n
   {\n
		for (uint i = 0; i < C_DSIZE; ++i) blockWarpScan[i][warpId] = val[i];\n
   }\n
   memoryBarrierShared();\n
   barrier();\n
   if (warpId == 0)\n
   {\n
		for (uint i = 0; i < C_DSIZE; ++i)\n
		{\n
			val[i] = blockWarpScan[i][laneId];\n
		}\n
		warpReduce(val, laneId, laneId);\n
		if (localId == WARP_SIZE-1)\n
		{\n
			for (uint i = 0; i < C_DSIZE; ++i)\n
			{\n
				outputArray[gl_WorkGroupID.x+2*i*gl_NumWorkGroups.x] = val[i]&((1<<BIT_OFFSET)-1);\n
				outputArray[gl_WorkGroupID.x+(2*i+1)*gl_NumWorkGroups.x] = val[i]>>BIT_OFFSET;\n
			}\n
		}\n
   }\n
   
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
	readonly SCALAR_TYPE inputArray[];\n
};\n

layout(binding = 1) buffer OutputArray\n
{
	coherent writeonly SCALAR_TYPE outputArray[];\n
};\n

layout(binding = 2) buffer BlockArray\n
{
	readonly SCALAR_TYPE blockArray[];\n
};\n

const uint N_PASSES = RADIX_SORT_NUM_PASSES;
const uint DSIZE = 1<<RADIX_SIZE;
const uint C_DSIZE = DSIZE>>1;
const uint BIT_OFFSET = 16;

shared SCALAR_TYPE blockWarpScan[C_DSIZE][WARP_SIZE];\n
shared uint sharedOffsets[DSIZE];\n
shared SCALAR_TYPE sharedReadInput[max(BLOCK_SIZE*C_DSIZE, BLOCK_SIZE*N_PASSES*ELEMENTS_PER_THREAD)];\n
shared uint sharedOutIndices[BLOCK_SIZE*N_PASSES*ELEMENTS_PER_THREAD];\n

void warpScanInclusive(inout SCALAR_TYPE value[C_DSIZE], in uint localId, in uint laneIndex, in uint warpSize)\n
{\n
	for (uint i = 0; i < C_DSIZE; ++i) sharedReadInput[i*BLOCK_SIZE+localId] = value[i];\n
	uint off = 1;\n
	while (off < warpSize)\n
	{\n
		uint prev = localId-off;\n
		if (off <= laneIndex)\n
		{\n
			for (uint i = 0; i < C_DSIZE; ++i)\n
			{\n
				value[i] += sharedReadInput[i*BLOCK_SIZE+prev];\n
				sharedReadInput[i*BLOCK_SIZE+localId] = value[i];\n
			}\n
		}\n
		off<<=1;\n
	}\n
}\n

SCALAR_TYPE decode(in SCALAR_TYPE item, in uint radixOffset)\n
{\n
    return (item >> radixOffset) & SCALAR_TYPE(DSIZE-1);\n
}\n

shared int VoteExit;

void main()\n
{\n
		uint threadId = gl_WorkGroupID.x * gl_WorkGroupSize.x * N_PASSES * ELEMENTS_PER_THREAD + gl_LocalInvocationID.x;\n
		uint localId = gl_LocalInvocationID.x;\n
		uint laneId = GET_LANE_ID(localId);\n
		uint warpId = GET_WARP_ID(localId);\n
		
		if (localId == 0)\n
		{\n
			VoteExit = -1;\n
		}\n
		
		memoryBarrierShared();\n
		barrier();\n
		
		if (localId < DSIZE)\n
		{\n
				int index = int(gl_NumWorkGroups.x*localId + gl_WorkGroupID.x - 1);\n
				uint tmp = index < 0 ? 0 : blockArray[index];\n
				if ((blockArray[index+1]-tmp) == (gl_WorkGroupSize.x * N_PASSES * ELEMENTS_PER_THREAD))\n
				{\n
					VoteExit = int(localId);\n
				}\n
				sharedOffsets[localId] = tmp;\n
		}\n
		
		memoryBarrierShared();\n
		barrier();\n
		
		if (VoteExit != -1)\n
		{\n
			localId += sharedOffsets[VoteExit];
			for (uint i = 0; i < N_PASSES*ELEMENTS_PER_THREAD && threadId < ArraySize; ++i)\n
			{\n
				outputArray[localId] = inputArray[threadId];\n
				localId += gl_WorkGroupSize.x;\n
				threadId += gl_WorkGroupSize.x;\n
			}\n
			return;\n
		}\n
		
		uint tmpIdx = localId;\n
		
		for (uint i = 0; i < N_PASSES*ELEMENTS_PER_THREAD && threadId < ArraySize; ++i)\n
		{\n
			sharedReadInput[tmpIdx] = inputArray[threadId];\n
			threadId += gl_WorkGroupSize.x;\n
			tmpIdx += gl_WorkGroupSize.x;\n
		}\n
		
		memoryBarrierShared();\n
		barrier();\n

		SCALAR_TYPE tmp[N_PASSES*ELEMENTS_PER_THREAD];\n
		for (uint i = 0; i < N_PASSES*ELEMENTS_PER_THREAD; ++i)\n
		{\n
			tmp[i] = sharedReadInput[localId*N_PASSES*ELEMENTS_PER_THREAD+i];\n
		}\n
		
		uint val[C_DSIZE];\n
		for (uint i = 0; i < C_DSIZE; ++i) val[i] = 0;\n
		
		for (uint i = 0; i < N_PASSES*ELEMENTS_PER_THREAD; ++i)\n
		{\n
			SCALAR_TYPE idx = decode(tmp[i], RadixOffset);\n
			val[idx>>1] += 1<<((idx&1)*BIT_OFFSET);\n
		}\n
		
		uint threadOffset[C_DSIZE];\n
		
		// begin scan\n
		
		warpScanInclusive(val, localId, laneId, WARP_SIZE);\n
		if (laneId == WARP_SIZE-1)\n
		{\n
			for (uint i = 0; i < C_DSIZE; ++i) blockWarpScan[i][warpId] = val[i];\n
		}\n
		
		if (laneId == 0)\n
		{\n
			for (uint i = 0; i < C_DSIZE; ++i) threadOffset[i] = 0;\n
		}\n
		else\n
		{\n
			for (uint i = 0; i < C_DSIZE; ++i) threadOffset[i] = sharedReadInput[BLOCK_SIZE*i + localId-1];\n	
		}\n
		
		memoryBarrierShared();\n
		barrier();\n
		
		if (warpId == 0)\n
		{\n
			for (uint i = 0; i < C_DSIZE; ++i) val[i] = blockWarpScan[i][laneId];\n
			warpScanInclusive(val, laneId, laneId, gl_WorkGroupSize.x / WARP_SIZE);\n
			for (uint i = 0; i < C_DSIZE; ++i) blockWarpScan[i][laneId] = val[i];
		}\n
		
		memoryBarrierShared();\n
		barrier();\n
		
		if (warpId != 0)\n
		{\n
			for (uint i = 0; i < C_DSIZE; ++i) threadOffset[i] += blockWarpScan[i][warpId-1];\n
 		}\n
		
		// end scan \n
		
		barrier();\n
		
		uint totalSum = blockWarpScan[0][gl_WorkGroupSize.x/WARP_SIZE-1];\n
		val[0] = (totalSum<<BIT_OFFSET);\n;
		totalSum += (totalSum<<BIT_OFFSET);\n
		
		for (uint i = 1; i < C_DSIZE; ++i)\n
		{\n
			uint ss = blockWarpScan[i][gl_WorkGroupSize.x/WARP_SIZE-1];\n
			val[i] = totalSum>>BIT_OFFSET;\n
			val[i] |= (val[i]+ss)<<BIT_OFFSET;\n
			totalSum += ss;\n
			totalSum += (ss<<BIT_OFFSET);\n
		}\n
		
		for (uint i = 0; i < N_PASSES*ELEMENTS_PER_THREAD; ++i)\n
		{\n
			SCALAR_TYPE qq = decode(tmp[i], RadixOffset);\n
			SCALAR_TYPE qq2 = qq>>1;\n
			SCALAR_TYPE qq3 = qq&1;\n
			SCALAR_TYPE mask = (1<<BIT_OFFSET)-1;
			uint shIdx = (threadOffset[qq2]>>(qq3 * BIT_OFFSET))&mask;\n
			threadOffset[qq2]+=(1<<(qq3*BIT_OFFSET));\n
			SCALAR_TYPE tt = ((val[qq2]>>(qq3 * BIT_OFFSET))&mask)+shIdx;
			sharedReadInput[tt] = tmp[i];\n
			sharedOutIndices[tt] = shIdx+sharedOffsets[qq];\n
		}\n
		
		memoryBarrierShared();\n
		barrier();\n
		
		for (uint i = 0; i < N_PASSES*ELEMENTS_PER_THREAD; ++i)\n
		{\n
			uint idx = (i*gl_WorkGroupSize.x + localId);\n
			outputArray[sharedOutIndices[idx]] = sharedReadInput[idx];\n
		}\n
}\n
);
