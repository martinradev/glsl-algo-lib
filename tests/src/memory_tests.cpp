#include <common/gl_setup.hpp>
#include <common/scan_utilities.hpp>
#include <common/macros.hpp>

#include <glsl_algo/memory.h>
#include <gtest/gtest.h>

class MemoryTest : public ::testing::Test
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

TEST_F(MemoryTest, SetBufferToZero)
{
    glsl_algo_configuration conf = {GARWTint1, 256, 32};
    glsl_algo_context ctx = glsl_algo_init(&mGLContext, conf);
  
    const unsigned n = 1024 * 1024;
    std::vector<int> vec = generateIntegralRandomVector(n, 0, 6);
    GLuint inputOutputBuffer = create_ssbo(n, vec.data());
    
    glsl_memory_set_to_zero(&mGLContext, &ctx, inputOutputBuffer, n, 1);
    
    std::vector<int> result(n);
    get_ssbo_data(inputOutputBuffer, n, result.data());
    
    std::vector<int> expectedResult(n, 0);
    
    EXPECT_EQ(expectedResult, result);
}

TEST_F(MemoryTest, SetBufferToZeroSmallArray)
{
    glsl_algo_configuration conf = {GARWTuint4, 384, 32};
    glsl_algo_context ctx = glsl_algo_init(&mGLContext, conf);
  
    const unsigned n = 256;
    std::vector<unsigned> vec = generateIntegralRandomVector(n, 0u, 199989u);
    GLuint inputOutputBuffer = create_ssbo(n, vec.data());
    
    glsl_memory_set_to_zero(&mGLContext, &ctx, inputOutputBuffer, n, 1);
    
    std::vector<unsigned> result(n);
    get_ssbo_data(inputOutputBuffer, n, result.data());
    
    std::vector<unsigned> expectedResult(n, 0);
    
    EXPECT_EQ(expectedResult, result);
}

TEST_F(MemoryTest, SetBufferToZeroMultipleElementsPerThread)
{
    glsl_algo_configuration conf = {GARWTint2, 1024, 32};
    glsl_algo_context ctx = glsl_algo_init(&mGLContext, conf);
  
    const unsigned n = 1024 * 1024;
    std::vector<int> vec = generateIntegralRandomVector(n, 0, 6);
    GLuint inputOutputBuffer = create_ssbo(n, vec.data());
    
    glsl_memory_set_to_zero(&mGLContext, &ctx, inputOutputBuffer, n, 4);
    
    std::vector<int> result(n);
    get_ssbo_data(inputOutputBuffer, n, result.data());
    
    std::vector<int> expectedResult(n, 0);
    
    EXPECT_EQ(expectedResult, result);
}

TEST_F(MemoryTest, CopyBuffer)
{
    glsl_algo_configuration conf = {GARWTint1, 256, 32};
    glsl_algo_context ctx = glsl_algo_init(&mGLContext, conf);
  
    const unsigned n = 1024 * 1024;
    std::vector<int> vec = generateIntegralRandomVector(n, 0, 6);
    GLuint inputBuffer = create_ssbo(n, vec.data());
    GLuint outputBuffer = create_ssbo(n);
    
    glsl_copy_memory(&mGLContext, &ctx, inputBuffer, outputBuffer, n, 1);
    
    std::vector<int> result(n);
    get_ssbo_data(outputBuffer, n, result.data());
    
    EXPECT_EQ(vec, result);
}

TEST_F(MemoryTest, CopyBufferMultipleElementsPerThread)
{
    glsl_algo_configuration conf = {GARWTint1, 256, 32};
    glsl_algo_context ctx = glsl_algo_init(&mGLContext, conf);
  
    const unsigned n = 1024 * 1024;
    std::vector<int> vec = generateIntegralRandomVector(n, 0, 6);
    GLuint inputBuffer = create_ssbo(n, vec.data());
    GLuint outputBuffer = create_ssbo(n);
    
    glsl_copy_memory(&mGLContext, &ctx, inputBuffer, outputBuffer, n, 16);
    
    std::vector<int> result(n);
    get_ssbo_data(outputBuffer, n, result.data());
    
    EXPECT_EQ(vec, result);
}

TEST_F(MemoryTest, CopyBufferUint4)
{
    glsl_algo_configuration conf = {GARWTuint4, 512, 32};
    glsl_algo_context ctx = glsl_algo_init(&mGLContext, conf);
  
    const unsigned n = 1024 * 1024;
    std::vector<unsigned> vec = generateIntegralRandomVector(n, 0u, 6u);
    GLuint inputBuffer = create_ssbo(n, vec.data());
    GLuint outputBuffer = create_ssbo(n);
    
    glsl_copy_memory(&mGLContext, &ctx, inputBuffer, outputBuffer, n, 1);
    
    std::vector<unsigned> result(n);
    get_ssbo_data(outputBuffer, n, result.data());
    
    EXPECT_EQ(vec, result);
}

TEST_F(MemoryTest, CopyBufferFloat2)
{
    glsl_algo_configuration conf = {GARWTfloat2, 512, 32};
    glsl_algo_context ctx = glsl_algo_init(&mGLContext, conf);
  
    const unsigned n = 1024 * 1024;
    std::vector<float> vec = generateFloatRandomVector(n, 0.0f, 6.0f);
    GLuint inputBuffer = create_ssbo(n, vec.data());
    GLuint outputBuffer = create_ssbo(n);
    
    glsl_copy_memory(&mGLContext, &ctx, inputBuffer, outputBuffer, n, 1);
    
    std::vector<float> result(n);
    get_ssbo_data(outputBuffer, n, result.data());
    
    EXPECT_EQ(vec, result);
}