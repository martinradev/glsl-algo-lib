#ifndef GLSL_ALGO_PREFIX_SCAN_H
#define GLSL_ALGO_PREFIX_SCAN_H

#include "init.h"

#include <GL/gl.h>

#ifdef __cplusplus
extern "C"{
#endif

void glsl_local_reduce(const glsl_algo_context *ctx,
                       GLuint input_buffer, 
                       GLuint output_buffer,
                       unsigned int num_elements,
                       unsigned int block_size);

void glsl_local_scan(const glsl_algo_context *ctx,
                       GLuint input_buffer, 
                       GLuint output_buffer,
                       unsigned int num_elements,
                       unsigned int block_size,
                       unsigned char is_inclusive);

#ifdef __cplusplus
}
#endif

#endif // GLSL_ALGO_PREFIX_SCAN_H