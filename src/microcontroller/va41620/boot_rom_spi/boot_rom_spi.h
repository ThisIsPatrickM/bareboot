#pragma once

#include "memory_map.h"
#include "metadata.h"
#include "microcontroller/va41620/hal/hal_spi.h"

/* rodos includes */
#include "rodos_includes/bit_field.h"
#include "rodos_includes/string_pico.h" // For memcpy

namespace bootloader {

// TODO Get BOOTLOADER_OFFSET From somewhere else. Maybe sizeof(Interrupt Table)? or form Memory
// Map? __bootloader__
constexpr uint32_t METADATA_OFFSET = 0x350;
constexpr uint32_t METADATA_PREFERRED_IMAGE_OFFSET =
    METADATA_OFFSET + offsetof(GlobalImageMetadata, preferredImage);
constexpr uint32_t METADATA_IMAGES_OFFSET = METADATA_OFFSET + offsetof(GlobalImageMetadata, images);

constexpr size_t SPI_ADDRESS_SPACE_BYTES = 3;
constexpr size_t SPI_RECEIVE_ADDRESSED_DATA_OFFSET = 4;
constexpr rodos::SpiIdx BOOTSPI_IDX = rodos::SPI_IDX3;
constexpr uint8_t SPI_STATUS_REGISTER_WRITE_ALL = 0b0100'0000;
constexpr uint32_t SPI_FREQUENCY = 1'000'000;

// TODO Calculate MAX Image size
constexpr uint32_t MAX_IMAGE_LENGTH = 0xFFFF;

/**
 * @brief Expected OpCodes that are used during SPI communication to identify the desired operation.
 *
 */
enum class SpiOpCodes : uint8_t {
    WREN = 0x06,
    WRDI = 0x04,
    RDSR = 0x05,
    WRSR = 0x01,
    READ = 0x03,
    FSTRD = 0x0B,
    WRITE = 0x02,
    SLEEP = 0xB9,
    RDID = 0x9F
};

/**
 * @brief Status Register to manage Writing and Protecting Memory over SPI.
 * - WriteEnable: 1 = Write Enable
 * - BlockProtect combinations:
 * (BP0, BP1)   | Protected Addresses
 * (0,0)        | None
 * (0,1)        | 0x30000 to 0x3FFFF
 * (1,0)        | 0x20000 to 0x3FFFF
 * (1,1)        | 0x00000 to 0x3FFFF
 *
 *  - WriteProtectEnable:
 * Used to enable the function of Write Protect Pin (NVM_PROTn)
 * WEL WPEN NVM_PROTn (Protected Blocks)   (Unprotected Blocks) (Status Register)
 * 0   X    X          Protected           Protected               Protected
 * 1   0    X          Protected           Unprotected             Unprotected
 * 1   1    0          Protected           Unprotected             Protected
 * 1   1    1          Protected           Unprotected             Unprotected
 *
 * Default value: 0b11001100 or 204 in decimal.
 *
 * @TODO Check byteorder. Maybe I'm reading it the wrong way. I could write with 0b01000000
 */
using SpiStatusRegister = rodos::UInt8BitField;
using SpiStatusDontCare = rodos::UInt8SubValue<0, 1>;
using SpiStatusWriteEnable = rodos::UInt8SubValue<1, 1>;
using SpiStatusBlockProtect0 = rodos::UInt8SubValue<2, 1>;
using SpiStatusBlockProtect1 = rodos::UInt8SubValue<3, 1>;
using SpiStatusDontCare2 = rodos::UInt8SubValue<4, 3>;
using SpiStatusWriteProtectEnable = rodos::UInt8SubValue<7, 1>;

/**
 * @brief Reads data over SPI from the given 18bit address.
 *
 */
struct [[gnu::packed]] SpiRead {
    const uint8_t opcode = static_cast<uint8_t>(SpiOpCodes::READ);
    uint8_t address18bit[SPI_ADDRESS_SPACE_BYTES] = { 0 };
};

/**
 * @brief The FSTRD opcode is similar to the READ opcode except that a ‘dummy’ byte must be issued
 * between the three bytes of address and the first data byte being driven out from the NVM.
 * Subsequent data bytes will follow the first byte as in a READ operation. The NVM internal
 * address is incremented automatically as long as the NVM chip-enable (ROM_SS) pin is held
 * low and the rising edge of ROM_SS will terminate the read operation.
 *
 */
struct [[gnu::packed]] SpiFastRead {
    const uint8_t opcode = static_cast<uint8_t>(SpiOpCodes::FSTRD);
    uint8_t address18bit[SPI_ADDRESS_SPACE_BYTES] = { 0 };
    uint8_t dummyByte = { 0 };
};

/**
 * @brief Reads the Status Register that manages Write Protoction. Returns 1 byte
 *
 */
struct [[gnu::packed]] SpiReadStatusRegister {
    const uint8_t opcode = static_cast<uint8_t>(SpiOpCodes::RDSR);
};

/**
 * @brief Enable Writing Status Register and Memory. Needs to be send before both of them.
 *
 */
struct [[gnu::packed]] SpiWriteEnable {
    const uint8_t opcode = static_cast<uint8_t>(SpiOpCodes::WREN);
};

/**
 * @brief Attempt to write to Status Register to Write/Protect Memory.
 *
 */
struct [[gnu::packed]] SpiWriteStatusRegister {
    const uint8_t opcode = static_cast<uint8_t>(SpiOpCodes::WRSR);
    // WEL and not WPEN enabled
    // Default  is 204.
    uint8_t statusRegister = 0;
    // TODO Maybe close write again
};

/**
 * @brief Attempt to write data to the given 18bit address
 *
 * @tparam DATA_LENGTH Length of the data to write.
 */
template <size_t DATA_LENGTH>
struct [[gnu::packed]] SpiWrite {
    const uint8_t opcode = static_cast<uint8_t>(SpiOpCodes::WRITE);
    uint8_t address18bit[SPI_ADDRESS_SPACE_BYTES] = { 0 };
    uint8_t data[DATA_LENGTH] = {};
    // TODO test this further, but its complicated
    // uint32_t padding : 14 = 0;
    // uint32_t a : 18 = 15;
};

/**
 * @brief Reset Write Enable status
 *
 */
struct [[gnu::packed]] SpiResetWriteEnable {
    const uint8_t opcode = static_cast<uint8_t>(SpiOpCodes::WRDI);
};

/**
 * @brief RDIDT he first six bytes will read 0x7F. The seventh byte
 * will return the manufacturer ID, 0xC2. The eighth and ninth bytes will return the two-byte
 * product ID, 0x2508.
 *
 */
struct [[gnu::packed]] SpiDeviceIdRegister {
    const uint8_t opcode = static_cast<uint8_t>(SpiOpCodes::RDID);
};

/**
 * @brief The NVM can be put into a low-power mode by issuing the SLEEP opcode. The SLEEP
 * command is a single byte (opcode only). While is SLEEP, the ROM_SCK and ROM_MOSI
 * pins are ignored, but the NVM continues to monitor the state of the ROM_SS pin. On the
 * next falling edge of ROM_SS, the NVM device will wake up from SLEEP and return to normal
 * operation
 *
 */
struct [[gnu::packed]] SpiSleep {
    const uint8_t opcode = static_cast<uint8_t>(SpiOpCodes::SLEEP);
};

/**
 * @brief Interface class to interact with Non-Volatile-Memory over SPI to manage Metadata and
 * Images.
 *
 */
class BootRomSpi {
public:
    // TODO Fix return types without dosis!
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
     * @return void Returns < 0 in case of errors
     */
    void updatePreferredImageOverSpi(size_t preferredImageIndex);

