#include "bootloader.h"

namespace bootloader {

using namespace std;

void Bootloader::run()
{
    int32_t selectedImage = selectImageSlot();

    if (selectedImage < 0 || selectedImage >= static_cast<int32_t>(NUMBER_OF_IMAGES)) {
        // TODO Error Handling
    }

    loadImage(selectedImage);
    // TODO Disable Interrupts before moving vector table?

    // Logic to run the bootloader
    Move_Vector_Table();
    // Memory_Barrier();
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
    // TODO Implement
    return -1;
}

bool Bootloader::isImageValid(size_t index)
{
    return m_metadataInterface.getGlobalImageMetadata()->images[index].complete &&
           verifyChecksum(index);
}

bool Bootloader::verifyChecksum([[maybe_unused]] size_t index)
{
    // TODO Implement
    return true;
}

void Bootloader::loadImage(size_t index)
{
    // TODO Lengthcheck at another place
    memcpy(
        &__approm_start__,
        m_metadataInterface.getGlobalImageMetadata()->images[index].imageBegin,
        m_metadataInterface.getGlobalImageMetadata()->images[index].length);
}

void* Bootloader::memcpy(void* destP, const void* srcP, size_t len)
{
    if (destP == nullptr || srcP == nullptr) {
        return nullptr;
    }
    auto* dest = static_cast<uint8_t*>(destP);
    const auto* src = static_cast<const uint8_t*>(srcP);

    while (len > 0) {
        *dest = *src;
        len--;
        src++; // NOLINT (cppcoreguidelines-pro-bounds-pointer-arithmetic)
        dest++; // NOLINT (cppcoreguidelines-pro-bounds-pointer-arithmetic)
    }
    return destP;
}

} // namespace bootloader
