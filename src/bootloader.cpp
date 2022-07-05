#include "bootloader.h"

namespace bootloader {

using namespace std;
using namespace architecture;

void Bootloader::run()
{
    m_metadataInterface.init();

    size_t selectedImage = selectImageSlot();

    m_metadataInterface.updateCurrentImage(selectedImage);

    Memory_Barrier();

    loadImage(selectedImage);
    Disable_Interrupts();
    Move_Vector_Table();
    Enable_Interrupts();

    Memory_Barrier();

    Start_App();
}

size_t Bootloader::selectImageSlot()
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

size_t Bootloader::selectBestGuessImageSlot()
{
    // TODO Implement Advanced Select logic

    // Find Any image where Completionstatus and Checksum is correct, so that round robin system is
    // used
    size_t currentImage = m_metadataInterface.getGlobalImageMetadata()->currentImage;
    for (size_t i = 0; i < MetadataInterface::getNumberOfImages(); i++) {
        if (isImageValid((currentImage + i) % MetadataInterface::getNumberOfImages())) {
            return (currentImage + i) % MetadataInterface::getNumberOfImages();
        }
    }

    // Otherwise only use round robin
    return (currentImage + 1) % MetadataInterface::getNumberOfImages();
}

bool Bootloader::isImageValid(size_t index)
{
    return m_metadataInterface.getGlobalImageMetadata()->images[index].completionStatus ==
               CompletionStatus::COMPLETE &&
           verifyChecksum(index);
}

bool Bootloader::verifyChecksum(size_t index)
{
    return m_metadataInterface.verifyChecksum(index);
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
