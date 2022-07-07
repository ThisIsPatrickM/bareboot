#pragma once

#include "architecture/architecture_dependent.h"
#include "metadata.h"
#include "microcontroller/metadata_interface.h"
#include "microcontroller/microcontroller_dependent.h"

/* RODOS includes */
#include "memcpy/memcpy.h"

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
     * @return size_t Index of the image to use
     */
    size_t selectImageSlot();

    /**
     * @brief Second stage Image selection if first stage failed.
     *
     * @return size_t Index of the image to use
     */
    size_t selectBestGuessImageSlot();

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

    MetadataInterface m_metadataInterface {};
};

} // namespace bootloader
