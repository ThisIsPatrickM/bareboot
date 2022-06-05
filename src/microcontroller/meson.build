# microcontroller dependent 
bootloader_va41620_sources = []
subdir('va41620')

bootloader_generic_sources = []
subdir('generic-dummy')

linker_args = []

if host_machine.system() == 'va41620'
    linker_args = ['-T' + meson.current_source_dir() / 'va41620' / 'bootloader.ld']
    bootloader_sources += bootloader_va41620_sources
else
    bootloader_sources += bootloader_generic_sources
endif
