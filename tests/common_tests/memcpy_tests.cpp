

#include "memcpy/memcpy.h"
#include "gtest/gtest.h"
#include <cstring>
#include <gmock/gmock-matchers.h>

using namespace bootloader::memcpy;
using namespace testing;
namespace {

const auto& defaultMemcpy = memcpy;
const auto& myMemcpy = Memcpy;

class MemCopyTestSuite : public ::testing::Test {
public:
    static constexpr int BUFFER_SIZE = 12;

    uint8_t buffer1[BUFFER_SIZE] = { 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12 };
    uint8_t buffer2[BUFFER_SIZE] = { 'a', 'b', 'c', 'd', 'e', 'f', 'g', 'h', 'i', 'j', 'k', 'l' };
    uint8_t buffer3[BUFFER_SIZE] = { 'A', 'B', 'C', 'D', 'E', 'F', 'G', 'H', 'I', 'J', 'K', 'L' };

    void SetUp() override
    {
        for (int i = 0; i < BUFFER_SIZE; i++) {
            buffer1[i] = i + 1;
            buffer2[i] = 'a' + i;
            buffer3[i] = 'A' + i;
        }
    }

    void buffer1HasInitialValues()
    {
        for (int i = 0; i < BUFFER_SIZE; i++) {
            ASSERT_EQ(buffer1[i], i + 1);
        }
    }
};

TEST_F(MemCopyTestSuite, FullCopy)
{
    myMemcpy(buffer2, buffer1, BUFFER_SIZE);
    defaultMemcpy(buffer3, buffer1, BUFFER_SIZE);

    ASSERT_EQ(memcmp(buffer1, buffer2, BUFFER_SIZE), 0);
    ASSERT_EQ(memcmp(buffer1, buffer3, BUFFER_SIZE), 0);
    ASSERT_EQ(memcmp(buffer2, buffer3, BUFFER_SIZE), 0);
}

TEST_F(MemCopyTestSuite, CopyZero)
{
    constexpr size_t COPY_SIZE = 0;
    myMemcpy(buffer2, buffer1, COPY_SIZE);
    defaultMemcpy(buffer3, buffer1, COPY_SIZE);

    ASSERT_EQ(memcmp(buffer1, buffer2, COPY_SIZE), 0);
    ASSERT_EQ(memcmp(buffer1, buffer3, COPY_SIZE), 0);
    ASSERT_EQ(memcmp(buffer2, buffer3, COPY_SIZE), 0);
    ASSERT_EQ(buffer1[0], 1);
    ASSERT_EQ(buffer2[0], 'a');
    ASSERT_EQ(buffer3[0], 'A');
}

TEST_F(MemCopyTestSuite, CopyNine)
{
    constexpr size_t COPY_SIZE = 9;
    myMemcpy(buffer2, buffer1, COPY_SIZE);
    defaultMemcpy(buffer3, buffer1, COPY_SIZE);

    ASSERT_EQ(memcmp(buffer1, buffer2, COPY_SIZE), 0);
    ASSERT_EQ(memcmp(buffer1, buffer3, COPY_SIZE), 0);
    ASSERT_EQ(memcmp(buffer2, buffer3, COPY_SIZE), 0);
}

TEST_F(MemCopyTestSuite, CopyTen)
{
    constexpr size_t COPY_SIZE = 10;
    myMemcpy(buffer2, buffer1, COPY_SIZE);
    defaultMemcpy(buffer3, buffer1, COPY_SIZE);

    ASSERT_EQ(memcmp(buffer1, buffer2, COPY_SIZE), 0);
    ASSERT_EQ(memcmp(buffer1, buffer3, COPY_SIZE), 0);
    ASSERT_EQ(memcmp(buffer2, buffer3, COPY_SIZE), 0);
}

TEST_F(MemCopyTestSuite, CopyEleven)
{
    constexpr size_t COPY_SIZE = 11;
    myMemcpy(buffer2, buffer1, COPY_SIZE);
    defaultMemcpy(buffer3, buffer1, COPY_SIZE);

    ASSERT_EQ(memcmp(buffer1, buffer2, COPY_SIZE), 0);
    ASSERT_EQ(memcmp(buffer1, buffer3, COPY_SIZE), 0);
    ASSERT_EQ(memcmp(buffer2, buffer3, COPY_SIZE), 0);
}

TEST_F(MemCopyTestSuite, CopyThree)
{
    constexpr size_t COPY_SIZE = 3;
    myMemcpy(buffer2, buffer1, COPY_SIZE);
    defaultMemcpy(buffer3, buffer1, COPY_SIZE);

    ASSERT_EQ(memcmp(buffer1, buffer2, COPY_SIZE), 0);
    ASSERT_EQ(memcmp(buffer1, buffer3, COPY_SIZE), 0);
    ASSERT_EQ(memcmp(buffer2, buffer3, COPY_SIZE), 0);
    ASSERT_EQ(buffer1[0], 1);
    ASSERT_EQ(buffer2[0], 1);
    ASSERT_EQ(buffer3[0], 1);

    ASSERT_EQ(buffer1[1], 2);
    ASSERT_EQ(buffer2[1], 2);
    ASSERT_EQ(buffer3[1], 2);

    ASSERT_EQ(buffer1[2], 3);
    ASSERT_EQ(buffer2[2], 3);
    ASSERT_EQ(buffer3[2], 3);

    // Unchanged after COPY_SIZE
    ASSERT_EQ(buffer1[3], 4);
    ASSERT_EQ(buffer2[3], 'd');
    ASSERT_EQ(buffer3[3], 'D');
}

TEST_F(MemCopyTestSuite, CopyTwo)
{
    constexpr size_t COPY_SIZE = 2;
    myMemcpy(buffer2, buffer1, COPY_SIZE);
    defaultMemcpy(buffer3, buffer1, COPY_SIZE);

    ASSERT_EQ(memcmp(buffer1, buffer2, COPY_SIZE), 0);
    ASSERT_EQ(memcmp(buffer1, buffer3, COPY_SIZE), 0);
    ASSERT_EQ(memcmp(buffer2, buffer3, COPY_SIZE), 0);
    ASSERT_EQ(buffer1[0], 1);
    ASSERT_EQ(buffer2[0], 1);
    ASSERT_EQ(buffer3[0], 1);

    ASSERT_EQ(buffer1[1], 2);
    ASSERT_EQ(buffer2[1], 2);
    ASSERT_EQ(buffer3[1], 2);
    // Unchanged after COPY_SIZE

    ASSERT_EQ(buffer1[2], 3);
    ASSERT_EQ(buffer2[2], 'c');
    ASSERT_EQ(buffer3[2], 'C');

    ASSERT_EQ(buffer1[3], 4);
    ASSERT_EQ(buffer2[3], 'd');
    ASSERT_EQ(buffer3[3], 'D');
}

TEST_F(MemCopyTestSuite, CopyOne)
{
    constexpr size_t COPY_SIZE = 1;
    myMemcpy(buffer2, buffer1, COPY_SIZE);
    defaultMemcpy(buffer3, buffer1, COPY_SIZE);

    ASSERT_EQ(memcmp(buffer1, buffer2, COPY_SIZE), 0);
    ASSERT_EQ(memcmp(buffer1, buffer3, COPY_SIZE), 0);
    ASSERT_EQ(memcmp(buffer2, buffer3, COPY_SIZE), 0);
    ASSERT_EQ(buffer1[0], 1);
    ASSERT_EQ(buffer2[0], 1);
    ASSERT_EQ(buffer3[0], 1);
    // Unchanged after COPY_SIZE
    ASSERT_EQ(buffer1[1], 2);
    ASSERT_EQ(buffer2[1], 'b');
    ASSERT_EQ(buffer3[1], 'B');

    ASSERT_EQ(buffer1[2], 3);
    ASSERT_EQ(buffer2[2], 'c');
    ASSERT_EQ(buffer3[2], 'C');

    ASSERT_EQ(buffer1[3], 4);
    ASSERT_EQ(buffer2[3], 'd');
    ASSERT_EQ(buffer3[3], 'D');
}

TEST_F(MemCopyTestSuite, CopySameAddress)
{
    constexpr size_t COPY_SIZE = 3;
    myMemcpy(buffer1, buffer1, COPY_SIZE);
    buffer1HasInitialValues();
    defaultMemcpy(buffer1, buffer1, COPY_SIZE);
    buffer1HasInitialValues();
}

TEST_F(MemCopyTestSuite, CopyNullptr)
{
    constexpr size_t COPY_SIZE = 3;
    myMemcpy(buffer1, nullptr, COPY_SIZE);
    myMemcpy(nullptr, buffer1, COPY_SIZE);
    myMemcpy(nullptr, nullptr, COPY_SIZE);
    buffer1HasInitialValues();
}

TEST_F(MemCopyTestSuite, CopyBackwardOverlappingOffsetOne)
{
    constexpr size_t COPY_SIZE = 3;
    myMemcpy(buffer1, &buffer1[1], COPY_SIZE);

    EXPECT_THAT(buffer1, ElementsAreArray({ 2, 3, 4, 4, 5, 6, 7, 8, 9, 10, 11, 12 }));
}

TEST_F(MemCopyTestSuite, CopyBackwardOverlappingOffsetTwo)
{
    constexpr size_t COPY_SIZE = 3;
    myMemcpy(buffer1, &buffer1[2], COPY_SIZE);

    EXPECT_THAT(buffer1, ElementsAreArray({ 3, 4, 5, 4, 5, 6, 7, 8, 9, 10, 11, 12 }));
}

TEST_F(MemCopyTestSuite, CopyBackwardOverlappingOffsetThree)
{
    constexpr size_t COPY_SIZE = 3;
    myMemcpy(buffer1, &buffer1[3], COPY_SIZE);

    EXPECT_THAT(buffer1, ElementsAreArray({ 4, 5, 6, 4, 5, 6, 7, 8, 9, 10, 11, 12 }));
}

TEST_F(MemCopyTestSuite, CopyBackwardOverlappingOffsetFour)
{
    constexpr size_t COPY_SIZE = 3;
    myMemcpy(buffer1, &buffer1[4], COPY_SIZE);

    EXPECT_THAT(buffer1, ElementsAreArray({ 5, 6, 7, 4, 5, 6, 7, 8, 9, 10, 11, 12 }));
}

TEST_F(MemCopyTestSuite, CopyForwardOverlappingOffsetOne)
{
    constexpr size_t COPY_SIZE = 3;
    myMemcpy(&buffer1[1], buffer1, COPY_SIZE);

    EXPECT_THAT(buffer1, ElementsAreArray({ 1, 1, 2, 3, 5, 6, 7, 8, 9, 10, 11, 12 }));
}

TEST_F(MemCopyTestSuite, CopyForwardOverlappingOffsetTwo)
{
    constexpr size_t COPY_SIZE = 3;
    myMemcpy(&buffer1[2], buffer1, COPY_SIZE);

    EXPECT_THAT(buffer1, ElementsAreArray({ 1, 2, 1, 2, 3, 6, 7, 8, 9, 10, 11, 12 }));
}

TEST_F(MemCopyTestSuite, CopyForwardOverlappingOffsetThree)
{
    constexpr size_t COPY_SIZE = 3;
    myMemcpy(&buffer1[3], buffer1, COPY_SIZE);

    EXPECT_THAT(buffer1, ElementsAreArray({ 1, 2, 3, 1, 2, 3, 7, 8, 9, 10, 11, 12 }));
}

}
