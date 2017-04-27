#ifndef GLSL_ALGO_CONTEXT_H
#define GLSL_ALGO_CONTEXT_H

#include "glsl_algo/init.h"

#ifdef __cplusplus
extern "C"{
#endif

typedef struct
{
    GLSL_ALGO_READ_WRITE_TYPE rw_type;
    unsigned local_block_size;
    
    GLuint set_value_program;
    GLuint copy_buffer_program;
} glsl_algo_memory_context;

glsl_algo_memory_context get_memory_context(const glsl_algo_gl_context *gl_context, GLSL_ALGO_READ_WRITE_TYPE read_write_type, unsigned local_block_size);

typedef struct
{
    GLSL_ALGO_READ_WRITE_TYPE rw_type;
    unsigned local_block_size;
    GLSL_AlGO_SCAN_OPERATOR scan_operator;
    
    GLuint local_scan_program;
    GLuint reduce_program;
} glsl_algo_scan_context;

glsl_algo_scan_context get_scan_context(const glsl_algo_gl_context *gl_context, GLSL_ALGO_READ_WRITE_TYPE read_write_type, GLSL_AlGO_SCAN_OPERATOR scan_operator, unsigned local_block_size);

typedef struct
{
    GLSL_ALGO_READ_WRITE_TYPE rw_type;
    unsigned local_block_size;
    unsigned radix_size;
    unsigned radix_sort_num_passes;
    
    GLuint local_scan_program;
    GLuint decode_reduce_program;
    GLuint scan_scatter_program;
} glsl_algo_radix_sort_context;

glsl_algo_radix_sort_context get_radix_sort_context(const glsl_algo_gl_context *gl_context, GLSL_ALGO_READ_WRITE_TYPE read_write_type, unsigned local_block_size, unsigned radix_size, unsigned radix_sort_num_passes);

#ifdef __cplusplus
}
#endif

#endif // GLSL_ALGO_CONTEXT_H