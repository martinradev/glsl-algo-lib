#include <common/gl_setup.hpp>
#include <common/radix_sort_util.hpp>
#include <common/macros.hpp>

#include <glsl_algo/radix_sort.h>
#include <gtest/gtest.h>

#include <algorithm>

class RadixSortTest : public ::testing::Test
{
protected:
    virtual void SetUp()
    {
        init_window_and_gl_context(&mGLContext);
    }
    virtual void TearDown()
    {
        destroy_window_and_gl_context();
    }
    glsl_algo_gl_context mGLContext;
};

TEST_F(RadixSortTest, GatherBaseTest)
{
    glsl_algo_configuration conf = {GARWTuint1, 256, 32, 1, 1};
    glsl_algo_context ctx = glsl_algo_init(&mGLContext, conf);
  
    const unsigned numBlocks = 32;
    const unsigned n = 256*numBlocks;
    std::vector<unsigned> vec = generateIntegralRandomVector(n, 0u, 138791u);
    GLuint inputBuffer = create_ssbo(n, vec.data());
    
    const unsigned radixBufferSize = 2 * n / conf.local_block_size;
    GLuint radixBuffer = create_ssbo(radixBufferSize);
    
    glsl_radix_sort_gather(&mGLContext, &ctx, inputBuffer, radixBuffer, n, 1u, 0u);
    
    std::vector<unsigned> result(radixBufferSize);
    get_ssbo_data(radixBuffer, result.size(), result.data());
    
    std::vector<unsigned> expectedResult(result.size());
    
    radix_sort_gather(vec.data(), expectedResult.data(), n, 256, 0, 1);
    
    EXPECT_EQ(expectedResult, result);
}

TEST_F(RadixSortTest, GatherBaseTestInt4)
{
    glsl_algo_configuration conf = {GARWTuint4, 256, 32, 1, 1};
    glsl_algo_context ctx = glsl_algo_init(&mGLContext, conf);
  
    const unsigned numBlocks = 10;
    const unsigned n = 4*256*numBlocks;
    std::vector<unsigned> vec = generateIntegralRandomVector(n, 0u, 138791u);
    GLuint inputBuffer = create_ssbo(n, vec.data());
    
    const unsigned radixBufferSize = 2 * n / (conf.local_block_size*4);
    GLuint radixBuffer = create_ssbo(radixBufferSize);
    
    glsl_radix_sort_gather(&mGLContext, &ctx, inputBuffer, radixBuffer, n, 1u, 0u);
    
    std::vector<unsigned> result(radixBufferSize);
    get_ssbo_data(radixBuffer, result.size(), result.data());
    
    std::vector<unsigned> expectedResult(result.size());
    
    radix_sort_gather(vec.data(), expectedResult.data(), n, 1024, 0, 1);
    
    EXPECT_EQ(expectedResult, result);
}

TEST_F(RadixSortTest, GatherBaseTestBiggerBlockSize)
{
    glsl_algo_configuration conf = {GARWTuint4, 512, 32, 1, 1};
    glsl_algo_context ctx = glsl_algo_init(&mGLContext, conf);
  
    const unsigned numBlocks = 10;
    const unsigned n = 4*512*numBlocks;
    std::vector<unsigned> vec = generateIntegralRandomVector(n, 0u, 138791u);
    GLuint inputBuffer = create_ssbo(n, vec.data());
    
    const unsigned radixBufferSize = 2 * n / (conf.local_block_size*4);
    GLuint radixBuffer = create_ssbo(radixBufferSize);
    
    glsl_radix_sort_gather(&mGLContext, &ctx, inputBuffer, radixBuffer, n, 1u, 0u);
    
    std::vector<unsigned> result(radixBufferSize);
    get_ssbo_data(radixBuffer, result.size(), result.data());
    
    std::vector<unsigned> expectedResult(result.size());
    
    radix_sort_gather(vec.data(), expectedResult.data(), n, 4*512, 0, 1);
    
    EXPECT_EQ(expectedResult, result);
}

