#include "gl_setup.hpp"
#include "scan_utilities.hpp"

#include <glsl_algo/prefix_scan.h>
#include <gtest/gtest.h>

class PrefixScanTest : public ::testing::Test
{
protected:
    virtual void SetUp()
    {
        init_window_and_gl_context();
    }
    virtual void TearDown()
    {
        destroy_window_and_gl_context();
    }
    glsl_algo_context mGlslAlgoContext;
};

TEST_F(PrefixScanTest, LocalReduceSmallInputSmallBlockSize)
{
    glsl_algo_configuration conf = {GARWTint1, 256, 32};
    glsl_algo_context ctx = glsl_algo_init(conf);
  
    const unsigned n = ctx.conf.local_block_size;
    std::vector<int> vec = generateIntegralRandomVector(n, 0, 6);
    GLuint inputBuffer = create_ssbo(n, vec.data());
    GLuint outputBuffer = create_ssbo(1);
    glsl_local_reduce(&ctx, inputBuffer, outputBuffer, n, ctx.conf.local_block_size);
    
    int expectedResult = 0;
    reduce(vec.data(), &expectedResult, n, ctx.conf.local_block_size);
    
    int result = 0;
    get_ssbo_data(outputBuffer, 1, &result);
    
    EXPECT_EQ(expectedResult, result);
}

TEST_F(PrefixScanTest, LocalReduceBigInputSmallBlockSize)
{
    glsl_algo_configuration conf = {GARWTint1, 256, 32};
    glsl_algo_context ctx = glsl_algo_init(conf);
  
    const unsigned numBlocks = 256;
    const unsigned n = ctx.conf.local_block_size * numBlocks;
    std::vector<int> vec = generateIntegralRandomVector(n, 0, 6);
    GLuint inputBuffer = create_ssbo(n, vec.data());
    GLuint outputBuffer = create_ssbo(numBlocks);
    glsl_local_reduce(&ctx, inputBuffer, outputBuffer, n, ctx.conf.local_block_size);
    
    std::vector<int> expectedResult(numBlocks, 0);
    reduce(vec.data(), expectedResult.data(), n, ctx.conf.local_block_size);
    
    std::vector<int> result(numBlocks, 0);
    get_ssbo_data(outputBuffer, numBlocks, result.data());
    
    EXPECT_EQ(expectedResult, result);
}

TEST_F(PrefixScanTest, LocalReduceBigInputBigBlockSize)
{
    glsl_algo_configuration conf = {GARWTint1, 1024, 32};
    glsl_algo_context ctx = glsl_algo_init(conf);
  
    const unsigned numBlocks = 32;
    const unsigned n = ctx.conf.local_block_size * numBlocks;
    std::vector<int> vec = generateIntegralRandomVector(n, 0, 6);
    GLuint inputBuffer = create_ssbo(n, vec.data());
    GLuint outputBuffer = create_ssbo(numBlocks);
    glsl_local_reduce(&ctx, inputBuffer, outputBuffer, n, ctx.conf.local_block_size);
    
    std::vector<int> expectedResult(numBlocks, 0);
    reduce(vec.data(), expectedResult.data(), n, ctx.conf.local_block_size);
    
    std::vector<int> result(numBlocks, 0);
    get_ssbo_data(outputBuffer, numBlocks, result.data());
    
    EXPECT_EQ(expectedResult, result);
}

TEST_F(PrefixScanTest, LocalReduceIvec2)
{
    glsl_algo_configuration conf = {GARWTint2, 256, 32};
    glsl_algo_context ctx = glsl_algo_init(conf);
  
    const unsigned numBlocks = 4;
    const unsigned elementsPerRead = 2;
    const unsigned elementsPerBlock = ctx.conf.local_block_size * elementsPerRead;
    const unsigned n = elementsPerBlock * numBlocks;
    std::vector<int> vec = generateIntegralRandomVector(n, 0, 6);
    GLuint inputBuffer = create_ssbo(n, vec.data());
    GLuint outputBuffer = create_ssbo(numBlocks);
    glsl_local_reduce(&ctx, inputBuffer, outputBuffer, n, elementsPerBlock);
    
    std::vector<int> expectedResult(numBlocks, 0);
    reduce(vec.data(), expectedResult.data(), n, elementsPerBlock);
    
    std::vector<int> result(numBlocks, 0);
    get_ssbo_data(outputBuffer, numBlocks, result.data());
    
    EXPECT_EQ(expectedResult, result);
}

