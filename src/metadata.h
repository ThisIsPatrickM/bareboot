#pragma once

#include <cstdint>

namespace bootloader {

using namespace std;

struct ImageMetadata {
    uint32_t version = 1;
    uint32_t crc = 2;
    uint32_t bootCounter = 3;
    uint32_t successCounter = 4;
    uint32_t lastSuccessStatus = 5;
    bool complete = false;
    bool alwaysKeep = false;
    const void* imageBegin = nullptr;
    uint32_t length = 6;
};

constexpr size_t NUMBER_OF_IMAGES = 1;
constexpr size_t HMAC_LENGTH = 64;

struct GlobalImageMetadata {
    ImageMetadata images[NUMBER_OF_IMAGES] = {};
    size_t preferredImage = 7;
    size_t currentImage = 8;
    // uint8_t hmac1[HMAC_LENGTH] = { 0 };
    // uint8_t hmac2[HMAC_LENGTH] = { 0 };
    // uint8_t hmac3[HMAC_LENGTH] = { 0 };
    uint32_t bootcounter = 19; // NOLINT
};

} // namespace bootloader