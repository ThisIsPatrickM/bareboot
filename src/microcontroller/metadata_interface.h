#pragma once

/* Bootloader Includes */
#include "memory_map.h"
#include "metadata.h"

namespace bootloader {

// TODO Sync some Method names of Bootloader and Bootmanager

/**
 * @brief Provides interfaces to Image Metadata and functionality to make changes.
 * Implementation is hardware dependent.
 */

class MetadataInterface {
public:
    MetadataInterface() = default;

    [[nodiscard]] const GlobalImageMetadata* getGlobalImageMetadata();

    /**
     * @brief Sets the preferred image for next reboot.
     *
     * @param imageIndex
     * @return size_t Index of the image for next boot.
     */
    size_t updatePreferredImage(size_t imageIndex);

    /**
     * @brief Sets the preferred image for next reboot.
     *
     * @param bootcounter
     * @return size_t Index of the image for next boot.
     */
    uint32_t updateGlobalBootcounter(uint32_t bootcounter);

    /**
     * @brief Set image version of selected image
     *
     * @param version
     * @param imageIndex
     * @return uint32_t Version of selected image, 0 in case of wrong imageIndex
     */
    uint32_t updateImageVersion(uint32_t version, size_t imageIndex);

    /**
     * @brief Set image crc of selected image
     *
     * @param crc
     * @param imageIndex
     * @return uint32_t crc of selected image, 0 in case of wrong imageIndex
     */
    uint32_t updateImageCrc(uint32_t crc, size_t imageIndex);

    /**
     * @brief Set image Complete status of selected image
     *
     * @param complete
     * @param imageIndex
     * @return complete Status
     */
    bool updateImageComplete(bool complete, size_t imageIndex);

    /**
     * @brief Set image alwaysKeep status of selected image
     *
     * @param complete
     * @param imageIndex
     * @return alwaysKeep Status
     */
    bool updateImageAlwaysKeep(bool alwaysKeep, size_t imageIndex);

    /**
     * @brief Set image length of selected image
     *
     * @param length
     * @param imageIndex
     * @return uint32_t Length of the selected image
     */
    uint32_t updateImageLength(uint32_t length, size_t imageIndex);

    /**
     * @brief Get the Number of actual Images in the system. Depends on Hardware.
     *
     * @return size_t
     */
    static size_t getNumberOfImages();

    /**
     * @brief Get the Max Image Length object. Depends on hardware.
     *
     * @return size_t
     */
    static size_t getMaxImageLength();

private:
    GlobalImageMetadata* m_globalImageMetadata =
        reinterpret_cast<GlobalImageMetadata*>(__bootloader__); // NOLINT
};

}
