#include <common/gl_setup.hpp>
#include <common/scan_utilities.hpp>

#include <glsl_algo/prefix_scan.h>
#include <benchmark/benchmark.h>

#include "benchmark/util.hpp"

static void BM_FullScan(benchmark::State &state) {
    glsl_algo_gl_context glContext;
    init_window_and_gl_context(&glContext);
    
    glsl_algo_configuration conf = {GARWTint1, 1024, 32};
    glsl_algo_context ctx = glsl_algo_init(&glContext, conf);

    const unsigned numBlocks = 1024;
    const unsigned n = 1024 * numBlocks;
    std::vector<unsigned> vec = generateIntegralRandomVector(n, 0u, 3u);
    GLuint inputBuffer = create_ssbo(n, vec.data());
    GLuint intermediateBuffer = create_ssbo(numBlocks);
    GLuint outputBuffer = create_ssbo(n);
    
    while (state.KeepRunning())
    {
        BENCHMARK_GPU(glsl_scan, glsl_scan(&glContext, &ctx, inputBuffer, intermediateBuffer, outputBuffer, n, 1, 1));
    }

	state.SetBytesProcessed(state.iterations() * size_t(n) * sizeof(unsigned) * size_t(3));

    destroy_window_and_gl_context();
}

static void BM_FullScanRW(benchmark::State &state)
{
    glsl_algo_gl_context glContext;
    init_window_and_gl_context(&glContext);
    
    const unsigned numBlocks = 1024;
    const unsigned n = 16 * 1024 * numBlocks;
    std::vector<unsigned> vec = generateIntegralRandomVector(n, 0u, 3u);
    GLuint inputBuffer = create_ssbo(n, vec.data());
    GLuint intermediateBuffer = create_ssbo(numBlocks);
    GLuint outputBuffer = create_ssbo(n);
    
    while (state.KeepRunning())
    {
        
        GLSL_ALGO_READ_WRITE_TYPE dataType = static_cast<GLSL_ALGO_READ_WRITE_TYPE>(state.range(0));
      
        glsl_algo_configuration conf = {dataType, 1024, 32};
        glsl_algo_context ctx = glsl_algo_init(&glContext, conf);
      
        BENCHMARK_GPU(glsl_scan, glsl_scan(&glContext, &ctx, inputBuffer, intermediateBuffer, outputBuffer, n, 1, 1));
    }

	state.SetBytesProcessed(state.iterations() * size_t(n) * sizeof(unsigned) * size_t(3));

    destroy_window_and_gl_context();
}

static void BM_FullScanBlockSize(benchmark::State &state)
{
    glsl_algo_gl_context glContext;
    init_window_and_gl_context(&glContext);
    
    const unsigned numBlocks = 1024 * 128;
    const unsigned n = 16 * 1024 * 1024;
    std::vector<unsigned> vec = generateIntegralRandomVector(n, 0u, 3u);
    GLuint inputBuffer = create_ssbo(n, vec.data());
    GLuint intermediateBuffer = create_ssbo(numBlocks);
    GLuint outputBuffer = create_ssbo(n);
    
    while (state.KeepRunning())
    {
        unsigned int blockSize = static_cast<unsigned int>(state.range(0));
        glsl_algo_configuration conf = {GARWTint1, blockSize, 32};
        glsl_algo_context ctx = glsl_algo_init(&glContext, conf);
      
        BENCHMARK_GPU(glsl_scan, glsl_scan(&glContext, &ctx, inputBuffer, intermediateBuffer, outputBuffer, n, 1, 1));
    }

	state.SetBytesProcessed(state.iterations() * size_t(n) * sizeof(unsigned) * size_t(3));

    destroy_window_and_gl_context();
}

static void BM_FullScanElementsPerThread(benchmark::State &state) {
    glsl_algo_gl_context glContext;
    init_window_and_gl_context(&glContext);
    
    glsl_algo_configuration conf = {GARWTint1, 1024, 32};
    glsl_algo_context ctx = glsl_algo_init(&glContext, conf);

    const unsigned numBlocks = 128 * 1024;
    const unsigned n = 1024 * 1024 * 16;
    std::vector<unsigned> vec = generateIntegralRandomVector(n, 0u, 3u);
    GLuint inputBuffer = create_ssbo(n, vec.data());
    GLuint intermediateBuffer = create_ssbo(numBlocks);
    GLuint outputBuffer = create_ssbo(n);
    
    while (state.KeepRunning())
    {
        unsigned int elementsPerThread = static_cast<unsigned int>(state.range(0));
        BENCHMARK_GPU(glsl_scan, glsl_scan(&glContext, &ctx, inputBuffer, intermediateBuffer, outputBuffer, n, elementsPerThread, 1));
    }

	state.SetBytesProcessed(state.iterations() * size_t(n) * sizeof(unsigned) * size_t(3));

    destroy_window_and_gl_context();
}

static void BM_FullScanMultipleRanges(benchmark::State &state)
{
    glsl_algo_gl_context glContext;
    init_window_and_gl_context(&glContext);

    const unsigned n = 8 * 1024 * 1024;
    std::vector<unsigned> vec = generateIntegralRandomVector(n, 0u, 3u);
    GLuint inputBuffer = create_ssbo(n, vec.data());
    GLuint intermediateBuffer = create_ssbo(1024*128);
    GLuint outputBuffer = create_ssbo(n);

    while (state.KeepRunning())
    {
        GLSL_ALGO_READ_WRITE_TYPE type = static_cast<GLSL_ALGO_READ_WRITE_TYPE>(state.range(0));
        unsigned int blockSize = static_cast<unsigned int>(state.range(1));
        unsigned int readsPerThread = static_cast<unsigned int>(state.range(2));
        glsl_algo_configuration conf = {type, blockSize, 32};
        glsl_algo_context ctx = glsl_algo_init(&glContext, conf);
      
        BENCHMARK_GPU(glsl_scan, glsl_scan(&glContext, &ctx, inputBuffer, intermediateBuffer, outputBuffer, n, readsPerThread, 1));

    }

	state.SetBytesProcessed(state.iterations() * size_t(n) * sizeof(unsigned) * size_t(3));
    destroy_window_and_gl_context();
}

BENCHMARK(BM_FullScan)->UseManualTime();
BENCHMARK(BM_FullScanRW)->Arg(GARWTint1)->Arg(GARWTint2)->Arg(GARWTint4)->UseManualTime();
BENCHMARK(BM_FullScanBlockSize)->Arg(128)->Arg(256)->Arg(384)->Arg(512)->Arg(768)->Arg(1024)->UseManualTime();
BENCHMARK(BM_FullScanElementsPerThread)->Arg(1)->Arg(2)->Arg(4)->Arg(8)->Arg(16)->Arg(32)->UseManualTime();

static void GenerateFullBenchmark(benchmark::internal::Benchmark* b) {
    GLSL_ALGO_READ_WRITE_TYPE rwTypes[] = {GARWTint1, GARWTint2, GARWTint4};
    
    for (unsigned int t = 0u; t < 3u; ++t)
    {
        GLSL_ALGO_READ_WRITE_TYPE type = rwTypes[t];
        for (unsigned int blockSize = 128u; blockSize <= 1024u; blockSize += 128u)
        {
            for (unsigned int readsPerThread = 1u; readsPerThread <= 32u; readsPerThread*=2)
            {
                b->Args({static_cast<int>(type), static_cast<int>(blockSize), static_cast<int>(readsPerThread)});
            }
        }
    }
}

BENCHMARK(BM_FullScanMultipleRanges)->Apply(GenerateFullBenchmark)->UseManualTime();