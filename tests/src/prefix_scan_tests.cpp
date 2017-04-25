#include <common/gl_setup.hpp>
#include <common/scan_utilities.hpp>
#include <common/macros.hpp>

#include <glsl_algo/prefix_scan.h>
#include <gtest/gtest.h>

class PrefixScanTest : public ::testing::Test
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

struct ScanParameterVariation
{
    GLSL_ALGO_READ_WRITE_TYPE type;
    unsigned int blockSize;
    unsigned int elementsPerThread;
    unsigned char isInclusive;
};

std::ostream &operator<<(std::ostream &os, const ScanParameterVariation &variation)
{
    os << "( " << glsl_algo_get_rw_type_name(variation.type) 
       << " " << variation.blockSize 
       << " " << variation.elementsPerThread 
       << " " << (variation.isInclusive ? "inclusive" : "exclusive")
       << " )";
    return os;
}

class PrefixScanTestWithMultipleParameters : public ::testing::TestWithParam<ScanParameterVariation>
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

template<typename T>
static GLSL_ALGO_READ_WRITE_TYPE ConvertType(GLSL_ALGO_READ_WRITE_TYPE inputType)
{
    UNIMPLEMENTED("ConvertType<T>");
    return GARWTundefined;
}

template<>
GLSL_ALGO_READ_WRITE_TYPE ConvertType<int>(GLSL_ALGO_READ_WRITE_TYPE inputType)
{
    switch (inputType)
    {
        case GARWTint1:
        case GARWTuint1:
        case GARWTfloat1:
            return GARWTint1;
        case GARWTint2:
        case GARWTuint2:
        case GARWTfloat2:
            return GARWTint2;
        case GARWTint4:
        case GARWTuint4:
        case GARWTfloat4:
            return GARWTint4;
        default:
            UNIMPLEMENTED("");
    }
    return GARWTundefined;
}

template<>
GLSL_ALGO_READ_WRITE_TYPE ConvertType<unsigned>(GLSL_ALGO_READ_WRITE_TYPE inputType)
{
    switch (inputType)
    {
        case GARWTint1:
        case GARWTuint1:
        case GARWTfloat1:
            return GARWTuint1;
        case GARWTint2:
        case GARWTuint2:
        case GARWTfloat2:
            return GARWTuint2;
        case GARWTint4:
        case GARWTuint4:
        case GARWTfloat4:
            return GARWTuint4;
        default:
            UNIMPLEMENTED("");
    }
    return GARWTundefined;
}

template<>
GLSL_ALGO_READ_WRITE_TYPE ConvertType<float>(GLSL_ALGO_READ_WRITE_TYPE inputType)
{
    switch (inputType)
    {
        case GARWTint1:
        case GARWTuint1:
        case GARWTfloat1:
            return GARWTfloat1;
        case GARWTint2:
        case GARWTuint2:
        case GARWTfloat2:
            return GARWTfloat2;
        case GARWTint4:
        case GARWTuint4:
        case GARWTfloat4:
            return GARWTfloat4;
        default:
            UNIMPLEMENTED("");
    }
    return GARWTundefined;
}

TEST_F(PrefixScanTest, LocalReduceSmallInputSmallBlockSize)
{
    glsl_algo_configuration conf = {GARWTint1, 256, 32, 1, 1};
    glsl_algo_context ctx = glsl_algo_init(&mGLContext, conf);
  
    const unsigned n = ctx.conf.local_block_size;
    std::vector<int> vec = generateIntegralRandomVector(n, 0, 6);
    GLuint inputBuffer = create_ssbo(n, vec.data());
    GLuint outputBuffer = create_ssbo(1);
    glsl_local_reduce(&mGLContext, &ctx, inputBuffer, outputBuffer, n, ctx.conf.local_block_size);
    
    int expectedResult = 0;
    reduce(vec.data(), &expectedResult, n, ctx.conf.local_block_size);
    
    int result = 0;
    get_ssbo_data(outputBuffer, 1, &result);
    
    EXPECT_EQ(expectedResult, result);
}

