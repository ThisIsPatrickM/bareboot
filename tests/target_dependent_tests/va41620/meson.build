va41620_unit_tests_sources =  [ bootloader_microcontroller_bootloader_dummy_sources,
                                bootloader_va41620_common_sources,
                                bootloader_va41620_boot_rom_spi_sources,
                                bootloader_common_sources,
                                bootloader_architecture_dummy_sources]


va41620_includes_mocks =  [
        include_directories('mocks'),
        bootloader_va41620_dependent_inc]

# Unit Tests
va41620_unit_tests += {
    'name'   : 'Bootloader Va41620 Metadata Interface Unit Test',
    'src'    : files('metadata_interface_tests.cpp') + va41620_unit_tests_sources,
    'inc'    : va41620_includes_mocks
}
