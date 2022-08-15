

#include "metadata.h"
#include "microcontroller/metadata_interface.h"
#include "platform_parameters.h"
#include "gtest/gtest.h"

using namespace bootloader;

TEST(GenericMeta, NumberOfImages)
{
    EXPECT_EQ(PlatformParameters::NUMBER_OF_IMAGES, 4);
}

TEST(GenericMeta, Meta)
{
    EXPECT_EQ(true, true);
    MetadataInterface metadata;
}