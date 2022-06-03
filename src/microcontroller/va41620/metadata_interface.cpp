#include "microcontroller/metadata_interface.h"

/* Controller specific includes */
#include "microcontroller/va41620/boot_rom_spi.h"

namespace bootloader {

/*******************************************************************************
 * Controller Specific
 ******************************************************************************/

BootRomSpi bootRomSpi {};
constexpr uint32_t ROM_PROT_ADDRESS = 0x40010010;
uint32_t* romProtection = reinterpret_cast<uint32_t*>(ROM_PROT_ADDRESS); // NOLINT

void Enable_Code_Memory_Protection()
{
    *romProtection = 0x1;
}

void Disable_Code_Memory_Protection()
{
    *romProtection = 0x0;
}

/*******************************************************************************
 * MetadataInterface Implementations
 ******************************************************************************/

const GlobalImageMetadata* MetadataInterface::getGlobalImageMetadata()
{
    // TODO When to read? Always?
    bootRomSpi.getBootRomGlobalImageMetadataOverSpi(*m_globalImageMetadata);
    return m_globalImageMetadata;
}

size_t MetadataInterface::updatePreferredImage(size_t imageIndex)
{
    if (imageIndex >= NUMBER_OF_IMAGES) {
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

uint32_t MetadataInterface::updateImageVersion(uint32_t version, size_t imageIndex)
{
    if (imageIndex >= NUMBER_OF_IMAGES) {
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
    if (imageIndex >= NUMBER_OF_IMAGES) {
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
    if (imageIndex >= NUMBER_OF_IMAGES) {
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
    if (imageIndex >= NUMBER_OF_IMAGES) {
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
    if (imageIndex >= NUMBER_OF_IMAGES || length >= MAX_IMAGE_LENGTH) {
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

}