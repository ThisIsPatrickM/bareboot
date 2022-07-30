#pragma once

#include "hash/sha_256/sha_256.h"

#include <cstdint>

namespace bootloader::hmac {

/**
 * @brief Contains all required information to continue calculating HMAC when data is not all
 * available in one chunk.
 *
 */
struct HMacContext {
    /**
     * @brief Will hold the Key XOR Padding for the outer hashing step, until computation is
     * complete
     *
     */
    uint8_t keyedOuterPad[hash::sha_256::Sha256::SHA256_BLOCK_SIZE] = { 0 };
    /**
     * @brief Contains context of used hashing algorithm
     *
     */
    hash::sha_256::Sha256Context shaContext {};
};

/**
 * @brief Class Providing functionality to compute HMAC using SHA256.
 * Keylength: Recommendation is 32 bytes.
 *
 * Usage:
 * - Either compute full HMAC-SHA256 using call to hmacSha256(..)
 * - Or process data blockwise by calling createContextForBlockwiseHMacSha256Transformation(),
 * hmacSha256TransformBlock multiple times and  hmacSha256FinaliseLastBlock with the last block.
 * Blocks must be of size SHA256_BLOCK_SIZE (64 bytes) except the last block.
 *
 *
 */
class HMac {
private:
    static constexpr uint8_t HMAC_OUTER_PAD = 0x5c;
    static constexpr uint8_t HMAC_INNER_PAD = 0x36;

public:
    static constexpr std::size_t HMAC_DIGEST_SIZE = 32;

    /**
     * @brief Computes HMAC using SHA256 algortihm and puts result into @param hmac.
     *
     * @param key The key to use
     * @param keyLength Length of the key.
     * @param data The data that will be used to compute message authentication code.
     * @param dataLength Length of the data
     * @param hmac Output parameter that will contain the result.
     */
    static void hmacSha256(
        const uint8_t key[],
        std::size_t keyLength,
        const void* data,
        std::size_t dataLength,
        uint8_t hmac[HMAC_DIGEST_SIZE]);

    /**
     * @brief Create a Context for blockwise HMAC computation.
     *
     * @param key
     * @param keyLength
     * @return HMacContext
     */
    static HMacContext createContextForBlockwiseHMacSha256Transformation(
        const uint8_t key[], std::size_t keyLength);

    /**
     * @brief Adds a single block to blockwise HMAC computation. Fragmentation has to be done by
     * calleer. Blocks have to be of size SHA256_BLOCK_SIZE(64 bytes)
     *
     * @param context Context previously created by
     * createContextForBlockwiseHMacSha256Transformation
     * @param block Data block to include into HMAC Computation
     */
    static void hmacSha256TransformBlock(
        HMacContext& context, const uint8_t block[hash::sha_256::Sha256::SHA256_BLOCK_SIZE]);

    /**
     * @brief Finalise blockwise HMAC Computation. Hast to be called with the last block that is
     * supposed to be included in the message authenticaiton code.
     * Size of the last block only has to be <= SHA256_BLOCK_SIZE(64 bytes). Padding is taken care
     * of
     *
     * @param context
     * @param lastBlock
     * @param lastBlockLength
     * @param hmac
     */
    static void hmacSha256FinaliseLastBlock(
        HMacContext& context,
        const uint8_t lastBlock[hash::sha_256::Sha256::SHA256_BLOCK_SIZE],
        std::size_t lastBlockLength,
        uint8_t hmac[HMAC_DIGEST_SIZE]);
};
}