TEST_F(PrefixScanTest, LocalReduceBigInputSmallBlockSize)
{
    glsl_algo_configuration conf = {GARWTint1, 256, 32, 1, 1};
    glsl_algo_context ctx = glsl_algo_init(&mGLContext, conf);
  
    const unsigned numBlocks = 256;
    const unsigned n = ctx.conf.local_block_size * numBlocks;
    std::vector<int> vec = generateIntegralRandomVector(n, 0, 6);
    GLuint inputBuffer = create_ssbo(n, vec.data());
    GLuint outputBuffer = create_ssbo(numBlocks);
    glsl_local_reduce(&mGLContext, &ctx, inputBuffer, outputBuffer, n, ctx.conf.local_block_size);
    
    std::vector<int> expectedResult(numBlocks, 0);
    reduce(vec.data(), expectedResult.data(), n, ctx.conf.local_block_size);
    
    std::vector<int> result(numBlocks, 0);
    get_ssbo_data(outputBuffer, numBlocks, result.data());
    
    EXPECT_EQ(expectedResult, result);
}

TEST_F(PrefixScanTest, LocalReduceBigInputBigBlockSize)
{
    glsl_algo_configuration conf = {GARWTint1, 1024, 32, 1, 1};
    glsl_algo_context ctx = glsl_algo_init(&mGLContext, conf);
  
    const unsigned numBlocks = 32;
    const unsigned n = ctx.conf.local_block_size * numBlocks;
    std::vector<int> vec = generateIntegralRandomVector(n, 0, 6);
    GLuint inputBuffer = create_ssbo(n, vec.data());
    GLuint outputBuffer = create_ssbo(numBlocks);
    glsl_local_reduce(&mGLContext, &ctx, inputBuffer, outputBuffer, n, ctx.conf.local_block_size);
    
    std::vector<int> expectedResult(numBlocks, 0);
    reduce(vec.data(), expectedResult.data(), n, ctx.conf.local_block_size);
    
    std::vector<int> result(numBlocks, 0);
    get_ssbo_data(outputBuffer, numBlocks, result.data());
    
    EXPECT_EQ(expectedResult, result);
}

TEST_F(PrefixScanTest, LocalReduceIvec2)
{
    glsl_algo_configuration conf = {GARWTint2, 256, 32, 1, 1};
    glsl_algo_context ctx = glsl_algo_init(&mGLContext, conf);
  
    const unsigned numBlocks = 4;
    const unsigned elementsPerRead = 2;
    const unsigned elementsPerBlock = ctx.conf.local_block_size * elementsPerRead;
    const unsigned n = elementsPerBlock * numBlocks;
    std::vector<int> vec = generateIntegralRandomVector(n, 0, 6);
    GLuint inputBuffer = create_ssbo(n, vec.data());
    GLuint outputBuffer = create_ssbo(numBlocks);
    glsl_local_reduce(&mGLContext, &ctx, inputBuffer, outputBuffer, n, elementsPerBlock);
    
    std::vector<int> expectedResult(numBlocks, 0);
    reduce(vec.data(), expectedResult.data(), n, elementsPerBlock);
    
    std::vector<int> result(numBlocks, 0);
    get_ssbo_data(outputBuffer, numBlocks, result.data());
    
    EXPECT_EQ(expectedResult, result);
}

TEST_F(PrefixScanTest, LocalReduceIvec4)
{
    glsl_algo_configuration conf = {GARWTint4, 1024, 32, 1, 1};
    glsl_algo_context ctx = glsl_algo_init(&mGLContext, conf);
  
    const unsigned numBlocks = 4;
    const unsigned elementsPerRead = 4;
    const unsigned elementsPerBlock = ctx.conf.local_block_size * elementsPerRead;
    const unsigned n = elementsPerBlock * numBlocks;
    std::vector<int> vec = generateIntegralRandomVector(n, 0, 6);
    GLuint inputBuffer = create_ssbo(n, vec.data());
    GLuint outputBuffer = create_ssbo(numBlocks);
    glsl_local_reduce(&mGLContext, &ctx, inputBuffer, outputBuffer, n, elementsPerBlock);
    
    std::vector<int> expectedResult(numBlocks, 0);
    reduce(vec.data(), expectedResult.data(), n, elementsPerBlock);
    
    std::vector<int> result(numBlocks, 0);
    get_ssbo_data(outputBuffer, numBlocks, result.data());
    
    EXPECT_EQ(expectedResult, result);
}

