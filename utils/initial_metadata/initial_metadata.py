import argparse
import hashlib
import zlib
import os
import hmac
import subprocess
from enum import Enum
from platform_config import CONFIG_MAP


CONFIG = None


class ImageMetadata:
    def __init__(self):
        self.crc = 0
        self.completion_status = 1
        self.hmac_signature = None
        self.length = 0
        self.file_name = ""


class GlobalImageMetadata:
    def __init__(self):
        self.key_file = None
        self.images: list(ImageMetadata) = []


def calculate_crc(image_file):
    """
    Calculate the CRC32 of the given file
    """
    with open(image_file, "rb") as f:
        # checksum = 0xFFFFFFFF
        # checksum = 0
        checksum = zlib.crc32(f.read())

        return checksum


def get_length(file_name):
    return os.path.getsize(file_name)


def calculate_hmac_signature(image_file, key_file):
    with open(key_file, 'rb') as key:
        with open(image_file, 'rb') as image:
            h = hmac.new(key.read(), image.read(), hashlib.sha256)
            print("HMAC SIGNATURE is , ", h.hexdigest())
            return h.digest()


def extract_image_metadata(global_metadata: GlobalImageMetadata, image_file, key_file):
    global_metadata.hmac_key_file = key_file
    image_meta = ImageMetadata()
    image_meta.crc = calculate_crc(image_file)
    image_meta.length = get_length(image_file)
    image_meta.hmac_signature = calculate_hmac_signature(image_file, key_file)
    image_meta.file_name = image_file
    global_metadata.images.append(image_meta)


def create_output_binary(bootloader_file, output_file):
    length = get_length(bootloader_file)
    if length > CONFIG.BOOTLOADER_SIZE:
        raise Exception(
            f"Bootloader is too big. Has {length} but only {CONFIG.BOOTLOADER_SIZE} allowed. Check linkerfile")

    print(
        f"DD: Creating target binary {output_file} , based on bootloader:{bootloader_file}")
    cmd = ["dd", "conv=notrunc",  f"if={bootloader_file}", f"of={output_file}",
           "bs=1", f"seek={0}", "status=progress"]

    process = subprocess.Popen(
        cmd, stderr=subprocess.STDOUT, stdout=subprocess.PIPE, close_fds=True, universal_newlines=True)
    print(process.stdout.read())


def append_images_to_binary(images, output_file):

    for index, image_file in enumerate(images):
        length = get_length(image_file)
        if length > CONFIG.MAX_BINARY_IMAGE_SIZE:
            print("Files should already be objcopied to binary!")
            raise Exception(
                f"Image {image_file} wit index {index} has {length} bytes, but only {CONFIG.MAX_BINARY_IMAGE_SIZE} fit in Slot")

        offset = CONFIG.IMAGE_BEGIN_ADDRESSES[index]
        print(
            f"DD: Creating target binary {output_file} , based on image {image_file} with index {index}")
        cmd = ["dd", "conv=notrunc",  f"if={image_file}", f"of={output_file}",
               "bs=1", f"seek={offset}", "status=progress"]

        process = subprocess.Popen(
            cmd, stderr=subprocess.STDOUT, stdout=subprocess.PIPE, close_fds=True, universal_newlines=True)
        print(process.stdout.read())


def write_hmac_key(output_file, key_file):
    with open(output_file, 'rb+') as f:
        with open(key_file, 'rb') as key:
            f.seek(CONFIG.HMAC_KEY_OFFSET)
            f.write(key.read())


def write_crc(output_file, crc, index):
    """
    Put the given CRC in the metadata of the given file to the given imageIndex
    """
    print("Checksum is ", hex(crc))
    print("Checksum of ABC is ", zlib.crc32(b"ABC"))
    print("Checksum of ABC is ", zlib.crc32(b"ABC", 0))
    print("Checksum of ABC is ", zlib.crc32(b"ABC", 0xFFFFFFFF))

    offset = CONFIG.FIRST_CRC_OFFSET + CONFIG.SIZE_OF_IMAGE_METADATA * index
    with open(output_file, 'rb+') as f:
        f.seek(offset)
        f.write(crc.to_bytes(CONFIG.UINT32_T_SIZE, byteorder=CONFIG.BYTE_ORDER))


def write_length(output_file, length, index):
    offset = CONFIG.FIRST_LENGTH_OFFSET + CONFIG.SIZE_OF_IMAGE_METADATA * index

    with open(output_file, 'rb+') as f:
        f.seek(offset)
        f.write(length.to_bytes(CONFIG.UINT32_T_SIZE, byteorder=CONFIG.BYTE_ORDER))


def write_completion_status(output_file, completion_status, index):
    offset = CONFIG.FIRST_COMPLETE_OFFSET + CONFIG.SIZE_OF_IMAGE_METADATA * index
    print(
        f"Write to complete to {hex(offset)}")
    with open(output_file, 'rb+') as f:
        f.seek(offset)
        f.write((1).to_bytes(
            CONFIG.COMPLETION_STATUS_SIZE, byteorder=CONFIG.BYTE_ORDER))


def write_hmac_signature(output_file, hmac_signature, index):
    offset = CONFIG.FIRST_HMAC_OFFSET + CONFIG.SIZE_OF_IMAGE_METADATA * index
    with open(output_file, 'rb+') as f:
        f.seek(offset)
        f.write(hmac_signature)


def fix_metadata(output_file, global_metadata: GlobalImageMetadata):
    write_hmac_key(output_file, global_metadata.hmac_key_file)
    for index, image in enumerate(global_metadata.images):
        write_crc(output_file, image.crc, index)
        write_length(output_file, image.length, index)
        write_completion_status(
            output_file, image.completion_status, index)
        write_hmac_signature(output_file, image.hmac_signature, index)


if __name__ == '__main__':
    parser = argparse.ArgumentParser(description='Patch Binary')
    parser.add_argument("--images", nargs="+", help='paths to image files')
    parser.add_argument('--bootloader', type=str,
                        help='path to bootloader file')
    parser.add_argument("--key_file", type=str,
                        help="path to the keyfile for hmac")
    parser.add_argument('--out', type=str, default="python-vorago.bin",
                        help='path to output image file')
    parser.add_argument('--config', type=str, default="va41620_dev_board",
                        help='name of the configuration. See platform_config.py for reference')

    args = parser.parse_args()
    # Select Configuration:
    CONFIG = CONFIG_MAP[args.config]

    # Construct Metadata
    global_metadata = GlobalImageMetadata()
    for index, image_file in enumerate(args.images):
        extract_image_metadata(global_metadata, image_file, args.key_file)

    # Create Target Binary
    create_output_binary(args.bootloader, args.out)

    # Append Images to Binary
    append_images_to_binary(args.images, args.out)

    # Write Metadata
    fix_metadata(args.out, global_metadata)

    print("Done")
