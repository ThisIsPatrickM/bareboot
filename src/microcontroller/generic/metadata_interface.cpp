#include "microcontroller/metadata_interface.h"

/* Controller specific includes */
#include "generic/platform-parameters.h"

namespace bootloader {

MetadataInterface::MetadataInterface() {}

void MetadataInterface::init()
{
    // Reset imageBegin
    // for (size_t i = 0; i < PlatformParameters::NUMBER_OF_IMAGES; i++) {
    //     updateImageBegin(reinterpret_cast<Image*>(IMAGE_BEGIN_ADDRESSES[i]), i);
    // }

    // Reset imageLength, But where do I get the value?

    // Or where do I get the Initial CRC value? Maybe prepend Infos at Image Slot?

    // Init complete?

    // Init alwaysKeep?
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
    m_globalImageMetadata->preferredImage = imageIndex;
    return m_globalImageMetadata->preferredImage;
}

bool MetadataInterface::updateGlobalInitialized(bool initialized)
{
    m_globalImageMetadata->initialized = initialized;
    return m_globalImageMetadata->initialized;
}

uint32_t MetadataInterface::updateGlobalBootcounter(uint32_t bootcounter)
{
    m_globalImageMetadata->globalBootcounter = bootcounter;
    return m_globalImageMetadata->globalBootcounter;
}

uint32_t MetadataInterface::updateImageVersion(uint32_t version, size_t imageIndex)
{
    if (imageIndex >= PlatformParameters::NUMBER_OF_IMAGES) {
        return m_globalImageMetadata->images[imageIndex].version;
    }
    m_globalImageMetadata->images[imageIndex].version = version;
    return m_globalImageMetadata->images[imageIndex].version;
}

uint32_t MetadataInterface::updateImageCrc(uint32_t crc, size_t imageIndex)
{
    if (imageIndex >= PlatformParameters::NUMBER_OF_IMAGES) {
        return m_globalImageMetadata->images[imageIndex].crc;
    }
    m_globalImageMetadata->images[imageIndex].version = crc;
    return m_globalImageMetadata->images[imageIndex].crc;
}

bool MetadataInterface::updateImageComplete(bool complete, size_t imageIndex)
{
    if (imageIndex >= PlatformParameters::NUMBER_OF_IMAGES) {
        return m_globalImageMetadata->images[imageIndex].complete;
    }
    m_globalImageMetadata->images[imageIndex].complete = complete;
    return m_globalImageMetadata->images[imageIndex].complete;
}

bool MetadataInterface::updateImageAlwaysKeep(bool alwaysKeep, size_t imageIndex)
{
    if (imageIndex >= PlatformParameters::NUMBER_OF_IMAGES) {
        return m_globalImageMetadata->images[imageIndex].alwaysKeep;
    }
    m_globalImageMetadata->images[imageIndex].alwaysKeep = alwaysKeep;
    return m_globalImageMetadata->images[imageIndex].alwaysKeep;
}

uint32_t MetadataInterface::updateImageLength(uint32_t length, size_t imageIndex)
{
    if (imageIndex >= PlatformParameters::NUMBER_OF_IMAGES ||
        length > PlatformParameters::MAX_IMAGE_LENGTH) {
        return m_globalImageMetadata->images[imageIndex].length;
    }
    m_globalImageMetadata->images[imageIndex].length = length;
    return m_globalImageMetadata->images[imageIndex].length;
}

void* MetadataInterface::updateImageBegin(void* imageBegin, size_t imageIndex)
{
    if (imageIndex >= PlatformParameters::NUMBER_OF_IMAGES) {
        return nullptr;
    }
    m_globalImageMetadata->images[imageIndex].imageBegin = imageBegin;
    return m_globalImageMetadata->images[imageIndex].imageBegin;
}

size_t MetadataInterface::getNumberOfImages()
{
    return PlatformParameters::NUMBER_OF_IMAGES;
}

size_t MetadataInterface::getMaxImageLength()
{
    return PlatformParameters::MAX_IMAGE_LENGTH;
}

}
