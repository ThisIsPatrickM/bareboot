#include "hash/sha_256/sha_256.h"
#include "gtest/gtest.h"

#include <cstring>
#include <iostream>

using namespace testing;
namespace {

using namespace bootloader::hash::sha_256;
using namespace std;

class Sha256TestSuite : public ::testing::Test {
public:
    void SetUp() override {}

    static constexpr uint8_t SAMPLE_MESSAGE[] = { 'A', 'B', 'C', 'D', 'E', 'F',
                                                  'G', 'H', 'I', 'J', 'K', 'L' };
    static constexpr uint8_t EXPECTED_DIGEST_SAMPLE_MESSAGE[Sha256::SHA256_DIGEST_SIZE] = {
        0x92, 0x24, 0x29, 0xcc, 0xdb, 0x70, 0x45, 0xd1, 0x11, 0x43, 0xe2,
        0xe3, 0x98, 0x2a, 0x11, 0xaf, 0xc1, 0x1b, 0x53, 0x7b, 0xf2, 0x59,
        0xd8, 0x8d, 0x24, 0x25, 0xfa, 0x88, 0x06, 0xe8, 0x6e, 0x78
    };

    static constexpr uint8_t LONG_MESSAGE[] =
        "hello worldhello worldhello worldhello worldhello worldhello worldhello worldhello "
        "worldhello worldhello worldhello worldhello worldhello worldhello worldhello world";
    static constexpr uint8_t EXPECTED_DIGEST_LONG_MESSAGE[Sha256::SHA256_DIGEST_SIZE] = {
        0xc2, 0xae, 0x19, 0xf3, 0x87, 0xb8, 0xdd, 0xc8, 0x8e, 0x93, 0xda,
        0x83, 0xd0, 0xf1, 0xde, 0x44, 0xfb, 0x2a, 0x9d, 0xc3, 0xae, 0xae,
        0x53, 0xb2, 0x3f, 0xa3, 0xa1, 0x95, 0x13, 0x9c, 0x4a, 0x7a
    };

    static constexpr uint8_t BLOCK_SIZED_MESSAGE[] =
        "oneblockoneblockoneblockoneblockoneblockoneblockoneblockonebloc";
    static_assert(sizeof(BLOCK_SIZED_MESSAGE) == Sha256::SHA256_BLOCK_SIZE);
    static constexpr uint8_t EXPECTED_DIGEST_BLOCK_SIZED_MESSAGE[Sha256::SHA256_DIGEST_SIZE] = {
        0x10, 0xdc, 0xea, 0xd3, 0xa7, 0x8,  0x5e, 0x63, 0xff, 0x78, 0x6c,
        0x5f, 0xa8, 0xc4, 0xba, 0xaf, 0xeb, 0x7d, 0x3e, 0x2e, 0x4a, 0xb9,
        0xb5, 0x75, 0x51, 0x73, 0x9b, 0xb3, 0x8d, 0xb3, 0x7e, 0xfc
    };

    // Initialized with string is \0 terminated
    static constexpr uint8_t UNPADDED_MESSAGE[] =
        "oneblockoneblockoneblockoneblockoneblockoneblockoneblo";
    static constexpr uint8_t EXPECTED_DIGEST_UNPADDED_MESSAGE[Sha256::SHA256_DIGEST_SIZE] = {
        0x9e, 0x41, 0xff, 0x49, 0xb8, 0x70, 0xe4, 0x10, 0xbf, 0x5a, 0x11,
        0x96, 0xf8, 0x12, 0x79, 0x17, 0x79, 0x20, 0xbc, 0x93, 0x8a, 0x23,
        0xdb, 0xe4, 0xd,  0xdc, 0x54, 0x93, 0xea, 0x44, 0xed, 0x20
    };

    static constexpr uint8_t PADDED_MESSAGE[] =
        "oneblockoneblockoneblockoneblockoneblockoneblockonebloc";
    static constexpr uint8_t EXPECTED_DIGEST_PADDED_MESSAGE[Sha256::SHA256_DIGEST_SIZE] = {
        0xcb, 0x8,  0xc2, 0x46, 0xea, 0x59, 0x34, 0x6b, 0x5a, 0xb7, 0xfe,
        0x66, 0x7f, 0xc6, 0xc2, 0xc3, 0xc0, 0xa6, 0xa7, 0xf0, 0xb4, 0x8e,
        0x59, 0x46, 0x9d, 0xce, 0xa1, 0x1a, 0x37, 0x7d, 0xca, 0xb2
    };

