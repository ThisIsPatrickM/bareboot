
bootloader_va41620_common_sources = files(
    'metadata_interface.cpp',
    'microcontroller_dependent.cpp',
    'weak_irq_handlers.cpp'
)

bootloader_microcontroller_va41620_sources = bootloader_va41620_common_sources

# Collect bootloader_va41620_sources in all subdirectories
subdir('boot_rom_spi')
subdir('rodos_includes')
subdir('init_structs')
