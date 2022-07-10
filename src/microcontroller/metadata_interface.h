#pragma once

/* Bootloader Includes */
#include "checksums/checksums.h"
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
    /**
     * @brief Construct a new Metadata Interface object. Sets pointer to m_globalImageMetadata based
     * on label __bootloader__.
     *
     */
    MetadataInterface();

    /**
     * @brief Get the Global Image Metadata object
     *
     * @return const GlobalImageMetadata*
     */
    [[nodiscard]] const GlobalImageMetadata* getGlobalImageMetadata();

    /**
     * @brief Sets the preferred image for next reboot.
     *
     * @param imageIndex
     * @return std::size_t Index of the image for next boot.
     */
    std::size_t updatePreferredImage(std::size_t imageIndex);

    /**
     * @brief Updates the current image in the metadata-
     *
     * @param imageIndex Image to set as current image
     * @return std::size_t Image in the metadata after the update operation
     */
    std::size_t updateCurrentImage(std::size_t imageIndex);

    /**
     * @brief Sets the preferred image for next reboot.
     *
     * @param bootcounter
     * @return std::size_t Index of the image for next boot.
     */
    uint32_t updateGlobalBootcounter(uint32_t bootcounter);

    /**
     * @brief Set image version of selected image
     *
     * @param version
     * @param imageIndex
     * @return uint32_t Version of selected image, 0 in case of wrong imageIndex
     */
    uint32_t updateImageVersion(uint32_t version, std::size_t imageIndex);

    /**
     * @brief Set image crc of selected image
     *
     * @param crc
     * @param imageIndex
     * @return uint32_t crc of selected image, 0 in case of wrong imageIndex
     */
    uint32_t updateImageCrc(uint32_t crc, std::size_t imageIndex);

    /**
     * @brief Set bootcounter of selected image
     *
     * @param bootcounter
     * @param imageIndex
     * @return uint32_t
     */
    uint32_t updateImageBootcounter(uint32_t bootcounter, std::size_t imageIndex);

    /**
     * @brief Set image CompletionStatus of selected image
     *
     * @param completionStatus
     * @param imageIndex
     * @return complete Status
     */
    CompletionStatus updateImageCompletionStatus(
        CompletionStatus completionStatus, std::size_t imageIndex);

    /**
     * @brief Set image ProtectionStatus status of selected image
     *
     * @param protectionStatus
     * @param imageIndex
     * @return alwaysKeep Status
     */
    ProtectionStatus updateImageProtectionStatus(
        ProtectionStatus protectionStatus, std::size_t imageIndex);

    /**
     * @brief Set image length of selected image
     *
     * @param length
     * @param imageIndex
     * @return uint32_t Length of the selected image
     */
    uint32_t updateImageLength(uint32_t length, std::size_t imageIndex);

    /**
     * @brief Get the Number of actual Images in the system. Depends on Hardware.
     *
     * @return std::size_t
     */
    static std::size_t getNumberOfImages();

    /**
     * @brief Get the Max Image Length object. Depends on hardware.
     *
     * @return std::size_t
     */
    static std::size_t getMaxImageLength();

    void init();

    void copyImage(std::size_t srcImageIndex, std::size_t dstImageIndex);

    void updateImage(
        const void* data, int32_t length, std::size_t imageIndex, uint32_t imageOffset);

    void loadImage(std::size_t imageIndex);

    bool verifyChecksum(std::size_t index);

private:
    GlobalImageMetadata* m_globalImageMetadata;
};

}
