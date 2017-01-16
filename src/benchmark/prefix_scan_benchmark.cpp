#include "gl_setup.hpp"
#include "scan_utilities.hpp"

#include "util.hpp"
#include <glsl_algo/prefix_scan.h>
#include <benchmark/benchmark.h>

static void BM_FullScan(benchmark::State &state) {
    init_window_and_gl_context();
    
    glsl_algo_configuration conf = {GARWTint1, 1024, 32};
    glsl_algo_context ctx = glsl_algo_init(conf);

    const unsigned numBlocks = 1024;
    const unsigned n = 1024 * numBlocks;
    std::vector<unsigned> vec = generateIntegralRandomVector(n, 0u, 3u);
    GLuint inputBuffer = create_ssbo(n, vec.data());
    GLuint intermediateBuffer = create_ssbo(numBlocks);
    GLuint outputBuffer = create_ssbo(n);
    
    GLuint queryObject;
    glGenQueries(1, &queryObject);
    
    while (state.KeepRunning())
    {
        BENCHMARK_GPU(glsl_scan(&ctx, inputBuffer, intermediateBuffer, outputBuffer, n, 1, 1), queryObject);
    }

    destroy_window_and_gl_context();
}

static void BM_FullScanRW(benchmark::State &state)
{
    init_window_and_gl_context();
    
    const unsigned numBlocks = 1024;
    const unsigned n = 16 * 1024 * numBlocks;
    std::vector<unsigned> vec = generateIntegralRandomVector(n, 0u, 3u);
    GLuint inputBuffer = create_ssbo(n, vec.data());
    GLuint intermediateBuffer = create_ssbo(numBlocks);
    GLuint outputBuffer = create_ssbo(n);
    
    GLuint queryObject;
    glGenQueries(1, &queryObject);
    
    while (state.KeepRunning())
    {
        
        GLSL_ALGO_READ_WRITE_TYPE dataType = static_cast<GLSL_ALGO_READ_WRITE_TYPE>(state.range(0));
      
        glsl_algo_configuration conf = {dataType, 1024, 32};
        glsl_algo_context ctx = glsl_algo_init(conf);
      
        BENCHMARK_GPU(glsl_scan(&ctx, inputBuffer, intermediateBuffer, outputBuffer, n, 1, 1), queryObject);
    }
    destroy_window_and_gl_context();
}

static void BM_FullScanBlockSize(benchmark::State &state)
{
    init_window_and_gl_context();
    
    const unsigned numBlocks = 1024;
    const unsigned n = 16 * 1024 * numBlocks;
    std::vector<unsigned> vec = generateIntegralRandomVector(n, 0u, 3u);
    GLuint inputBuffer = create_ssbo(n, vec.data());
    GLuint intermediateBuffer = create_ssbo(numBlocks);
    GLuint outputBuffer = create_ssbo(n);
    
    GLuint queryObject;
    glGenQueries(1, &queryObject);
    
    while (state.KeepRunning())
    {
        unsigned int blockSize = static_cast<unsigned int>(state.range(0));
        glsl_algo_configuration conf = {GARWTint1, blockSize, 32};
        glsl_algo_context ctx = glsl_algo_init(conf);
      
        BENCHMARK_GPU(glsl_scan(&ctx, inputBuffer, intermediateBuffer, outputBuffer, n, 1, 1), queryObject);
    }
    destroy_window_and_gl_context();
}

// Register the function as a benchmark
BENCHMARK(BM_FullScan);
BENCHMARK(BM_FullScanRW)->Arg(GARWTint1)->Arg(GARWTint2)->Arg(GARWTint4);
BENCHMARK(BM_FullScanBlockSize)->Arg(128)->Arg(256)->Arg(384)->Arg(512)->Arg(768)->Arg(1024);
