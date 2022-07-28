#include "hmac/hmac.h"
#include "gtest/gtest.h"

using namespace testing;
namespace {

class HMACTestSuite : public ::testing::Test {
public:
    void SetUp() override {}
};

TEST_F(HMACTestSuite, Dummy)
{
    ASSERT_EQ(true, true);
}

}
