
# TODO Split tests into different Test Groups

common_tests_sources = [
    'basic.cpp',
    'memcpy_tests.cpp',
    'crc_tests.cpp',
]

common_tests_deps = [

]

common_tests_lib_deps = [
    bootloader_lib
]

common_tests = executable('common_tests', 
    [common_tests_sources, test_main], 
    include_directories : bootloader_inc,
    dependencies : [common_tests_deps,test_deps], 
    link_with : [common_tests_lib_deps, test_dep_libs])

test('Common Tests', common_tests)

all_test_deps += common_tests_deps
all_test_dep_libs += common_tests_lib_deps