TEST_F(PrefixScanTest, LocalReduceUvec4)
{
    glsl_algo_configuration conf = {GARWTuint4, 1024, 32, 1, 1};
    glsl_algo_context ctx = glsl_algo_init(&mGLContext, conf);
  
    const unsigned numBlocks = 4;
    const unsigned elementsPerRead = 4;
    const unsigned elementsPerBlock = ctx.conf.local_block_size * elementsPerRead;
    const unsigned n = elementsPerBlock * numBlocks;
    std::vector<unsigned> vec = generateIntegralRandomVector(n, 0u, 6u);
    GLuint inputBuffer = create_ssbo(n, vec.data());
    GLuint outputBuffer = create_ssbo(numBlocks);
    glsl_local_reduce(&mGLContext, &ctx, inputBuffer, outputBuffer, n, elementsPerBlock);
    
    std::vector<unsigned> expectedResult(numBlocks, 0);
    reduce(vec.data(), expectedResult.data(), n, elementsPerBlock);
    
    std::vector<unsigned> result(numBlocks, 0);
    get_ssbo_data(outputBuffer, numBlocks, result.data());
    
    EXPECT_EQ(expectedResult, result);
}

TEST_F(PrefixScanTest, LocalReduceVerySmall)
{
    glsl_algo_configuration conf = {GARWTint1, 256, 32, 1, 1};
    glsl_algo_context ctx = glsl_algo_init(&mGLContext, conf);
  
    const unsigned n = 16;
    std::vector<int> vec = generateIntegralRandomVector(n, 0, 6);
    GLuint inputBuffer = create_ssbo(n, vec.data());
    GLuint outputBuffer = create_ssbo(1);
    glsl_local_reduce(&mGLContext, &ctx, inputBuffer, outputBuffer, n, ctx.conf.local_block_size);
    
    int expectedResult = 0;
    reduce(vec.data(), &expectedResult, n, ctx.conf.local_block_size);
    
    int result = 0;
    get_ssbo_data(outputBuffer, 1, &result);
    
    EXPECT_EQ(expectedResult, result);
}

TEST_F(PrefixScanTest, LocalReduceMultipleIvec4)
{
    glsl_algo_configuration conf = {GARWTint4, 1024, 32, 1, 1};
    glsl_algo_context ctx = glsl_algo_init(&mGLContext, conf);
  
    const unsigned numBlocks = 4;
    const unsigned elementsPerRead = 4;
    const unsigned elementsPerThread = 16; 
    const unsigned elementsPerBlock = ctx.conf.local_block_size * elementsPerRead * elementsPerThread;
    const unsigned n = elementsPerBlock * numBlocks;
    std::vector<int> vec = generateIntegralRandomVector(n, 0, 3);
    GLuint inputBuffer = create_ssbo(n, vec.data());
    GLuint outputBuffer = create_ssbo(numBlocks);
    glsl_local_reduce(&mGLContext, &ctx, inputBuffer, outputBuffer, n, elementsPerBlock);
    
    std::vector<int> expectedResult(numBlocks, 0);
    reduce(vec.data(), expectedResult.data(), n, elementsPerBlock);
    
    std::vector<int> result(numBlocks, 0);
    get_ssbo_data(outputBuffer, numBlocks, result.data());
    
    EXPECT_EQ(expectedResult, result);
}

