#include "microcontroller/metadata_interface.h"

/* Controller specific includes */
#include "microcontroller/va41620/boot_rom_spi/boot_rom_spi.h"
#include "platform_parameters.h"

namespace bootloader {

/*******************************************************************************
 * Controller Specific
 ******************************************************************************/

using namespace bootloader::va41620::boot_rom_spi;
BootRomSpi bootRomSpi {};
uint32_t* romProtection =
    reinterpret_cast<uint32_t*>(PlatformParameters::ROM_PROT_ADDRESS); // NOLINT

void Disable_Code_Memory_Protection()
{
    __asm__ volatile("" : : : "memory");
    *romProtection = 0x1;
    __asm__ volatile("" : : : "memory");
}

void Enable_Code_Memory_Protection()
{
    __asm__ volatile("" : : : "memory");
    *romProtection = 0x0;
    __asm__ volatile("" : : : "memory");
}

/*******************************************************************************
 * MetadataInterface Implementations
 ******************************************************************************/

MetadataInterface::MetadataInterface()
    : m_globalImageMetadata { reinterpret_cast<GlobalImageMetadata*>(&__bootloader__) } // NOLINT
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

size_t MetadataInterface::updatePreferredImage(size_t imageIndex)
{
    if (imageIndex >= PlatformParameters::NUMBER_OF_IMAGES) {
        return m_globalImageMetadata->preferredImage;
    }
    // Update SPI
    bootRomSpi.updatePreferredImageOverSpi(imageIndex);
    // Update code Memory
    Disable_Code_Memory_Protection();
    m_globalImageMetadata->preferredImage = imageIndex;
    Enable_Code_Memory_Protection();
    return m_globalImageMetadata->preferredImage;
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

uint32_t MetadataInterface::updateImageVersion(uint32_t version, size_t imageIndex)
{
    if (imageIndex >= PlatformParameters::NUMBER_OF_IMAGES) {
        return 0;
    }
    // Update SPI
    bootRomSpi.updateImageVersionOverSpi(version, imageIndex);
    // Update code Memory
    Disable_Code_Memory_Protection();
    m_globalImageMetadata->images[imageIndex].version = version;
    Enable_Code_Memory_Protection();
    return m_globalImageMetadata->images[imageIndex].version;
}

uint32_t MetadataInterface::updateImageCrc(uint32_t crc, size_t imageIndex)
{
    if (imageIndex >= PlatformParameters::NUMBER_OF_IMAGES) {
        return 0;
    }
    // Update SPI
    bootRomSpi.updateImageCrcOverSpi(crc, imageIndex);
    // Update code Memory
    Disable_Code_Memory_Protection();
    m_globalImageMetadata->images[imageIndex].crc = crc;
    Enable_Code_Memory_Protection();
    return m_globalImageMetadata->images[imageIndex].crc;
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

CompletionStatus MetadataInterface::updateImageCompletionStatus(
    CompletionStatus completionStatus, size_t imageIndex)
{
    if (imageIndex >= PlatformParameters::NUMBER_OF_IMAGES) {
        return m_globalImageMetadata->images[imageIndex].completionStatus;
    }
    // Update SPI
    bootRomSpi.updateImageCompletionStatusOverSpi(completionStatus, imageIndex);
    // Update code Memory
    Disable_Code_Memory_Protection();
    m_globalImageMetadata->images[imageIndex].completionStatus = completionStatus;
    Enable_Code_Memory_Protection();
    return m_globalImageMetadata->images[imageIndex].completionStatus;
}

ProtectionStatus MetadataInterface::updateImageProtectionStatus(
    ProtectionStatus protectionStatus, size_t imageIndex)
{
    if (imageIndex >= PlatformParameters::NUMBER_OF_IMAGES) {
        return m_globalImageMetadata->images[imageIndex].protectionStatus;
    }
    // Update SPI
    bootRomSpi.updateImageProtectionStatusOverSpi(protectionStatus, imageIndex);
    // Update code Memory
    Disable_Code_Memory_Protection();
    m_globalImageMetadata->images[imageIndex].protectionStatus = protectionStatus;
    Enable_Code_Memory_Protection();
    return m_globalImageMetadata->images[imageIndex].protectionStatus;
}

uint32_t MetadataInterface::updateImageLength(uint32_t length, size_t imageIndex)
{
    if (imageIndex >= PlatformParameters::NUMBER_OF_IMAGES ||
        length >= PlatformParameters::MAX_IMAGE_LENGTH) {
        return 0;
    }
    // Update SPI
    bootRomSpi.updateImageLengthOverSpi(length, imageIndex);
    // Update code Memory
    Disable_Code_Memory_Protection();
    m_globalImageMetadata->images[imageIndex].length = length;
    Enable_Code_Memory_Protection();
    return m_globalImageMetadata->images[imageIndex].length;
}

size_t MetadataInterface::getNumberOfImages()
{
    return PlatformParameters::NUMBER_OF_IMAGES;
}

size_t MetadataInterface::getMaxImageLength()
{
    return PlatformParameters::MAX_IMAGE_LENGTH;
}

void MetadataInterface::copyImage(size_t srcImageIndex, size_t dstImageIndex)
{
    uintptr_t sourceImagePointer = m_globalImageMetadata->images[srcImageIndex].imageBegin;
    uintptr_t destinationImagePointer = m_globalImageMetadata->images[dstImageIndex].imageBegin;
    auto length = static_cast<int32_t>(m_globalImageMetadata->images[srcImageIndex].length);

    if (sourceImagePointer == 0 || destinationImagePointer == 0) {
        return;
    }

    if (srcImageIndex == dstImageIndex) {
        return;
    }
    bootRomSpi.copyImage(sourceImagePointer, length, destinationImagePointer);
}

void MetadataInterface::updateImage(
    const void* data, int32_t length, size_t imageIndex, uint32_t imageOffset)
{
    uintptr_t imagePointer = m_globalImageMetadata->images[imageIndex].imageBegin;
    imagePointer += imageOffset;

    bootRomSpi.updateImage(data, length, imagePointer);
}

void MetadataInterface::loadImage(size_t imageIndex)
{
    if (imageIndex >= MetadataInterface::getNumberOfImages()) {
        return;
    }

    if (reinterpret_cast<uintptr_t>(__approm_start__) == // NOLINT
        m_globalImageMetadata->images[imageIndex].imageBegin) {
        // Nothing to do
        return;
    }

    if (m_globalImageMetadata->images[imageIndex].imageBegin == 0) {
        // TODO Error Handling
        return;
    }

    void* destination = reinterpret_cast<void*>(&__approm_start__); // NOLINT
    Disable_Code_Memory_Protection();

    bootRomSpi.loadImage(
        destination,
        static_cast<int32_t>(m_globalImageMetadata->images[imageIndex].length),
        m_globalImageMetadata->images[imageIndex].imageBegin);

    Enable_Code_Memory_Protection();
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
    uint32_t iterativeChecksum = checksums::Checksums::CRC_INITIAL_VALUE;

    while (remainingLength > 0) {
        uint32_t fragmentSize = remainingLength > BUFFER_SIZE ? BUFFER_SIZE : remainingLength;

        uint8_t* localDataBeginPtr = bootRomSpi.getDataOverSpi(currentData, fragmentSize, buffer);
        iterativeChecksum = checksums::Checksums::calculateIterativeCrc32NoTable(
            localDataBeginPtr, fragmentSize, iterativeChecksum);
        remainingLength -= static_cast<int32_t>(fragmentSize);
        currentData += fragmentSize;
    }

    return expectedChecksum == (iterativeChecksum ^ checksums::Checksums::CRC_FINAL_XOR_VALUE);
}

}
