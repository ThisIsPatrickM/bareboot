import argparse
import hashlib
import zlib
import os
import hmac
import subprocess
from platform_config import CONFIG_MAP

CONFIG = None

# TODO Refactor this file to collect all Metadata and then write in one batch!


def calc_crc(filename):
    """
    Calculate the CRC32 of the given file
    """
    with open(filename, "rb") as f:
        checksum = 0
        while (chunk := f.read()):
            checksum = zlib.crc32(chunk, checksum)
        return checksum


def write_crc(file_name, crc, index):
    """
    Put the given CRC in the metadata of the given file to the given imageIndex
    """
    offset = CONFIG.FIRST_CRC_OFFSET + CONFIG.SIZE_OF_IMAGE_METADATA * index
    print(
        f"Write Crc {hex(crc)} to {hex(offset)}")
    with open(file_name, 'rb+') as f:
        f.seek(offset)
        f.write(crc.to_bytes(CONFIG.UINT32_T_SIZE, byteorder=CONFIG.BYTE_ORDER))


def write_hmac_key(file_name, key_file):
    with open(file_name, 'rb+') as f:
        with open(key_file, 'rb') as key:
            f.seek(CONFIG.HMAC_KEY_OFFSET)
            f.write(key.read())


def calc_hmac(image_file, key_file):
    with open(key_file, 'rb') as key:
        with open(image_file, 'rb') as image:
            h = hmac.new(key.read(), image.read(), hashlib.sha256)
            return h.digest()


def write_hmac(output_file, hmac_signature, index):
    offset = CONFIG.FIRST_HMAC_OFFSET + CONFIG.SIZE_OF_IMAGE_METADATA * index
    with open(output_file, 'rb+') as f:
        f.seek(offset)
        f.write(hmac_signature)


def get_length(file_name):
    return os.path.getsize(file_name)


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


def merge_image_and_fix_metadata(image_file,  output_file, index, key_file):
    print(
        f"\nMerge Image {image_file}:{index} into file {output_file} and fix Metadata")
    put_image_to_file_at_index(image_file, output_file, index)

    crc = calc_crc(image_file)
    write_crc(output_file, crc, index)

    length = get_length(image_file)
    write_length(output_file, length, index)

    write_complete(output_file, index)

    hmac_signature = calc_hmac(image_file, key_file)
    write_hmac(output_file, hmac_signature, index)

    if index == None and length > CONFIG.BOOTLOADER_SIZE:
        raise Exception(
            f"Bootloader is too big. Has {length} but only {CONFIG.BOOTLOADER_SIZE} allowed. Check linkerfile")
    if index != None and length > CONFIG.MAX_BINARY_IMAGE_SIZE:
        print("Files should already be objcopied to binary!")
        raise Exception(
            f"Image {image_file} has {length} bytes, but only {CONFIG.MAX_BINARY_IMAGE_SIZE} fit in Slot")


if __name__ == '__main__':
    parser = argparse.ArgumentParser(description='Patch Binary')
    parser.add_argument("--images", nargs="+", help='paths to image files')
    parser.add_argument('--bootloader', type=str,
                        help='path to bootloader file')
    parser.add_argument("--keyfile", type=str,
                        help="path to the keyfile for hmac")
    parser.add_argument('--out', type=str, default="python-vorago.bin",
                        help='path to output image file')
    parser.add_argument('--config', type=str, default="va41620_dev_board",
                        help='name of the configuration. See platform_config.py for reference')

    args = parser.parse_args()

    CONFIG = CONFIG_MAP[args.config]

    put_image_to_file_at_index(
        input_file=args.bootloader, output_file=args.out, index=None)

    write_hmac_key(args.out, args.keyfile)

    for index, image_file in enumerate(args.images):
        merge_image_and_fix_metadata(
            image_file=image_file, output_file=args.out, index=index, key_file=args.keyfile)

    print("Done")
