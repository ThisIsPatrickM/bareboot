#pragma once

#include <cstdint>

namespace bootloader {

using namespace std;

// struct Image {
//     size_t length = 14;
//     uint32_t crc = 15;
//     // Rest is data
// };

struct ImageMetadata {
    uint32_t version = 1;
    uint32_t crc = 2; // Prepend to Image Slot? No Calculate it the first time with Init over
    // full length?
    uint32_t bootCounter = 0;
    uint32_t successCounter = 4;
    uint32_t lastSuccessStatus = 5;
    void* imageBegin = nullptr; // TODO maybe uintptr_t?
    uint32_t length = 6; // Prepend to Image Slot ?
    bool complete = false;
    bool alwaysKeep = false;
};

// Depending on the size of the memory, less images might be used. So this number is higher than the
// actual number, and amount of metadata is low.
constexpr size_t MAX_NUMBER_OF_IMAGES = 10;
constexpr size_t HMAC_LENGTH = 64;

struct GlobalImageMetadata {
    uint32_t globalBootcounter = 0;
    size_t preferredImage = 7;
    size_t currentImage = 8;
    bool initialized = false;
    ImageMetadata images[MAX_NUMBER_OF_IMAGES] = {};
    // uint8_t hmac1[HMAC_LENGTH] = { 0 };
    // uint8_t hmac2[HMAC_LENGTH] = { 0 };
    // uint8_t hmac3[HMAC_LENGTH] = { 0 };
};

} // namespace bootloader
