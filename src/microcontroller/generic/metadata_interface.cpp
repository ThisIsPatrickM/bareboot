#include "microcontroller/metadata_interface.h"

/* Controller specific includes */
#include "generic/platform-parameters.h"
#include "rodos_includes/string_pico.h"

namespace bootloader {

MetadataInterface::MetadataInterface()
    : m_globalImageMetadata { reinterpret_cast<GlobalImageMetadata*>(__bootloader__) } // NOLINT
{
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

size_t MetadataInterface::getNumberOfImages()
{
    return PlatformParameters::NUMBER_OF_IMAGES;
}

size_t MetadataInterface::getMaxImageLength()
{
    return PlatformParameters::MAX_IMAGE_LENGTH;
}

void MetadataInterface::init() {}

void MetadataInterface::copyImage(size_t srcImageIndex, size_t dstImageIndex)
{
    void* sourceImagePointer =
        reinterpret_cast<void*>(m_globalImageMetadata->images[srcImageIndex].imageBegin); // NOLINT
    void* destinationImagePointer =
        reinterpret_cast<void*>(m_globalImageMetadata->images[dstImageIndex].imageBegin); // NOLINT
    size_t length = m_globalImageMetadata->images[srcImageIndex].length;

    if (sourceImagePointer == nullptr || destinationImagePointer == nullptr) {
        return;
    }

    rodos::memcpy(destinationImagePointer, sourceImagePointer, length);
}

void MetadataInterface::updateImage(
    const void* data, int32_t length, size_t imageIndex, uint32_t imageOffset)
{
    void* imagePointer = reinterpret_cast<void*>( // NOLINT
        m_globalImageMetadata->images[imageIndex].imageBegin + imageOffset);

    if (imagePointer == nullptr) {
        return;
    }
    // TODO Length check?

    rodos::memcpy(imagePointer, data, length);
}

void MetadataInterface::loadImage(void* destination, size_t imageIndex)
{
    void* source =
        reinterpret_cast<void*>(m_globalImageMetadata->images[imageIndex].imageBegin); // NOLINT
    if (source == nullptr) {
        return;
    }
    rodos::memcpy(destination, source, m_globalImageMetadata->images[imageIndex].length);
}

}
