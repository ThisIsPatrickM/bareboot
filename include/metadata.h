#pragma once

#include <cstdint>

namespace bootloader {

using namespace std;

/**
 * @brief Number of metadata images to allocate. Real number can be further reduced depending on
 * microcontroller, its memory size and the expected size of images. This is just the upper limit.
 *
 */
constexpr size_t MAX_NUMBER_OF_IMAGES = 10;

/**
 * @brief Number of bytes for an HMAC.
 *
 */
constexpr size_t HMAC_LENGTH = 64;

/**
 * @brief Describe Metadata information for one Image.
 *
 */
struct ImageMetadata {
    uint32_t version = 1;
    uint32_t crc = 0;
    uint32_t bootCounter = 0;
    uint32_t successCounter = 4;
    uint32_t lastSuccessStatus = 5;
    const uintptr_t imageBegin = 0; // TODO maybe uintptr_t? Yes for Initialization at compiletime

    uint32_t length = 0;
    bool complete = false;
    bool alwaysKeep = false;
};

/**
 * @brief Describes metadata for all images, and contains metadata for each Image.
 *
 */
struct GlobalImageMetadata {
    uint32_t globalBootcounter = 0;
    size_t preferredImage = 7;
    size_t currentImage = 8;
    ImageMetadata images[MAX_NUMBER_OF_IMAGES] = {};
    // uint8_t hmac1[HMAC_LENGTH] = { 0 };
    // uint8_t hmac2[HMAC_LENGTH] = { 0 };
    // uint8_t hmac3[HMAC_LENGTH] = { 0 };
};

} // namespace bootloader
