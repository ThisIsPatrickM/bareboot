
# Migrate bootloader to other platforms

A few steps are necessary to use the bootloader for other platforms. 

1. Investigate available memory locations
2. Investigate default boot behaviour
3. Place metadata table with linkerfile to fixed address
4. Create platform specific settings
5. Implement Metadata Interface
6. Implement architecture specific instructions
7. Adjust build system
8. Put bootloader and applications together and fix metadata

[TOC]

## 1. Investigate available memory locations
Important properties of the memory are:
- Volatility / Resetted after power is out
- Accessability, i.e. by pointer, using SPI, only pagewise write access, ...
- Is it executable
- Size
- Protection mechanisms, i.e. by a register

With this information you should be able to decide, where the images should be placed, how many there can be and how big the slots should be. 

## 2. Investigate default boot behaviour
This can be different on each platform. In case of the initial implementation of the Vorago Microcontroller the first 256 kilobytes of the boot ROM are copied to code SROM and then executed there.
This is then the entry point of any code.
The code SROM is emptied after power is off. 




## 3. Place metadata table with linkerfile to fixed address
The metadata table is contained in the bootloader and placed manually at a fixed address, for example right after the interrupt routing table. This gives it a fixed address that can be specified in each linkerfile.

Bootloader and the Application both use a linkerfile that defines the same lable at the same address.
The only difference is that the Application code begins after a fixed offset, which is the length of the bootloader (~4KB).

## 4. Create platform specific settings
See files in `include/<platform>/platform_parameters.h`.
Used to specify how many images and where they should be placed.

## 5. Implement Metadata Interface
Is an interface to read/write to the metadata table and the image slots.
Defined in `src/microcontroller/metadata_interface.h` and implemented in `src/microcontroller/<microcontroller>/microcontroller.cpp`

In case of the Vorago this is more complex since the metadata table in the code SRAM and persitent boot ROM needs to be both changed and kept in sync. 

## 6. Implement architecture specific instructions
Probably a shorter task. Currently only 3 functions defined in `src/architecture/architecture_dependent.h` are defined:
- Start_App()
- Disable_Interrupts
- Enable_Interrupts

On ARMv7 I was using inline assembly to implement this and dummy implementations exist, that do nothing. 

## 7. Adjust build system
TLDR: Make sure to collect all required source files, especially microcontroller/architecture dependent sources 
AND set the correct include paths (`include/<platform> and bootloader_independent_inc)
AND use the linker file in both projects (bootloader and application/dosis/rodos)

Note: Collecting the correct sources to test platform dependent code is a bit complicated, but possible. 

## 8. Put bootloader and applications together and fix metadata
To put bootloader and application binaries together while also fixing metadata this repo can be used:
[Initial-Metadata Script](https://github.com/ThisIsPatrickM/bareboot-initial-metadata)

1. Create a new `platform_description/<platform>.yml` file and insert the correct values
2. `python3 generate_platform_config.py`
3. Call initial_metdata.py and specify images, bootloader, private key and config to use. For example `python3 initial_metadata.py --images example.bin --bootloader bootloader.bin --key_file my.key --config va41620`

