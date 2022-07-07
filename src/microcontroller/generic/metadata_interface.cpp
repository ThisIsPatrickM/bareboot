#include "microcontroller/metadata_interface.h"

/* Controller specific includes */
#include "memcpy/memcpy.h"
#include "platform_parameters.h"

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

size_t MetadataInterface::updateCurrentImage(size_t imageIndex)
{
    if (imageIndex >= PlatformParameters::NUMBER_OF_IMAGES) {
        return m_globalImageMetadata->currentImage;
    }
    m_globalImageMetadata->currentImage = imageIndex;
    return m_globalImageMetadata->currentImage;
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

CompletionStatus MetadataInterface::updateImageCompletionStatus(
    CompletionStatus completionStatus, size_t imageIndex)
{
    if (imageIndex >= PlatformParameters::NUMBER_OF_IMAGES) {
        return m_globalImageMetadata->images[imageIndex].completionStatus;
    }
    m_globalImageMetadata->images[imageIndex].completionStatus = completionStatus;
    return m_globalImageMetadata->images[imageIndex].completionStatus;
}

ProtectionStatus MetadataInterface::updateImageProtectionStatus(
    ProtectionStatus protectionStatus, size_t imageIndex)
{
    if (imageIndex >= PlatformParameters::NUMBER_OF_IMAGES) {
        return m_globalImageMetadata->images[imageIndex].protectionStatus;
    }
    m_globalImageMetadata->images[imageIndex].protectionStatus = protectionStatus;
    return m_globalImageMetadata->images[imageIndex].protectionStatus;
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

    void* destination = reinterpret_cast<void*>(__approm_start__); // NOLINT
    void* source =
        reinterpret_cast<void*>(m_globalImageMetadata->images[imageIndex].imageBegin); // NOLINT

    rodos::memcpy(destination, source, m_globalImageMetadata->images[imageIndex].length);
}

bool MetadataInterface::verifyChecksum(size_t index)
{
    if (m_globalImageMetadata->images[index].imageBegin == 0) {
        return false;
    }
    uint32_t expectedChecksum = m_globalImageMetadata->images[index].crc;

    uint32_t actualChecksum = Checksums::calculateCrc32NoTable(
        reinterpret_cast<uint8_t*>( // NOLINT
            m_globalImageMetadata->images[index].imageBegin),
        m_globalImageMetadata->images[index].length);
    return expectedChecksum == actualChecksum;
}

}
