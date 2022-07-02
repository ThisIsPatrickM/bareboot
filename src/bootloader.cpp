#include "bootloader.h"

namespace bootloader {

using namespace std;
using namespace architecture;

void Bootloader::run()
{
    m_metadataInterface.init();

    int32_t selectedImage = selectImageSlot();
    // int32_t selectedImage = 1;

    if (selectedImage < 0) {
        // TODO Error Handling, random Image?
        selectedImage = 0;
    }

    m_metadataInterface.updateCurrentImage(selectedImage);

    Memory_Barrier();
    // TODO Error when loading Image?
    // TODO If FRAM is too small?
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
    return m_metadataInterface.getGlobalImageMetadata()->images[index].completionStatus ==
               CompletionStatus::COMPLETE &&
           verifyChecksum(index);
}

bool Bootloader::verifyChecksum(size_t index)
{
    // if (m_metadataInterface.getGlobalImageMetadata()->images[index].imageBegin == 0) {
    //     return false;
    // }
    return m_metadataInterface.verifyChecksum(index);
    // uint32_t expectedChecksum = m_metadataInterface.getGlobalImageMetadata()->images[index].crc;
    // TODO Need to laod Image to check checksum. Also Fix in Main

    // uint32_t actualChecksum = checksums::Checksums::calculateCrc32NoTable(
    //     reinterpret_cast<uint8_t*>( // NOLINT
    //         m_metadataInterface.getGlobalImageMetadata()->images[index].imageBegin),
    //     m_metadataInterface.getGlobalImageMetadata()->images[index].length);
    // return true;
    // return expectedChecksum == actualChecksum;
}

void Bootloader::loadImage(size_t index)
{
    if (index >= MetadataInterface::getNumberOfImages()) {
        return;
    }

    if (reinterpret_cast<uintptr_t>(&__approm_start__) == // NOLINT
        m_metadataInterface.getGlobalImageMetadata()->images[index].imageBegin) {
        // Nothing to do
        return;
    }

    if (m_metadataInterface.getGlobalImageMetadata()->images[index].imageBegin == 0) {
        // TODO Error Handling
        return;
    }

    m_metadataInterface.loadImage(reinterpret_cast<void*>(&__approm_start__), index); // NOLINT
}

} // namespace bootloader
