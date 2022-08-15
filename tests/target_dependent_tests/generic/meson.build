# Collected sources
genertic_unit_tests_sources = [ bootloader_microcontroller_generic_sources,
                                bootloader_microcontroller_bootloader_dummy_sources,
                                bootloader_common_sources,
                                bootloader_architecture_dummy_sources]

# Additional includes
generic_additional_includes =  [
    include_directories('../../../include/generic'),]


# Unit Tests
generic_unit_tests += {
    'name'   : 'Bootloader Metadata Generic Unit Test',
    'src'    : files('platform_parameter_tests.cpp') + genertic_unit_tests_sources,
    'inc'    : generic_additional_includes
}
