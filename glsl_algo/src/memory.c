#include "glsl_algo/memory.h"
#include "benchmark_helper.h"
#include <assert.h>
#include <stdio.h>

void glsl_memory_set_to_zero(const glsl_algo_gl_context *gl,
                             const glsl_algo_context *ctx,
                             GLuint output_buffer, 
                             unsigned int num_elements,
                             unsigned int elements_per_thread)
{
    assert(elements_per_thread >= 1u);
    unsigned superScalarNumElements = glsl_algo_get_rw_num_elements(ctx->conf.rw_type);
    unsigned threadBlockSize = elements_per_thread*superScalarNumElements*ctx->conf.local_block_size;

    unsigned gridSize = (num_elements + threadBlockSize - 1u) / threadBlockSize;
    unsigned adjustedArraySize = (num_elements + superScalarNumElements - 1u) / superScalarNumElements;
    
    gl->glUseProgram(ctx->set_value_program);
    
    gl->glUniform1ui(0, elements_per_thread);
    gl->glUniform1ui(1, adjustedArraySize);
    
    gl->glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 0, output_buffer);
    
    gl->glDispatchCompute(gridSize, 1, 1);
    
    gl->glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 0, 0);
    
    gl->glUseProgram(0);
}

void glsl_copy_memory(const glsl_algo_gl_context *gl,
                      const glsl_algo_context *ctx,
                      GLuint input_buffer, 
                      GLuint output_buffer,
                      unsigned int num_elements,
                      unsigned int elements_per_thread)
{
    assert(elements_per_thread >= 1u);
    unsigned superScalarNumElements = glsl_algo_get_rw_num_elements(ctx->conf.rw_type);
    unsigned threadBlockSize = elements_per_thread*superScalarNumElements*ctx->conf.local_block_size;

    unsigned gridSize = (num_elements + threadBlockSize - 1u) / threadBlockSize;
    unsigned adjustedArraySize = (num_elements + superScalarNumElements - 1u) / superScalarNumElements;
    
    gl->glUseProgram(ctx->copy_buffer_program);
    
    gl->glUniform1ui(0, elements_per_thread);
    gl->glUniform1ui(1, adjustedArraySize);
    
    gl->glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 0, input_buffer);
    gl->glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 1, output_buffer);
    
    gl->glDispatchCompute(gridSize, 1, 1);
    
    gl->glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 0, 0);
    gl->glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 1, 0);
    
    gl->glUseProgram(0);                
}