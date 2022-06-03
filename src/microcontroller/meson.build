# microcontroller dependent 
bootloader_va41620_sources = files(
    'va41620' / 'microcontroller_dependent.cpp',
    'va41620' / 'hal' / 'hal_spi.cpp',
    'va41620' / 'hw_hal' / 'hw_hal_spi.cpp',
    'va41620' / 'metadata_interface.cpp',
    'va41620' / 'boot_rom_spi.cpp'
    )

bootloader_generic_sources = files(
    'generic' / 'microcontroller_dependent.cpp',
    'generic' / 'metadata_interface.cpp')
linker_args = []

if host_machine.system() == 'va41620'
    linker_args = ['-T' + meson.current_source_dir() / 'va41620' / 'bootloader.ld']
    bootloader_sources += bootloader_va41620_sources
else
    bootloader_sources += bootloader_generic_sources
endif
