#pragma once

/* Bootloader Includes */
#include "checksums/crc32.h"
#include "memory_map.h"
#include "metadata.h"

namespace bootloader {

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
     * @brief Construct a new Metadata Interface object
     *
     * @param globalImageMetadata Pointer to the GlobalImageMetadata
     */
    MetadataInterface(GlobalImageMetadata* globalImageMetadata);

    /**
     * @brief Get the Global Image Metadata object
     *
     * @return const GlobalImageMetadata*
     */
    [[nodiscard]] const GlobalImageMetadata* getGlobalImageMetadata();

    /**
     * @brief Updates the current image in the metadata
     *
     * @param imageIndex Image to set as current image
     * @return std::size_t Image in the metadata after the update operation
     */
    std::size_t updateCurrentImage(std::size_t imageIndex);

    /**
     * @brief Update the global bootcounter
     *
     * @param bootcounter
     * @return uint32_t The bootcounter after the update
     */
    uint32_t updateGlobalBootcounter(uint32_t bootcounter);

    /**
     * @brief Set bootcounter of selected image
     *
     * @param bootcounter
     * @param imageIndex
     * @return uint32_t The new bootcounter of the image, 0 in case of an wrong input
     */
    uint32_t updateImageBootcounter(uint32_t bootcounter, std::size_t imageIndex);

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

    /**
     * @brief Calls any initialization functions if required.
     * I.e. VA41620 needs to initialize SPI Interface
     */
    void init();

    /**
     * @brief Load image from Index to dedicated, executable memory location
     *
     * @param imageIndex
     */
    void loadImage(std::size_t imageIndex);

    /**
     * @brief Verify checksum from given image.
     *
     * @param index
     * @return true
     * @return false
     */
    bool verifyChecksum(std::size_t index);

private:
    GlobalImageMetadata* m_globalImageMetadata;

    static constexpr unsigned LOAD_RETRIES = 3;
};

}