TEST_F(PrefixScanTest, LocalScanSimpleTest)
{
    glsl_algo_configuration conf = {GARWTint1, 1024, 32, 1, 1};
    glsl_algo_context ctx = glsl_algo_init(&mGLContext, conf);
    
    const unsigned n = 1024;
    std::vector<int> vec = generateIntegralRandomVector(n, 0, 3);
    GLuint inputBuffer = create_ssbo(n, vec.data());
    GLuint outputBuffer = create_ssbo(n);
    glsl_local_scan(&mGLContext, &ctx, inputBuffer, outputBuffer, n, conf.local_block_size, 0);
    
    std::vector<int> expectedResult(n, 0);
    scan(vec.data(), expectedResult.data(), n, conf.local_block_size, 0);
    
    std::vector<int> result(n, 0);
    get_ssbo_data(outputBuffer, n, result.data());
    
    EXPECT_EQ(expectedResult, result);
}

TEST_F(PrefixScanTest, LocalScanUvec4)
{
    glsl_algo_configuration conf = {GARWTuint4, 1024, 32, 1, 1};
    glsl_algo_context ctx = glsl_algo_init(&mGLContext, conf);
    
    const unsigned typeSize = 4;
    const unsigned n = 1024 * typeSize;
    std::vector<unsigned> vec = generateIntegralRandomVector(n, 0u, 6u);
    GLuint inputBuffer = create_ssbo(n, vec.data());
    GLuint outputBuffer = create_ssbo(n);
    glsl_local_scan(&mGLContext, &ctx, inputBuffer, outputBuffer, n, n, 0);
    
    std::vector<unsigned> expectedResult(n, 0);
    scan(vec.data(), expectedResult.data(), n, n, 0);
    
    std::vector<unsigned> result(n, 0);
    get_ssbo_data(outputBuffer, n, result.data());
    
    EXPECT_EQ(expectedResult, result);
}

TEST_F(PrefixScanTest, LocalScanMultipleElementsPerThread)
{
    glsl_algo_configuration conf = {GARWTuint4, 1024, 32, 1, 1};
    glsl_algo_context ctx = glsl_algo_init(&mGLContext, conf);
    
    const unsigned elementsPerThread = 16;
    const unsigned typeSize = 4;
    const unsigned n = 1024 * typeSize * elementsPerThread;
    std::vector<unsigned> vec = generateIntegralRandomVector(n, 0u, 6u);
    GLuint inputBuffer = create_ssbo(n, vec.data());
    GLuint outputBuffer = create_ssbo(n);
    glsl_local_scan(&mGLContext, &ctx, inputBuffer, outputBuffer, n, n, 0);
    
    std::vector<unsigned> expectedResult(n, 0);
    scan(vec.data(), expectedResult.data(), n, n, 0);
    
    std::vector<unsigned> result(n, 0);
    get_ssbo_data(outputBuffer, n, result.data());
    
    EXPECT_EQ(expectedResult, result);
}

TEST_F(PrefixScanTest, LocalScanMultipleBlocks)
{
    glsl_algo_configuration conf = {GARWTuint4, 1024, 32, 1, 1};
    glsl_algo_context ctx = glsl_algo_init(&mGLContext, conf);
    
    const unsigned numBlocks = 16;
    const unsigned typeSize = 4;
    const unsigned blockSize = typeSize * conf.local_block_size;
    const unsigned n = 1024 * typeSize * numBlocks;
    std::vector<unsigned> vec = generateIntegralRandomVector(n, 0u, 6u);
    GLuint inputBuffer = create_ssbo(n, vec.data());
    GLuint outputBuffer = create_ssbo(n);
    glsl_local_scan(&mGLContext, &ctx, inputBuffer, outputBuffer, n, blockSize, 0);
    
    std::vector<unsigned> expectedResult(n, 0);
    scan(vec.data(), expectedResult.data(), n, blockSize, 0);
    
    std::vector<unsigned> result(n, 0);
    get_ssbo_data(outputBuffer, n, result.data());
    
    EXPECT_EQ(expectedResult, result);
}

