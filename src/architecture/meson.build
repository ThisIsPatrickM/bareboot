# Architecture dependent 
bootloader_armv7_sources = files('arm' / 'architecture_dependent.cpp')
bootloader_x64_sources = files('x64' / 'architecture_dependent.cpp')

if host_machine.cpu_family() == 'arm'
    bootloader_sources += bootloader_armv7_sources
else 
    bootloader_sources += bootloader_x64_sources
endif
