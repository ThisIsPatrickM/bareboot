#include "hmac/hmac.h"
#include "gtest/gtest.h"

#include <iostream>

using namespace testing;
namespace {

using namespace bootloader::hash::sha_256;
using namespace bootloader::hmac;
using namespace std;

class HMacTestSuite : public ::testing::Test {
public:
    void SetUp() override {}

    static constexpr uint8_t SAMPLE_SHORT_KEY[] = { 'A', 'B', 'C', 'D', 'E', 'F' };

    static constexpr uint8_t SAMPLE_MESSAGE[] = { 'A', 'B', 'C', 'D', 'E', 'F',
                                                  'G', 'H', 'I', 'J', 'K', 'L' };
    static constexpr uint8_t EXPECTED_DIGEST_SAMPLE_MESSAGE[HMac::HMAC_DIGEST_SIZE] = {
        0x70, 0xb5, 0x81, 0x6f, 0xe8, 0x8b, 0x7b, 0x5c, 0xaa, 0x4f, 0x62,
        0x2b, 0xb7, 0x4d, 0x27, 0xb0, 0x82, 0xc4, 0x86, 0x82, 0x11, 0x3,
        0x34, 0x37, 0x52, 0x6a, 0x35, 0xb,  0x2,  0x3e, 0xc6, 0x7
    };

    static constexpr uint8_t LONG_MESSAGE[] =
        "hello worldhello worldhello worldhello worldhello worldhello worldhello worldhello "
        "worldhello worldhello worldhello worldhello worldhello worldhello worldhello world";
    static constexpr uint8_t EXPECTED_DIGEST_LONG_MESSAGE[HMac::HMAC_DIGEST_SIZE] = {
        0x71, 0x17, 0xb3, 0xf0, 0x7,  0x2b, 0x5f, 0x2b, 0x89, 0xad, 0x90,
        0xb9, 0xbd, 0xa9, 0x5,  0x1c, 0x78, 0x77, 0xb7, 0xb4, 0xf,  0x9f,
        0xae, 0xb9, 0x86, 0x2b, 0xf5, 0xc2, 0x78, 0x6c, 0x79, 0xc0
    };

    static constexpr uint8_t BLOCK_SIZED_MESSAGE[] =
        "oneblockoneblockoneblockoneblockoneblockoneblockoneblockonebloc";
    static_assert(sizeof(BLOCK_SIZED_MESSAGE) == Sha256::SHA256_BLOCK_SIZE);
    static constexpr uint8_t EXPECTED_DIGEST_BLOCK_SIZED_MESSAGE[HMac::HMAC_DIGEST_SIZE] = {
        0x7d, 0xa3, 0x36, 0xd,  0x7a, 0xc8, 0x12, 0xca, 0x78, 0xbf, 0x50,
        0xef, 0xd5, 0x1,  0x5c, 0x5d, 0x3,  0x4b, 0x61, 0xbe, 0x8f, 0xb2,
        0xe4, 0x36, 0x8,  0x4c, 0xb6, 0x28, 0x4d, 0xf2, 0xf1, 0x4d
    };

    // Initialized with string is \0 terminated
    static constexpr uint8_t UNPADDED_MESSAGE[] =
        "oneblockoneblockoneblockoneblockoneblockoneblockoneblo";
    static constexpr uint8_t EXPECTED_DIGEST_UNPADDED_MESSAGE[HMac::HMAC_DIGEST_SIZE] = {
        0x3b, 0x6a, 0xb,  0x56, 0xea, 0x9b, 0x1,  0x58, 0x11, 0x24, 0xe1,
        0x99, 0xe1, 0x23, 0x35, 0xa9, 0xb3, 0xc5, 0xa0, 0x5a, 0x27, 0x78,
        0xf7, 0x7a, 0x2f, 0x90, 0x35, 0xd3, 0xf6, 0x18, 0x56, 0xbb
    };

    static constexpr uint8_t PADDED_MESSAGE[] =
        "oneblockoneblockoneblockoneblockoneblockoneblockonebloc";
    static constexpr uint8_t EXPECTED_DIGEST_PADDED_MESSAGE[HMac::HMAC_DIGEST_SIZE] = {
        0x72, 0xf5, 0x3f, 0xd,  0xc3, 0xfd, 0xd,  0xba, 0x24, 0x40, 0x98,
        0x26, 0x2f, 0x1,  0xc6, 0x6e, 0x20, 0xd6, 0xbc, 0x28, 0x2b, 0xbf,
        0x9c, 0x49, 0x7e, 0x28, 0x6c, 0xfb, 0xcb, 0x5c, 0xae, 0xf1
    };

