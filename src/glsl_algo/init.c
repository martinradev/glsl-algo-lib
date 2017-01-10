#include "init.h"

#include <assert.h>
#include <stdlib.h>
#include <string.h>

static GLuint create_compute_program(const char *source, int len)
{
    GLuint program = glCreateProgram();
    assert(program != 0);
    
    GLuint shader = glCreateShader(GL_COMPUTE_SHADER);
    assert(shader != 0);
    
    glShaderSource(shader, 1, &source, &len);
    glCompileShader(shader);
    
    int buffer_len;
    glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &buffer_len);
    if (buffer_len > 0)
    {
        GLchar *buffer = (GLchar*)malloc(sizeof(GLchar)*buffer_len);
        glGetShaderInfoLog(shader, buffer_len, 0, buffer);
        GLSL_ALGO_SHADER_ERROR(buffer, source);
        free(buffer);
    }
    
    int success;
    glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
    if (success != GL_TRUE)
    { 
        GLSL_ALGO_SHADER_ERROR("Failed to compile shader.", source);
    }
    
    glAttachShader(program, shader);
    glLinkProgram(program);
    
    glGetProgramiv(program, GL_LINK_STATUS, &success);
    if (success != GL_TRUE)
    { 
        GLSL_ALGO_SHADER_ERROR("Failed to link program.", source);
    }
    
    return program;
}

glsl_algo_context glsl_algo_init(glsl_algo_configuration conf)
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
    ctx.reduce_program = create_compute_program(buffer, shaderSourceLen);

    free(buffer);
    
    return ctx;
}