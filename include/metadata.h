#pragma once

#include <cstdint>

namespace bootloader {

/**
 * @brief Enum to indicate if an image is Complete. Completion requires a correct crc and hmac.
 *
 */
enum class CompletionStatus : uint16_t { INCOMPLETE, COMPLETE };

/**
 * @brief Enum to create an additional protection layer before overwriting images.
 *
 */
enum class ProtectionStatus : uint16_t { UNPROTECTED, PROTECTED };

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
    /** @brief Version number of the image, only serves as user information */
    uint32_t version = 1;
    /** @brief CRC32 value of this image */
    uint32_t crc = 0;
    /** @brief Number of times this image was booted */
    uint32_t bootcounter = 0;
    /** @brief Equals the value of bootcounter, that was considered succcessful. I.e. if
     * lastSuccessStatus == bootcounter -1 means the last boot attempt had an error, while the the
     * boot attempt prior to the last one was successful. */
    uint32_t lastSuccessStatus = 0; // Equals bootcounter if image was fine the last time
    /** @brief Address to the beginning of an image. Has to be set in instantiation */
    const uintptr_t imageBegin = 0;
    /** @brief Length of this image. Initially set with initial-metadata script */
    uint32_t length = 0;
    /** @brief Indicates if the image is considered complete, aka currently not in update process
     * and crc and hmac were correct. */
    CompletionStatus completionStatus = CompletionStatus::INCOMPLETE;
    /** @brief Indicates if an image can be overwritten. */
    ProtectionStatus protectionStatus = ProtectionStatus::UNPROTECTED;
    /** @brief HMAC of this image */
    uint8_t hmacSignature[HMAC_DIGEST_LENGTH] = { 0 };
};

/**
 * @brief Describes metadata for all images, and contains metadata for each Image.
 *
 */
struct GlobalImageMetadata {
    /** @brief Incremented on every restart */
    uint32_t globalBootcounter = 0;
    /** @brief Index of the image, that will be tried first on restart. */
    std::size_t preferredImage = 0;
    /** @brief Index of the image that is currently running */
    std::size_t currentImage = 0;
    /** @brief Private Keys used for authentication */
    uint8_t hmacKey1[HMAC_KEY_LENGTH] = { 0 };
    uint8_t hmacKey2[HMAC_KEY_LENGTH] = { 0 };
    uint8_t hmacKey3[HMAC_KEY_LENGTH] = { 0 };
    /** @brief Metadata of each image. MAX_NUMBER_OF_IMAGES can be > than actual number of images
     * used */
    ImageMetadata images[MAX_NUMBER_OF_IMAGES] {};
};

} // namespace bootloader
