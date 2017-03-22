#include <common/gl_setup.hpp>
#include <common/scan_utilities.hpp>

#include <glsl_algo/radix_sort.h>
#include <benchmark/benchmark.h>

#include "benchmark/util.hpp"

static void BM_RadixSortMultipleRanges(benchmark::State &state)
{
    glsl_algo_gl_context glContext;
    init_window_and_gl_context(&glContext);

    const unsigned n = 8 * 1024 * 1024;
    std::vector<int> vec = generateIntegralRandomVector(n, 0, 1<<30);
    GLuint inputBuffer = create_ssbo(n, vec.data());
    GLuint pingPongBuffer = create_ssbo(n);
    GLuint radixBuffer = create_ssbo(1024*256);
    GLuint outputBuffer = create_ssbo(n);
    
    GLuint queryObject;
    glGenQueries(1, &queryObject);
    
    while (state.KeepRunning())
    {
        GLSL_ALGO_READ_WRITE_TYPE type = static_cast<GLSL_ALGO_READ_WRITE_TYPE>(state.range(0));
        unsigned int blockSize = static_cast<unsigned int>(state.range(1));
        unsigned int readsPerThread = static_cast<unsigned int>(state.range(2));
        unsigned int radixSize = static_cast<unsigned int>(state.range(3));
        glsl_algo_configuration conf = {type, blockSize, 32, radixSize};
        glsl_algo_context ctx = glsl_algo_init(&glContext, conf);
      
        BENCHMARK_GPU(glsl_radix_sort(&glContext, &ctx, inputBuffer, radixBuffer, pingPongBuffer, outputBuffer, n, readsPerThread), queryObject);

    }

	state.SetBytesProcessed(state.iterations() * size_t(n) * sizeof(unsigned) * size_t(3));
    destroy_window_and_gl_context();
}

static void GenerateFullBenchmark(benchmark::internal::Benchmark* b) {
    GLSL_ALGO_READ_WRITE_TYPE rwTypes[] = {GARWTint1, GARWTint2, GARWTint4};
    
    for (unsigned r = 1u; r <= 4; ++r)
    {
        for (unsigned int t = 0u; t < 3u; ++t)
        {
            GLSL_ALGO_READ_WRITE_TYPE type = rwTypes[t];
            for (unsigned int blockSize = 128u; blockSize <= 1024u; blockSize += 128u)
            {
                for (unsigned int readsPerThread = 1u; readsPerThread <= 128u; readsPerThread*=2)
                {
                    b->Args({static_cast<int>(type), static_cast<int>(blockSize), static_cast<int>(readsPerThread), static_cast<int>(r)});
                }
            }
        }
    }
}

BENCHMARK(BM_RadixSortMultipleRanges)->Apply(GenerateFullBenchmark)->UseManualTime();