# microcontroller dependent 
bootloader_va41620_sources = []
subdir('va41620')

bootloader_generic_sources = []
subdir('generic')

# For implementations without linker script
bootloader_dummy_sources = []
subdir('bootloader_dummy')

# Initstructs
bootloader_va41620_init_structs = files('va41620/init_structs.cpp')
bootloader_va41620_dev_board_init_structs = files('va41620_dev_board/init_structs.cpp')


if host_machine.system() == 'va41620'
  bootloader_sources += bootloader_va41620_sources
  if meson.get_cross_property('development_board') == 'true'
    bootloader_sources += bootloader_va41620_dev_board_init_structs
  else
    bootloader_sources += bootloader_va41620_init_structs
  endif
else
  bootloader_sources += bootloader_generic_sources + bootloader_dummy_sources
endif
