

#include "metadata.h"
#include "microcontroller/metadata_interface.h"
#include "microcontroller/va41620/boot_rom_spi/boot_rom_spi.h"
#include "platform_parameters.h"
#include "gtest/gtest.h"

using namespace bootloader;

TEST(Va41620, NumberOfImages)
{
    EXPECT_EQ(PlatformParameters::NUMBER_OF_IMAGES, 7);
    MetadataInterface meta;
    meta.updateImageLength(20, 1);
    const GlobalImageMetadata* glob = meta.getGlobalImageMetadata();
    EXPECT_EQ(glob->images[1].length, 20);
}
