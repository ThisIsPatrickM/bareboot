# Architecture dependent 
bootloader_armv7_sources = files('arm' / 'architecture_dependent.cpp')
bootloader_dummy_sources = files('dummy' / 'architecture_dependent.cpp')

if host_machine.cpu_family() == 'arm'
    bootloader_sources += bootloader_armv7_sources
else 
    bootloader_sources += bootloader_dummy_sources
endif
