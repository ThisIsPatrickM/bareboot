

#include "bootloader.h"
#include "metadata.h"
#include "gtest/gtest.h"

using namespace bootloader;

TEST(Basic, Empty)
{
    GlobalImageMetadata img;
    // EXPECT_EQ(img.currentImage, 0);
}

TEST(Basic, MetadataGlobal)
{
    // EXPECT_EQ(blTable.globalImageMetadata.bootcounter, 0);
    // blTable.globalImageMetadata.bootcounter++;
    // EXPECT_EQ(blTable.globalImageMetadata.bootcounter, 1);
}