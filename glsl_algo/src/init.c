#include "glsl_algo/init.h"

#include <assert.h>
#include <stdlib.h>
#include <string.h>

static GLuint create_compute_program(const glsl_algo_gl_context *gl_context, const char *source, int len)
{
    GLuint program = gl_context->glCreateProgram();
    assert(program != 0);
    
    GLuint shader = gl_context->glCreateShader(GL_COMPUTE_SHADER);
    assert(shader != 0);
    
    gl_context->glShaderSource(shader, 1, &source, &len);
    gl_context->glCompileShader(shader);
    
	  GLchar *buffer = NULL;
    int buffer_len;
    gl_context->glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &buffer_len);
    if (buffer_len > 0)
    {
        buffer = (GLchar*)malloc(sizeof(GLchar)*buffer_len);
        gl_context->glGetShaderInfoLog(shader, buffer_len, 0, buffer);
        GLSL_ALGO_SHADER_ERROR(buffer, source);
        free(buffer);
    }
    
    int success;
    gl_context->glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
    if (success != GL_TRUE)
    { 
        GLSL_ALGO_SHADER_ERROR("Failed to compile shader.", source);
    }
    
    gl_context->glAttachShader(program, shader);
    gl_context->glLinkProgram(program);
    
    gl_context->glGetProgramiv(program, GL_LINK_STATUS, &success);
    if (success != GL_TRUE)
    { 
        GLSL_ALGO_SHADER_ERROR("Failed to link program.", source);
    }
    
    return program;
}

glsl_algo_context glsl_algo_init(const glsl_algo_gl_context *gl_context, glsl_algo_configuration conf)
{
    const int buffer_max_size = 4096;
    glsl_algo_context ctx;
    ctx.conf = conf;
    
    char *buffer = (char*)malloc(buffer_max_size);
    assert(buffer != NULL);
    
    unsigned num_elements = glsl_algo_get_rw_num_elements(conf.rw_type);
    const char *const type_name = glsl_algo_get_rw_type_name(conf.rw_type);
    const char *const scalar_type_name = glsl_algo_get_rw_type_name(get_equivalent_scalar_type(conf.rw_type));
        
    int shaderSourceLen = snprintf(buffer, buffer_max_size, GLSL_ALGO_LOCAL_REDUCE_SHADER_SRC, type_name, scalar_type_name, conf.local_block_size, num_elements, conf.warp_size);
    assert(shaderSourceLen >= 0 && shaderSourceLen < buffer_max_size);
    ctx.reduce_program = create_compute_program(gl_context, buffer, shaderSourceLen);
    
    shaderSourceLen = snprintf(buffer, buffer_max_size, GLSL_ALGO_LOCAL_SCAN_SHADER_SRC, type_name, scalar_type_name, conf.local_block_size, num_elements, conf.warp_size);
    assert(shaderSourceLen >= 0 && shaderSourceLen < buffer_max_size);
    ctx.scan_program = create_compute_program(gl_context, buffer, shaderSourceLen);
    
    shaderSourceLen = snprintf(buffer, buffer_max_size, GLSL_ALGO_SET_MEMORY_TO_ZERO_SRC, type_name, scalar_type_name, conf.local_block_size, num_elements, conf.warp_size);
    assert(shaderSourceLen >= 0 && shaderSourceLen < buffer_max_size);
    ctx.set_value_program = create_compute_program(gl_context, buffer, shaderSourceLen);
    
    shaderSourceLen = snprintf(buffer, buffer_max_size, GLSL_ALGO_COPY_MEMORY_SRC, type_name, scalar_type_name, conf.local_block_size, num_elements, conf.warp_size);
    assert(shaderSourceLen >= 0 && shaderSourceLen < buffer_max_size);
    ctx.copy_buffer_program = create_compute_program(gl_context, buffer, shaderSourceLen);

    free(buffer);
    
    return ctx;
}