TEST_F(PrefixScanTest, LocalReduceIvec4)
{
    glsl_algo_configuration conf = {GARWTint4, 1024, 32};
    glsl_algo_context ctx = glsl_algo_init(conf);
  
    const unsigned numBlocks = 4;
    const unsigned elementsPerRead = 4;
    const unsigned elementsPerBlock = ctx.conf.local_block_size * elementsPerRead;
    const unsigned n = elementsPerBlock * numBlocks;
    std::vector<int> vec = generateIntegralRandomVector(n, 0, 6);
    GLuint inputBuffer = create_ssbo(n, vec.data());
    GLuint outputBuffer = create_ssbo(numBlocks);
    glsl_local_reduce(&ctx, inputBuffer, outputBuffer, n, elementsPerBlock);
    
    std::vector<int> expectedResult(numBlocks, 0);
    reduce(vec.data(), expectedResult.data(), n, elementsPerBlock);
    
    std::vector<int> result(numBlocks, 0);
    get_ssbo_data(outputBuffer, numBlocks, result.data());
    
    EXPECT_EQ(expectedResult, result);
}

TEST_F(PrefixScanTest, LocalReduceVec4)
{
    glsl_algo_configuration conf = {GARWTfloat4, 1024, 32};
    glsl_algo_context ctx = glsl_algo_init(conf);
  
    const unsigned numBlocks = 4;
    const unsigned elementsPerRead = 4;
    const unsigned elementsPerBlock = ctx.conf.local_block_size * elementsPerRead;
    const unsigned n = elementsPerBlock * numBlocks;
    std::vector<float> vec = generateFloatRandomVector(n, 0.0f, 6.0f);
    GLuint inputBuffer = create_ssbo(n, vec.data());
    GLuint outputBuffer = create_ssbo(numBlocks);
    glsl_local_reduce(&ctx, inputBuffer, outputBuffer, n, elementsPerBlock);
    
    std::vector<float> expectedResult(numBlocks, 0);
    reduce(vec.data(), expectedResult.data(), n, elementsPerBlock);
    
    std::vector<float> result(numBlocks, 0);
    get_ssbo_data(outputBuffer, numBlocks, result.data());
    
    const float eps = 0.1f;
    for (size_t i = 0; i < expectedResult.size(); ++i)
    {
        EXPECT_NEAR(expectedResult[i], result[i], eps);
    }
}

TEST_F(PrefixScanTest, LocalReduceUvec4)
{
    glsl_algo_configuration conf = {GARWTuint4, 1024, 32};
    glsl_algo_context ctx = glsl_algo_init(conf);
  
    const unsigned numBlocks = 4;
    const unsigned elementsPerRead = 4;
    const unsigned elementsPerBlock = ctx.conf.local_block_size * elementsPerRead;
    const unsigned n = elementsPerBlock * numBlocks;
    std::vector<unsigned> vec = generateIntegralRandomVector(n, 0u, 6u);
    GLuint inputBuffer = create_ssbo(n, vec.data());
    GLuint outputBuffer = create_ssbo(numBlocks);
    glsl_local_reduce(&ctx, inputBuffer, outputBuffer, n, elementsPerBlock);
    
    std::vector<unsigned> expectedResult(numBlocks, 0);
    reduce(vec.data(), expectedResult.data(), n, elementsPerBlock);
    
    std::vector<unsigned> result(numBlocks, 0);
    get_ssbo_data(outputBuffer, numBlocks, result.data());
    
    EXPECT_EQ(expectedResult, result);
}

TEST_F(PrefixScanTest, LocalReduceFloat)
{
    glsl_algo_configuration conf = {GARWTfloat1, 1024, 32};
    glsl_algo_context ctx = glsl_algo_init(conf);
  
    const unsigned numBlocks = 64;
    const unsigned elementsPerRead = 1;
    const unsigned elementsPerBlock = ctx.conf.local_block_size * elementsPerRead;
    const unsigned n = elementsPerBlock * numBlocks;
    std::vector<float> vec = generateFloatRandomVector(n, 0.0f, 6.0f);
    GLuint inputBuffer = create_ssbo(n, vec.data());
    GLuint outputBuffer = create_ssbo(numBlocks);
    glsl_local_reduce(&ctx, inputBuffer, outputBuffer, n, elementsPerBlock);
    
    std::vector<float> expectedResult(numBlocks, 0);
    reduce(vec.data(), expectedResult.data(), n, elementsPerBlock);
    
    std::vector<float> result(numBlocks, 0);
    get_ssbo_data(outputBuffer, numBlocks, result.data());
    
    const float eps = 0.1f;
    for (size_t i = 0; i < expectedResult.size(); ++i)
    {
        EXPECT_NEAR(expectedResult[i], result[i], eps);
    }
}

