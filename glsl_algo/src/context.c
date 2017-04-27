#include "glsl_algo/context.h"
#include "glsl_algo/rw_types.h"
#include "shader_utils.h"

#include <assert.h>

glsl_algo_memory_context get_memory_context(const glsl_algo_gl_context *gl_context, GLSL_ALGO_READ_WRITE_TYPE read_write_type, unsigned local_block_size)
{
    const int buffer_max_size = 3*4096;
    char *buffer = (char*)malloc(buffer_max_size);
    assert(buffer != NULL);
    
    unsigned num_elements = glsl_algo_get_rw_num_elements(read_write_type);
    const char *const type_name = glsl_algo_get_rw_type_name(read_write_type);
    const char *const scalar_type_name = glsl_algo_get_rw_type_name(get_equivalent_scalar_type(read_write_type));
    
    int shaderSourceLen = snprintf(buffer, buffer_max_size, GLSL_ALGO_SET_MEMORY_TO_ZERO_SRC, type_name, scalar_type_name, 0, local_block_size, num_elements, 32, 0, 0, 0);
    assert(shaderSourceLen >= 0 && shaderSourceLen < buffer_max_size);
    GLuint set_value_program = create_compute_program(gl_context, buffer, shaderSourceLen);
    
    shaderSourceLen = snprintf(buffer, buffer_max_size, GLSL_ALGO_COPY_MEMORY_SRC, type_name, scalar_type_name, 0, local_block_size, num_elements, 32, 0, 0, 0);
    assert(shaderSourceLen >= 0 && shaderSourceLen < buffer_max_size);
    GLuint copy_buffer_program = create_compute_program(gl_context, buffer, shaderSourceLen);
    
    free(buffer);
    
    glsl_algo_memory_context ctx = {read_write_type, local_block_size, set_value_program, copy_buffer_program};
    
    return ctx;
}

glsl_algo_scan_context get_scan_context(const glsl_algo_gl_context *gl_context, GLSL_ALGO_READ_WRITE_TYPE read_write_type, GLSL_AlGO_SCAN_OPERATOR scan_operator, unsigned local_block_size)
{
    const int buffer_max_size = 3*4096;
    char *buffer = (char*)malloc(buffer_max_size);
    assert(buffer != NULL);
    
    unsigned scan_operator_key = glsl_algo_convert_scan_operator(scan_operator);
    unsigned num_elements = glsl_algo_get_rw_num_elements(read_write_type);
    const char *const type_name = glsl_algo_get_rw_type_name(read_write_type);
    const char *const scalar_type_name = glsl_algo_get_rw_type_name(get_equivalent_scalar_type(read_write_type));
    unsigned type_key = glsl_algo_convert_rw_type(read_write_type);
    
    int shaderSourceLen = snprintf(buffer, buffer_max_size, GLSL_ALGO_LOCAL_REDUCE_SHADER_SRC, type_name, scalar_type_name, type_key, local_block_size, num_elements, 32, 0, 0, scan_operator_key);
    assert(shaderSourceLen >= 0 && shaderSourceLen < buffer_max_size);
    GLuint reduce_program = create_compute_program(gl_context, buffer, shaderSourceLen);
    
    shaderSourceLen = snprintf(buffer, buffer_max_size, GLSL_ALGO_LOCAL_SCAN_SHADER_SRC, type_name, scalar_type_name, type_key, local_block_size, num_elements, 32, 0, 0, scan_operator_key);
    assert(shaderSourceLen >= 0 && shaderSourceLen < buffer_max_size);
    GLuint scan_program = create_compute_program(gl_context, buffer, shaderSourceLen);
    
    free(buffer);
    
    glsl_algo_scan_context ctx = {read_write_type, local_block_size, scan_operator, scan_program, reduce_program};
    
    return ctx;
}

glsl_algo_radix_sort_context get_radix_sort_context(const glsl_algo_gl_context *gl_context, GLSL_ALGO_READ_WRITE_TYPE read_write_type, unsigned local_block_size, unsigned radix_size, unsigned radix_sort_num_passes)
{
    const int buffer_max_size = 3*4096;
    char *buffer = (char*)malloc(buffer_max_size);
    assert(buffer != NULL);

    unsigned num_elements = glsl_algo_get_rw_num_elements(read_write_type);
    const char *const type_name = glsl_algo_get_rw_type_name(read_write_type);
    const char *const scalar_type_name = glsl_algo_get_rw_type_name(get_equivalent_scalar_type(read_write_type));
    unsigned type_key = glsl_algo_convert_rw_type(GARWTuint1);
    
    GLSL_ALGO_READ_WRITE_TYPE radixIntType = get_equivalent_vector_type(GARWTuint1, num_elements);
    const char *const radixVectorTypeName = glsl_algo_get_rw_type_name(radixIntType);
    
    int shaderSourceLen = snprintf(buffer, buffer_max_size, GLSL_ALGO_RADIX_SORT_GATHER_SRC, radixVectorTypeName, "uint", 1u, local_block_size, num_elements, 32, radix_size, radix_sort_num_passes, GASOadd);
    assert(shaderSourceLen >= 0 && shaderSourceLen < buffer_max_size);
    GLuint decode_reduce_program = create_compute_program(gl_context, buffer, shaderSourceLen);

    shaderSourceLen = snprintf(buffer, buffer_max_size, GLSL_ALGO_RADIX_SORT_SCATTER_SRC, radixVectorTypeName, "uint", 1u, local_block_size, num_elements, 32, radix_size, radix_sort_num_passes, GASOadd);
    assert(shaderSourceLen >= 0 && shaderSourceLen < buffer_max_size);
    GLuint scan_scatter_program = create_compute_program(gl_context, buffer, shaderSourceLen);
    
    shaderSourceLen = snprintf(buffer, buffer_max_size, GLSL_ALGO_LOCAL_SCAN_SHADER_SRC, type_name, scalar_type_name, type_key, local_block_size, num_elements, 32, 0, 0, GASOadd);
    assert(shaderSourceLen >= 0 && shaderSourceLen < buffer_max_size);
    GLuint scan_program = create_compute_program(gl_context, buffer, shaderSourceLen);
    
    free(buffer);
    
    glsl_algo_radix_sort_context ctx = {read_write_type, local_block_size, radix_size, radix_sort_num_passes, scan_program, decode_reduce_program, scan_scatter_program};
    
    return ctx;
}