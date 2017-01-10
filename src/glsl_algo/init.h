#ifndef GLSL_ALGO_INIT_H
#define GLSL_ALGO_INIT_H

#define GL_GLEXT_PROTOTYPES
#include <GL/gl.h>

#include <stdio.h>
#include <stdlib.h>
#define GLSL_ALGO_ERROR(error_msg) \
fprintf(stderr, "Error: %s\n", error_msg); \
exit(1);

#define GLSL_ALGO_SHADER_ERROR(error_msg, shader) \
fprintf(stderr, "Error: %s\nShader:\n%s\n", error_msg, shader); \
exit(1);

#ifdef __cplusplus
extern "C"{
#endif

extern const char *const GLSL_ALGO_LOCAL_REDUCE_SHADER_SRC;

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

typedef struct
{
    // read/write type whenever applicable
    GLSL_ALGO_READ_WRITE_TYPE rw_type;
    unsigned local_block_size;
    unsigned warp_size;
} glsl_algo_configuration;

typedef struct
{
    glsl_algo_configuration conf;
    GLuint reduce_program;
} glsl_algo_context;

glsl_algo_context glsl_algo_init(glsl_algo_configuration conf);

#ifdef __cplusplus
}
#endif

#endif // GLSL_ALGO_INIT_H