TEST_F(RadixSortTest, GatherBaseTestMultipleElementsPerThread)
{
    glsl_algo_configuration conf = {GARWTuint4, 256, 32, 1, 2};
    glsl_algo_context ctx = glsl_algo_init(&mGLContext, conf);
  
    const unsigned numElementsPerThread = 2;
    const unsigned numBlocks = 10;
    const unsigned n = 4* 256 *numBlocks;
    std::vector<unsigned> vec = generateIntegralRandomVector(n, 0u, 138791u);
    GLuint inputBuffer = create_ssbo(n, vec.data());
    
    const unsigned elementsPerBlock = numElementsPerThread*4*conf.local_block_size;
    const unsigned radixBufferSize = 2 * ((n + elementsPerBlock - 1) / elementsPerBlock);
    GLuint radixBuffer = create_ssbo(radixBufferSize);
    
    glsl_radix_sort_gather(&mGLContext, &ctx, inputBuffer, radixBuffer, n, numElementsPerThread, 0u);
    
    std::vector<unsigned> result(radixBufferSize);
    get_ssbo_data(radixBuffer, result.size(), result.data());
    
    std::vector<unsigned> expectedResult(result.size());
    
    radix_sort_gather(vec.data(), expectedResult.data(), n, elementsPerBlock, 0, 1);
    
    EXPECT_EQ(expectedResult, result);
}

TEST_F(RadixSortTest, GatherBaseBiggerRadix)
{
    glsl_algo_configuration conf = {GARWTuint4, 256, 32, 4, 2};
    glsl_algo_context ctx = glsl_algo_init(&mGLContext, conf);
  
    const unsigned numElementsPerThread = 2;
    const unsigned numBlocks = 10;
    const unsigned n = 4* 256 *numBlocks;
    std::vector<unsigned> vec = generateIntegralRandomVector(n, 0u, 138791u);
    GLuint inputBuffer = create_ssbo(n, vec.data());
    
    const unsigned elementsPerBlock = numElementsPerThread*4*conf.local_block_size;
    const unsigned radixBufferSize = 16 * ((n + elementsPerBlock - 1) / elementsPerBlock);
    GLuint radixBuffer = create_ssbo(radixBufferSize);
    
    glsl_radix_sort_gather(&mGLContext, &ctx, inputBuffer, radixBuffer, n, numElementsPerThread, 0u);
    
    std::vector<unsigned> result(radixBufferSize);
    get_ssbo_data(radixBuffer, result.size(), result.data());
    
    std::vector<unsigned> expectedResult(result.size());
    
    radix_sort_gather(vec.data(), expectedResult.data(), n, elementsPerBlock, 0, 4);
    
    EXPECT_EQ(expectedResult, result);
}

TEST_F(RadixSortTest, GatherBaseBiggerRadixWithOffset)
{
    glsl_algo_configuration conf = {GARWTuint4, 256, 32, 4, 2};
    glsl_algo_context ctx = glsl_algo_init(&mGLContext, conf);
  
    const unsigned numElementsPerThread = 2;
    const unsigned numBlocks = 10;
    const unsigned n = 4* 256 *numBlocks;
    std::vector<unsigned> vec = generateIntegralRandomVector(n, 0u, 138791u);
    GLuint inputBuffer = create_ssbo(n, vec.data());
    
    const unsigned elementsPerBlock = numElementsPerThread*4*conf.local_block_size;
    const unsigned radixBufferSize = 16 * ((n + elementsPerBlock - 1) / elementsPerBlock);
    GLuint radixBuffer = create_ssbo(radixBufferSize);
    
    glsl_radix_sort_gather(&mGLContext, &ctx, inputBuffer, radixBuffer, n, numElementsPerThread, 16u);
    
    std::vector<unsigned> result(radixBufferSize);
    get_ssbo_data(radixBuffer, result.size(), result.data());
    
    std::vector<unsigned> expectedResult(result.size());
    
    radix_sort_gather(vec.data(), expectedResult.data(), n, elementsPerBlock, 16, 4);
    
    EXPECT_EQ(expectedResult, result);
}

TEST_F(RadixSortTest, TestUtilSinglePass)
{
    unsigned n = 1<<14;
    std::vector<unsigned> vec = generateIntegralRandomVector(n, 0u, (1u<<16)-1);
    std::vector<unsigned> result(n);
    radix_sort_single_pass(vec.data(), result.data(), n, 0u, 16);
    std::sort(vec.begin(), vec.end());
    EXPECT_EQ(vec, result);
}

TEST_F(RadixSortTest, TestScatterSingleBlock)
{
    glsl_algo_configuration conf = {GARWTuint1, 512, 32, 1, 1};
    glsl_algo_context ctx = glsl_algo_init(&mGLContext, conf);
  

    const unsigned n = 512;
    std::vector<unsigned> vec = generateIntegralRandomVector(n, 0u, 16u);
    GLuint inputBuffer = create_ssbo(n, vec.data());
    
    GLuint outputBuffer = create_ssbo(n);
    
    const unsigned radixBufferSize = 2;
    GLuint radixBuffer = create_ssbo(radixBufferSize);
    
    glsl_radix_sort_pass(&mGLContext, &ctx, inputBuffer, radixBuffer, outputBuffer, n, 1, 0);
    
    std::vector<unsigned> result(n);
    get_ssbo_data(outputBuffer, result.size(), result.data());

    std::vector<unsigned> expectedResult(result.size());
    radix_sort_single_pass(vec.data(), expectedResult.data(), n, 0u, 1u);

    EXPECT_EQ(expectedResult, result);
}

