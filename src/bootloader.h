#include "architecture/architecture_dependent.h"
#include "metadata.h"
#include "microcontroller/metadata_interface.h"
#include "microcontroller/microcontroller_dependent.h"

namespace bootloader {

class Bootloader {
public:
    void run();

private:
    MetadataInterface m_metadataInterface {};
};

} // namespace bootloader
