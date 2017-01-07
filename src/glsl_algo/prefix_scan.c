#include "prefix_scan.h"

#include <stdio.h>

void glsl_block_prefix_scan(GLuint input_buffer, 
                            GLuint output_buffer, 
                            unsigned int num_elements, 
                            unsigned int block_size, 
                            unsigned char is_inclusive)
{
    printf("call glsl_block_prefix_scan");
}

void glsl_prefix_scan(GLuint input_buffer, 
                      GLuint output_buffer, 
                      unsigned int num_elements, 
                      unsigned char is_inclusive)
{
    printf("call glsl_prefix_scan");
}