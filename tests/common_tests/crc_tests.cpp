

#include "checksums/checksums.h"
#include "gtest/gtest.h"

using namespace testing;
using namespace bootloader::checksums;
namespace {

class CrcTestSuite : public ::testing::Test {
public:
    static constexpr size_t MESSAGE_SIZE = 10;
    uint8_t message[MESSAGE_SIZE] = { 1, 2, 3, 4, 5, 6, 7, 8, 9, 10 };
    void SetUp() override {}
};

TEST_F(CrcTestSuite, IterativeChecksumTwoStepsEqualsChecksum)
{
    uint32_t fullCrc = Checksums::calculateCrc32NoTable(message, MESSAGE_SIZE);

    uint32_t iterativeCrc = Checksums::CRC_INITIAL_VALUE;
    iterativeCrc =
        Checksums::calculateIterativeCrc32NoTable(message, MESSAGE_SIZE / 2, iterativeCrc);
    iterativeCrc =
        Checksums::calculateIterativeCrc32NoTable(&message[5], MESSAGE_SIZE / 2, iterativeCrc);
    iterativeCrc = ~iterativeCrc;

    ASSERT_EQ(fullCrc, iterativeCrc);
}

TEST_F(CrcTestSuite, IterativeChecksumBytewiseEqualsChecksum)
{
    uint32_t fullCrc = Checksums::calculateCrc32NoTable(message, MESSAGE_SIZE);

    uint32_t iterativeCrc = Checksums::CRC_INITIAL_VALUE;
    iterativeCrc = Checksums::calculateIterativeCrc32NoTable(&message[0], 1, iterativeCrc);
    iterativeCrc = Checksums::calculateIterativeCrc32NoTable(&message[1], 1, iterativeCrc);
    iterativeCrc = Checksums::calculateIterativeCrc32NoTable(&message[2], 1, iterativeCrc);
    iterativeCrc = Checksums::calculateIterativeCrc32NoTable(&message[3], 1, iterativeCrc);
    iterativeCrc = Checksums::calculateIterativeCrc32NoTable(&message[4], 1, iterativeCrc);
    iterativeCrc = Checksums::calculateIterativeCrc32NoTable(&message[5], 1, iterativeCrc);
    iterativeCrc = Checksums::calculateIterativeCrc32NoTable(&message[6], 1, iterativeCrc);
    iterativeCrc = Checksums::calculateIterativeCrc32NoTable(&message[7], 1, iterativeCrc);
    iterativeCrc = Checksums::calculateIterativeCrc32NoTable(&message[8], 1, iterativeCrc);
    iterativeCrc = Checksums::calculateIterativeCrc32NoTable(&message[9], 1, iterativeCrc);
    iterativeCrc = ~iterativeCrc;

    ASSERT_EQ(fullCrc, iterativeCrc);
}

}
