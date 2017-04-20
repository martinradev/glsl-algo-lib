#include <common/gl_setup.hpp>
#include <common/scan_utilities.hpp>

#include <glsl_algo/radix_sort.h>
#include <benchmark/benchmark.h>

#include "benchmark/util.hpp"

#include <iostream>
#include <algorithm>
#include <chrono>

static void BM_RadixSortMultipleRanges(benchmark::State &state)
{
    glsl_algo_gl_context glContext;
    init_window_and_gl_context(&glContext);

    const unsigned n = 16 * 1024 * 1024;
    std::vector<unsigned> vec = generateIntegralRandomVector(n, 0u, 1u<<30);
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
        glsl_algo_configuration conf = {type, blockSize, 32, radixSize, readsPerThread};
        glsl_algo_context ctx = glsl_algo_init(&glContext, conf);
      
        BENCHMARK_GPU(glsl_radix_sort(&glContext, &ctx, inputBuffer, radixBuffer, pingPongBuffer, outputBuffer, n, readsPerThread), queryObject);
    }

	state.SetItemsProcessed(state.iterations() * n);
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
                for (unsigned int readsPerThread = 1u; readsPerThread <= 32u; readsPerThread*=2)
                {
					if (glsl_algo_get_rw_num_elements(type) * blockSize * readsPerThread * 8 > 32 * 1024) continue;
                    b->Args({static_cast<int>(type), static_cast<int>(blockSize), static_cast<int>(readsPerThread), static_cast<int>(r)});
                }
            }
        }
    }
}

BENCHMARK(BM_RadixSortMultipleRanges)->Apply(GenerateFullBenchmark)->UseManualTime();


static void BM_RadixSortBenchmarkScalability(benchmark::State &state)
{
	glsl_algo_gl_context glContext;
	init_window_and_gl_context(&glContext);

	glsl_algo_configuration conf = { GARWTint1, 256, 32, 2, 8};
	glsl_algo_context ctx = glsl_algo_init(&glContext, conf);

	const unsigned n = state.range(0);
	std::vector<unsigned> vec = generateIntegralRandomVector(n, 0u, 1u << 30);
	GLuint inputBuffer = create_ssbo(n, vec.data());
	GLuint pingPongBuffer = create_ssbo(n);

	const unsigned elementsPerBlock = 256 * 8;
	const unsigned radixBufferSize = 4 * (n + elementsPerBlock - 1) / elementsPerBlock;
	GLuint radixBuffer = create_ssbo(radixBufferSize);
	GLuint outputBuffer = create_ssbo(n);

	GLuint queryObject;
	glGenQueries(1, &queryObject);

	while (state.KeepRunning())
	{
		BENCHMARK_GPU(glsl_radix_sort(&glContext, &ctx, inputBuffer, radixBuffer, pingPongBuffer, outputBuffer, n, 8), queryObject);
	}

	/*std::string prog = get_program_binary(ctx.radix_sort_scatter_program);
	std::cout << prog << std::endl;
	exit(1);*/

	state.SetItemsProcessed(state.iterations() * n);
	destroy_window_and_gl_context();
}

static void GenerateScalabilityBenchmark(benchmark::internal::Benchmark* b) {
	const unsigned maxn = 128 * 1024 * 1024;
	const unsigned step = 4 *32 * 1024;
	for (int r = step; r <= maxn; r += step)
	{
		b->Args({r});
	}
}

BENCHMARK(BM_RadixSortBenchmarkScalability)->Apply(GenerateScalabilityBenchmark)->UseManualTime();

static void BM_STLSortBenchmarkScalability(benchmark::State &state)
{
	const unsigned n = state.range(0);

	while (state.KeepRunning())
	{
		std::vector<unsigned> vec = generateIntegralRandomVector(n, 0u, 1u << 30);

		auto start = std::chrono::high_resolution_clock::now();
		std::sort(vec.begin(), vec.end());
		auto end = std::chrono::high_resolution_clock::now();
		std::chrono::duration<double> diff = end - start;
		state.SetIterationTime(diff.count());
	}

	state.SetItemsProcessed(state.iterations() * n);
}

BENCHMARK(BM_STLSortBenchmarkScalability)->Apply(GenerateScalabilityBenchmark)->UseManualTime();