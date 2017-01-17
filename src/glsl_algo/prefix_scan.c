#include "prefix_scan.h"

#include <assert.h>
#include <stdio.h>

void glsl_local_reduce(const glsl_algo_context *ctx,
                       GLuint input_buffer, 
                       GLuint output_buffer,
                       unsigned int num_elements,
                       unsigned int block_size)
{
    assert(block_size >= ctx->conf.local_block_size);
    unsigned superScalarNumElements = glsl_algo_get_rw_num_elements(ctx->conf.rw_type);
    unsigned threadBlockSize = superScalarNumElements*ctx->conf.local_block_size;
    assert(block_size % threadBlockSize == 0);
    
    unsigned elementsProcessedPerThread = block_size / threadBlockSize;
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
    
    glUseProgram(0);
}

void glsl_local_scan(const glsl_algo_context *ctx,
                     GLuint input_buffer, 
                     GLuint output_buffer,
                     unsigned int num_elements,
                     unsigned int block_size,
                     unsigned char is_inclusive)
{
    unsigned superScalarNumElements = glsl_algo_get_rw_num_elements(ctx->conf.rw_type);
    unsigned threadBlockSize = superScalarNumElements*ctx->conf.local_block_size;
    
    unsigned elementsProcessedPerThread = block_size / threadBlockSize;
    unsigned gridSize = (num_elements + block_size - 1u) / block_size;
    unsigned adjustedArraySize = (num_elements + superScalarNumElements - 1u) / superScalarNumElements;
    
    glUseProgram(ctx->scan_program);
    glUniform1ui(0, elementsProcessedPerThread);
    glUniform1ui(1, adjustedArraySize);
    glUniform1ui(2, (unsigned int)is_inclusive);
    glUniform1ui(3, 0u);
    
    glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 0, input_buffer);
    glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 1, output_buffer);
    
    glDispatchCompute(gridSize, 1, 1);
    
    glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 0, 0);
    glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 1, 0);
    
    glUseProgram(0);
}

static void glsl_local_scan_and_add_block(const glsl_algo_context *ctx,
                                          GLuint input_buffer,
                                          GLuint block_buffer,
                                          GLuint output_buffer,
                                          unsigned int num_elements,
                                          unsigned int block_size,
                                          unsigned char is_inclusive)
{
    unsigned superScalarNumElements = glsl_algo_get_rw_num_elements(ctx->conf.rw_type);
    unsigned threadBlockSize = superScalarNumElements*ctx->conf.local_block_size;
    
    unsigned elementsProcessedPerThread = block_size / threadBlockSize;
    unsigned gridSize = (num_elements + block_size - 1u) / block_size;
    unsigned adjustedArraySize = (num_elements + superScalarNumElements - 1u) / superScalarNumElements;
    
    glUseProgram(ctx->scan_program);
    glUniform1ui(0, elementsProcessedPerThread);
    glUniform1ui(1, adjustedArraySize);
    glUniform1ui(2, (unsigned int)is_inclusive);
    glUniform1ui(3, 1u);
    
    glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 0, input_buffer);
    glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 1, output_buffer);
    glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 2, block_buffer);
    
    glDispatchCompute(gridSize, 1, 1);
    
    glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 0, 0);
    glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 1, 0);
    glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 2, 0);
    
    glUseProgram(0);
}

void glsl_scan(const glsl_algo_context *ctx,
               GLuint input_buffer,
               GLuint intermediate_buffer,
               GLuint output_buffer,
               unsigned int num_elements,
               unsigned int rw_per_thread,
               unsigned char is_inclusive)
{
    unsigned elements_per_block = rw_per_thread * ctx->conf.local_block_size * glsl_algo_get_rw_num_elements(ctx->conf.rw_type);
    unsigned num_blocks = (num_elements + elements_per_block - 1u) / elements_per_block;
    glsl_local_reduce(ctx, input_buffer, intermediate_buffer, num_elements, elements_per_block);
    glMemoryBarrier(GL_SHADER_STORAGE_BARRIER_BIT);
    unsigned intemediateBlockScanBlockSize = elements_per_block > num_blocks ? elements_per_block : num_blocks;
    glsl_local_scan(ctx, intermediate_buffer, intermediate_buffer, num_blocks, intemediateBlockScanBlockSize, 0);
    glMemoryBarrier(GL_SHADER_STORAGE_BARRIER_BIT);
    glsl_local_scan_and_add_block(ctx, input_buffer, intermediate_buffer, output_buffer, num_elements, elements_per_block, is_inclusive);
}