#ifndef GLSL_ALGO_PREFIX_SCAN_H
#define GLSL_ALGO_PREFIX_SCAN_H

#include "glsl_algo/init.h"

#ifdef __cplusplus
extern "C"{
#endif

void glsl_local_reduce(const glsl_algo_gl_context *gl_context,
                       const glsl_algo_context *ctx,
                       GLuint input_buffer,
                       GLuint output_buffer,
                       unsigned int num_elements,
                       unsigned int block_size);

void glsl_local_scan(const glsl_algo_gl_context *gl_context,
                     const glsl_algo_context *ctx,
                     GLuint input_buffer,
                     GLuint output_buffer,
                     unsigned int num_elements,
                     unsigned int block_size,
                     unsigned char is_inclusive);

void glsl_scan(const glsl_algo_gl_context *gl_context,
               const glsl_algo_context *ctx,
               GLuint input_buffer,
               GLuint intermediate_buffer,
               GLuint output_buffer,
               unsigned int num_elements,
               unsigned int rw_per_thread,
               unsigned char is_inclusive);

#ifdef __cplusplus
}
#endif

#endif // GLSL_ALGO_PREFIX_SCAN_H
