#include "glsl_algo/memory.h"

void glsl_memory_set_to_zero(const glsl_algo_context *ctx,
                             GLuint input_buffer, 
                             unsigned int num_elements,
                             unsigned int elements_per_thread)
{
    /*assert(elements_per_thread >= 1u);
    unsigned superScalarNumElements = glsl_algo_get_rw_num_elements(ctx->conf.rw_type);
    unsigned threadBlockSize = superScalarNumElements*ctx->conf.local_block_size;

    unsigned gridSize = (num_elements + block_size - 1u) / block_size;
    unsigned adjustedArraySize = (num_elements + superScalarNumElements - 1u) / superScalarNumElements;
    
    glUseProgram(ctx->reduce_program);
    glUniform1ui(0, elementsProcessedPerThread);
    glUniform1ui(1, adjustedArraySize);
    
    glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 0, input_buffer);
    glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 1, output_buffer);
    
    glDispatchCompute(gridSize, 1, 1);
    
    glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 0, 0);
    glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 1, 0);
    
    glUseProgram(0);*/
}
                                  
void glsl_copy_memory(const glsl_algo_context *ctx,
                      GLuint input_buffer,
                      GLuint output_buffer,
                      unsigned int num_elements,
                      unsigned int elements_per_thread)
{
  
}