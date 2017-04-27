#ifndef GLSL_ALGO_GL_H
#define GLSL_ALGO_GL_H

#if defined(WIN32) || defined(_WIN32) || defined(__WIN32) && !defined(__CYGWIN__)
#ifndef WIN32_LEAN_AND_MEAN
#define WIN32_LEAN_AND_MEAN 1
#endif
#include <windows.h>
#endif

#include <GL/gl.h>
#include "gl/glcorearb.h"

#include <stdio.h>
#include <stdlib.h>

#define GLSL_ALGO_ERROR(error_msg) \
fprintf(stderr, "Error: %s\n", error_msg); \
exit(1);

#define GLSL_ALGO_SHADER_ERROR(error_msg, shader) \
fprintf(stderr, "Error: %s\nShader:\n%s\n", error_msg, shader); \
exit(1);

typedef struct
{
    PFNGLCREATEPROGRAMPROC glCreateProgram;
    PFNGLUSEPROGRAMPROC glUseProgram;
    PFNGLLINKPROGRAMPROC glLinkProgram;
    PFNGLGETPROGRAMIVPROC glGetProgramiv;
    
    PFNGLCREATESHADERPROC glCreateShader;
    PFNGLSHADERSOURCEPROC glShaderSource;
    PFNGLCOMPILESHADERPROC glCompileShader;
    PFNGLGETSHADERIVPROC glGetShaderiv;
    PFNGLGETSHADERINFOLOGPROC glGetShaderInfoLog;
    PFNGLATTACHSHADERPROC glAttachShader;
    
    
    PFNGLUNIFORM1IPROC glUniform1i;
    PFNGLUNIFORM1UIPROC glUniform1ui;
    
    PFNGLBINDBUFFERBASEPROC glBindBufferBase;
    
    PFNGLDISPATCHCOMPUTEPROC glDispatchCompute;
    PFNGLMEMORYBARRIERPROC glMemoryBarrier;

    PFNGLGENQUERIESPROC glGenQueries;
    PFNGLBEGINQUERYPROC glBeginQuery;
    PFNGLENDQUERYPROC glEndQuery;
    PFNGLGETQUERYOBJECTIVPROC glGetQueryObjectiv;
    PFNGLGETQUERYOBJECTUI64VPROC glGetQueryObjectui64v;

} glsl_algo_gl_context;

#endif // GLSL_ALGO_GL_H