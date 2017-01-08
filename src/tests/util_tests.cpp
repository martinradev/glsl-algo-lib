#include "gl_setup.hpp"

#include <gtest/gtest.h>

#include <vector>

class UtilTest : public ::testing::Test
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

TEST_F(UtilTest, TestSmallSSBO)
{
    std::vector<unsigned> data = {1u,2u,3u,4u};
    GLuint ssbo = create_ssbo(4u, data.data());
    std::vector<unsigned> ssbo_data(4);
    get_ssbo_data(ssbo, 4u, ssbo_data.data());
    EXPECT_TRUE(data == ssbo_data);
}

TEST_F(UtilTest, TestBigSSBO)
{
    const size_t n = 10000;
    std::vector<unsigned> data(n, 1000u);
    GLuint ssbo = create_ssbo(n, data.data());
    std::vector<unsigned> ssbo_data(n);
    get_ssbo_data(ssbo, n, ssbo_data.data());
    EXPECT_TRUE(data == ssbo_data);
}