#pragma once

#include "memory_map.h"
#include "metadata.h"
#include "microcontroller/va41620/boot_rom_spi/spi_messages.h"
#include "microcontroller/va41620/hal/hal_spi.h"
#include "platform_parameters.h"

#include "memcpy/memcpy.h"

namespace bootloader::va41620::boot_rom_spi {

// TODO Get BOOTLOADER_OFFSET From somewhere else. Maybe sizeof(Interrupt Table)? or form Memory
// Map? __bootloader__
// TODO Sync with rodos boot_rom_spi
constexpr uint32_t METADATA_OFFSET = 0x350;
constexpr uint32_t METADATA_PREFERRED_IMAGE_OFFSET =
    METADATA_OFFSET + offsetof(GlobalImageMetadata, preferredImage);
constexpr uint32_t METADATA_CURRENT_IMAGE_OFFSET =
    METADATA_OFFSET + offsetof(GlobalImageMetadata, currentImage);
constexpr uint32_t METADATA_IMAGES_OFFSET = METADATA_OFFSET + offsetof(GlobalImageMetadata, images);

constexpr uint32_t METADATA_GLOBAL_BOOTCOUNTER_OFFSET =
    METADATA_OFFSET + offsetof(GlobalImageMetadata, globalBootcounter);

constexpr size_t SPI_RECEIVE_ADDRESSED_DATA_OFFSET = 4;
constexpr RODOS::SpiIdx BOOTSPI_IDX = RODOS::SPI_IDX3;
constexpr uint8_t SPI_STATUS_REGISTER_WRITE_ALL = 0b0100'0000;
constexpr uint32_t SPI_FREQUENCY = 1'000'000;

constexpr int32_t BUFFER_SIZE = 128;

// TODO Fix return types without dosis!

/**
 * @brief Interface class to interact with Non-Volatile-Memory over SPI to manage Metadata and
 * Images.
 *
 */
class BootRomSpi {
public:
    /**
     * @brief Get ImageMetadata from NVM over SPI and save it to globalImageMetadata.
     *
     * @param globalImageMetadata
     * @return void
     */
    void getBootRomGlobalImageMetadataOverSpi(GlobalImageMetadata& globalImageMetadata);

    /**
     * @brief Set the preferred Image for the next boot process.
     *
     * @param preferredImageIndex
     * @return void
     */
    void updatePreferredImageOverSpi(size_t preferredImageIndex);

    /**
     * @brief Update the current image in the Metadata over SPI
     *
     * @param currentImageIndex
     */
    void updateCurrentImageOverSpi(size_t currentImageIndex);

    /**
     * @brief Set the global bootcounter.
     *
     * @param bootcounter
     * @return void
     */
    void updateGlobalBootcounterOverSpi(uint32_t bootcounter);

    /**
     * @brief Set the global initialized status.
     *
     * @param initialized
     * @return void
     */
    void updateGlobalInitializedOverSpi(bool initialized);

    /**
     * @brief Set image version of selected image
     *
     * @param version
     * @param imageIndex Index of the image that should have new version.
     * @return void
     */
    void updateImageVersionOverSpi(uint32_t version, size_t imageIndex);

    /**
     * @brief Set image crc of selected image
     *
     * @param crc
     * @param imageIndex
     * @return void
     */
    void updateImageCrcOverSpi(uint32_t crc, size_t imageIndex);

    /**
     * @brief Set bootcounter of selected image
     *
     * @param bootcounter
     * @param imageIndex
     */
    void updateImageBootcounterOverSpi(uint32_t bootcounter, size_t imageIndex);

    /**
     * @brief Set image complete status of selected image
     *
     * @param completionStatus
     * @param imageIndex
     */
    void updateImageCompletionStatusOverSpi(CompletionStatus completionStatus, size_t imageIndex);

    /**
     * @brief Set image protectionStatus of selected image
     *
     * @param protectionStatus
     * @param imageIndex
     */
    void updateImageProtectionStatusOverSpi(ProtectionStatus protectionStatus, size_t imageIndex);

    /**
     * @brief Set image length of selected image
     *
     * @param length
     * @param imageIndex
     * @return Returns
     */
    void updateImageLengthOverSpi(uint32_t length, size_t imageIndex);

    /**
     * @brief Set imageBegin of selected image
     *
     * @param imageBegin
     * @param imageIndex
     * @return Returns
     */
    void updateImageBeginOverSpi(void* imageBegin, size_t imageIndex);

    uint8_t* getDataOverSpi(uintptr_t address, uint32_t length, uint8_t* receiveBuffer);

    void init();

    void copyImage(uintptr_t srcImagePointer, int32_t length, uintptr_t dstImagePointer);

    void updateImage(const void* data, int32_t length, uintptr_t imagePointer);

    void loadImage(void* destination, int32_t length, uintptr_t imagePointer);

private:
    void enableWriting();
    void disableWriting();
    static void putAddressOffsetIntoMessage(
        uint8_t address[SPI_ADDRESS_SPACE_BYTES], uint32_t addressOffset);

    RODOS::HalSpi m_halSpi { BOOTSPI_IDX };
};

}
