#ifndef GLSL_ALGO_PREFIX_SCAN_H
#define GLSL_ALGO_PREFIX_SCAN_H

#include <GL/gl.h>

#ifdef __cplusplus
extern "C"{
#endif 

// Does prefix scan per block.
void glsl_block_prefix_scan(GLuint input_buffer, 
                            GLuint output_buffer, 
                            unsigned int num_elements, 
                            unsigned int block_size, 
                            unsigned char is_inclusive);

// Does prefix scan of the whole buffer.
void glsl_prefix_scan(GLuint input_buffer, 
                      GLuint output_buffer, 
                      unsigned int num_elements, 
                      unsigned char is_inclusive);

#ifdef __cplusplus
}
#endif

#endif // GLSL_ALGO_PREFIX_SCAN_H