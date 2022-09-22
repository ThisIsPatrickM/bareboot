
# Project Stuctrue

- *include*: Files that should be included by the booted software. `metadata.h` and `memory_map.h` are for every platform, while `platform_parameters.h` and the cross_files and linker scripts are dependent on the platform. 
- *src*: Source files seperated into architecture dependent and microcontroller dependent code. 
- *tests*: Contains test code
- *utils*: Contains scripts to flash onto VA41620

# How to build and test

For native systems:
```
meson build-x64
ninja -C build-x64 test
```

For va41620 and va41620 dev board:
```
meson build-va41620 --cross-file include/va41620/cross_files/cross.ini
ninja -C build-va41620 test
meson build-va41620_dev_board --cross-file include/va41620_dev_board/cross_files/cross.ini
ninja -C build-va41620_dev_board test
```

# How to flash bootloader and firmware

1. **Compile firmware and bootloader**:

```
meson build-va41620_dev_board --cross-file cross-files/va41620_dev_board.ini -Db_lto=true 
ninja -C build-va41620_dev_board test
```
Link time optimization is set to true. Not using this option might increase the required memory and the linker script might need to be adjusted. 

2. **ObjCopy binaries and put to a path of your choice**:
Optionally delete old binaries from previous step first.
```
arm-none-eabi-objcopy build-va41620_dev_board/subprojects/bootloader/bootloader -O binary subprojects/bootloader/utils/initial_metadata/bootloader.bin
cp build-va41620_dev_board/experimental/examples/example-vorago.bin subprojects/bootloader/utils/initial_metadata/example.bin
```

3. **Use Initial Metadata Script**:

This step puts bootloader and rodos/dosis application to the expected location and fixed the metadata table to include a valid entries (crc, hmac, length, completion).
See [Github InitialMetadata](https://github.com/ThisIsPatrickM/bareboot-initial-metadata) for further instructions.
This creates a single file that can be flashed with the default name `python-vorago.bin`

4. **Flash to microcontroller**:

```
subprojects/bootloader/utils/va41620_flash/vaflash.py python-vorago.bin --gdb_path=gdb-multiarch
```

# How to port to other systems


# GDB Quick Tips

- **GDB Server**: `subprojects/bootloader/utils/va41620_flash/gdbserversetup.sh`
- **Connect to GDB**: `	gdb-multiarch build-va41620_dev_board/experimental/examples/example-vorago.elf -ex "target remote :2331"`

- **Remove ROM Protection**: `set {int}0x40010010 = 1`
- **Show Metadata Table**: `x/20 0x350`  Shows 20 bytes starting from address 0x350, the address of the metadata table on VA41620
