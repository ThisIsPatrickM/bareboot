

#include "checksums/crc32.h"
#include "gtest/gtest.h"

using namespace testing;
using namespace bootloader::checksums;
namespace {

class Crc32TestSuite : public ::testing::Test {
public:
    static constexpr uint8_t TEST_DATA1[] = "Testdata";
    static constexpr uint8_t TEST_DATA2[] = { 'A', 'B', 'C' };

    static constexpr uint32_t EXPECTED_CRC1 = 308318084;
    static constexpr uint32_t EXPECTED_CRC2 = 2285480319;
};

TEST_F(Crc32TestSuite, Test_CRC_IterativeEqualsCompleteCrc)
{
    uint32_t crc = Crc32::calculateCRC32(TEST_DATA1, sizeof(TEST_DATA1));

    uint32_t iterativeCRC = Crc32::CRC_INITIAL_VALUE;
    iterativeCRC = Crc32::calculateCRC32Step(TEST_DATA1, 4, iterativeCRC);

    iterativeCRC = Crc32::calculateCRC32Step(&TEST_DATA1[4], 5, iterativeCRC);
    iterativeCRC = (iterativeCRC ^ Crc32::CRC_FINAL_XOR_VALUE);

    EXPECT_EQ(iterativeCRC, crc);
}

TEST_F(Crc32TestSuite, Test_CRC_TestData1)
{
    uint32_t crc = Crc32::calculateCRC32(TEST_DATA1, sizeof(TEST_DATA1));
    EXPECT_EQ(crc, EXPECTED_CRC1);
}

TEST_F(Crc32TestSuite, Test_CRC_TestData2)
{
    uint32_t crc = Crc32::calculateCRC32(TEST_DATA2, sizeof(TEST_DATA2));
    EXPECT_EQ(crc, EXPECTED_CRC2);
}

TEST_F(Crc32TestSuite, Test_CRC_TestData2Iterative)
{
    uint32_t iterative = Crc32::CRC_INITIAL_VALUE;
    iterative = Crc32::calculateCRC32Step(&TEST_DATA2[0], 1, iterative);
    iterative = Crc32::calculateCRC32Step(&TEST_DATA2[1], 1, iterative);
    iterative = Crc32::calculateCRC32Step(&TEST_DATA2[2], 1, iterative);
    EXPECT_EQ(iterative ^ Crc32::CRC_FINAL_XOR_VALUE, EXPECTED_CRC2);
}

}
