#include "glsl_algo/init.h"
#include "shader_utils.h"

#include <assert.h>
#include <stdlib.h>
#include <string.h>

GLuint EVENT;

void glsl_algo_init(const glsl_algo_gl_context *gl_context)
{
	 gl_context->glGenQueries(1, &EVENT);
}