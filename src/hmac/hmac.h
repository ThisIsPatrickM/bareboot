#pragma once

#include "hash/sha_256/sha_256.h"

#include <cstdint>

namespace bootloader::hmac {
class HMAC {
private:
    static constexpr uint8_t HMAC_OUTER_PAD = 0x5c;
    static constexpr uint8_t HMAC_INNER_PAD = 0x36;

public:
    static constexpr std::size_t HMAC_KEY_LENGTH = 32;
    static constexpr std::size_t HMAC_DIGEST_LENGTH = 32;

    static void hmacSha256(
        uint8_t key[HMAC_KEY_LENGTH],
        std::size_t keyLength,
        const void* data,
        std::size_t dataLength,
        uint8_t hmac[HMAC_DIGEST_LENGTH]);

    static_assert(
        bootloader::hash::sha_256::Sha256::SHA256_DIGEST_SIZE == HMAC_DIGEST_LENGTH, "TODO");
};
}