TEST_F(PrefixScanTest, LocalReduceVerySmall)
{
    glsl_algo_configuration conf = {GARWTint1, 256, 32};
    glsl_algo_context ctx = glsl_algo_init(conf);
  
    const unsigned n = 16;
    std::vector<int> vec = generateIntegralRandomVector(n, 0, 6);
    GLuint inputBuffer = create_ssbo(n, vec.data());
    GLuint outputBuffer = create_ssbo(1);
    glsl_local_reduce(&ctx, inputBuffer, outputBuffer, n, ctx.conf.local_block_size);
    
    int expectedResult = 0;
    reduce(vec.data(), &expectedResult, n, ctx.conf.local_block_size);
    
    int result = 0;
    get_ssbo_data(outputBuffer, 1, &result);
    
    EXPECT_EQ(expectedResult, result);
}

TEST_F(PrefixScanTest, LocalReduceMultipleIvec4)
{
    glsl_algo_configuration conf = {GARWTint4, 1024, 32};
    glsl_algo_context ctx = glsl_algo_init(conf);
  
    const unsigned numBlocks = 4;
    const unsigned elementsPerRead = 4;
    const unsigned elementsPerThread = 16; 
    const unsigned elementsPerBlock = ctx.conf.local_block_size * elementsPerRead * elementsPerThread;
    const unsigned n = elementsPerBlock * numBlocks;
    std::vector<int> vec = generateIntegralRandomVector(n, 0, 3);
    GLuint inputBuffer = create_ssbo(n, vec.data());
    GLuint outputBuffer = create_ssbo(numBlocks);
    glsl_local_reduce(&ctx, inputBuffer, outputBuffer, n, elementsPerBlock);
    
    std::vector<int> expectedResult(numBlocks, 0);
    reduce(vec.data(), expectedResult.data(), n, elementsPerBlock);
    
    std::vector<int> result(numBlocks, 0);
    get_ssbo_data(outputBuffer, numBlocks, result.data());
    
    EXPECT_EQ(expectedResult, result);
}

TEST_F(PrefixScanTest, LocalScanSimpleTest)
{
    glsl_algo_configuration conf = {GARWTint1, 1024, 32};
    glsl_algo_context ctx = glsl_algo_init(conf);
    
    const unsigned n = 1024;
    std::vector<int> vec = generateIntegralRandomVector(n, 0, 3);
    GLuint inputBuffer = create_ssbo(n, vec.data());
    GLuint outputBuffer = create_ssbo(n);
    glsl_local_scan(&ctx, inputBuffer, outputBuffer, n, conf.local_block_size, 0);
    
    std::vector<int> expectedResult(n, 0);
    scan(vec.data(), expectedResult.data(), n, conf.local_block_size, 0);
    
    std::vector<int> result(n, 0);
    get_ssbo_data(outputBuffer, n, result.data());
    
    EXPECT_EQ(expectedResult, result);
}

TEST_F(PrefixScanTest, LocalScanFloat1)
{
    glsl_algo_configuration conf = {GARWTfloat1, 1024, 32};
    glsl_algo_context ctx = glsl_algo_init(conf);
    
    const unsigned n = 1024;
    std::vector<float> vec = generateFloatRandomVector(n, 0.0f, 3.0f);
    GLuint inputBuffer = create_ssbo(n, vec.data());
    GLuint outputBuffer = create_ssbo(n);
    glsl_local_scan(&ctx, inputBuffer, outputBuffer, n, conf.local_block_size, 0);
    
    std::vector<float> expectedResult(n, 0);
    scan(vec.data(), expectedResult.data(), n, conf.local_block_size, 0);
    
    std::vector<float> result(n, 0);
    get_ssbo_data(outputBuffer, n, result.data());
    
    const float eps = 0.1f;
    for (size_t i = 0; i < expectedResult.size(); ++i)
    {
        EXPECT_NEAR(expectedResult[i], result[i], eps);
    }
}

