#include "gl_setup.hpp"

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
    EXPECT_EQ(1,1);
}