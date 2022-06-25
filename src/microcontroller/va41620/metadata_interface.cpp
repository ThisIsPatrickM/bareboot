#include "microcontroller/metadata_interface.h"

/* Controller specific includes */
#include "microcontroller/va41620/boot_rom_spi/boot_rom_spi.h"
#include "va41620/platform-parameters.h"

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
    *romProtection = 0x1;
    __asm__ volatile("" : : : "memory");
}

void Enable_Code_Memory_Protection()
{
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
    Enable_Code_Memory_Protection();
    m_globalImageMetadata->preferredImage = imageIndex;
    Disable_Code_Memory_Protection();
    return m_globalImageMetadata->preferredImage;
}

uint32_t MetadataInterface::updateGlobalBootcounter(uint32_t bootcounter)
{
    // Update SPI
    bootRomSpi.updateGlobalBootcounterOverSpi(bootcounter);
    // Update code Memory
    Enable_Code_Memory_Protection();
    m_globalImageMetadata->globalBootcounter = bootcounter;
    Disable_Code_Memory_Protection();
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
    Enable_Code_Memory_Protection();
    m_globalImageMetadata->images[imageIndex].version = version;
    Disable_Code_Memory_Protection();
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
    Enable_Code_Memory_Protection();
    m_globalImageMetadata->images[imageIndex].crc = crc;
    Disable_Code_Memory_Protection();
    return m_globalImageMetadata->images[imageIndex].crc;
}

bool MetadataInterface::updateImageComplete(bool complete, size_t imageIndex)
{
    if (imageIndex >= PlatformParameters::NUMBER_OF_IMAGES) {
        return false;
    }
    // Update SPI
    bootRomSpi.updateImageCompleteOverSpi(complete, imageIndex);
    // Update code Memory
    Enable_Code_Memory_Protection();
    m_globalImageMetadata->images[imageIndex].complete = complete;
    Disable_Code_Memory_Protection();
    return m_globalImageMetadata->images[imageIndex].complete;
}

bool MetadataInterface::updateImageAlwaysKeep(bool alwaysKeep, size_t imageIndex)
{
    if (imageIndex >= PlatformParameters::NUMBER_OF_IMAGES) {
        return false;
    }
    // Update SPI
    bootRomSpi.updateImageAlwaysKeepOverSpi(alwaysKeep, imageIndex);
    // Update code Memory
    Enable_Code_Memory_Protection();
    m_globalImageMetadata->images[imageIndex].alwaysKeep = alwaysKeep;
    Disable_Code_Memory_Protection();
    return m_globalImageMetadata->images[imageIndex].alwaysKeep;
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
    Enable_Code_Memory_Protection();
    m_globalImageMetadata->images[imageIndex].length = length;
    Disable_Code_Memory_Protection();
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

void MetadataInterface::loadImage(void* destination, size_t imageIndex)
{
    Disable_Code_Memory_Protection();

    bootRomSpi.loadImage(
        destination,
        static_cast<int32_t>(m_globalImageMetadata->images[imageIndex].length),
        m_globalImageMetadata->images[imageIndex].imageBegin);

    Enable_Code_Memory_Protection();
}

}