TEST_F(PrefixScanTest, LocalScanUvec4)
{
    glsl_algo_configuration conf = {GARWTuint4, 1024, 32};
    glsl_algo_context ctx = glsl_algo_init(conf);
    
    const unsigned typeSize = 4;
    const unsigned n = 1024 * typeSize;
    std::vector<unsigned> vec = generateIntegralRandomVector(n, 0u, 6u);
    GLuint inputBuffer = create_ssbo(n, vec.data());
    GLuint outputBuffer = create_ssbo(n);
    glsl_local_scan(&ctx, inputBuffer, outputBuffer, n, n, 0);
    
    std::vector<unsigned> expectedResult(n, 0);
    scan(vec.data(), expectedResult.data(), n, n, 0);
    
    std::vector<unsigned> result(n, 0);
    get_ssbo_data(outputBuffer, n, result.data());
    
    EXPECT_EQ(expectedResult, result);
}

TEST_F(PrefixScanTest, LocalScanFloat4)
{
    glsl_algo_configuration conf = {GARWTfloat4, 1024, 32};
    glsl_algo_context ctx = glsl_algo_init(conf);
    
    const unsigned typeSize = 4;
    const unsigned n = 1024 * typeSize;
    std::vector<float> vec = generateFloatRandomVector(n, 0.0f, 3.0f);
    GLuint inputBuffer = create_ssbo(n, vec.data());
    GLuint outputBuffer = create_ssbo(n);
    glsl_local_scan(&ctx, inputBuffer, outputBuffer, n, n, 0);
    
    std::vector<float> expectedResult(n, 0);
    scan(vec.data(), expectedResult.data(), n, n, 0);
    
    std::vector<float> result(n, 0);
    get_ssbo_data(outputBuffer, n, result.data());
    
    const float eps = 0.1f;
    for (size_t i = 0; i < expectedResult.size(); ++i)
    {
        EXPECT_NEAR(expectedResult[i], result[i], eps);
    }
}

TEST_F(PrefixScanTest, LocalScanMultipleElementsPerThread)
{
    glsl_algo_configuration conf = {GARWTuint4, 1024, 32};
    glsl_algo_context ctx = glsl_algo_init(conf);
    
    const unsigned elementsPerThread = 16;
    const unsigned typeSize = 4;
    const unsigned n = 1024 * typeSize * elementsPerThread;
    std::vector<unsigned> vec = generateIntegralRandomVector(n, 0u, 6u);
    GLuint inputBuffer = create_ssbo(n, vec.data());
    GLuint outputBuffer = create_ssbo(n);
    glsl_local_scan(&ctx, inputBuffer, outputBuffer, n, n, 0);
    
    std::vector<unsigned> expectedResult(n, 0);
    scan(vec.data(), expectedResult.data(), n, n, 0);
    
    std::vector<unsigned> result(n, 0);
    get_ssbo_data(outputBuffer, n, result.data());
    
    EXPECT_EQ(expectedResult, result);
}

TEST_F(PrefixScanTest, LocalScanMultipleBlocks)
{
    glsl_algo_configuration conf = {GARWTuint4, 1024, 32};
    glsl_algo_context ctx = glsl_algo_init(conf);
    
    const unsigned numBlocks = 16;
    const unsigned typeSize = 4;
    const unsigned blockSize = typeSize * conf.local_block_size;
    const unsigned n = 1024 * typeSize * numBlocks;
    std::vector<unsigned> vec = generateIntegralRandomVector(n, 0u, 6u);
    GLuint inputBuffer = create_ssbo(n, vec.data());
    GLuint outputBuffer = create_ssbo(n);
    glsl_local_scan(&ctx, inputBuffer, outputBuffer, n, blockSize, 0);
    
    std::vector<unsigned> expectedResult(n, 0);
    scan(vec.data(), expectedResult.data(), n, blockSize, 0);
    
    std::vector<unsigned> result(n, 0);
    get_ssbo_data(outputBuffer, n, result.data());
    
    EXPECT_EQ(expectedResult, result);
}

