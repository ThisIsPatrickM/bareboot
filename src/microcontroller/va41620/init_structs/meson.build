
bootloader_va41620_init_structs = files('init_structs_va41620.cpp')
bootloader_va41620_dev_board_init_structs = files('init_structs_va41620_dev_board.cpp')

if meson.get_cross_property('development_board') == 'true'
    bootloader_sources += bootloader_va41620_dev_board_init_structs
else
    bootloader_sources += bootloader_va41620_init_structs
endif