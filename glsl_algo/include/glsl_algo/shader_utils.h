#ifndef SHADER_UTILS_H
#define SHADER_UTILS_H

#define SHADER(version,src) "#version " #version "\n" \
"#define TYPE %s\n" \
"#define SCALAR_TYPE %s\n" \
"#define BLOCK_SIZE %u\n" \
"#define ELEMENTS_PER_THREAD %u\n" \
"#define WARP_SIZE %u\n" \
"#if ELEMENTS_PER_THREAD == 1\n" \
"#define SUM(v) v\n" \
"#define SCAN(v) 0\n" \
"#define INCLUSIVE_SCAN(v) v\n" \
"#elif ELEMENTS_PER_THREAD == 2\n" \
"#define SUM(v) v[0]+v[1]\n" \
"#define SCAN(v) TYPE(0, v[0])\n" \
"#define INCLUSIVE_SCAN(v) TYPE(v[0], v[0]+v[1])\n" \
"#elif ELEMENTS_PER_THREAD == 4\n" \
"#define SUM(v) v[0]+v[1]+v[2]+v[3]\n" \
"#define SCAN(v) TYPE(0, v[0], v[0]+v[1], v[0]+v[1]+v[2])\n" \
"#define INCLUSIVE_SCAN(v) TYPE(v[0], v[0]+v[1], v[0]+v[1]+v[2], v[0]+v[1]+v[2]+v[3])\n" \
"#endif\n" \
"#define DEFINE_MACRO #define\n" \
"#define IF_MACRO #if\n" \
"#define ENDIF_MACRO #endif\n" \
#src

#endif // SHADER_UTILS_H