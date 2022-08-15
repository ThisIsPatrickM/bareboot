va41620_unit_tests_sources =  [ bootloader_va41620_common_sources,
                                bootloader_microcontroller_bootloader_dummy_sources,
                                bootloader_va41620_boot_rom_spi_sources,
                                bootloader_common_sources,
                                bootloader_architecture_dummy_sources]


va41620_includes_mocks =  [
        include_directories('mocks'),
        include_directories('../../../include/va41620')]

# Unit Tests
va41620_unit_tests += {
    'name'   : 'Bootloader Va41620 Metadata Unit Test',
    'src'    : files('platform_parameter_tests.cpp') + va41620_unit_tests_sources,
    'inc'    : va41620_includes_mocks
}
