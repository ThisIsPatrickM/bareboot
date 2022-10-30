#include "microcontroller/metadata_interface.h"

/* Controller specific includes */
#include "architecture/architecture_dependent.h"
#include "microcontroller/va41620/boot_rom_spi/boot_rom_spi.h"
#include "microcontroller/va41620/rodos_includes/peripheral_ctrl/peripheral_defs.h"
#include "platform_parameters.h"

namespace bootloader {

/*******************************************************************************
 * Controller Specific
 ******************************************************************************/

using namespace bootloader::va41620::boot_rom_spi;
using namespace bootloader::architecture;
BootRomSpi bootRomSpi {};

inline void Memory_Barrier()
{
    __asm__ volatile("" : : : "memory");
}

void Disable_Code_Memory_Protection()
{
    Memory_Barrier();
    RODOS::SYSCONFIG->ROM_PROT.write(RODOS::SYSCONFIG_ROM_PROT::WREN(1));
    Memory_Barrier();
}

void Enable_Code_Memory_Protection()
{
    Memory_Barrier();
    RODOS::SYSCONFIG->ROM_PROT.write(RODOS::SYSCONFIG_ROM_PROT::WREN(0));
    Memory_Barrier();
}

/*******************************************************************************
 * MetadataInterface Implementations
 ******************************************************************************/

MetadataInterface::MetadataInterface()
    : m_globalImageMetadata { reinterpret_cast<GlobalImageMetadata*>(&__bootloader__) } // NOLINT
{
}

MetadataInterface::MetadataInterface(GlobalImageMetadata* globalImageMetadata)
    : m_globalImageMetadata(globalImageMetadata)
{
}

void MetadataInterface::init() // NOLINT(readability-convert-member-functions-to-static)
{
    bootRomSpi.init();
}

const GlobalImageMetadata* MetadataInterface::getGlobalImageMetadata()
{
    return m_globalImageMetadata;
}

size_t MetadataInterface::updateCurrentImage(size_t imageIndex)
{
    if (imageIndex >= PlatformParameters::NUMBER_OF_IMAGES) {
        return m_globalImageMetadata->currentImage;
    }
    // Update SPI
    bootRomSpi.updateCurrentImageOverSpi(imageIndex);
    // Update code Memory
    Disable_Code_Memory_Protection();
    m_globalImageMetadata->currentImage = imageIndex;
    Enable_Code_Memory_Protection();
    return m_globalImageMetadata->currentImage;
}

uint32_t MetadataInterface::updateGlobalBootcounter(uint32_t bootcounter)
{
    // Update SPI
    bootRomSpi.updateGlobalBootcounterOverSpi(bootcounter);
    // Update code Memory
    Disable_Code_Memory_Protection();
    m_globalImageMetadata->globalBootcounter = bootcounter;
    Enable_Code_Memory_Protection();
    return m_globalImageMetadata->globalBootcounter;
}

uint32_t MetadataInterface::updateImageBootcounter(uint32_t bootcounter, size_t imageIndex)
{
    if (imageIndex >= PlatformParameters::NUMBER_OF_IMAGES) {
        return 0;
    }
    // Update SPI
    bootRomSpi.updateImageBootcounterOverSpi(bootcounter, imageIndex);
    // Update code Memory
    Disable_Code_Memory_Protection();

    m_globalImageMetadata->images[imageIndex].bootcounter = bootcounter;
    Enable_Code_Memory_Protection();
    return m_globalImageMetadata->images[imageIndex].bootcounter;
}

size_t MetadataInterface::getNumberOfImages()
{
    return PlatformParameters::NUMBER_OF_IMAGES;
}

size_t MetadataInterface::getMaxImageLength()
{
    return PlatformParameters::MAX_IMAGE_LENGTH;
}

void MetadataInterface::loadImage(size_t imageIndex)
{
    if (imageIndex >= MetadataInterface::getNumberOfImages()) {
        return;
    }

    if (reinterpret_cast<uintptr_t>(&__approm_start__) == // NOLINT
        m_globalImageMetadata->images[imageIndex].imageBegin) {
        return;
    }

    if (m_globalImageMetadata->images[imageIndex].imageBegin == 0) {
        return;
    }

    void* destination = reinterpret_cast<void*>(&__approm_start__); // NOLINT

    for (unsigned i = 0; i < LOAD_RETRIES; i++) {
        // Load
        Disable_Code_Memory_Protection();
        bootRomSpi.loadImage(
            destination,
            static_cast<int32_t>(m_globalImageMetadata->images[imageIndex].length),
            m_globalImageMetadata->images[imageIndex].imageBegin);
        Enable_Code_Memory_Protection();

        // Verify
        if (checksums::Crc32::calculateCRC32(
                static_cast<const uint8_t*>(destination),
                m_globalImageMetadata->images[imageIndex].length) ==
            m_globalImageMetadata->images[imageIndex].crc) {
            return;
        }
    }

    // TODO Reset System, load image failed 3 times in a row!
}

bool MetadataInterface::verifyChecksum(size_t index)
{
    if (index >= MetadataInterface::getNumberOfImages()) {
        return false;
    }

    if (m_globalImageMetadata->images[index].imageBegin == 0) {
        return false;
    }

    uint32_t expectedChecksum = m_globalImageMetadata->images[index].crc;

    uintptr_t currentData = m_globalImageMetadata->images[index].imageBegin;
    uint8_t buffer[BUFFER_SIZE + SPI_RECEIVE_ADDRESSED_DATA_OFFSET] = { 0 };
    auto remainingLength = static_cast<int32_t>(m_globalImageMetadata->images[index].length);
    uint32_t iterativeChecksum = checksums::Crc32::CRC_INITIAL_VALUE;

    while (remainingLength > 0) {
        uint32_t fragmentSize = remainingLength > BUFFER_SIZE ? BUFFER_SIZE : remainingLength;

        uint8_t* localDataBeginPtr = bootRomSpi.getDataOverSpi(currentData, fragmentSize, buffer);
        iterativeChecksum = checksums::Crc32::calculateCRC32Step(
            localDataBeginPtr, fragmentSize, iterativeChecksum);
        remainingLength -= static_cast<int32_t>(fragmentSize);
        currentData += fragmentSize;
    }

    return expectedChecksum == (iterativeChecksum ^ checksums::Crc32::CRC_FINAL_XOR_VALUE);
}

}