TEST_F(RadixSortTest, TestScatterMultipleBlocks)
{
    glsl_algo_configuration conf = {GARWTuint1, 512, 32, 1, 1};
    glsl_algo_context ctx = glsl_algo_init(&mGLContext, conf);
  

    const unsigned n = 512*16;
    std::vector<unsigned> vec = generateIntegralRandomVector(n, 0u, 16u);
    GLuint inputBuffer = create_ssbo(n, vec.data());
    
    GLuint outputBuffer = create_ssbo(n);
    
    const unsigned radixBufferSize = 2*16;
    GLuint radixBuffer = create_ssbo(radixBufferSize);
    
    glsl_radix_sort_pass(&mGLContext, &ctx, inputBuffer, radixBuffer, outputBuffer, n, 1, 0);
    
    std::vector<unsigned> result(n);
    get_ssbo_data(outputBuffer, result.size(), result.data());
    
    std::vector<unsigned> expectedResult(result.size());
    radix_sort_single_pass(vec.data(), expectedResult.data(), n, 0u, 1u);
    
    EXPECT_EQ(expectedResult, result);
}

TEST_F(RadixSortTest, TestScatterMultipleBlocksBigRadix)
{
    glsl_algo_configuration conf = {GARWTuint1, 512, 32, 4, 1};
    glsl_algo_context ctx = glsl_algo_init(&mGLContext, conf);
  

    const unsigned n = 512*16;
    std::vector<unsigned> vec = generateIntegralRandomVector(n, 0u, 16u);
    GLuint inputBuffer = create_ssbo(n, vec.data());
    
    GLuint outputBuffer = create_ssbo(n);
    
    const unsigned radixBufferSize = 16*16;
    GLuint radixBuffer = create_ssbo(radixBufferSize);
    
    glsl_radix_sort_pass(&mGLContext, &ctx, inputBuffer, radixBuffer, outputBuffer, n, 1, 0);
    
    std::vector<unsigned> result(n);
    get_ssbo_data(outputBuffer, result.size(), result.data());
    
    std::vector<unsigned> expectedResult(result.size());
    radix_sort_single_pass(vec.data(), expectedResult.data(), n, 0u, 4u);
    
    EXPECT_EQ(expectedResult, result);
}

TEST_F(RadixSortTest, TestScatterSingleBlockInt4)
{
    glsl_algo_configuration conf = {GARWTuint4, 512, 32, 1, 1};
    glsl_algo_context ctx = glsl_algo_init(&mGLContext, conf);
  

    const unsigned n = 4*512;
    std::vector<unsigned> vec = generateIntegralRandomVector(n, 0u, 16u);
    GLuint inputBuffer = create_ssbo(n, vec.data());
    
    GLuint outputBuffer = create_ssbo(n);
    
    const unsigned radixBufferSize = 2;
    GLuint radixBuffer = create_ssbo(radixBufferSize);
    
    glsl_radix_sort_pass(&mGLContext, &ctx, inputBuffer, radixBuffer, outputBuffer, n, 1, 0);
    
    std::vector<unsigned> result(n);
    get_ssbo_data(outputBuffer, result.size(), result.data());
    
    std::vector<unsigned> expectedResult(result.size());
    radix_sort_single_pass(vec.data(), expectedResult.data(), n, 0u, 1u);

    EXPECT_EQ(expectedResult, result);
}

TEST_F(RadixSortTest, TestScatterSingleSmallBlock)
{
    glsl_algo_configuration conf = {GARWTuint1, 384, 32, 1, 1};
    glsl_algo_context ctx = glsl_algo_init(&mGLContext, conf);
  

    const unsigned n = 384;
    std::vector<unsigned> vec = generateIntegralRandomVector(n, 0u, 16u);
    GLuint inputBuffer = create_ssbo(n, vec.data());
    
    GLuint outputBuffer = create_ssbo(n);
    
    const unsigned radixBufferSize = 2;
    GLuint radixBuffer = create_ssbo(radixBufferSize);
    
    glsl_radix_sort_pass(&mGLContext, &ctx, inputBuffer, radixBuffer, outputBuffer, n, 1, 0);
    
    std::vector<unsigned> result(n);
    get_ssbo_data(outputBuffer, result.size(), result.data());
    
    std::vector<unsigned> expectedResult(result.size());
    radix_sort_single_pass(vec.data(), expectedResult.data(), n, 0u, 1u);
    
    EXPECT_EQ(expectedResult, result);
}

