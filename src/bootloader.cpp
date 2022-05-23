#include "bootloader.h"

namespace bootloader {

using namespace std;

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

constinit const Bootloader BLTABLE __attribute__((used, section(".bootloader_table")));

} // namespace bootloader