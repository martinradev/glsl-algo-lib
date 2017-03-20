#include "glsl_algo/radix_sort.h"
#include "glsl_algo/prefix_scan.h"

void glsl_radix_sort_gather(const glsl_algo_gl_context *gl,
                       const glsl_algo_context *ctx,
                       GLuint input_buffer,
                       GLuint temporary_radix_buffer,
                       unsigned int num_elements,
                       unsigned int rw_per_thread,
                       unsigned int radixOffset)
{
  unsigned superScalarNumElements = glsl_algo_get_rw_num_elements(ctx->conf.rw_type);
  unsigned block_size = ctx->conf.local_block_size * superScalarNumElements * rw_per_thread;
  unsigned gridSize = (num_elements + block_size - 1u) / block_size;
  unsigned adjustedArraySize = (num_elements + superScalarNumElements - 1u) / superScalarNumElements;
  
  gl->glUseProgram(ctx->radix_sort_gather_program);
  gl->glUniform1ui(0, rw_per_thread);
  gl->glUniform1ui(1, adjustedArraySize);
  gl->glUniform1ui(2, radixOffset);
  
  gl->glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 0, input_buffer);
  gl->glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 1, temporary_radix_buffer);
  
  gl->glDispatchCompute(gridSize, 1, 1);
  
  gl->glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 0, 0);
  gl->glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 1, 0);
  
  gl->glUseProgram(0);
}

void glsl_radix_sort(const glsl_algo_gl_context *gl_context,
                       const glsl_algo_context *ctx,
                       GLuint input_buffer,
                       GLuint temporary_radix_buffer,
                       GLuint block_scan_buffer,
                       GLuint output_buffer,
                       unsigned int num_elements,
                       unsigned int rw_per_thread)
{
  
}
