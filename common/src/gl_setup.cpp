#include "common/gl_setup.hpp"

#include <GLFW/glfw3.h>

#include <cstdio>

int init_window_and_gl_context(glsl_algo_gl_context *gl)
{
    if (!glfwInit()) {
        fprintf(stderr, "ERROR: could not start GLFW3\n");
        return 1;
    }
    
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 5);
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    
    GLFWwindow* window = glfwCreateWindow(640, 480, "Window", NULL, NULL);
    if (!window) {
      fprintf(stderr, "ERROR: could not open window with GLFW3\n");
      glfwTerminate();
      return 1;
    }
    glfwMakeContextCurrent(window);
    
    if (!gladLoadGLLoader((GLADloadproc) glfwGetProcAddress))
    {
        fprintf(stderr, "ERROR: Failed to initialize OpenGL context");
        return 1;
    }
    
    gl->glCreateProgram = glad_glCreateProgram;
    gl->glUseProgram = glad_glUseProgram;
    gl->glLinkProgram = glad_glLinkProgram;
    gl->glGetProgramiv = glad_glGetProgramiv;
    gl->glCreateShader = glad_glCreateShader;
    gl->glShaderSource = glad_glShaderSource;
    gl->glCompileShader = glad_glCompileShader;
    gl->glGetShaderiv = glad_glGetShaderiv;
    gl->glGetShaderInfoLog = glad_glGetShaderInfoLog;
    gl->glAttachShader = glad_glAttachShader;
    gl->glUniform1i = glad_glUniform1i;
    gl->glUniform1ui = glad_glUniform1ui;
    gl->glBindBufferBase = glad_glBindBufferBase;
    gl->glDispatchCompute = glad_glDispatchCompute;
    gl->glMemoryBarrier = glad_glMemoryBarrier;
    gl->glGenQueries = glad_glGenQueries;
    gl->glBeginQuery = glad_glBeginQuery;
    gl->glEndQuery = glad_glEndQuery;
    gl->glGetQueryObjectiv = glad_glGetQueryObjectiv;
    gl->glGetQueryObjectui64v = glad_glGetQueryObjectui64v;

    glsl_algo_init(gl);
    
    return 0;
}

void destroy_window_and_gl_context()
{
    glfwTerminate();
}

GLuint create_ssbo(unsigned n, void *data)
{
    GLuint buffer;
    glGenBuffers(1, &buffer);
    glBindBuffer(GL_SHADER_STORAGE_BUFFER, buffer);
    glBufferData(GL_SHADER_STORAGE_BUFFER, sizeof(int)*n, data, GL_STATIC_DRAW);
    glBindBuffer(GL_SHADER_STORAGE_BUFFER, 0);
    return buffer;
}

std::string get_program_binary(GLuint program)
{
    GLint binaryLength;
    glGetProgramiv(program, GL_PROGRAM_BINARY_LENGTH, &binaryLength);
    
    std::string result(binaryLength, '0');
    char *resultDatrPtr = &result[0];
    
    GLenum format;
    glGetProgramBinary(program, binaryLength, NULL, &format, resultDatrPtr);
    
    return result;
}