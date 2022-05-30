# microcontroller dependent 
bootloader_va41620_sources = files('va41620' / 'microcontroller_dependent.cpp')
bootloader_generic_sources = files('generic' / 'microcontroller_dependent.cpp')
linker_args = []

if host_machine.system() == 'va41620'
    linker_args = ['-T' + meson.current_source_dir() / 'va41620' / 'bootloader.ld']
    bootloader_sources += bootloader_va41620_sources
else
    bootloader_sources += bootloader_generic_sources
endif
