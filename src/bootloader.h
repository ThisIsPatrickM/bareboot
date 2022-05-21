#pragma once

#include "metadata.h"

namespace bootloader {

using namespace std;

const struct Bootloader {
public:
    // bool checkCRC(size_t index, uint32_t crc);
    void updatePrefferedImage(size_t index);
    void updateCurrentImage(size_t index);
    void updateVersion(uint32_t version, size_t imageIndex);
    void updateCRC(uint32_t crc, size_t imageIndex);
    void updateBootcounter(uint32_t counter, size_t imageIndex);
    void updateSuccessCounter(uint32_t counter, size_t imageIndex);
    void updateLastSuccessStatus(uint32_t status, size_t imageIndex);
    void updateImageComplete(bool status, size_t imageIndex);
    void updateImageAlwaysKeep(bool status, size_t imageIndex);

    GlobalImageMetadata& getGlobalImageMetadata();

private:
    GlobalImageMetadata m_globalImageMetadata = {};
} BL_TABLE __attribute__((used, section(".bootloader")));

} // namespace bootloader