# A basic configurator which generates the best config.h and glsl shaders for you.

import os

warpSize = 32
vectorSize = 4
scanReduceBlockSize = 1024
scanBlockBlockSize = 1024

base = os.path.dirname(__file__)

fileHandle = open(base + "/../glsl_algo/config_orig.h", "r")
fileContent = fileHandle.read()
fileContent = fileContent.replace("$(WARP_SIZE)", str(warpSize))
fileContent = fileContent.replace("$(VECTOR_SIZE)", str(vectorSize))
fileContent = fileContent.replace("$(SCAN_REDUCE_THREAD_BLOCK_SIZE)", str(scanReduceBlockSize))
fileContent = fileContent.replace("$(SCAN_BLOCKS_THREAD_BLOCK_SIZE)", str(scanBlockBlockSize))
fileHandle.close()

fileHandle = open(base + "/../glsl_algo/config.h", "w")

fileHandle.seek(0)
fileHandle.write(fileContent)
fileHandle.close()
