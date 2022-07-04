# microcontroller dependent 
bootloader_va41620_sources = []
subdir('va41620')

bootloader_generic_sources = []
subdir('generic')

# For implementations without linker script
bootloader_dummy_sources = []
subdir('bootloader_dummy')

if host_machine.system() == 'va41620'
    linker_args = ['-T' + meson.current_source_dir() / 'va41620' / 'bootloader.ld']
    bootloader_sources += bootloader_va41620_sources
else
    bootloader_sources += bootloader_generic_sources + bootloader_dummy_sources
endif
