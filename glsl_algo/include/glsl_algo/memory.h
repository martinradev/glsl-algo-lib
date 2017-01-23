#pragma once

#include "glsl_algo/init.h"

#include <GL/gl.h>

#ifdef __cplusplus
extern "C"{
#endif

void glsl_memory_set_to_zero(const glsl_algo_context *ctx,
                             GLuint output_buffer, 
                             unsigned int num_elements,
                             unsigned int elements_per_thread);

void glsl_copy_memory(const glsl_algo_context *ctx,
                      GLuint input_buffer, 
                      GLuint output_buffer,
                      unsigned int num_elements,
                      unsigned int elements_per_thread);
                  
#ifdef __cplusplus
}
#endif