TEST_F(PrefixScanTest, LocalScanSmallArray)
{
    glsl_algo_configuration conf = {GARWTuint4, 1024, 32, 1, 1};
    glsl_algo_context ctx = glsl_algo_init(&mGLContext, conf);
    
    const unsigned n = 256;
    std::vector<unsigned> vec = generateIntegralRandomVector(n, 0u, 6u);
    GLuint inputBuffer = create_ssbo(n, vec.data());
    GLuint outputBuffer = create_ssbo(n);
    glsl_local_scan(&mGLContext, &ctx, inputBuffer, outputBuffer, n, 4096, 0);
    
    std::vector<unsigned> expectedResult(n, 0);
    scan(vec.data(), expectedResult.data(), n, n, 0);
    
    std::vector<unsigned> result(n, 0);
    get_ssbo_data(outputBuffer, n, result.data());
    
    EXPECT_EQ(expectedResult, result);
}

TEST_F(PrefixScanTest, LocalScanInclusive)
{
    glsl_algo_configuration conf = {GARWTuint1, 1024, 32, 1, 1};
    glsl_algo_context ctx = glsl_algo_init(&mGLContext, conf);
    
    const unsigned numBlocks = 16;
    const unsigned typeSize = 1;
    const unsigned blockSize = typeSize * conf.local_block_size;
    const unsigned n = 1024 * typeSize * numBlocks;
    std::vector<unsigned> vec = generateIntegralRandomVector(n, 0u, 6u);
    GLuint inputBuffer = create_ssbo(n, vec.data());
    GLuint outputBuffer = create_ssbo(n);
    glsl_local_scan(&mGLContext, &ctx, inputBuffer, outputBuffer, n, blockSize, 1);
    
    std::vector<unsigned> expectedResult(n, 0);
    scan(vec.data(), expectedResult.data(), n, blockSize, 1);
    
    std::vector<unsigned> result(n, 0);
    get_ssbo_data(outputBuffer, n, result.data());
    
    EXPECT_EQ(expectedResult, result);
}

TEST_F(PrefixScanTest, LocalScanInt4Inclusive)
{
    glsl_algo_configuration conf = {GARWTint4, 1024, 32, 1, 1};
    glsl_algo_context ctx = glsl_algo_init(&mGLContext, conf);
    
    const unsigned elementsPerThread = 3;
    const unsigned numBlocks = 16;
    const unsigned typeSize = 4;
    const unsigned blockSize = typeSize * conf.local_block_size * elementsPerThread;
    const unsigned n = 1024 * typeSize * numBlocks * elementsPerThread;
    std::vector<unsigned> vec = generateIntegralRandomVector(n, 0u, 6u);
    GLuint inputBuffer = create_ssbo(n, vec.data());
    GLuint outputBuffer = create_ssbo(n);
    glsl_local_scan(&mGLContext, &ctx, inputBuffer, outputBuffer, n, blockSize, 1);
    
    std::vector<unsigned> expectedResult(n, 0);
    scan(vec.data(), expectedResult.data(), n, blockSize, 1);
    
    std::vector<unsigned> result(n, 0);
    get_ssbo_data(outputBuffer, n, result.data());
    
    EXPECT_EQ(expectedResult, result);
}

TEST_F(PrefixScanTest, LocalScanSameInputAndOutputBuffers)
{
    glsl_algo_configuration conf = {GARWTuint4, 1024, 32, 1, 1};
    glsl_algo_context ctx = glsl_algo_init(&mGLContext, conf);
    
    const unsigned numBlocks = 16;
    const unsigned typeSize = 4;
    const unsigned blockSize = typeSize * conf.local_block_size;
    const unsigned n = 1024 * typeSize * numBlocks;
    std::vector<unsigned> vec = generateIntegralRandomVector(n, 0u, 6u);
    GLuint inputOutputBuffer = create_ssbo(n, vec.data());
    glsl_local_scan(&mGLContext, &ctx, inputOutputBuffer, inputOutputBuffer, n, blockSize, 0);
    
    std::vector<unsigned> expectedResult(n, 0);
    scan(vec.data(), expectedResult.data(), n, blockSize, 0);
    
    std::vector<unsigned> result(n, 0);
    get_ssbo_data(inputOutputBuffer, n, result.data());
    
    EXPECT_EQ(expectedResult, result);
}

