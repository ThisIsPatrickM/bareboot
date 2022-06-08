all_test_deps = []
all_test_dep_libs = []
project_test_sources = []

gtest_dep = gtest_proj.get_variable('gtest_dep')
thread_dep = dependency('threads')
test_main = files('tests_main.cpp')

test_deps = [
    gtest_dep, 
    thread_dep
]

test_dep_libs = [
]

subdir('common_tests')

# This executable contains all the tests
project_test_sources += test_main
all_test_deps += test_deps
all_test_dep_libs += test_dep_libs

all_tests = executable('all_tests', 
    project_test_sources, 
    include_directories : bootloader_inc, 
    dependencies : all_test_deps, 
    link_with : all_test_dep_libs)
