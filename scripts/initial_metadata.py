import argparse
import zlib
import os
import subprocess

# TODO This Offset might change, when memory_map.h is changed. And needs to be calculated for future images
# Check BOOTROM SPI / Platform parameters for Constexpr offset calculation

FIRST_CRC_OFFSET = 876
FIRST_LENGTH_OFFSET = 904
SIZE_OF_IMAGE_METADATA = 40
IMAGE_BEGIN_ADDRESSES = [4096, 262144,
                         520192, 778240, 1036288, 1294336, 1552384]


def calc_crc(filename):
    with open(filename, "rb") as f:
        checksum = 0
        while (chunk := f.read()):
            checksum = zlib.crc32(chunk, checksum)
        return checksum


def get_length(file_name):
    return os.path.getsize(file_name)


def write_crc(file_name, crc, index):
    with open(file_name, 'rb+') as f:
        f.seek(FIRST_CRC_OFFSET + SIZE_OF_IMAGE_METADATA * index)
        f.write(crc.to_bytes(4, byteorder='little'))


def write_length(file_name, length, index):
    with open(file_name, 'rb+') as f:
        f.seek(FIRST_LENGTH_OFFSET + SIZE_OF_IMAGE_METADATA * index)
        f.write(length.to_bytes(4, byteorder='little'))


def dd_prototype(input_file, output_file, index=None):
    if index is None:
        # Bootloader, no offset
        offset = 0
    else:
        offset = IMAGE_BEGIN_ADDRESSES[index]

    print(f"DD: Writing file {input_file} to {output_file} with index {index}")
    cmd = ["dd", "conv=notrunc",  f"if={input_file}", f"of={output_file}",
           "bs=1", f"seek={offset}", "status=progress"]

    process = subprocess.Popen(cmd, stderr=subprocess.PIPE)

    line = ''
    while True:
        out = process.stderr.read(1)
        if out == b'' and process.poll() != None:
            break
        if out != b'':
            s = out.decode("utf-8")
            if s == '\r':
                print(line)
                line = ''
            else:
                line = line + s


def merge_image_and_bootloader(image_file,  outfile, index):
    dd_prototype(image_file, outfile, index)

    print(f"Calculating CRC for file: {image_file}")
    crc = calc_crc(image_file)

    print(f"Writing CRC {hex(crc)} to outfile: {outfile}")
    print(
        f"Please compare it to the result after executing 'crc32 {image_file}'")
    write_crc(outfile, crc, index)

    print("Getting Length of Binary")
    length = get_length(image_file)
    print(f"Writing Length {hex(length)} to outfile: {outfile}")
    write_length(outfile, length, index)


if __name__ == '__main__':
    print("Files should already be objcopied to binary!")
    parser = argparse.ArgumentParser(description='Patch Binary')
    parser.add_argument('--image0', type=str,
                        help='path to first image file')
    parser.add_argument('--image1', type=str, default=None,
                        help='path to second image file')
    parser.add_argument('--image2', type=str, default=None,
                        help='path to third image file')
    parser.add_argument('--bootloader', type=str,
                        help='path to bootloader file')
    parser.add_argument('--out', type=str, default="python-vorago.bin",
                        help='path to third image file')

    args = parser.parse_args()

    # TODO Get newest result of objcopy? Or  just do it in Shell?

    dd_prototype(args.bootloader, args.out)

    merge_image_and_bootloader(args.image0,  args.out, 0)
    if args.image1 is not None:
        merge_image_and_bootloader(args.image1, args.out, 1)
    if args.image2 is not None:
        merge_image_and_bootloader(args.image2, args.out, 2)

    print("Done")
