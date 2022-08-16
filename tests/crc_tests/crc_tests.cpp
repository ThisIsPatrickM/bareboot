

#include "checksums/crc32.h"
#include "gtest/gtest.h"

using namespace testing;
using namespace bootloader::checksums;
namespace {

class Crc32TestSuite : public ::testing::Test {
public:
    static constexpr size_t MESSAGE_SIZE = 10;
    uint8_t message[MESSAGE_SIZE] = { 1, 2, 3, 4, 5, 6, 7, 8, 9, 10 };
    void SetUp() override {}
};

TEST_F(Crc32TestSuite, IterativeChecksumTwoStepsEqualsChecksum)
{
    uint32_t fullCrc = Crc32::calculateCrc32(message, MESSAGE_SIZE);

    uint32_t iterativeCrc = Crc32::CRC_INITIAL_VALUE;
    iterativeCrc = Crc32::calculateCrc32Step(message, MESSAGE_SIZE / 2, iterativeCrc);
    iterativeCrc = Crc32::calculateCrc32Step(&message[5], MESSAGE_SIZE / 2, iterativeCrc);
    iterativeCrc = ~iterativeCrc;

    ASSERT_EQ(fullCrc, iterativeCrc);
}

TEST_F(Crc32TestSuite, IterativeChecksumBytewiseEqualsChecksum)
{
    uint32_t fullCrc = Crc32::calculateCrc32(message, MESSAGE_SIZE);

    uint32_t iterativeCrc = Crc32::CRC_INITIAL_VALUE;
    iterativeCrc = Crc32::calculateCrc32Step(&message[0], 1, iterativeCrc);
    iterativeCrc = Crc32::calculateCrc32Step(&message[1], 1, iterativeCrc);
    iterativeCrc = Crc32::calculateCrc32Step(&message[2], 1, iterativeCrc);
    iterativeCrc = Crc32::calculateCrc32Step(&message[3], 1, iterativeCrc);
    iterativeCrc = Crc32::calculateCrc32Step(&message[4], 1, iterativeCrc);
    iterativeCrc = Crc32::calculateCrc32Step(&message[5], 1, iterativeCrc);
    iterativeCrc = Crc32::calculateCrc32Step(&message[6], 1, iterativeCrc);
    iterativeCrc = Crc32::calculateCrc32Step(&message[7], 1, iterativeCrc);
    iterativeCrc = Crc32::calculateCrc32Step(&message[8], 1, iterativeCrc);
    iterativeCrc = Crc32::calculateCrc32Step(&message[9], 1, iterativeCrc);
    iterativeCrc = ~iterativeCrc;

    ASSERT_EQ(fullCrc, iterativeCrc);
}

}
