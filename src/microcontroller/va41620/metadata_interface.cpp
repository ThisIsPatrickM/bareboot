#include "microcontroller/metadata_interface.h"

/* Controller specific includes */
#include "microcontroller/va41620/boot_rom_spi/boot_rom_spi.h"
#include "va41620/platform-parameters.h"

namespace bootloader {

/*******************************************************************************
 * Controller Specific
 ******************************************************************************/

constexpr uintptr_t ROM_PROT_ADDRESS = 0x40010010;

using namespace bootloader::va41620::boot_rom_spi;
BootRomSpi bootRomSpi {};
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

MetadataInterface::MetadataInterface() {}

const GlobalImageMetadata* MetadataInterface::getGlobalImageMetadata()
{
    // TODO When to read? Always?
    bootRomSpi.getBootRomGlobalImageMetadataOverSpi(*m_globalImageMetadata);
    return m_globalImageMetadata;
}

void MetadataInterface::init()
{
    // Reset imageBegin
    for (size_t i = 0; i < NUMBER_OF_IMAGES; i++) {
        updateImageBegin(reinterpret_cast<void*>(IMAGE_BEGIN_ADDRESSES[i]), i); // NOLINT
    }
    // TODO
    // Reset imageLength, But where do I get the value?

    // Or where do I get the Initial CRC value? Maybe prepend Infos at Image Slot?

    // Init complete?

    // Init alwaysKeep?
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

bool MetadataInterface::updateGlobalInitialized(bool initialized)
{
    // Update SPI
    bootRomSpi.updateGlobalInitializedOverSpi(initialized);
    // Update code Memory
    Enable_Code_Memory_Protection();
    m_globalImageMetadata->initialized = initialized;
    Disable_Code_Memory_Protection();
    return m_globalImageMetadata->initialized;
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

void* MetadataInterface::updateImageBegin(void* imageBegin, size_t imageIndex)
{
    if (imageIndex >= NUMBER_OF_IMAGES) {
        return nullptr;
    }
    // Update SPI
    bootRomSpi.updateImageBeginOverSpi(imageBegin, imageIndex);
    // Update code Memory
    Enable_Code_Memory_Protection();
    m_globalImageMetadata->images[imageIndex].imageBegin = imageBegin;
    Disable_Code_Memory_Protection();
    return m_globalImageMetadata->images[imageIndex].imageBegin;
}

size_t MetadataInterface::getNumberOfImages()
{
    return NUMBER_OF_IMAGES;
}

size_t MetadataInterface::getMaxImageLength()
{
    return MAX_IMAGE_LENGTH;
}

}