TEST_F(PrefixScanTest, FullScan)
{
    glsl_algo_configuration conf = {GARWTint1, 1024, 32, 1, 1};
    glsl_algo_context ctx = glsl_algo_init(&mGLContext, conf);

    const unsigned numBlocks = 256;
    const unsigned n = 1024 * numBlocks;
    std::vector<unsigned> vec = generateIntegralRandomVector(n, 0u, 6u);
    GLuint inputBuffer = create_ssbo(n, vec.data());
    GLuint intermediateBuffer = create_ssbo(numBlocks);
    GLuint outputBuffer = create_ssbo(n);
    glsl_scan(&mGLContext, &ctx, inputBuffer, intermediateBuffer, outputBuffer, n, 1, 0);
    
    std::vector<unsigned> expectedResult(n, 0);
    scan(vec.data(), expectedResult.data(), n, n, 0);
    
    std::vector<unsigned> result(n, 0);
    get_ssbo_data(outputBuffer, n, result.data());
    
    EXPECT_EQ(expectedResult, result);
}

TEST_F(PrefixScanTest, FullScanLarge)
{
    glsl_algo_configuration conf = {GARWTint1, 1024, 32, 1, 1};
    glsl_algo_context ctx = glsl_algo_init(&mGLContext, conf);

    const unsigned numBlocks = 2048;
    const unsigned n = 1024 * numBlocks;
    std::vector<unsigned> vec = generateIntegralRandomVector(n, 0u, 3u);
    GLuint inputBuffer = create_ssbo(n, vec.data());
    GLuint intermediateBuffer = create_ssbo(numBlocks);
    GLuint outputBuffer = create_ssbo(n);
    glsl_scan(&mGLContext, &ctx, inputBuffer, intermediateBuffer, outputBuffer, n, 1, 0);
    
    std::vector<unsigned> expectedResult(n, 0);
    scan(vec.data(), expectedResult.data(), n, n, 0);
    
    std::vector<unsigned> result(n, 0);
    get_ssbo_data(outputBuffer, n, result.data());
    
    EXPECT_EQ(expectedResult, result);
}

TEST_F(PrefixScanTest, FullScanLargeUint4)
{
    glsl_algo_configuration conf = {GARWTuint4, 1024, 32, 1, 1};
    glsl_algo_context ctx = glsl_algo_init(&mGLContext, conf);
    
    const unsigned numBlocks = 2048;
    const unsigned n = 1024 * numBlocks;
    std::vector<unsigned> vec = generateIntegralRandomVector(n, 0u, 3u);
    GLuint inputBuffer = create_ssbo(n, vec.data());
    GLuint intermediateBuffer = create_ssbo(numBlocks);
    GLuint outputBuffer = create_ssbo(n);
    glsl_scan(&mGLContext, &ctx, inputBuffer, intermediateBuffer, outputBuffer, n, 1, 0);
    
    std::vector<unsigned> expectedResult(n, 0);
    scan(vec.data(), expectedResult.data(), n, n, 0);
    
    std::vector<unsigned> result(n, 0);
    get_ssbo_data(outputBuffer, n, result.data());
    
    EXPECT_EQ(expectedResult, result);
}

TEST_F(PrefixScanTest, FullInclusiveScan)
{
    glsl_algo_configuration conf = {GARWTint1, 1024, 32, 1, 1};
    glsl_algo_context ctx = glsl_algo_init(&mGLContext, conf);

    const unsigned numBlocks = 256;
    const unsigned n = 1024 * numBlocks;
    std::vector<unsigned> vec = generateIntegralRandomVector(n, 0u, 3u);
    GLuint inputBuffer = create_ssbo(n, vec.data());
    GLuint intermediateBuffer = create_ssbo(numBlocks);
    GLuint outputBuffer = create_ssbo(n);
    glsl_scan(&mGLContext, &ctx, inputBuffer, intermediateBuffer, outputBuffer, n, 1, 1);
    
    std::vector<unsigned> expectedResult(n, 0);
    scan(vec.data(), expectedResult.data(), n, n, 1);
    
    std::vector<unsigned> result(n, 0);
    get_ssbo_data(outputBuffer, n, result.data());
    
    EXPECT_EQ(expectedResult, result);
}