TEST_F(PrefixScanTest, LocalScanSmallArray)
{
    glsl_algo_configuration conf = {GARWTuint4, 1024, 32};
    glsl_algo_context ctx = glsl_algo_init(conf);
    
    const unsigned n = 256;
    std::vector<unsigned> vec = generateIntegralRandomVector(n, 0u, 6u);
    GLuint inputBuffer = create_ssbo(n, vec.data());
    GLuint outputBuffer = create_ssbo(n);
    glsl_local_scan(&ctx, inputBuffer, outputBuffer, n, 4096, 0);
    
    std::vector<unsigned> expectedResult(n, 0);
    scan(vec.data(), expectedResult.data(), n, n, 0);
    
    std::vector<unsigned> result(n, 0);
    get_ssbo_data(outputBuffer, n, result.data());
    
    EXPECT_EQ(expectedResult, result);
}

TEST_F(PrefixScanTest, LocalScanInclusive)
{
    glsl_algo_configuration conf = {GARWTuint1, 1024, 32};
    glsl_algo_context ctx = glsl_algo_init(conf);
    
    const unsigned numBlocks = 16;
    const unsigned typeSize = 1;
    const unsigned blockSize = typeSize * conf.local_block_size;
    const unsigned n = 1024 * typeSize * numBlocks;
    std::vector<unsigned> vec = generateIntegralRandomVector(n, 0u, 6u);
    GLuint inputBuffer = create_ssbo(n, vec.data());
    GLuint outputBuffer = create_ssbo(n);
    glsl_local_scan(&ctx, inputBuffer, outputBuffer, n, blockSize, 1);
    
    std::vector<unsigned> expectedResult(n, 0);
    scan(vec.data(), expectedResult.data(), n, blockSize, 1);
    
    std::vector<unsigned> result(n, 0);
    get_ssbo_data(outputBuffer, n, result.data());
    
    EXPECT_EQ(expectedResult, result);
}

TEST_F(PrefixScanTest, LocalScanInt4Inclusive)
{
    glsl_algo_configuration conf = {GARWTint4, 1024, 32};
    glsl_algo_context ctx = glsl_algo_init(conf);
    
    const unsigned elementsPerThread = 3;
    const unsigned numBlocks = 16;
    const unsigned typeSize = 4;
    const unsigned blockSize = typeSize * conf.local_block_size * elementsPerThread;
    const unsigned n = 1024 * typeSize * numBlocks * elementsPerThread;
    std::vector<unsigned> vec = generateIntegralRandomVector(n, 0u, 6u);
    GLuint inputBuffer = create_ssbo(n, vec.data());
    GLuint outputBuffer = create_ssbo(n);
    glsl_local_scan(&ctx, inputBuffer, outputBuffer, n, blockSize, 1);
    
    std::vector<unsigned> expectedResult(n, 0);
    scan(vec.data(), expectedResult.data(), n, blockSize, 1);
    
    std::vector<unsigned> result(n, 0);
    get_ssbo_data(outputBuffer, n, result.data());
    
    EXPECT_EQ(expectedResult, result);
}

TEST_F(PrefixScanTest, LocalScanSameInputAndOutputBuffers)
{
    glsl_algo_configuration conf = {GARWTuint4, 1024, 32};
    glsl_algo_context ctx = glsl_algo_init(conf);
    
    const unsigned numBlocks = 16;
    const unsigned typeSize = 4;
    const unsigned blockSize = typeSize * conf.local_block_size;
    const unsigned n = 1024 * typeSize * numBlocks;
    std::vector<unsigned> vec = generateIntegralRandomVector(n, 0u, 6u);
    GLuint inputOutputBuffer = create_ssbo(n, vec.data());
    glsl_local_scan(&ctx, inputOutputBuffer, inputOutputBuffer, n, blockSize, 0);
    
    std::vector<unsigned> expectedResult(n, 0);
    scan(vec.data(), expectedResult.data(), n, blockSize, 0);
    
    std::vector<unsigned> result(n, 0);
    get_ssbo_data(inputOutputBuffer, n, result.data());
    
    EXPECT_EQ(expectedResult, result);
}

TEST_F(PrefixScanTest, FullScan)
{
    glsl_algo_configuration conf = {GARWTint1, 1024, 32};
    glsl_algo_context ctx = glsl_algo_init(conf);

    const unsigned numBlocks = 256;
    const unsigned n = 1024 * numBlocks;
    std::vector<unsigned> vec = generateIntegralRandomVector(n, 0u, 6u);
    GLuint inputBuffer = create_ssbo(n, vec.data());
    GLuint intermediateBuffer = create_ssbo(numBlocks);
    GLuint outputBuffer = create_ssbo(n);
    glsl_scan(&ctx, inputBuffer, intermediateBuffer, outputBuffer, n, 1, 0);
    
    std::vector<unsigned> expectedResult(n, 0);
    scan(vec.data(), expectedResult.data(), n, n, 0);
    
    std::vector<unsigned> result(n, 0);
    get_ssbo_data(outputBuffer, n, result.data());
    
    EXPECT_EQ(expectedResult, result);
}

