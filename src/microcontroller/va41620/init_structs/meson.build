
bootloader_va41620_init_structs = files('init_structs_va41620.cpp')
bootloader_va41620_dev_board_init_structs = files('init_structs_va41620_dev_board.cpp')

if meson.get_cross_property('development_board', 'false') == 'true'
    bootloader_sources += bootloader_va41620_dev_board_init_structs
    bootloader_inc += include_directories('../../../../include/va41620_dev_board') 
else
    bootloader_sources += bootloader_va41620_init_structs
    bootloader_inc += include_directories('../../../../include/va41620') 
endif
