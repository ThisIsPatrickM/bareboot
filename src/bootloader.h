#pragma once
#include "architecture/architecture_dependent.h"
#include "metadata.h"
#include "microcontroller/metadata_interface.h"
#include "microcontroller/microcontroller_dependent.h"

namespace bootloader {

class Bootloader {
public:
    void run();

private:
    int32_t selectImageSlot();
    int32_t selectBestGuessImageSlot();
    bool isImageValid(size_t index);
    static bool verifyChecksum(size_t index);
    void loadImage(size_t index);
    static void* memcpy(void* destP, const void* srcP, size_t len);

    MetadataInterface m_metadataInterface {};
};

} // namespace bootloader