    uint8_t digestBuffer[HMac::HMAC_DIGEST_SIZE] = { 0 };
};

TEST_F(HMacTestSuite, SampleMessage)
{
    HMac::hmacSha256(
        SAMPLE_SHORT_KEY,
        sizeof(SAMPLE_SHORT_KEY),
        SAMPLE_MESSAGE,
        sizeof(SAMPLE_MESSAGE),
        digestBuffer);

    // for (std::size_t i = 0; i < HMac::HMAC_DIGEST_SIZE; i++) {
    //     std::cerr << std::hex << (unsigned)digestBuffer[i] << " ";
    // }
    ASSERT_EQ(memcmp(EXPECTED_DIGEST_SAMPLE_MESSAGE, digestBuffer, HMac::HMAC_DIGEST_SIZE), 0);
}

TEST_F(HMacTestSuite, LongMessage)
{
    HMac::hmacSha256(
        SAMPLE_SHORT_KEY,
        sizeof(SAMPLE_SHORT_KEY),
        LONG_MESSAGE,
        sizeof(LONG_MESSAGE),
        digestBuffer);

    ASSERT_EQ(memcmp(EXPECTED_DIGEST_LONG_MESSAGE, digestBuffer, HMac::HMAC_DIGEST_SIZE), 0);
}

TEST_F(HMacTestSuite, BlockSizedMessage)
{
    HMac::hmacSha256(
        SAMPLE_SHORT_KEY,
        sizeof(SAMPLE_SHORT_KEY),
        BLOCK_SIZED_MESSAGE,
        sizeof(BLOCK_SIZED_MESSAGE),
        digestBuffer);

    ASSERT_EQ(memcmp(EXPECTED_DIGEST_BLOCK_SIZED_MESSAGE, digestBuffer, HMac::HMAC_DIGEST_SIZE), 0);
}

TEST_F(HMacTestSuite, UnpaddedMessage)
{
    HMac::hmacSha256(
        SAMPLE_SHORT_KEY,
        sizeof(SAMPLE_SHORT_KEY),
        UNPADDED_MESSAGE,
        sizeof(UNPADDED_MESSAGE),
        digestBuffer);

    ASSERT_EQ(memcmp(EXPECTED_DIGEST_UNPADDED_MESSAGE, digestBuffer, HMac::HMAC_DIGEST_SIZE), 0);
}

TEST_F(HMacTestSuite, PaddedMessage)
{
    HMac::hmacSha256(
        SAMPLE_SHORT_KEY,
        sizeof(SAMPLE_SHORT_KEY),
        PADDED_MESSAGE,
        sizeof(PADDED_MESSAGE),
        digestBuffer);

    ASSERT_EQ(memcmp(EXPECTED_DIGEST_PADDED_MESSAGE, digestBuffer, HMac::HMAC_DIGEST_SIZE), 0);
}

TEST_F(HMacTestSuite, SampleMessageManualUsage)
{
    // There is only 1 block, so we can immediately finalize
    HMacContext context = HMac::createContextForBlockwiseHMacSha256Transformation(
        SAMPLE_SHORT_KEY, sizeof(SAMPLE_SHORT_KEY));
    HMac::hmacSha256FinaliseLastBlock(
        context, SAMPLE_MESSAGE, sizeof(SAMPLE_MESSAGE), digestBuffer);
    ASSERT_EQ(memcmp(EXPECTED_DIGEST_SAMPLE_MESSAGE, digestBuffer, Sha256::SHA256_DIGEST_SIZE), 0);
}

TEST_F(HMacTestSuite, LongMessageManualUsage)
{
    // There is only 1 block, so we can immediately finalize
    HMacContext context = HMac::createContextForBlockwiseHMacSha256Transformation(
        SAMPLE_SHORT_KEY, sizeof(SAMPLE_SHORT_KEY));

    int remainingLength = sizeof(LONG_MESSAGE);
    int blockIndex = 0;
    while (remainingLength >= static_cast<int>(Sha256::SHA256_BLOCK_SIZE)) {
        HMac::hmacSha256TransformBlock(context, &LONG_MESSAGE[blockIndex]);
        blockIndex += Sha256::SHA256_BLOCK_SIZE;
        remainingLength -= Sha256::SHA256_BLOCK_SIZE;
    }

    HMac::hmacSha256FinaliseLastBlock(
        context, &LONG_MESSAGE[blockIndex], remainingLength, digestBuffer);
    ASSERT_EQ(memcmp(EXPECTED_DIGEST_LONG_MESSAGE, digestBuffer, Sha256::SHA256_DIGEST_SIZE), 0);
}

}
