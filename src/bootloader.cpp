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

    // TODO Reset last successStatus when updating/copying image!

    Memory_Barrier();

    m_metadataInterface.loadImage(selectedImage);
    Disable_Interrupts();
    Move_Vector_Table();
    Enable_Interrupts();

    Memory_Barrier();

    Trigger_Watchdog();

    Start_App();
}

size_t Bootloader::selectImageSlot()
{
    // TODO Check last success status!
    // TODO Fix naming of lastbootSuccessful
    size_t preferredImage = m_metadataInterface.getGlobalImageMetadata()->preferredImage;
    if (isImageValid(preferredImage) && lastBootSuccessful(preferredImage)) {
        return static_cast<int32_t>(preferredImage);
    }
    size_t lastImage = m_metadataInterface.getGlobalImageMetadata()->currentImage;
    if (isImageValid(lastImage) && lastBootSuccessful(lastImage)) {
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
