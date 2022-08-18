bootloader_tests_va41620_sources =  [ bootloader_microcontroller_bootloader_dummy_sources,
                                    bootloader_va41620_common_sources,
                                    bootloader_va41620_boot_rom_spi_sources,
                                    bootloader_common_sources,
                                    bootloader_architecture_dummy_sources]


bootloader_tests_va41620_inc =  [
        include_directories('mocks'),
        bootloader_va41620_dependent_inc]

# Unit Tests
va41620_unit_tests += {
    'name'   : 'Bootloader Metadata Interface Va41620 Unit Test',
    'src'    : files('metadata_interface_tests.cpp') + bootloader_tests_va41620_sources,
    'inc'    : bootloader_tests_va41620_inc
}
