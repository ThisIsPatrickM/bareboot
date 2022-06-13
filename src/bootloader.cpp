#include "bootloader.h"

namespace bootloader {

using namespace std;
using namespace architecture;

void Bootloader::run()
{
    int32_t selectedImage = selectImageSlot();

    if (selectedImage < 0) {
        // TODO Error Handling, random Image?
        selectedImage = 0;
    }

    loadImage(selectedImage);
    // TODO Disable Interrupts before moving vector table?
    Disable_Interrupts();
    Move_Vector_Table();
    Enable_Interrupts();

    Memory_Barrier();

    Start_App();
}

int32_t Bootloader::selectImageSlot()
{
    size_t preferredImage = m_metadataInterface.getGlobalImageMetadata()->preferredImage;
    if (isImageValid(preferredImage)) {
        return static_cast<int32_t>(preferredImage);
    }
    size_t lastImage = m_metadataInterface.getGlobalImageMetadata()->currentImage;
    if (isImageValid(lastImage)) {
        return static_cast<int32_t>(lastImage);
    }
    return selectBestGuessImageSlot();
}

int32_t Bootloader::selectBestGuessImageSlot()
{
    // TODO Implement Advanced Select logic
    return static_cast<int32_t>(m_metadataInterface.getGlobalImageMetadata()->preferredImage);
}

bool Bootloader::isImageValid(size_t index)
{
    return m_metadataInterface.getGlobalImageMetadata()->images[index].complete &&
           verifyChecksum(index);
}

bool Bootloader::verifyChecksum(size_t index)
{
    if (m_metadataInterface.getGlobalImageMetadata()->images[index].imageBegin == 0) {
        return false;
    }
    uint32_t expectedChecksum = m_metadataInterface.getGlobalImageMetadata()->images[index].crc;
    uint32_t actualChecksum = dosis::support::crc::CastagnoliCrc32::calculateCRC32(
        reinterpret_cast<uint8_t*>( // NOLINT
            m_metadataInterface.getGlobalImageMetadata()->images[index].imageBegin),
        m_metadataInterface.getGlobalImageMetadata()->images[index].length);
    return expectedChecksum == actualChecksum;
}

void Bootloader::loadImage(size_t index)
{
    if (index >= MetadataInterface::getNumberOfImages()) {
        return;
    }

    if (__approm_start__ ==
        m_metadataInterface.getGlobalImageMetadata()->images[index].imageBegin) {
        // Nothing to do
        return;
    }

    if (m_metadataInterface.getGlobalImageMetadata()->images[index].imageBegin == 0) {
        // TODO Error Handling
        return;
    }

    // memcpy(
    //     &__approm_start__,
    //     m_metadataInterface.getGlobalImageMetadata()->images[index].imageBegin,
    //     MetadataInterface::getMaxImageLength());
    rodos::memcpy(
        reinterpret_cast<void*>(__approm_start__), // NOLINT
        reinterpret_cast<void*>( // NOLINT
            m_metadataInterface.getGlobalImageMetadata()->images[index].imageBegin),
        m_metadataInterface.getGlobalImageMetadata()->images[index].length);
}

} // namespace bootloader
