#ifndef GLSL_ALGO_INIT_H
#define GLSL_ALGO_INIT_H

#include "glsl_algo/algo_gl.h"
#include "glsl_algo/rw_types.h"

#include <stdio.h>
#include <stdlib.h>

extern GLuint EVENT;

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

void glsl_algo_init(const glsl_algo_gl_context *gl_context);

#ifdef __cplusplus
}
#endif

#endif // GLSL_ALGO_INIT_H