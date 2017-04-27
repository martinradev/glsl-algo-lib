#include "shader_utils.h"

#include <assert.h>

GLuint create_compute_program(const glsl_algo_gl_context *gl_context, const char *source, int len)
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