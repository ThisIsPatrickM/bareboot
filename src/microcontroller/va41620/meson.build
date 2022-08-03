
bootloader_va41620_sources = files(
    'microcontroller_dependent.cpp',
    'metadata_interface.cpp',
    'weak_irq_handlers.cpp'
    )

subdir('boot_rom_spi')
subdir('hal')
subdir('hw_hal')
subdir('init_structs')

bootloader_sources += bootloader_va41620_sources
