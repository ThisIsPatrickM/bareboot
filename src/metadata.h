#pragma once

#include <cstdint>

namespace bootloader {

using namespace std;

struct ImageMetadata {
    uint32_t version = 0;
    uint32_t crc = 0;
    uint32_t bootCounter = 0;
    uint32_t successCounter = 0;
    uint32_t lastSuccessStatus = 0;
    bool complete = false;
    bool alwaysKeep = false;
    const void* imageBegin = nullptr;
    uint32_t length = 0;
};

constexpr size_t NUMBER_OF_IMAGES = 5;
constexpr size_t HMAC_LENGTH = 64;

struct GlobalImageMetadata {
    ImageMetadata images[NUMBER_OF_IMAGES] = {};
    size_t preferredImage = 0;
    size_t currentImage = 0;
    uint8_t hmac1[HMAC_LENGTH] = { 0 };
    uint8_t hmac2[HMAC_LENGTH] = { 0 };
    uint8_t hmac3[HMAC_LENGTH] = { 0 };
};

} // namespace bootloader