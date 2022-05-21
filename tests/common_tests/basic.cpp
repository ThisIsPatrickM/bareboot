

#include "metadata.h"
#include "gtest/gtest.h"

using namespace bootloader;

TEST(FooTest, Sample)
{
    GlobalImageMetadata img;
    EXPECT_EQ(img.currentImage, 0);
}