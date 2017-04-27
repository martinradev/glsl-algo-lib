#ifndef SHADER_UTILS_H
#define SHADER_UTILS_H

#include "glsl_algo/algo_gl.h"

#define SHADER(version,src) "#version " #version "\n" \
"#define TYPE %s\n" \
"#define SCALAR_TYPE %s\n" \
"#define SCALAR_TYPE_KEY %u\n" \
"#define BLOCK_SIZE %u\n" \
"#define ELEMENTS_PER_THREAD %u\n" \
"#define WARP_SIZE %u\n" \
"#define RADIX_SIZE %u\n" \
"#define GET_LANE_ID(t) (t&(WARP_SIZE-1u))\n" \
"#define GET_WARP_ID(t) (t/WARP_SIZE)\n" \
"#define RADIX_SORT_NUM_PASSES %u\n" \
"#define SCAN_BINARY_OPERATOR %u\n" \
"#if SCALAR_TYPE_KEY == 0\n" \
"#define MIN_TYPE_VALUE (int(1)<<31)\n" \
"#define MAX_TYPE_VALUE (MIN_TYPE_VALUE-1)\n" \
"#elif SCALAR_TYPE_KEY == 1\n" \
"#define MIN_TYPE_VALUE uint(0)\n" \
"#define MAX_TYPE_VALUE (~uint(0))\n" \
"#elif SCALAR_TYPE_KEY == 2\n" \
"#define MIN_TYPE_VALUE (-(1./0.))\n" \
"#define MAX_TYPE_VALUE (1./0.)\n" \
"#else\n" \
"#error \"Unknown scalar type\"\n" \
"#endif\n" \
"#if SCAN_BINARY_OPERATOR == 0\n" \
"#define INITIAL_SCAN_VALUE SCALAR_TYPE(0)\n" \
"#define SCAN_OP(a,b) (a+b)\n" \
"#elif SCAN_BINARY_OPERATOR == 1\n" \
"#define INITIAL_SCAN_VALUE MAX_TYPE_VALUE\n" \
"#define SCAN_OP(a,b) min(a,b)\n" \
"#elif SCAN_BINARY_OPERATOR == 2\n" \
"#define INITIAL_SCAN_VALUE MIN_TYPE_VALUE\n" \
"#define SCAN_OP(a,b) max(a,b)\n" \
"#else\n" \
"#error \"Unknown binary scan operator\"\n" \
"#endif\n" \
"#if ELEMENTS_PER_THREAD == 1\n" \
"#define SUM(v) v\n" \
"#define SCAN(out, v) TYPE out = TYPE(INITIAL_SCAN_VALUE);\n" \
"#define TO_INCLUSIVE(out, v) out = v;\n" \
"#define READ_INDEX(item, index) item\n" \
"#elif ELEMENTS_PER_THREAD == 2\n" \
"#define SUM(v) SCAN_OP(v[0],v[1])\n" \
"#define SCAN(out, v) TYPE out = TYPE(INITIAL_SCAN_VALUE, v[0])\n" \
"#define TO_INCLUSIVE(out, v) out = TYPE(v[0], SCAN_OP(v[0],v[1]))\n" \
"#define READ_INDEX(item, index) item[index]\n" \
"#elif ELEMENTS_PER_THREAD == 4\n" \
"#define SUM(v) SCAN_OP(SCAN_OP(v[0],v[1]),SCAN_OP(v[2],v[3]))\n" \
"#define SCAN(out, v) TYPE out;\\\n" \
"out[0] = INITIAL_SCAN_VALUE;\\\n" \
"out[1] = v[0];\\\n" \
"out[2] = SCAN_OP(v[0],v[1]);\\\n" \
"out[3] = SCAN_OP(out[2],v[2]);\n" \
"#define TO_INCLUSIVE(out, v) out.xyz = out.yzw; \\\n" \
"out.w = SCAN_OP(out.z,v.w);\n" \
"#define READ_INDEX(item, index) item[index]\n" \
"#endif\n" \
#src

GLuint create_compute_program(const glsl_algo_gl_context *gl_context, const char *source, int len);

#endif // SHADER_UTILS_H