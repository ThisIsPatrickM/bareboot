#include "sha_256.h"

namespace bootloader::hash::sha_256 {

using namespace std;

void Sha256Context::reset()
{
    bitLength = 0;
    state[0] = DEFAULT_STATE[0];
    state[1] = DEFAULT_STATE[1];
    state[2] = DEFAULT_STATE[2];
    state[3] = DEFAULT_STATE[3];
    state[4] = DEFAULT_STATE[4];
    state[5] = DEFAULT_STATE[5];
    state[6] = DEFAULT_STATE[6];
    state[7] = DEFAULT_STATE[7];
}

void Sha256::sha256Transform(Sha256Context& context, const uint8_t block[SHA256_BLOCK_SIZE])
{
    uint32_t t1 = 0; // NOLINT(readability-identifier-length)
    uint32_t t2 = 0; // NOLINT(readability-identifier-length)
    uint32_t m[SHA256_BLOCK_SIZE] = { 0 }; // NOLINT(readability-identifier-length)
    // Often specified as a,b,c,d,e,f,g,h
    uint32_t roundState[8] = { 0 };

    for (uint32_t i = 0; i < 8; i++) {
        roundState[i] = context.state[i];
    }

    // Copy the state into 512-bits into m[0..15]
    for (uint32_t i = 0, j = 0; i < 16; ++i, j += 4) {
        // NOLINTNEXTLINE(cppcoreguidelines-pro-bounds-pointer-arithmetic)
        m[i] = (block[j] << 24) | (block[j + 1] << 16) | (block[j + 2] << 8) | (block[j + 3]);
    }
    // Fill m[16..63]
    for (uint32_t i = 16; i < SHA256_BLOCK_SIZE; ++i) {
        m[i] = sSig1(m[i - 2]) + m[i - 7] + sSig0(m[i - 15]) + m[i - 16];
    }

    // Main hash computation
    for (uint32_t i = 0; i < SHA256_BLOCK_SIZE; ++i) {
        t1 = roundState[7] + bSig1(roundState[4]) +
             choice(roundState[4], roundState[5], roundState[6]) + K[i] + m[i];
        t2 = bSig0(roundState[0]) + majority(roundState[0], roundState[1], roundState[2]);
        roundState[7] = roundState[6];
        roundState[6] = roundState[5];
        roundState[5] = roundState[4];
        roundState[4] = roundState[3] + t1;
        roundState[3] = roundState[2];
        roundState[2] = roundState[1];
        roundState[1] = roundState[0];
        roundState[0] = t1 + t2;
    }

    // Feedback
    for (uint32_t i = 0; i < 8; i++) {
        context.state[i] += roundState[i];
    }
    context.bitLength += SHA256_BLOCK_LENGTH_IN_BITS;
}

void Sha256::sha256(const uint8_t* data, size_t length, uint8_t hash[SHA256_DIGEST_SIZE])
{
    Sha256Context context {};
    int remainingLength = static_cast<int>(length);
    size_t blockIndex = 0;
    while (remainingLength >= static_cast<int>(SHA256_BLOCK_SIZE)) {
        // Handle all complete blocks
        sha256Transform(
            context, &data[blockIndex]); // NOLINT(cppcoreguidelines-pro-bounds-pointer-arithmetic)
        remainingLength -= SHA256_BLOCK_SIZE;
        blockIndex += SHA256_BLOCK_SIZE;
    }
    // Handle last block and return hash
    sha256Finalise(
        context,
        &data[blockIndex], // NOLINT(cppcoreguidelines-pro-bounds-pointer-arithmetic)
        remainingLength,
        hash);
}

void Sha256::sha256Finalise(
    Sha256Context& context,
    const uint8_t lastBlock[SHA256_BLOCK_SIZE],
    const size_t lastBlockLength,
    uint8_t hash[SHA256_DIGEST_SIZE])
{
    if (lastBlockLength > SHA256_BLOCK_SIZE) {
        // TODO Error handling
        return;
    }

    // Transform last data block ending with first bit set to 1 and following zeroes
    uint8_t temporaryLastBlock[SHA256_BLOCK_SIZE] = { 0 };
    RODOS::memcpy(temporaryLastBlock, lastBlock, lastBlockLength);
    temporaryLastBlock[lastBlockLength] = SHA256_PAD_BYTE; // Set first bit to 1

    // Decide if an additional block is needed for padding
    if (lastBlockLength >= SHA256_EXTRA_PADDING_BLOCK_THRESHHOLD) {
        // Additional block needed.
        sha256Transform(context, temporaryLastBlock);
        // Reinitialize temporary block for adding length
        for (size_t i = 0; i < SHA256_EXTRA_PADDING_BLOCK_THRESHHOLD; i++) {
            temporaryLastBlock[i] = 0;
        }
    }

    // Append padding to total messages length in bits and transform
    uint64_t calculatedBitLength = context.bitLength + lastBlockLength * 8;
    temporaryLastBlock[63] = calculatedBitLength; // NOLINT
    temporaryLastBlock[62] = calculatedBitLength >> 8; // NOLINT
    temporaryLastBlock[61] = calculatedBitLength >> 16; // NOLINT
    temporaryLastBlock[60] = calculatedBitLength >> 24; // NOLINT
    temporaryLastBlock[59] = calculatedBitLength >> 32; // NOLINT
    temporaryLastBlock[58] = calculatedBitLength >> 40; // NOLINT
    temporaryLastBlock[57] = calculatedBitLength >> 48; // NOLINT
    temporaryLastBlock[56] = calculatedBitLength >> 56; // NOLINT
    sha256Transform(context, temporaryLastBlock);

    // Since this implementation uses little endian byte ordering and SHA uses big endian,
    // reverse all the bytes when copying the final state to the output hash.
    // TODO Explain this magic
    // TODO Check byteorder like in memcpy
    for (int i = 0; i < 4; ++i) {
        hash[i] = (context.state[0] >> (24 - i * 8)) & 0x000000ff; // NOLINT
        hash[i + 4] = (context.state[1] >> (24 - i * 8)) & 0x000000ff; // NOLINT
        hash[i + 8] = (context.state[2] >> (24 - i * 8)) & 0x000000ff; // NOLINT
        hash[i + 12] = (context.state[3] >> (24 - i * 8)) & 0x000000ff; // NOLINT
        hash[i + 16] = (context.state[4] >> (24 - i * 8)) & 0x000000ff; // NOLINT
        hash[i + 20] = (context.state[5] >> (24 - i * 8)) & 0x000000ff; // NOLINT
        hash[i + 24] = (context.state[6] >> (24 - i * 8)) & 0x000000ff; // NOLINT
        hash[i + 28] = (context.state[7] >> (24 - i * 8)) & 0x000000ff; // NOLINT
    }
}

bool Sha256::sha256Verify(
    const uint8_t* data, size_t length, const uint8_t expectedHash[Sha256::SHA256_DIGEST_SIZE])
{
    uint8_t calculatedHash[Sha256::SHA256_DIGEST_SIZE] = { 0 };
    sha256(data, length, calculatedHash);
    for (size_t i = 0; i < Sha256::SHA256_DIGEST_SIZE; i++) {
        if (calculatedHash[i] != expectedHash[i]) { // NOLINT
            return false;
        }
    }
    return true;
}

inline uint32_t Sha256::rotateRight(uint32_t value, uint32_t rotation)
{
    return (value >> rotation) | (value << (32 - rotation));
}

inline uint32_t Sha256::shiftRight(uint32_t value, uint32_t shift)
{
    return (value >> shift);
}

inline uint32_t Sha256::choice(
    uint32_t x, uint32_t y, uint32_t z) // NOLINT(readability-identifier-length)
{
    return ((x) & (y)) ^ (~(x) & (z));
}

inline uint32_t Sha256::majority(
    uint32_t x, uint32_t y, uint32_t z) // NOLINT(readability-identifier-length)
{
    return ((x) & (y)) ^ ((x) & (z)) ^ ((y) & (z));
}

inline uint32_t Sha256::bSig0(uint32_t x) // NOLINT(readability-identifier-length)
{
    return rotateRight(x, 2) ^ rotateRight(x, 13) ^ rotateRight(x, 22);
}

inline uint32_t Sha256::bSig1(uint32_t x) // NOLINT(readability-identifier-length)
{
    return rotateRight(x, 6) ^ rotateRight(x, 11) ^ rotateRight(x, 25);
}

inline uint32_t Sha256::sSig0(uint32_t x) // NOLINT(readability-identifier-length)
{
    return rotateRight(x, 7) ^ rotateRight(x, 18) ^ shiftRight(x, 3);
}

inline uint32_t Sha256::sSig1(uint32_t x) // NOLINT(readability-identifier-length)
{
    return rotateRight(x, 17) ^ rotateRight(x, 19) ^ shiftRight(x, 10);
}

}
