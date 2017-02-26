#include <common/gl_setup.hpp>
#include <common/scan_utilities.hpp>

#include <glsl_algo/memory.h>
#include <benchmark/benchmark.h>

#include "benchmark/util.hpp"
#include <iostream>

static void BM_CopyMemory(benchmark::State &state) {
    glsl_algo_gl_context glContext;
    init_window_and_gl_context(&glContext);

    const unsigned n = 8 * 1024 * 1024;
    std::vector<unsigned> vec(n);
    GLuint inputBuffer = create_ssbo(n, vec.data());
    GLuint outputBuffer = create_ssbo(n);
    
    while (state.KeepRunning())
    {
        GLSL_ALGO_READ_WRITE_TYPE type = static_cast<GLSL_ALGO_READ_WRITE_TYPE>(state.range(0));
        unsigned int blockSize = static_cast<unsigned int>(state.range(1));
        unsigned int readsPerThread = static_cast<unsigned int>(state.range(2));
        glsl_algo_configuration conf = {type, blockSize, 32};
        glsl_algo_context ctx = glsl_algo_init(&glContext, conf);
        
        BENCHMARK_GPU(glsl_copy_memory, glsl_copy_memory(&glContext, &ctx, inputBuffer, outputBuffer, n, readsPerThread));
    }

	state.SetBytesProcessed(state.iterations() * size_t(n) * sizeof(unsigned) * size_t(2));

    destroy_window_and_gl_context();
}

static void GenerateFullBenchmark(benchmark::internal::Benchmark* b) {
    GLSL_ALGO_READ_WRITE_TYPE rwTypes[] = {GARWTint1, GARWTint2, GARWTint4};
    
    for (unsigned int t = 0u; t < 3u; ++t)
    {
        GLSL_ALGO_READ_WRITE_TYPE type = rwTypes[t];
        for (unsigned int blockSize = 128u; blockSize <= 1024u; blockSize += 128u)
        {
            for (unsigned int readsPerThread = 1u; readsPerThread <= 8; readsPerThread*=2)
            {
                b->Args({static_cast<int>(type), static_cast<int>(blockSize), static_cast<int>(readsPerThread)});
            }
        }
    }
}

BENCHMARK(BM_CopyMemory)->Apply(GenerateFullBenchmark)->UseManualTime();