#include "glsl_algo/prefix_scan.h"
#include <assert.h>
#include <stdio.h>

void glsl_local_reduce(const glsl_algo_gl_context *gl,
                       const glsl_algo_scan_context *ctx,
                       GLuint input_buffer, 
                       GLuint output_buffer,
                       unsigned int num_elements,
                       unsigned int block_size)
{
    assert(block_size >= ctx->local_block_size);
    unsigned superScalarNumElements = glsl_algo_get_rw_num_elements(ctx->rw_type);
    unsigned threadBlockSize = superScalarNumElements*ctx->local_block_size;
    assert(block_size % threadBlockSize == 0);
    
    unsigned elementsProcessedPerThread = block_size / threadBlockSize;
    unsigned gridSize = (num_elements + block_size - 1u) / block_size;
    unsigned adjustedArraySize = (num_elements + superScalarNumElements - 1u) / superScalarNumElements;
    
    gl->glUseProgram(ctx->reduce_program);
    gl->glUniform1ui(0, elementsProcessedPerThread);
    gl->glUniform1ui(1, adjustedArraySize);
    
    gl->glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 0, input_buffer);
    gl->glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 1, output_buffer);
    
    gl->glDispatchCompute(gridSize, 1, 1);
    
    gl->glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 0, 0);
    gl->glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 1, 0);
    
    gl->glUseProgram(0);
}

void glsl_local_scan(const glsl_algo_gl_context *gl,
                     const glsl_algo_scan_context *ctx,
                     GLuint input_buffer, 
                     GLuint output_buffer,
                     unsigned int num_elements,
                     unsigned int block_size,
                     unsigned char is_inclusive)
{
    unsigned superScalarNumElements = glsl_algo_get_rw_num_elements(ctx->rw_type);
    unsigned threadBlockSize = superScalarNumElements*ctx->local_block_size;
    
    unsigned elementsProcessedPerThread = (threadBlockSize + block_size - 1u) / threadBlockSize;
    unsigned gridSize = (num_elements + block_size - 1u) / block_size;
    unsigned adjustedArraySize = (num_elements + superScalarNumElements - 1u) / superScalarNumElements;
    
    gl->glUseProgram(ctx->local_scan_program);
    gl->glUniform1ui(0, elementsProcessedPerThread);
    gl->glUniform1ui(1, adjustedArraySize);
    gl->glUniform1ui(2, (unsigned int)is_inclusive);
    gl->glUniform1ui(3, 0u);
    
    gl->glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 0, input_buffer);
    gl->glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 1, output_buffer);
    
    gl->glDispatchCompute(gridSize, 1, 1);
    
    gl->glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 0, 0);
    gl->glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 1, 0);
    
    gl->glUseProgram(0);
}

static void glsl_local_scan_and_add_block(const glsl_algo_gl_context *gl,
                                          const glsl_algo_scan_context *ctx,
                                          GLuint input_buffer,
                                          GLuint block_buffer,
                                          GLuint output_buffer,
                                          unsigned int num_elements,
                                          unsigned int block_size,
                                          unsigned char is_inclusive)
{
    unsigned superScalarNumElements = glsl_algo_get_rw_num_elements(ctx->rw_type);
    unsigned threadBlockSize = superScalarNumElements*ctx->local_block_size;
    
    unsigned elementsProcessedPerThread = block_size / threadBlockSize;
    unsigned gridSize = (num_elements + block_size - 1u) / block_size;
    unsigned adjustedArraySize = (num_elements + superScalarNumElements - 1u) / superScalarNumElements;
    
    gl->glUseProgram(ctx->local_scan_program);
    gl->glUniform1ui(0, elementsProcessedPerThread);
    gl->glUniform1ui(1, adjustedArraySize);
    gl->glUniform1ui(2, (unsigned int)is_inclusive);
    gl->glUniform1ui(3, 1u);
    
    gl->glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 0, input_buffer);
    gl->glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 1, output_buffer);
    gl->glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 2, block_buffer);
    
    gl->glDispatchCompute(gridSize, 1, 1);
    
    gl->glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 0, 0);
    gl->glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 1, 0);
    gl->glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 2, 0);
    
    gl->glUseProgram(0);
}

void glsl_scan(const glsl_algo_gl_context *gl,
               const glsl_algo_scan_context *ctx,
               GLuint input_buffer,
               GLuint intermediate_buffer,
               GLuint output_buffer,
               unsigned int num_elements,
               unsigned int rw_per_thread,
               unsigned char is_inclusive)
{
    unsigned elements_per_block = rw_per_thread * ctx->local_block_size * glsl_algo_get_rw_num_elements(ctx->rw_type);
    unsigned num_blocks = (num_elements + elements_per_block - 1u) / elements_per_block;
		glsl_local_reduce(gl, ctx, input_buffer, intermediate_buffer, num_elements, elements_per_block);
		gl->glMemoryBarrier(GL_SHADER_STORAGE_BARRIER_BIT);
		glsl_local_scan(gl, ctx, intermediate_buffer, intermediate_buffer, num_blocks, num_blocks, 0);
		gl->glMemoryBarrier(GL_SHADER_STORAGE_BARRIER_BIT);
		glsl_local_scan_and_add_block(gl, ctx, input_buffer, intermediate_buffer, output_buffer, num_elements, elements_per_block, is_inclusive);
}