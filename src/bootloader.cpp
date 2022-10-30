#include "bootloader.h"

using namespace bootloader::architecture;
namespace bootloader {

void Bootloader::run()
{
    m_metadataInterface.init();

    size_t selectedImage = selectImageSlot();
    // Update Metadata
    const GlobalImageMetadata* globalImageMetadata = m_metadataInterface.getGlobalImageMetadata();
    m_metadataInterface.updateCurrentImage(selectedImage);
    m_metadataInterface.updateGlobalBootcounter(globalImageMetadata->globalBootcounter + 1);
    m_metadataInterface.updateImageBootcounter(
        globalImageMetadata->images[selectedImage].bootcounter + 1, selectedImage);

    m_metadataInterface.loadImage(selectedImage);

    // Start Image
    Disable_Interrupts();
    Move_Vector_Table();
    Enable_Interrupts();

    Trigger_Watchdog();

    Start_App();
}

size_t Bootloader::selectImageSlot()
{
    size_t preferredImage = m_metadataInterface.getGlobalImageMetadata()->preferredImage;
    if (isImageValid(preferredImage) && lastBootSuccessful(preferredImage)) {
        return preferredImage;
    }
    size_t lastImage = m_metadataInterface.getGlobalImageMetadata()->currentImage;
    if (isImageValid(lastImage) && lastBootSuccessful(lastImage)) {
        return lastImage;
    }

    return selectBestGuessImageSlot();
}

size_t Bootloader::selectBestGuessImageSlot()
{
    // Find image that is complete, has correct checksum and last boot was successful
    size_t currentImage = m_metadataInterface.getGlobalImageMetadata()->currentImage;
    for (size_t i = 0; i < MetadataInterface::getNumberOfImages(); i++) {
        size_t imageIndex = (currentImage + i) % MetadataInterface::getNumberOfImages();
        if (isImageValid(imageIndex) && lastBootSuccessful(imageIndex)) {
            return (currentImage + i) % MetadataInterface::getNumberOfImages();
        }
    }

    // Find image that is complete and has correct checksum (no matter if last boot was successful)
    for (size_t i = 0; i < MetadataInterface::getNumberOfImages(); i++) {
        size_t imageIndex = (currentImage + i) % MetadataInterface::getNumberOfImages();
        if (isImageValid(imageIndex)) {
            return imageIndex;
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

bool Bootloader::lastBootSuccessful(size_t index)
{
    const GlobalImageMetadata* globalImageMetadata = m_metadataInterface.getGlobalImageMetadata();
    return globalImageMetadata->images[index].bootcounter ==
               globalImageMetadata->images[index].lastSuccessStatus ||
           globalImageMetadata->images[index].bootcounter ==
               globalImageMetadata->images[index].lastSuccessStatus + 1;
}

bool Bootloader::verifyChecksum(size_t index)
{
    return m_metadataInterface.verifyChecksum(index);
}

} // namespace bootloader
