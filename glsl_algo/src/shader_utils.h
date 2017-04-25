#ifndef SHADER_UTILS_H
#define SHADER_UTILS_H

#define SHADER(version,src) "#version " #version "\n" \
"#define TYPE %s\n" \
"#define SCALAR_TYPE %s\n" \
"#define BLOCK_SIZE %u\n" \
"#define ELEMENTS_PER_THREAD %u\n" \
"#define WARP_SIZE %u\n" \
"#define RADIX_SIZE %u\n" \
"#define GET_LANE_ID(t) (t&(WARP_SIZE-1u))\n" \
"#define GET_WARP_ID(t) (t/WARP_SIZE)\n" \
"#define RADIX_SORT_NUM_PASSES %u\n" \
"#if ELEMENTS_PER_THREAD == 1\n" \
"#define SUM(v) v\n" \
"#define SCAN(out, v) TYPE out = TYPE(0);\n" \
"#define TO_INCLUSIVE(out, v) out = v;\n" \
"#define READ_INDEX(item, index) item\n" \
"#elif ELEMENTS_PER_THREAD == 2\n" \
"#define SUM(v) v[0]+v[1]\n" \
"#define SCAN(out, v) TYPE out = TYPE(0, v[0])\n" \
"#define TO_INCLUSIVE(out, v) out = TYPE(v[0], v[0]+v[1])\n" \
"#define READ_INDEX(item, index) item[index]\n" \
"#elif ELEMENTS_PER_THREAD == 4\n" \
"#define SUM(v) (v[0]+v[1])+(v[2]+v[3])\n" \
"#define SCAN(out, v) TYPE out;\\\n" \
"out[0] = 0;\\\n" \
"out[1] = v[0];\\\n" \
"out[2] = v[0]+v[1];\\\n" \
"out[3] = out[2]+v[2];\n" \
"#define TO_INCLUSIVE(out, v) out.xyz = out.yzw; \\\n" \
"out.w = out.z+v.w;\n" \
"#define READ_INDEX(item, index) item[index]\n" \
"#endif\n" \
#src

#endif // SHADER_UTILS_H