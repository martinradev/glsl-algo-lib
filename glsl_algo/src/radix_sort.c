#include "glsl_algo/radix_sort.h"
#include "glsl_algo/prefix_scan.h"

#include <assert.h>

#define LIB_BENCHMARK_GPU(gl, call, queryObject, name) {\
gl->glBeginQuery(GL_TIME_ELAPSED, queryObject);\
call;\
gl->glEndQuery(GL_TIME_ELAPSED);\
int stopTimerAvailable = 0;\
while (!stopTimerAvailable) {\
     gl->glGetQueryObjectiv(queryObject,\
     GL_QUERY_RESULT_AVAILABLE,\
     &stopTimerAvailable);\
}\
GLuint64 elapsedTime;\
gl->glGetQueryObjectui64v(queryObject, GL_QUERY_RESULT, &elapsedTime);\
printf("%s %lf\n", name, ((double)elapsedTime) / 1000000.0);\
}

void glsl_radix_sort_gather(const glsl_algo_gl_context *gl,
                       const glsl_algo_context *ctx,
                       GLuint input_buffer,
                       GLuint temporary_radix_buffer,
                       unsigned int num_elements,
                       unsigned int rw_per_thread,
                       unsigned int radix_offset)
{
	assert(ctx->conf.local_block_size <= 512);
    unsigned superScalarNumElements = glsl_algo_get_rw_num_elements(ctx->conf.rw_type);
    unsigned block_size = ctx->conf.local_block_size * superScalarNumElements * rw_per_thread;
    
    unsigned gridSize = (num_elements + block_size - 1u) / block_size;
    unsigned adjustedArraySize = (num_elements + superScalarNumElements - 1u) / superScalarNumElements;
    
    gl->glUseProgram(ctx->radix_sort_gather_program);
    gl->glUniform1ui(0, rw_per_thread);
    gl->glUniform1ui(1, adjustedArraySize);
    gl->glUniform1ui(2, radix_offset);
    
    gl->glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 0, input_buffer);
    gl->glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 1, temporary_radix_buffer);
    
    gl->glDispatchCompute(gridSize, 1, 1);
    
    gl->glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 0, 0);
    gl->glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 1, 0);
    
    gl->glUseProgram(0);
}

void glsl_radix_sort_scatter(const glsl_algo_gl_context *gl,
                             const glsl_algo_context *ctx,
                             GLuint input_buffer,
                             GLuint temporary_radix_buffer,
                             GLuint output_buffer,
                             unsigned int num_elements,
                             unsigned int rw_per_thread,
                             unsigned int radix_offset)
{
	assert(ctx->conf.local_block_size <= 512);
    unsigned superScalarNumElements = glsl_algo_get_rw_num_elements(ctx->conf.rw_type);
    unsigned elements_per_block = rw_per_thread * ctx->conf.local_block_size * superScalarNumElements;
    unsigned num_blocks = (num_elements + elements_per_block - 1u) / elements_per_block;
    unsigned adjustedArraySize = num_elements;
    
    gl->glUseProgram(ctx->radix_sort_scatter_program);
    gl->glUniform1ui(0, rw_per_thread);
    gl->glUniform1ui(1, adjustedArraySize);
    gl->glUniform1ui(2, radix_offset);
    
    gl->glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 0, input_buffer);
    gl->glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 1, output_buffer);
    gl->glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 2, temporary_radix_buffer);
    
    gl->glDispatchCompute(num_blocks, 1, 1);
    
    gl->glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 0, 0);
    gl->glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 1, 0);
    gl->glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 2, 0);
    
    gl->glUseProgram(0);
}

void glsl_radix_sort_pass(const glsl_algo_gl_context *gl_context,
                          const glsl_algo_context *ctx,
                          GLuint input_buffer,
                          GLuint temporary_radix_buffer,
                          GLuint output_buffer,
                          unsigned int num_elements,
                          unsigned int rw_per_thread,
                          unsigned int radix_offset)
{
	assert(ctx->conf.local_block_size <= 512);
	LIB_BENCHMARK_GPU(gl_context, 
					  glsl_radix_sort_gather(gl_context, ctx, input_buffer, temporary_radix_buffer, num_elements, rw_per_thread, radix_offset),
					  EVENT,
					  "gather")
    
    gl_context->glMemoryBarrier(GL_SHADER_STORAGE_BARRIER_BIT);
    
    unsigned radix_word = 1 << ctx->conf.radix_size;
    unsigned elements_per_block = rw_per_thread * ctx->conf.local_block_size * glsl_algo_get_rw_num_elements(ctx->conf.rw_type);
    unsigned num_blocks = radix_word * ((num_elements + elements_per_block - 1u) / elements_per_block);

	LIB_BENCHMARK_GPU(gl_context,
		glsl_local_scan(gl_context, ctx, temporary_radix_buffer, temporary_radix_buffer, num_blocks, num_blocks, 1),
		EVENT,
		"local_scan")
    
    gl_context->glMemoryBarrier(GL_SHADER_STORAGE_BARRIER_BIT);
    
	LIB_BENCHMARK_GPU(gl_context,
		glsl_radix_sort_scatter(gl_context, ctx, input_buffer, temporary_radix_buffer, output_buffer, num_elements, rw_per_thread, radix_offset),
		EVENT,
		"scatter")
}

void glsl_radix_sort(const glsl_algo_gl_context *gl_context,
                          const glsl_algo_context *ctx,
                          GLuint input_buffer,
                          GLuint temporary_radix_buffer,
                          GLuint ping_pong_buffer,
                          GLuint output_buffer,
                          unsigned int num_elements,
                          unsigned int rw_per_thread)
{
	assert(ctx->conf.local_block_size <= 512);
    for (unsigned i = 0u; i < 32u; i += ctx->conf.radix_size)
    {
        glsl_radix_sort_pass(gl_context, ctx, input_buffer, temporary_radix_buffer, ping_pong_buffer, num_elements, rw_per_thread, i);
        gl_context->glMemoryBarrier(GL_SHADER_STORAGE_BARRIER_BIT);
        input_buffer = ping_pong_buffer;
        GLuint tmp = ping_pong_buffer;
        ping_pong_buffer = output_buffer;
        output_buffer = tmp;
    }
}
