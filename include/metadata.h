#pragma once

#include <cstdint>

namespace bootloader {

enum class CompletionStatus : uint16_t { COMPLETE, INCOMPLETE };

enum class ProtectionStatus : uint16_t { PROTECTED, UNPROTECTED };

/**
 * @brief Number of metadata images to allocate. Real number can be further reduced depending on
 * microcontroller, its memory size and the expected size of images. This is just the upper limit.
 *
 */
constexpr std::size_t MAX_NUMBER_OF_IMAGES = 7;

/**
 * @brief Number of bytes for an HMAC.
 *
 */
constexpr std::size_t HMAC_KEY_LENGTH = 32;
constexpr std::size_t HMAC_DIGEST_LENGTH = 32;

/**
 * @brief Describe Metadata information for one Image.
 *
 */
struct ImageMetadata {
    uint32_t version = 1;
    uint32_t crc = 0; // Initially set when building binary
    uint32_t bootcounter = 0;
    uint32_t successCounter = 14;
    uint32_t lastSuccessStatus = 0;
    const uintptr_t imageBegin = 0; // Initially set during Initialization
    uint32_t length = 0; // Initially set when building binary
    CompletionStatus completionStatus = CompletionStatus::INCOMPLETE;
    ProtectionStatus protectionStatus = ProtectionStatus::UNPROTECTED;
    // TODO use uint32_t, because vorago cannot read uint8 and boolean ?
};

/**
 * @brief Describes metadata for all images, and contains metadata for each Image.
 *
 */
struct GlobalImageMetadata {
    uint32_t globalBootcounter = 0;
    std::size_t preferredImage = 0;
    std::size_t currentImage = 0;
    uint8_t hmacKey1[HMAC_KEY_LENGTH] = { 0 };
    ImageMetadata images[MAX_NUMBER_OF_IMAGES] {};
    // uint8_t hmacKey2[HMAC_KEY_LENGTH] = { 0 };
    // uint8_t hmacKey3[HMAC_KEY_LENGTH] = { 0 };
    // TODO Document every variable
};

} // namespace bootloader
