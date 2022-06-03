#include "bootloader.h"

namespace bootloader {

using namespace std;

void Bootloader::run()
{
    // Logic to run the bootloader
    Move_Vector_Table();
    // Memory_Barrier();
    Memory_Barrier();

    Start_App();
}

// bool Bootloader::checkCRC([[maybe_unused]] size_t index, [[maybe_unused]] uint32_t crc)
// {
//     return true;
// }

// void Bootloader::updatePrefferedImage([[maybe_unused]] size_t index) {}

// void Bootloader::updateCurrentImage([[maybe_unused]] size_t index) {}

// void Bootloader::updateVersion(
//     [[maybe_unused]] uint32_t version, [[maybe_unused]] size_t imageIndex)
// {
// }

// void Bootloader::updateCRC([[maybe_unused]] uint32_t crc, [[maybe_unused]] size_t imageIndex) {}

// void Bootloader::updateBootcounter(
//     [[maybe_unused]] uint32_t counter, [[maybe_unused]] size_t imageIndex)
// {
// }

// void Bootloader::updateSuccessCounter(
//     [[maybe_unused]] uint32_t counter, [[maybe_unused]] size_t imageIndex)
// {
// }

// void Bootloader::updateLastSuccessStatus(
//     [[maybe_unused]] uint32_t status, [[maybe_unused]] size_t imageIndex)
// {
// }

// void Bootloader::updateImageComplete(
//     [[maybe_unused]] bool status, [[maybe_unused]] size_t imageIndex)
// {
// }

// void Bootloader::updateImageAlwaysKeep(
//     [[maybe_unused]] bool status, [[maybe_unused]] size_t imageIndex)
// {
// }

// GlobalImageMetadata& Bootloader::getGlobalImageMetadata()
// {
//     return globalImageMetadata;
// }

} // namespace bootloader
