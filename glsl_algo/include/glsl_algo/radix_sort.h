#ifndef GLSL_ALGO_RADIX_SORT_H
#define GLSL_ALGO_RADIX_SORT_H

#include "glsl_algo/context.h"

#ifdef __cplusplus
extern "C"{
#endif

void glsl_radix_sort_gather(const glsl_algo_gl_context *gl_context,
                       const glsl_algo_radix_sort_context *ctx,
                       GLuint input_buffer,
                       GLuint temporary_radix_buffer,
                       unsigned int num_elements,
                       unsigned int radix_offset);

void glsl_radix_sort_scatter(const glsl_algo_gl_context *gl_context,
                             const glsl_algo_radix_sort_context *ctx,
                             GLuint input_buffer,
                             GLuint temporary_radix_buffer,
                             GLuint output_buffer,
                             unsigned int num_elements,
                             unsigned int radix_offset);

void glsl_radix_sort_pass(const glsl_algo_gl_context *gl_context,
                          const glsl_algo_radix_sort_context *ctx,
                          GLuint input_buffer,
                          GLuint temporary_radix_buffer,
                          GLuint output_buffer,
                          unsigned int num_elements,
                          unsigned int radix_offset);

void glsl_radix_sort(const glsl_algo_gl_context *gl_context,
                     const glsl_algo_radix_sort_context *ctx,
                     GLuint input_buffer,
                     GLuint temporary_radix_buffer,
                     GLuint ping_pong_buffer,
                     GLuint output_buffer,
                     unsigned int num_elements);
                                                    
#ifdef __cplusplus
}
#endif

#endif // GLSL_ALGO_RADIX_SORT_H