TEST_F(PrefixScanTest, FullInclusiveScanSameBuffer)
{
    glsl_algo_configuration conf = {GARWTint1, 1024, 32, 1, 1};
    glsl_algo_context ctx = glsl_algo_init(&mGLContext, conf);

    const unsigned numBlocks = 256;
    const unsigned n = 1024 * numBlocks;
    std::vector<unsigned> vec = generateIntegralRandomVector(n, 0u, 3u);
    GLuint inputOutputBuffer = create_ssbo(n, vec.data());
    GLuint intermediateBuffer = create_ssbo(numBlocks);
    glsl_scan(&mGLContext, &ctx, inputOutputBuffer, intermediateBuffer, inputOutputBuffer, n, 1, 1);
    
    std::vector<unsigned> expectedResult(n, 0);
    scan(vec.data(), expectedResult.data(), n, n, 1);
    
    std::vector<unsigned> result(n, 0);
    get_ssbo_data(inputOutputBuffer, n, result.data());
    
    EXPECT_EQ(expectedResult, result);
}

TEST_F(PrefixScanTest, FullScanSmallBlockSize)
{
    glsl_algo_configuration conf = {GARWTint1, 256, 32, 1, 1};
    glsl_algo_context ctx = glsl_algo_init(&mGLContext, conf);

    const unsigned numBlocks = 1024;
    const unsigned n = 1024 * 256;
    std::vector<unsigned> vec = generateIntegralRandomVector(n, 0u, 6u);
    GLuint inputBuffer = create_ssbo(n, vec.data());
    GLuint intermediateBuffer = create_ssbo(numBlocks);
    GLuint outputBuffer = create_ssbo(n);
    glsl_scan(&mGLContext, &ctx, inputBuffer, intermediateBuffer, outputBuffer, n, 1, 0);
    
    std::vector<unsigned> expectedResult(n, 0);
    scan(vec.data(), expectedResult.data(), n, n, 0);
    
    std::vector<unsigned> result(n, 0);
    get_ssbo_data(outputBuffer, n, result.data());
    
    EXPECT_EQ(expectedResult, result);
}

TEST_F(PrefixScanTest, FullScanMediumBlockSize)
{
    glsl_algo_configuration conf = {GARWTint1, 512, 32, 1, 1};
    glsl_algo_context ctx = glsl_algo_init(&mGLContext, conf);

    const unsigned numBlocks = 512;
    const unsigned n = 1024 * 256;
    std::vector<unsigned> vec = generateIntegralRandomVector(n, 0u, 6u);
    GLuint inputBuffer = create_ssbo(n, vec.data());
    GLuint intermediateBuffer = create_ssbo(numBlocks);
    GLuint outputBuffer = create_ssbo(n);
    glsl_scan(&mGLContext, &ctx, inputBuffer, intermediateBuffer, outputBuffer, n, 1, 0);
    
    std::vector<unsigned> expectedResult(n, 0);
    scan(vec.data(), expectedResult.data(), n, n, 0);
    
    std::vector<unsigned> result(n, 0);
    get_ssbo_data(outputBuffer, n, result.data());
    
    EXPECT_EQ(expectedResult, result);
}

TEST_F(PrefixScanTest, FullScanMultipleRWPerThread)
{
    glsl_algo_configuration conf = {GARWTint1, 1024, 32, 1, 1};
    glsl_algo_context ctx = glsl_algo_init(&mGLContext, conf);

    const unsigned numBlocks = 256;
    const unsigned n = 1024 * numBlocks;
    std::vector<unsigned> vec = generateIntegralRandomVector(n, 0u, 6u);
    GLuint inputBuffer = create_ssbo(n, vec.data());
    GLuint intermediateBuffer = create_ssbo(numBlocks);
    GLuint outputBuffer = create_ssbo(n);
    glsl_scan(&mGLContext, &ctx, inputBuffer, intermediateBuffer, outputBuffer, n, 16, 0);
    
    std::vector<unsigned> expectedResult(n, 0);
    scan(vec.data(), expectedResult.data(), n, n, 0);
    
    std::vector<unsigned> result(n, 0);
    get_ssbo_data(outputBuffer, n, result.data());
    
    EXPECT_EQ(expectedResult, result);
}

