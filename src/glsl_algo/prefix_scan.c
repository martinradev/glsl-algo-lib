#include "prefix_scan.h"

#include <assert.h>
#include <stdio.h>

void glsl_local_reduce(const glsl_algo_context *ctx,
                       GLuint input_buffer, 
                       GLuint output_buffer,
                       unsigned int num_elements,
                       unsigned int block_size)
{
    unsigned superScalarNumElements = glsl_algo_get_rw_num_elements(ctx->conf.rw_type);
    unsigned threadBlockSize = superScalarNumElements*ctx->conf.local_block_size;
    assert(block_size % threadBlockSize == 0);
    
    unsigned elementsProcessedPerThread = block_size / threadBlockSize;
    unsigned gridSize = (num_elements + threadBlockSize - 1u) / threadBlockSize;
    unsigned adjustedArraySize = (num_elements + superScalarNumElements - 1u) / superScalarNumElements;
    
    glUseProgram(ctx->reduce_program);
    glUniform1ui(0, elementsProcessedPerThread);
    glUniform1ui(1, adjustedArraySize);
    
    glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 0, input_buffer);
    glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 1, output_buffer);
    
    glDispatchCompute(gridSize, 1, 1);
    
    glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 0, 0);
    glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 1, 0);
    
    glUseProgram(0);
}