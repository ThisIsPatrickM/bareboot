#include "microcontroller/metadata_interface.h"

namespace bootloader {

constexpr uint32_t MAX_IMAGE_LENGTH = 0xFFFF; // TODO: Dunno what is a good value for generic

const GlobalImageMetadata* MetadataInterface::getGlobalImageMetadata()
{
    return m_globalImageMetadata;
}

size_t MetadataInterface::updatePreferredImage(size_t imageIndex)
{
    if (imageIndex >= NUMBER_OF_IMAGES) {
        return m_globalImageMetadata->preferredImage;
    }
    m_globalImageMetadata->preferredImage = imageIndex;
    return m_globalImageMetadata->preferredImage;
}

uint32_t MetadataInterface::updateImageVersion(uint32_t version, size_t imageIndex)
{
    if (imageIndex >= NUMBER_OF_IMAGES) {
        return m_globalImageMetadata->images[imageIndex].version;
    }
    m_globalImageMetadata->images[imageIndex].version = version;
    return m_globalImageMetadata->images[imageIndex].version;
}

uint32_t MetadataInterface::updateImageCrc(uint32_t crc, size_t imageIndex)
{
    if (imageIndex >= NUMBER_OF_IMAGES) {
        return m_globalImageMetadata->images[imageIndex].crc;
    }
    m_globalImageMetadata->images[imageIndex].version = crc;
    return m_globalImageMetadata->images[imageIndex].crc;
}

bool MetadataInterface::updateImageComplete(bool complete, size_t imageIndex)
{
    if (imageIndex >= NUMBER_OF_IMAGES) {
        return m_globalImageMetadata->images[imageIndex].complete;
    }
    m_globalImageMetadata->images[imageIndex].complete = complete;
    return m_globalImageMetadata->images[imageIndex].complete;
}

bool MetadataInterface::updateImageAlwaysKeep(bool alwaysKeep, size_t imageIndex)
{
    if (imageIndex >= NUMBER_OF_IMAGES) {
        return m_globalImageMetadata->images[imageIndex].alwaysKeep;
    }
    m_globalImageMetadata->images[imageIndex].alwaysKeep = alwaysKeep;
    return m_globalImageMetadata->images[imageIndex].alwaysKeep;
}

uint32_t MetadataInterface::updateImageLength(uint32_t length, size_t imageIndex)
{
    if (imageIndex >= NUMBER_OF_IMAGES || length > MAX_IMAGE_LENGTH) {
        return m_globalImageMetadata->images[imageIndex].length;
    }
    m_globalImageMetadata->images[imageIndex].length = length;
    return m_globalImageMetadata->images[imageIndex].length;
}

}
