#pragma once

#include <cstdint>

#include "memcpy/memcpy.h"

namespace bootloader::hash::sha_256 {

/**
 * @brief Class to hold the intermediate state of the Sha256 hashing algorithm
 *
 */
struct Sha256Context {
    static constexpr uint32_t DEFAULT_STATE[8] = { 0x6a09e667, 0xbb67ae85, 0x3c6ef372, 0xa54ff53a,
                                                   0x510e527f, 0x9b05688c, 0x1f83d9ab, 0x5be0cd19 };

    uint64_t bitLength = 0;
    uint32_t state[8] = { DEFAULT_STATE[0], DEFAULT_STATE[1], DEFAULT_STATE[2], DEFAULT_STATE[3],
                          DEFAULT_STATE[4], DEFAULT_STATE[5], DEFAULT_STATE[6], DEFAULT_STATE[7] };

    /**
     * @brief Reset the state of the context to the initial values. Has to be called if the same
     * context should be used when a new Hash should be calculated.
     *
     */
    void reset();
};

class Sha256 {
    // https://www.rfc-editor.org/rfc/rfc4634#page-6
    // https://github.com/B-Con/crypto-algorithms/blob/master/sha256.c
    // https://github.com/h5p9sl/hmac_sha256/blob/master/sha256.c
    static constexpr uint8_t SHA256_PAD_BYTE = 0x80;
    static constexpr std::size_t SHA256_EXTRA_PADDING_BLOCK_THRESHHOLD = 56;

public:
    static constexpr uint64_t SHA256_BLOCK_LENGTH_IN_BITS = 512;
    static constexpr std::size_t SHA256_BLOCK_SIZE = SHA256_BLOCK_LENGTH_IN_BITS / 8;
    static constexpr std::size_t SHA256_DIGEST_SIZE = 32;

    /**
     * @brief Computes the Sha256 Hash and writes result to hash.
     *
     * @param data Pointer to the data that should be hashed.
     * @param length Length of the data.
     * @param hash Output parameter that contains the resulting hash.
     */
    static void sha256(const uint8_t* data, std::size_t length, uint8_t hash[SHA256_DIGEST_SIZE]);

    /**
     * @brief Computes the transformation step for one Block
     *
     * @param context Context of the Hashing process
     * @param block Datablock that will be hashed. has a fixed size of SHA256_BLOCK_SIZE
     */
    static void sha256Transform(Sha256Context& context, const uint8_t block[SHA256_BLOCK_SIZE]);

    /**
     * @brief Last step of Sha256 Hashing Algorithm. Adds additional block for padding if required,
     * otherwise just pads message with the first bit set to 1 and length. Writes resulting hash
     * into hash parameter.
     *
     * @param context Context of previous hashing transformations
     * @param lastBlock last data block that should be hashed
     * @param lastBlockLength Length of the data in the last block. Has to be < SHA256_BLOCK_SIZE
     * @param hash Where the resulting hash will be written to.
     */
    static void sha256Finalise(
        Sha256Context& context,
        const uint8_t lastBlock[SHA256_BLOCK_SIZE],
        std::size_t lastBlockLength,
        uint8_t hash[SHA256_DIGEST_SIZE]);

    /**
     * @brief Verifies if the given data and length produce the same hash as expected hash.
     *
     * @param data
     * @param length
     * @param expectedHash
     * @return true
     * @return false
     */
    static bool sha256Verify(
        const uint8_t* data, std::size_t length, const uint8_t expectedHash[SHA256_DIGEST_SIZE]);

private:
    inline static uint32_t rotateRight(uint32_t value, uint32_t rotation);
    inline static uint32_t shiftRight(uint32_t value, uint32_t shift);
    inline static uint32_t choice(
        uint32_t x, uint32_t y, uint32_t z); // NOLINT(readability-identifier-length)
    inline static uint32_t majority(
        uint32_t x, uint32_t y, uint32_t z); // NOLINT(readability-identifier-length)
    inline static uint32_t bSig0(uint32_t x); // NOLINT(readability-identifier-length)
    inline static uint32_t bSig1(uint32_t x); // NOLINT(readability-identifier-length)
    inline static uint32_t sSig0(uint32_t x); // NOLINT(readability-identifier-length)
    inline static uint32_t sSig1(uint32_t x); // NOLINT(readability-identifier-length)

    //
    /**
     * @brief  Array of constants (first 32 bits of the fractional parts of the cube roots of the
     * first 64 prime numbers
     *
     */
    static constexpr uint32_t K[64] = { // NOLINT(readability-identifier-length)
        0x428a2f98, 0x71374491, 0xb5c0fbcf, 0xe9b5dba5, 0x3956c25b, 0x59f111f1, 0x923f82a4,
        0xab1c5ed5, 0xd807aa98, 0x12835b01, 0x243185be, 0x550c7dc3, 0x72be5d74, 0x80deb1fe,
        0x9bdc06a7, 0xc19bf174, 0xe49b69c1, 0xefbe4786, 0x0fc19dc6, 0x240ca1cc, 0x2de92c6f,
        0x4a7484aa, 0x5cb0a9dc, 0x76f988da, 0x983e5152, 0xa831c66d, 0xb00327c8, 0xbf597fc7,
        0xc6e00bf3, 0xd5a79147, 0x06ca6351, 0x14292967, 0x27b70a85, 0x2e1b2138, 0x4d2c6dfc,
        0x53380d13, 0x650a7354, 0x766a0abb, 0x81c2c92e, 0x92722c85, 0xa2bfe8a1, 0xa81a664b,
        0xc24b8b70, 0xc76c51a3, 0xd192e819, 0xd6990624, 0xf40e3585, 0x106aa070, 0x19a4c116,
        0x1e376c08, 0x2748774c, 0x34b0bcb5, 0x391c0cb3, 0x4ed8aa4a, 0x5b9cca4f, 0x682e6ff3,
        0x748f82ee, 0x78a5636f, 0x84c87814, 0x8cc70208, 0x90befffa, 0xa4506ceb, 0xbef9a3f7,
        0xc67178f2
    };
};

}
