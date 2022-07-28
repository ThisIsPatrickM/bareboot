
gtest_main_dep = gtest_proj.get_variable('gtest_main_dep')
# gtest_dep = gtest_proj.get_variable('gtest_dep')
# gtest_incdir = gtest_proj.get_variable('gtest_incdir')
gmock_dep = gtest_proj.get_variable('gmock_dep')
# gmock_incdir = gtest_proj.get_variable('gmock_incdir')

unit_tests = []
subdir('common_tests')
subdir('crc_tests')
subdir('hash_tests')
subdir('mac_tests')
subdir('memcpy_tests')

### Build and register tests ###
foreach t : unit_tests
    ### executable
    test_exe = executable(t['name'].underscorify()+ '_exe',
        sources             : t['src'],
        dependencies        : [
            t.get('deps', []),
            gtest_main_dep, gmock_dep
        ],
        include_directories : bootloader_inc,
        link_with : [bootloader_lib]
    )
    test(t['name'], test_exe, suite: 'unit-test')
endforeach
