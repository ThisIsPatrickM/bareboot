#pragma once

#include "architecture/architecture_dependent.h"
#include "checksums/castagnoli_crc32/castagnoli_crc32.h"
#include "metadata.h"
#include "microcontroller/metadata_interface.h"
#include "microcontroller/microcontroller_dependent.h"

/* RODOS includes */
#include "rodos_includes/string_pico.h"

#include <cstddef>

namespace bootloader {

/**
 * @brief Class to run the logic to select, verify and manage booting images.
 *
 */
class Bootloader {
public:
    /**
     * @brief Based on underlying Metadata selects an image and boots it.
     *
     */
    void run();

private:
    /**
     * @brief First stage Image selection based on preferred image and last image.
     *
     * @return int32_t Index of the image to use or -1 in case of error.
     */
    int32_t selectImageSlot();

    /**
     * @brief Second stage Image selection if first stage failed.
     *
     * @return int32_t Index of the image to use or -1 in case of error.
     */
    int32_t selectBestGuessImageSlot();

    /**
     * @brief Image is valid if it has a valid checksum and is considered complete.
     *
     * @param index
     * @return true
     * @return false
     */
    bool isImageValid(size_t index);

    /**
     * @brief Verifies Checksum of the given image
     *
     * @param index
     * @return true
     * @return false
     */
    bool verifyChecksum(size_t index);

    /**
     * @brief Load Image to to Application Memory
     *
     * @param index
     */
    void loadImage(size_t index);
    // static void* memcpy(void* destP, const void* srcP, size_t len);

    MetadataInterface m_metadataInterface {};
};

} // namespace bootloader
