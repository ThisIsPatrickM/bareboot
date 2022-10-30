supported_targets = ['va41620']

# Explore all targets and collect their sources
foreach target : supported_targets
  subdir(target)
endforeach
subdir('generic')
subdir('bootloader_dummy')


# Depending on compile options add used sources and includes
if host_machine.system() == 'va41620'
  bootloader_sources += bootloader_microcontroller_va41620_sources
  # Differentiate between dev_board and productive board
  if meson.get_cross_property('development_board', 'false') == 'true'
    bootloader_dependent_inc += bootloader_va41620_dev_board_dependent_inc
  else
    bootloader_dependent_inc += bootloader_va41620_dependent_inc
  endif

else 
  # Use dummy sources and generic includes
  bootloader_sources += [ bootloader_microcontroller_generic_sources,
                          bootloader_microcontroller_bootloader_dummy_sources,
                          bootloader_microcontroller_main_dummy_sources ]
  bootloader_dependent_inc += bootloader_generic_dependent_inc
endif
