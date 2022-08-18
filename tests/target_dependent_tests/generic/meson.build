# Collected sources
bootloader_tests_generic_sources = [ bootloader_microcontroller_generic_sources,
                                    bootloader_microcontroller_bootloader_dummy_sources,
                                    bootloader_common_sources,
                                    bootloader_architecture_dummy_sources]

# Additional includes
bootloader_tests_generic_inc =  [bootloader_generic_dependent_inc]


# Unit Tests
generic_unit_tests += {
    'name'   : 'Bootloader Metadata Interface Generic Unit Test',
    'src'    : files('metadata_interface_tests.cpp') + bootloader_tests_generic_sources,
    'inc'    : bootloader_tests_generic_inc
}
