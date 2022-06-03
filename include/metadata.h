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
    void const* imageBegin = nullptr;
    uint32_t length = 6;
    bool complete = false;
    bool alwaysKeep = false;
    // TODO Think about alignment?
};

constexpr size_t NUMBER_OF_IMAGES = 3;
constexpr size_t HMAC_LENGTH = 64;

struct GlobalImageMetadata {
    uint32_t bootcounter = 1; // NOLINT
    ImageMetadata images[NUMBER_OF_IMAGES] = {};
    size_t preferredImage = 7;
    size_t currentImage = 8;
    // uint8_t hmac1[HMAC_LENGTH] = { 0 };
    // uint8_t hmac2[HMAC_LENGTH] = { 0 };
    // uint8_t hmac3[HMAC_LENGTH] = { 0 };
};
// struct Bootloader {
//     // bool checkCRC(size_t index, uint32_t crc);
//     // void updatePrefferedImage(size_t index);
//     // void updateCurrentImage(size_t index);
//     // void updateVersion(uint32_t version, size_t imageIndex);
//     // void updateCRC(uint32_t ccrc, size_t imageIndex);
//     // void updateBootcounter(uint32_t counter, size_t imageIndex);
//     // void updateSuccessCounter(uint32_t counter, size_t imageIndex);
//     // void updateLastSuccessStatus(uint32_t status, size_t imageIndex);
//     // void updateImageComplete(bool status, size_t imageIndex);
//     // void updateImageAlwaysKeep(bool status, size_t imageIndex);

//     GlobalImageMetadata globalImageMetadata = {};
// };

} // namespace bootloader
