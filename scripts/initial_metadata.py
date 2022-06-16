import argparse
import zlib
import os
import subprocess
from platform_config import CONFIG_MAP

CONFIG = None


def calc_crc(filename):
    with open(filename, "rb") as f:
        checksum = 0
        while (chunk := f.read()):
            checksum = zlib.crc32(chunk, checksum)
        return checksum


def get_length(file_name):
    return os.path.getsize(file_name)


def write_crc(file_name, crc, index):
    offset = CONFIG.FIRST_CRC_OFFSET + CONFIG.SIZE_OF_IMAGE_METADATA * index
    print(
        f"Write Crc {hex(crc)} to {hex(offset)}")
    with open(file_name, 'rb+') as f:
        f.seek(offset)
        f.write(crc.to_bytes(CONFIG.UINT32_T_SIZE, byteorder=CONFIG.BYTE_ORDER))


def write_length(file_name, length, index):
    offset = CONFIG.FIRST_LENGTH_OFFSET + CONFIG.SIZE_OF_IMAGE_METADATA * index
    print(
        f"Write Length {(length)} to {hex(offset)} for index {index}")

    with open(file_name, 'rb+') as f:
        f.seek(offset)
        f.write(length.to_bytes(CONFIG.UINT32_T_SIZE, byteorder=CONFIG.BYTE_ORDER))


def write_complete(file_name, index):
    offset = CONFIG.FIRST_COMPLETE_OFFSET + CONFIG.SIZE_OF_IMAGE_METADATA * index
    print(
        f"Write to complete to {hex(offset)}")
    with open(file_name, 'rb+') as f:
        f.seek(offset)
        f.write((1).to_bytes(1, byteorder=CONFIG.BYTE_ORDER))


def put_image_to_file_at_index(input_file, output_file, index=None):
    if index is None:
        # Bootloader, no offset
        offset = 0
    else:
        offset = CONFIG.IMAGE_BEGIN_ADDRESSES[index]

    print(f"DD: Writing file {input_file} to {output_file} with index {index}")
    cmd = ["dd", "conv=notrunc",  f"if={input_file}", f"of={output_file}",
           "bs=1", f"seek={offset}", "status=progress"]

    process = subprocess.Popen(
        cmd, stderr=subprocess.STDOUT, stdout=subprocess.PIPE, close_fds=True, universal_newlines=True)

    print(process.stdout.read())


def merge_image_and_fix_metadata(image_file,  output_file, index):
    print(
        f"\nMerge Image {image_file}:{index} into file {output_file} and fix Metadata")
    put_image_to_file_at_index(image_file, output_file, index)

    crc = calc_crc(image_file)
    write_crc(output_file, crc, index)

    length = get_length(image_file)
    write_length(output_file, length, index)

    write_complete(output_file, index)

    if index == None and length > CONFIG.BOOTLOADER_SIZE:
        raise Exception("Bootloader is too big. Change linkerfile")
    if index != None and length > CONFIG.MAX_IMAGE_SIZE:
        raise Exception("Image is bigger than Slot")


if __name__ == '__main__':
    print("Files should already be objcopied to binary!")
    parser = argparse.ArgumentParser(description='Patch Binary')
    parser.add_argument("--images", nargs="+", help='paths to image files')
    parser.add_argument('--bootloader', type=str,
                        help='path to bootloader file')
    parser.add_argument('--out', type=str, default="python-vorago.bin",
                        help='path to output image file')
    parser.add_argument('--config', type=str, default="va41620",
                        help='name of the configuration. See platform_config.py for reference')

    args = parser.parse_args()

    CONFIG = CONFIG_MAP[args.config]

    put_image_to_file_at_index(
        input_file=args.bootloader, output_file=args.out, index=None)

    for index, image_file in enumerate(args.images):
        merge_image_and_fix_metadata(
            image_file=image_file, output_file=args.out, index=index)

    print("Done")