TEST_F(PrefixScanTest, FullScanLarge)
{
    glsl_algo_configuration conf = {GARWTint1, 1024, 32};
    glsl_algo_context ctx = glsl_algo_init(conf);

    const unsigned numBlocks = 2048;
    const unsigned n = 1024 * numBlocks;
    std::vector<unsigned> vec = generateIntegralRandomVector(n, 0u, 3u);
    GLuint inputBuffer = create_ssbo(n, vec.data());
    GLuint intermediateBuffer = create_ssbo(numBlocks);
    GLuint outputBuffer = create_ssbo(n);
    glsl_scan(&ctx, inputBuffer, intermediateBuffer, outputBuffer, n, 1, 0);
    
    std::vector<unsigned> expectedResult(n, 0);
    scan(vec.data(), expectedResult.data(), n, n, 0);
    
    std::vector<unsigned> result(n, 0);
    get_ssbo_data(outputBuffer, n, result.data());
    
    EXPECT_EQ(expectedResult, result);
}

TEST_F(PrefixScanTest, FullScanLargeUint4)
{
    glsl_algo_configuration conf = {GARWTuint4, 1024, 32};
    glsl_algo_context ctx = glsl_algo_init(conf);

    const unsigned numBlocks = 2048;
    const unsigned n = 1024 * numBlocks;
    std::vector<unsigned> vec = generateIntegralRandomVector(n, 0u, 3u);
    GLuint inputBuffer = create_ssbo(n, vec.data());
    GLuint intermediateBuffer = create_ssbo(numBlocks);
    GLuint outputBuffer = create_ssbo(n);
    glsl_scan(&ctx, inputBuffer, intermediateBuffer, outputBuffer, n, 1, 0);
    
    std::vector<unsigned> expectedResult(n, 0);
    scan(vec.data(), expectedResult.data(), n, n, 0);
    
    std::vector<unsigned> result(n, 0);
    get_ssbo_data(outputBuffer, n, result.data());
    
    EXPECT_EQ(expectedResult, result);
}

TEST_F(PrefixScanTest, FullInclusiveScan)
{
    glsl_algo_configuration conf = {GARWTint1, 1024, 32};
    glsl_algo_context ctx = glsl_algo_init(conf);

    const unsigned numBlocks = 256;
    const unsigned n = 1024 * numBlocks;
    std::vector<unsigned> vec = generateIntegralRandomVector(n, 0u, 3u);
    GLuint inputBuffer = create_ssbo(n, vec.data());
    GLuint intermediateBuffer = create_ssbo(numBlocks);
    GLuint outputBuffer = create_ssbo(n);
    glsl_scan(&ctx, inputBuffer, intermediateBuffer, outputBuffer, n, 1, 1);
    
    std::vector<unsigned> expectedResult(n, 0);
    scan(vec.data(), expectedResult.data(), n, n, 1);
    
    std::vector<unsigned> result(n, 0);
    get_ssbo_data(outputBuffer, n, result.data());
    
    EXPECT_EQ(expectedResult, result);
}

TEST_F(PrefixScanTest, FullInclusiveScanSameBuffer)
{
    glsl_algo_configuration conf = {GARWTint1, 1024, 32};
    glsl_algo_context ctx = glsl_algo_init(conf);

    const unsigned numBlocks = 256;
    const unsigned n = 1024 * numBlocks;
    std::vector<unsigned> vec = generateIntegralRandomVector(n, 0u, 3u);
    GLuint inputOutputBuffer = create_ssbo(n, vec.data());
    GLuint intermediateBuffer = create_ssbo(numBlocks);
    glsl_scan(&ctx, inputOutputBuffer, intermediateBuffer, inputOutputBuffer, n, 1, 1);
    
    std::vector<unsigned> expectedResult(n, 0);
    scan(vec.data(), expectedResult.data(), n, n, 1);
    
    std::vector<unsigned> result(n, 0);
    get_ssbo_data(inputOutputBuffer, n, result.data());
    
    EXPECT_EQ(expectedResult, result);
}