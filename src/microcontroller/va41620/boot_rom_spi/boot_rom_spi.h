#pragma once

#include "memory_map.h"
#include "metadata.h"
#include "microcontroller/va41620/boot_rom_spi/spi_messages.h"
#include "microcontroller/va41620/rodos_includes/hal/hal_spi.h"
#include "platform_parameters.h"

#include "memcpy/memcpy.h"

namespace bootloader::va41620::boot_rom_spi {

/**
 * @brief Offset where GlobalImageMetadata begins. Fixed by __bootloader__ label at link time and
 * placed right after Interupt Vector
 */
constexpr uint32_t METADATA_OFFSET = 0x350;
constexpr uint32_t METADATA_GLOBAL_BOOTCOUNTER_OFFSET =
    METADATA_OFFSET + offsetof(GlobalImageMetadata, globalBootcounter);
constexpr uint32_t METADATA_PREFERRED_IMAGE_OFFSET =
    METADATA_OFFSET + offsetof(GlobalImageMetadata, preferredImage);
constexpr uint32_t METADATA_CURRENT_IMAGE_OFFSET =
    METADATA_OFFSET + offsetof(GlobalImageMetadata, currentImage);
constexpr uint32_t METADATA_IMAGES_OFFSET = METADATA_OFFSET + offsetof(GlobalImageMetadata, images);

/**
 * @brief Number of bytes to skip when reading addressed data over SPI.
 */
constexpr size_t SPI_RECEIVE_ADDRESSED_DATA_OFFSET = 4;

constexpr RODOS::SpiIdx BOOTSPI_IDX = RODOS::SPI_IDX3;

/**
 * @brief Removes all Memory Protection
 */
constexpr uint8_t SPI_STATUS_REGISTER_WRITE_ALL = 0b0100'0000;
constexpr uint32_t SPI_FREQUENCY = 1'000'000;

/**
 * @brief Buffer size for Fragmentation
 */
constexpr int32_t BUFFER_SIZE = 128;

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
     * @brief Set bootcounter of selected image
     *
     * @param bootcounter
     * @param imageIndex
     */
    void updateImageBootcounterOverSpi(uint32_t bootcounter, size_t imageIndex);

    /**
     * @brief Get the length number of bytes over spi from address and writes them to receiveBuffer.
     * @note Receivebuffer needs to be able to hold length+4 elements, because 4 dummy bytes are
     * received.
     *
     * @param address Address to read from
     * @param length Number of bytes to receive
     * @param receiveBuffer Pointer where the data is written to. Needs to hold length+4 elements
     * @return uint8_t* Points into receiveBuffer where the data begins
     */
    uint8_t* getDataOverSpi(uintptr_t address, uint32_t length, uint8_t* receiveBuffer);

    /**
     * @brief Initializes the underlying HAL_SPI Interface. Needs to be called once.
     *
     */
    void init();

    /**
     * @brief Load image from imagePointer to destination with length number of bytes.
     *
     * @param destination
     * @param length
     * @param imagePointer
     */
    void loadImage(void* destination, int32_t length, uintptr_t imagePointer);

private:
    /**
     * @brief Enables writing over SPI. Needs to be send before every write message.
     *
     */
    void enableWriting();

    /**
     * @brief Disables writing over SPI.
     *
     */
    void disableWriting();

    /**
     * @brief Puts the address in the BOOT_ROM into 3 bytes.
     *
     * @param address
     * @param addressOffset
     */
    static void putAddressOffsetIntoMessage(
        uint8_t address[SPI_ADDRESS_SPACE_BYTES], uint32_t addressOffset);

    RODOS::HalSpi m_halSpi { BOOTSPI_IDX };
};

}