TEST_F(RadixSortTest, TestScatterSingleBlockMultipleElements)
{
    glsl_algo_configuration conf = {GARWTuint1, 512, 32, 1, 2};
    glsl_algo_context ctx = glsl_algo_init(&mGLContext, conf);
  
    const unsigned n = 2*512;
    std::vector<unsigned> vec = generateIntegralRandomVector(n, 0u, 16u);
    GLuint inputBuffer = create_ssbo(n, vec.data());
    
    GLuint outputBuffer = create_ssbo(n);
    
    const unsigned radixBufferSize = 2;
    GLuint radixBuffer = create_ssbo(radixBufferSize);
    
    glsl_radix_sort_pass(&mGLContext, &ctx, inputBuffer, radixBuffer, outputBuffer, n, 2, 0);
    
    std::vector<unsigned> result(n);
    get_ssbo_data(outputBuffer, result.size(), result.data());
    
    std::vector<unsigned> expectedResult(result.size());
    radix_sort_single_pass(vec.data(), expectedResult.data(), n, 0u, 1u);
    
    EXPECT_EQ(expectedResult, result);
}

TEST_F(RadixSortTest, TestSimpleSort)
{
    glsl_algo_configuration conf = {GARWTuint1, 512, 32, 1, 1};
    glsl_algo_context ctx = glsl_algo_init(&mGLContext, conf);
  
    const unsigned n = 256*512;
    std::vector<unsigned> vec = generateIntegralRandomVector(n, 0u, 1u<<30);
    GLuint inputBuffer = create_ssbo(n, vec.data());
    GLuint pingPongBuffer = create_ssbo(n);
    GLuint outputBuffer = create_ssbo(n);
    
    const unsigned radixBufferSize = 2*256;
    GLuint radixBuffer = create_ssbo(radixBufferSize);
    
    glsl_radix_sort(&mGLContext, &ctx, inputBuffer, radixBuffer, pingPongBuffer, outputBuffer, n, 1);
    
    std::vector<unsigned> result(n);
    get_ssbo_data(outputBuffer, result.size(), result.data());
    
    std::sort(vec.begin(), vec.end());
    
    EXPECT_EQ(vec, result);
}

TEST_F(RadixSortTest, TestComplicatedSort)
{
    glsl_algo_configuration conf = {GARWTuint4, 256, 32, 4, 2};
    glsl_algo_context ctx = glsl_algo_init(&mGLContext, conf);
  
    const unsigned n = 256*1024;
    std::vector<unsigned> vec = generateIntegralRandomVector(n, 0u, 1u<<30);
    GLuint inputBuffer = create_ssbo(n, vec.data());
    GLuint pingPongBuffer = create_ssbo(n);
    GLuint outputBuffer = create_ssbo(n);
    
    const unsigned radixBufferSize = 16*512;
    GLuint radixBuffer = create_ssbo(radixBufferSize);
    
    glsl_radix_sort(&mGLContext, &ctx, inputBuffer, radixBuffer, pingPongBuffer, outputBuffer, n, 2);
    
    std::vector<unsigned> result(n);
    get_ssbo_data(outputBuffer, result.size(), result.data());
    
    std::sort(vec.begin(), vec.end());
    
    EXPECT_EQ(vec, result);
}

TEST_F(RadixSortTest, TestEarlyExitCase)
{
	glsl_algo_configuration conf = { GARWTuint4, 256, 32, 4, 2 };
	glsl_algo_context ctx = glsl_algo_init(&mGLContext, conf);

	const unsigned n = 256 * 1024;
	std::vector<unsigned> vec = generateIntegralRandomVector(n, 0u, 1u << 2);
	GLuint inputBuffer = create_ssbo(n, vec.data());
	GLuint pingPongBuffer = create_ssbo(n);
	GLuint outputBuffer = create_ssbo(n);

	const unsigned radixBufferSize = 16 * 512;
	GLuint radixBuffer = create_ssbo(radixBufferSize);

	glsl_radix_sort(&mGLContext, &ctx, inputBuffer, radixBuffer, pingPongBuffer, outputBuffer, n, 2);

	std::vector<unsigned> result(n);
	get_ssbo_data(outputBuffer, result.size(), result.data());

	std::sort(vec.begin(), vec.end());

	EXPECT_EQ(vec, result);
}