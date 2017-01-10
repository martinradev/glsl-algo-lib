#ifndef SHADER_UTILS_H
#define SHADER_UTILS_H

#define SHADER(version,src) "#version " #version "\n" \
"#define TYPE %s\n" \
"#define SCALAR_TYPE %s\n" \
"#define BLOCK_SIZE %d\n" \
"#define ELEMENTS_PER_THREAD %d\n" \
"#define WARP_SIZE %d\n" \
"#if ELEMENTS_PER_THREAD == 1\n" \
"#define SUM(v) v\n" \
"#elif ELEMENTS_PER_THREAD == 2\n" \
"#define SUM(v) v[0]+v[1]\n" \
"#elif ELEMENTS_PER_THREAD == 4\n" \
"#define SUM(v) v[0]+v[1]+v[2]+v[3]\n" \
"#endif\n" \
#src

#endif // SHADER_UTILS_H