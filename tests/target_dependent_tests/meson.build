
va41620_unit_tests = []
generic_unit_tests = []

subdir('generic')
subdir('va41620')

### Build and register tests for VA41620 ###
foreach t : va41620_unit_tests
    ### executable
    test_exe = executable(t['name'].underscorify()+ '_exe',
        sources             : t['src'],
        dependencies        : [
            t.get('deps', []),
            gtest_main_dep, gmock_dep, gtest_dep
        ],
        include_directories : [
            t.get('inc', []),
            bootloader_independent_inc],
    )
    test(t['name'], test_exe, suite: 'unit-test')
endforeach

### Build and register tests ###
foreach t : generic_unit_tests
    ### executable
    test_exe = executable(t['name'].underscorify()+ '_exe',
        sources             : t['src'],
        dependencies        : [
            t.get('deps', []),
            gtest_main_dep, gmock_dep, gtest_dep
        ],
        include_directories : [t.get('inc', []), bootloader_independent_inc],
    )
    test(t['name'], test_exe, suite: 'unit-test')
endforeach
