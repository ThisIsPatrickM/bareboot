import argparse
import zlib
import os

# TODO This Offset might change, when memory_map.h is changed. And needs to be calculated for future images
# Check BOOTROM SPI for Constexpr offset calculation
METADATA_OFFSET = 0x350
FIRST_CRC_OFFSET = 876
FIRST_LENGTH_OFFSET = 904


def calc_crc(filename):
    with open(filename, "rb") as f:
        checksum = 0
        while (chunk := f.read()):
            checksum = zlib.crc32(chunk, checksum)
        return checksum


def get_length(file_name):
    return os.path.getsize(file_name)


def write_crc(file_name, crc):
    with open(file_name, 'rb+') as f:
        f.seek(FIRST_CRC_OFFSET)
        f.write(crc.to_bytes(4, byteorder='little'))


def write_length(file_name, length):
    with open(file_name, 'rb+') as f:
        f.seek(FIRST_LENGTH_OFFSET)
        f.write(length.to_bytes(4, byteorder='little'))


if __name__ == '__main__':

    parser = argparse.ArgumentParser(description='Patch Binary')
    parser.add_argument('--image_file', type=str,
                        help='path to image file')
    parser.add_argument('--bootloader', type=str,
                        help='path to bootloader file')
    parser.add_argument('--image_index', type=int, default=0,
                        help='TODO, not yet supported | Index of the image to be patched')

    args = parser.parse_args()
    print(f"Calculating CRC for file: {args.image_file}")
    crc = calc_crc(args.image_file)

    print(f"Writing CRC {hex(crc)} to bootloader {args.bootloader}")
    print(
        f"Please compare it to the result after executing 'crc32 {args.image_file}'")
    write_crc(args.bootloader, crc)

    print("Getting Length of Binary")
    length = get_length(args.image_file)
    print(f"Writing Length {hex(length)} to Bootloader")
    write_length(args.bootloader, length)
    print("Done. Files can now be connected with 'dd' ")
