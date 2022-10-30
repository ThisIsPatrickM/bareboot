#include "microcontroller/metadata_interface.h"

/* Controller specific includes */
#include "memcpy/memcpy.h"
#include "platform_parameters.h"

namespace bootloader {

MetadataInterface::MetadataInterface()
    : m_globalImageMetadata { reinterpret_cast<GlobalImageMetadata*>(__bootloader__) } // NOLINT
{
}

MetadataInterface::MetadataInterface(GlobalImageMetadata* globalImageMetadata)
    : m_globalImageMetadata(globalImageMetadata)
{
}

const GlobalImageMetadata* MetadataInterface::getGlobalImageMetadata()
{
    return m_globalImageMetadata;
}

std::size_t MetadataInterface::updateCurrentImage(std::size_t imageIndex)
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

uint32_t MetadataInterface::updateImageBootcounter(uint32_t bootcounter, std::size_t imageIndex)
{
    if (imageIndex >= PlatformParameters::NUMBER_OF_IMAGES) {
        return 0;
    }
    m_globalImageMetadata->images[imageIndex].bootcounter = bootcounter;
    return m_globalImageMetadata->images[imageIndex].bootcounter;
}

std::size_t MetadataInterface::getNumberOfImages()
{
    return PlatformParameters::NUMBER_OF_IMAGES;
}

std::size_t MetadataInterface::getMaxImageLength()
{
    return PlatformParameters::MAX_IMAGE_LENGTH;
}

void MetadataInterface::init() {}

void MetadataInterface::loadImage(std::size_t imageIndex)
{
    if (imageIndex >= MetadataInterface::getNumberOfImages()) {
        return;
    }

    if (reinterpret_cast<uintptr_t>(__approm_start__) == // NOLINT
        m_globalImageMetadata->images[imageIndex].imageBegin) {
        return;
    }

    if (m_globalImageMetadata->images[imageIndex].imageBegin == 0) {
        return;
    }

    void* destination = reinterpret_cast<void*>(__approm_start__); // NOLINT
    void* source =
        reinterpret_cast<void*>(m_globalImageMetadata->images[imageIndex].imageBegin); // NOLINT

    for (unsigned i = 0; i < LOAD_RETRIES; i++) {
        // Load
        RODOS::memcpy(destination, source, m_globalImageMetadata->images[imageIndex].length);
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

bool MetadataInterface::verifyChecksum(std::size_t index)
{
    if (m_globalImageMetadata->images[index].imageBegin == 0) {
        return false;
    }
    uint32_t expectedChecksum = m_globalImageMetadata->images[index].crc;

    uint32_t actualChecksum = checksums::Crc32::calculateCRC32(
        reinterpret_cast<uint8_t*>( // NOLINT
            m_globalImageMetadata->images[index].imageBegin),
        m_globalImageMetadata->images[index].length);
    return expectedChecksum == actualChecksum;
}

}
