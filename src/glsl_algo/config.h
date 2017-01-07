#ifndef GLSL_ALGO_CONFIG_H
#define GLSL_ALGO_CONFIG_H

// Warp size for the device.
#define WARP_SIZE 32

// Number of threads running every clock per processor.
#define CORES 384

// Best read/write size.
#define VECTOR_SIZE 4

// Thread block size when doing reduction and scan (Step 1 and 3 of scan).
#define SCAN_REDUCE_THREAD_BLOCK_SIZE 1024

// Thread block size when doing a scan of the reduced blocks (Step 2 of scan).
#define SCAN_BLOCKS_THREAD_BLOCK_SIZE 1024

#endif // GLSL_ALGO_CONFIG_H