#include "gl_setup.hpp"
#include "scan_utilities.hpp"

#include <prefix_scan.h>
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
};

TEST_F(PrefixScanTest, SimpleTest)
{
    const unsigned n = 128u;
    std::vector<int> vec = generateRandomVector(n, 0, 6);
    GLuint inputBuffer = create_ssbo(n, vec.data());
    GLuint outputBuffer = create_ssbo(n);
    glsl_prefix_scan(inputBuffer, outputBuffer, n, 0);
    
    std::vector<int> expectedResult(n);
    scan(vec.data(), expectedResult.data(), n, false);
    
    std::vector<int> result(n);
    get_ssbo_data(outputBuffer, n, result.data());
    
    EXPECT_EQ(expectedResult, result);
}