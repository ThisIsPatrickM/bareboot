#include "bootloader.h"

namespace bootloader {

using namespace std;
using namespace architecture;

void Bootloader::run()
{
    int32_t selectedImage = selectImageSlot();

    if (selectedImage < 0) {
        // TODO Error Handling, random Image?
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
    if (!m_metadataInterface.getGlobalImageMetadata()->initialized) {
        m_metadataInterface.init();
        // Boot first Image, if this is the first boot. This will set the addresses
        return 0;
    }

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
    return m_metadataInterface.getGlobalImageMetadata()->preferredImage; // NOLINT
}

bool Bootloader::isImageValid(size_t index)
{
    return m_metadataInterface.getGlobalImageMetadata()->images[index].complete &&
           verifyChecksum(index);
}

bool Bootloader::verifyChecksum([[maybe_unused]] size_t index)
{
    // TODO Implement
    // uint32_t expectedChecksum = m_metadataInterface.getGlobalImageMetadata()->images[index].crc;
    return true;
}

void Bootloader::loadImage([[maybe_unused]] size_t index)
{
    // TODO Lengthcheck at another place
    // memcpy(
    //     &__approm_start__,
    //     m_metadataInterface.getGlobalImageMetadata()->images[index].imageBegin,
    //     m_metadataInterface.getGlobalImageMetadata()->images[index].length);
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
