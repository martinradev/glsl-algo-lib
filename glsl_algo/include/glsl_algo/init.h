#ifndef GLSL_ALGO_INIT_H
#define GLSL_ALGO_INIT_H

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

extern GLuint EVENT;

#define GLSL_ALGO_SHADER_ERROR(error_msg, shader) \
fprintf(stderr, "Error: %s\nShader:\n%s\n", error_msg, shader); \
exit(1);

#ifdef __cplusplus
extern "C"{
#endif

extern const char *const GLSL_ALGO_LOCAL_REDUCE_SHADER_SRC;
extern const char *const GLSL_ALGO_LOCAL_SCAN_SHADER_SRC;
extern const char *const GLSL_ALGO_SET_MEMORY_TO_ZERO_SRC;
extern const char *const GLSL_ALGO_COPY_MEMORY_SRC;
extern const char *const GLSL_ALGO_RADIX_SORT_GATHER_SRC;
extern const char *const GLSL_ALGO_RADIX_SORT_SCATTER_SRC;

typedef enum
{
    GARWTint1,
    GARWTint2,
    GARWTint4,
    
    GARWTuint1,
    GARWTuint2,
    GARWTuint4,
    
    GARWTfloat1,
    GARWTfloat2,
    GARWTfloat4,
    
    GARWTundefined
} GLSL_ALGO_READ_WRITE_TYPE;

typedef enum
{
  GASOadd,
  GASOmin,
  GASOmax,
  
  GASOundefined
} GLSL_AlGO_SCAN_OPERATOR;

static inline unsigned glsl_algo_convert_scan_operator(GLSL_AlGO_SCAN_OPERATOR scan_operator)
{
    switch(scan_operator)
    {
      case GASOadd:
        return 0u;
      case GASOmin:
        return 1u;
      case GASOmax:
        return 2u;
      default:
        break;
    }
    return GASOundefined;
}

static inline unsigned glsl_algo_convert_rw_type(GLSL_ALGO_READ_WRITE_TYPE type)
{
    switch(type)
    {
      case GARWTint1:
      case GARWTint2:
      case GARWTint4:
        return 0u;
      case GARWTuint1:
      case GARWTuint2:
      case GARWTuint4:
        return 1u;
      case GARWTfloat1:
      case GARWTfloat2:
      case GARWTfloat4:
        return 2u;
      default:
        break;
    }
    return GARWTundefined;
}

static inline unsigned glsl_algo_get_rw_num_elements(GLSL_ALGO_READ_WRITE_TYPE type)
{
    switch(type)
    {
        case GARWTint1:
        case GARWTuint1:
        case GARWTfloat1:
            return 1;
        case GARWTint2:
        case GARWTuint2:
        case GARWTfloat2:
            return 2;
        case GARWTint4:
        case GARWTuint4:
        case GARWTfloat4:
            return 4;
        default:
            GLSL_ALGO_ERROR("Read/write type not defined");
    }
    return 0;
}

static inline const char *const glsl_algo_get_rw_type_name(GLSL_ALGO_READ_WRITE_TYPE type)
{
    switch(type)
    {
        case GARWTint1:
            return "int";
        case GARWTint2:
            return "ivec2";
        case GARWTint4:
            return "ivec4";
        case GARWTuint1:
            return "uint";
        case GARWTuint2:
            return "uvec2";
        case GARWTuint4:
            return "uvec4";
        case GARWTfloat1:
            return "float";
        case GARWTfloat2:
            return "vec2";
        case GARWTfloat4:
            return "vec4";
        default:
            GLSL_ALGO_ERROR("Read/write type not defined");
    }
    return NULL;
}

static inline GLSL_ALGO_READ_WRITE_TYPE get_equivalent_scalar_type(GLSL_ALGO_READ_WRITE_TYPE type)
{
    switch(type)
    {
        case GARWTint1:
            return GARWTint1;
        case GARWTint2:
            return GARWTint1;
        case GARWTint4:
            return GARWTint1;
        case GARWTuint1:
            return GARWTuint1;
        case GARWTuint2:
            return GARWTuint1;
        case GARWTuint4:
            return GARWTuint1;
        case GARWTfloat1:
            return GARWTfloat1;
        case GARWTfloat2:
            return GARWTfloat1;
        case GARWTfloat4:
            return GARWTfloat1;
        default:
            GLSL_ALGO_ERROR("Read/write type not defined");
    }
    return GARWTundefined;
}

static inline GLSL_ALGO_READ_WRITE_TYPE get_equivalent_vector_type(GLSL_ALGO_READ_WRITE_TYPE type, unsigned elements)
{
    switch(elements)
    {
        case 1:
        case 2:
        case 4:
            break;
        default:
            GLSL_ALGO_ERROR("Vector size can be only of size 1,2 and 4.");
    }
    
    switch(type)
    {
        case GARWTfloat1:
        case GARWTfloat2:
        case GARWTfloat4:
        {
            switch(elements)
            {
                case 1:
                    return GARWTfloat1;
                case 2:
                    return GARWTfloat2;
                case 4:
                    return GARWTfloat4;
            }
            break;
        }
        case GARWTint1:
        case GARWTint2:
        case GARWTint4:
        {
            switch(elements)
            {
                case 1:
                    return GARWTint1;
                case 2:
                    return GARWTint2;
                case 4:
                    return GARWTint4;
            }
            break;
        }
        case GARWTuint1:
        case GARWTuint2:
        case GARWTuint4:
        {
            switch(elements)
            {
                case 1:
                    return GARWTuint1;
                case 2:
                    return GARWTuint2;
                case 4:
                    return GARWTuint4;
            }
            break;
        }
    }
    return GARWTundefined;
}

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

typedef struct
{
    GLSL_ALGO_READ_WRITE_TYPE rw_type;
    GLSL_AlGO_SCAN_OPERATOR scan_operator;
    unsigned local_block_size;
    unsigned warp_size;
    unsigned radix_size;
    unsigned radix_sort_num_passes;
} glsl_algo_configuration;

typedef struct
{
    glsl_algo_configuration conf;
    GLuint reduce_program;
    GLuint scan_program;
    GLuint set_value_program;
    GLuint copy_buffer_program;
    GLuint radix_sort_gather_program;
    GLuint radix_sort_scatter_program;
} glsl_algo_context;

glsl_algo_context glsl_algo_init(const glsl_algo_gl_context *gl_context, glsl_algo_configuration conf);

#ifdef __cplusplus
}
#endif

#endif // GLSL_ALGO_INIT_H