#include "bootloader.h"

/* Controller specific includes */
#include <iostream>

using namespace bootloader;

int main([[maybe_unused]] int argc, [[maybe_unused]] char** argv) // NOLINT
{
    std::cout << "Starting bootloader" << std::endl;
    bootloader::Bootloader boot {};
    boot.run();
    return 0;
}
