#ifndef GLSL_ALGO_RADIX_SORT_H
#define GLSL_ALGO_RADIX_SORT_H

#include "glsl_algo/init.h"

#ifdef __cplusplus
extern "C"{
#endif

void glsl_radix_sort_gather(const glsl_algo_gl_context *gl_context,
                       const glsl_algo_context *ctx,
                       GLuint input_buffer,
                       GLuint temporary_radix_buffer,
                       unsigned int num_elements,
                       unsigned int rw_per_thread,
                       unsigned int radixOffset);

void glsl_radix_sort(const glsl_algo_gl_context *gl_context,
                       const glsl_algo_context *ctx,
                       GLuint input_buffer,
                       GLuint temporary_radix_buffer,
                       GLuint block_scan_buffer,
                       GLuint output_buffer,
                       unsigned int num_elements,
                       unsigned int rw_per_thread);

#ifdef __cplusplus
}
#endif

#endif // GLSL_ALGO_RADIX_SORT_H
