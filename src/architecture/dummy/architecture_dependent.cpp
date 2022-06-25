#include "architecture/architecture_dependent.h"

#include "metadata.h"

#include <iostream>

namespace bootloader::architecture {

void Dummy_App()
{
    std::cout << "Dummy App called. Example ends here." << std::endl;
    while (true) {
        ;
    }
}

void Start_App()
{
    Dummy_App();
}

void Memory_Barrier() {}

} // namespace bootloader::architecture