    /**
     * @brief Set image version of selected image
     *
     * @param version
     * @param imageIndex Index of the image that should have new version.
     * @return void Returns < 0 in case of errors
     */
    void updateImageVersionOverSpi(uint32_t version, size_t imageIndex);

    /**
     * @brief Set image crc of selected image
     *
     * @param crc
     * @param imageIndex
     * @return void Returns < 0 in case of errors
     */
    void updateImageCrcOverSpi(uint32_t crc, size_t imageIndex);

    /**
     * @brief Set image complete status of selected image
     *
     * @param size
     * @param imageIndex
     * @return void Returns < 0 in case of errors
     */
    void updateImageCompleteOverSpi(bool complete, size_t imageIndex);

    /**
     * @brief Set image alwaysKeep status of selected image
     *
     * @param complete
     * @param imageIndex
     * @return Returns < 0 in case of errors
     */
    void updateImageAlwaysKeepOverSpi(bool alwaysKeep, size_t imageIndex);

    /**
     * @brief Set image length of selected image
     *
     * @param length
     * @param imageIndex
     * @return Returns < 0 in case of errors
     */
    void updateImageLengthOverSpi(uint32_t length, size_t imageIndex);

    void init();

private:
    void enableWriting();
    void disableWriting();
    static void putAddressOffsetInto18bits(
        uint8_t address[SPI_ADDRESS_SPACE_BYTES], uint32_t addressOffset);

    rodos::HalSpi m_halSpi { BOOTSPI_IDX };
};

}