TEST_P(PrefixScanTestWithMultipleParameters, UnsignedPrefixScan)
{
    ScanParameterVariation variation = GetParam();
    glsl_algo_configuration conf = {ConvertType<unsigned>(variation.type), variation.blockSize, 32, 1, 1};
    glsl_algo_context ctx = glsl_algo_init(&mGLContext, conf);
    
    const unsigned n = 1024 * 1024 * 16 + 13;
    const unsigned numBlocks = (n+128-1) / 128;
    std::vector<unsigned> vec = generateIntegralRandomVector(n, 0u, 2u);
    GLuint inputBuffer = create_ssbo(n, vec.data());
    GLuint intermediateBuffer = create_ssbo(numBlocks);
    GLuint outputBuffer = create_ssbo(n);
    glsl_scan(&mGLContext, &ctx, inputBuffer, intermediateBuffer, outputBuffer, n, variation.elementsPerThread, variation.isInclusive);
    
    std::vector<unsigned> expectedResult(n, 0);
    scan(vec.data(), expectedResult.data(), n, n, variation.isInclusive);
    
    std::vector<unsigned> result(n, 0);
    get_ssbo_data(outputBuffer, n, result.data());
    
    EXPECT_EQ(expectedResult, result);
}

TEST_P(PrefixScanTestWithMultipleParameters, SignedPrefixScan)
{
    ScanParameterVariation variation = GetParam();
    glsl_algo_configuration conf = {ConvertType<int>(variation.type), variation.blockSize, 32, 1, 1};
    glsl_algo_context ctx = glsl_algo_init(&mGLContext, conf);
    
    const unsigned n = 1024 * 1024 * 16 + 13;
    const unsigned numBlocks = (n+128-1) / 128;
    std::vector<int> vec = generateIntegralRandomVector(n, 0, 2);
    GLuint inputBuffer = create_ssbo(n, vec.data());
    GLuint intermediateBuffer = create_ssbo(numBlocks);
    GLuint outputBuffer = create_ssbo(n);
    glsl_scan(&mGLContext, &ctx, inputBuffer, intermediateBuffer, outputBuffer, n, variation.elementsPerThread, variation.isInclusive);
    
    std::vector<int> expectedResult(n, 0);
    scan(vec.data(), expectedResult.data(), n, n, variation.isInclusive);
    
    std::vector<int> result(n, 0);
    get_ssbo_data(outputBuffer, n, result.data());
    
    EXPECT_EQ(expectedResult, result);
}

static std::vector<ScanParameterVariation> GetScanFullTestParameters()
{
    std::vector<ScanParameterVariation> result;
    
    GLSL_ALGO_READ_WRITE_TYPE rwTypes[] = {GARWTuint1, GARWTuint2, GARWTuint4};
    
    for (unsigned int t = 0u; t < 3u; ++t)
    {
        for (unsigned int blockSize = 128u; blockSize <= 1024u; blockSize += 128u)
        {
            for (unsigned int readsPerThread = 1u; readsPerThread <= 8u; readsPerThread*=2)
            {
                for (unsigned char isInclusive = 0; isInclusive <= 1; ++isInclusive)
                {
                    ScanParameterVariation variation = {rwTypes[t], blockSize, readsPerThread, isInclusive};
                    result.push_back(variation);
                }
            }
        }
    }
    return result;
}

INSTANTIATE_TEST_CASE_P(AllParameters, PrefixScanTestWithMultipleParameters,
                        ::testing::ValuesIn(GetScanFullTestParameters()));