    uint8_t digestBuffer[Sha256::SHA256_DIGEST_SIZE] = { 0 };
};

TEST_F(Sha256TestSuite, SampleMessage)
{
    Sha256::sha256(SAMPLE_MESSAGE, sizeof(SAMPLE_MESSAGE), digestBuffer);
    ASSERT_EQ(memcmp(EXPECTED_DIGEST_SAMPLE_MESSAGE, digestBuffer, Sha256::SHA256_DIGEST_SIZE), 0);
    ASSERT_TRUE(Sha256::sha256Verify(
        SAMPLE_MESSAGE, sizeof(SAMPLE_MESSAGE), EXPECTED_DIGEST_SAMPLE_MESSAGE));
}

TEST_F(Sha256TestSuite, LongMessage)
{
    Sha256::sha256(LONG_MESSAGE, sizeof(LONG_MESSAGE), digestBuffer);
    ASSERT_EQ(memcmp(EXPECTED_DIGEST_LONG_MESSAGE, digestBuffer, Sha256::SHA256_DIGEST_SIZE), 0);
    ASSERT_TRUE(
        Sha256::sha256Verify(LONG_MESSAGE, sizeof(LONG_MESSAGE), EXPECTED_DIGEST_LONG_MESSAGE));
}

TEST_F(Sha256TestSuite, BlockSizedMessage)
{
    Sha256::sha256(BLOCK_SIZED_MESSAGE, sizeof(BLOCK_SIZED_MESSAGE), digestBuffer);

    // for (std::size_t i = 0; i < Sha256::SHA256_DIGEST_SIZE; i++) {
    //     std::cerr << hex << (unsigned)digestBuffer[i] << " ";
    // }
    ASSERT_EQ(
        memcmp(EXPECTED_DIGEST_BLOCK_SIZED_MESSAGE, digestBuffer, Sha256::SHA256_DIGEST_SIZE), 0);
    ASSERT_TRUE(Sha256::sha256Verify(
        BLOCK_SIZED_MESSAGE, sizeof(BLOCK_SIZED_MESSAGE), EXPECTED_DIGEST_BLOCK_SIZED_MESSAGE));
}

TEST_F(Sha256TestSuite, UnpaddedMessage)
{
    Sha256::sha256(UNPADDED_MESSAGE, sizeof(UNPADDED_MESSAGE), digestBuffer);

    ASSERT_EQ(
        memcmp(EXPECTED_DIGEST_UNPADDED_MESSAGE, digestBuffer, Sha256::SHA256_DIGEST_SIZE), 0);
    ASSERT_TRUE(Sha256::sha256Verify(
        UNPADDED_MESSAGE, sizeof(UNPADDED_MESSAGE), EXPECTED_DIGEST_UNPADDED_MESSAGE));
}

TEST_F(Sha256TestSuite, PaddedMessage)
{
    Sha256::sha256(PADDED_MESSAGE, sizeof(PADDED_MESSAGE), digestBuffer);

    // for (std::size_t i = 0; i < Sha256::SHA256_DIGEST_SIZE; i++) {
    //     std::cerr << hex << (unsigned)digestBuffer[i] << " ";
    // }

    ASSERT_EQ(memcmp(EXPECTED_DIGEST_PADDED_MESSAGE, digestBuffer, Sha256::SHA256_DIGEST_SIZE), 0);
    ASSERT_TRUE(Sha256::sha256Verify(
        PADDED_MESSAGE, sizeof(PADDED_MESSAGE), EXPECTED_DIGEST_PADDED_MESSAGE));
}

TEST_F(Sha256TestSuite, SampleMessageManualUsage)
{
    // There is only 1 block, so we can immediately finalize
    Sha256Context context {};
    // The while loop will never be used here. To show usage it is still included for small messages
    int remainingLength = sizeof(SAMPLE_MESSAGE);
    int blockIndex = 0;
    while (remainingLength >= static_cast<int>(Sha256::SHA256_BLOCK_SIZE)) {
        Sha256::sha256Transform(context, &SAMPLE_MESSAGE[blockIndex]);
        blockIndex += Sha256::SHA256_BLOCK_SIZE;
        remainingLength -= Sha256::SHA256_BLOCK_SIZE;
    }
    Sha256::sha256Finalise(context, SAMPLE_MESSAGE, sizeof(SAMPLE_MESSAGE), digestBuffer);
    ASSERT_EQ(memcmp(EXPECTED_DIGEST_SAMPLE_MESSAGE, digestBuffer, Sha256::SHA256_DIGEST_SIZE), 0);
}

TEST_F(Sha256TestSuite, LongMessageManualUsage)
{
    // Message is > BLOCK_SIZE of 512 bits (64 bytes), so we need manual fragmentation/streaming
    Sha256Context context {};
    int remainingLength = sizeof(LONG_MESSAGE);
    int blockIndex = 0;
    while (remainingLength >= static_cast<int>(Sha256::SHA256_BLOCK_SIZE)) {
        Sha256::sha256Transform(context, &LONG_MESSAGE[blockIndex]);
        blockIndex += Sha256::SHA256_BLOCK_SIZE;
        remainingLength -= Sha256::SHA256_BLOCK_SIZE;
    }
    Sha256::sha256Finalise(context, &LONG_MESSAGE[blockIndex], remainingLength, digestBuffer);

    ASSERT_EQ(memcmp(EXPECTED_DIGEST_LONG_MESSAGE, digestBuffer, Sha256::SHA256_DIGEST_SIZE), 0);
}

}
