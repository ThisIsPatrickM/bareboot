project('BootloaderPlayground', ['c','cpp'],
  version : '0.1',
  default_options : ['warning_level=3',
        'cpp_std=c++2a',
        'optimization=s',
        'warning_level=3',
        'werror=true',
        'b_staticpic=false',
        'b_lto=true',
        'cpp_eh=none',
        'cpp_rtti=false',])


bootloader_inc = include_directories('src', 'include') 
bootloader_sources = []

subdir('src')

# TODO Later only run tests in subproject
if meson.can_run_host_binaries()
# if not meson.is_subproject() and meson.can_run_host_binaries()
    gtest_proj = subproject('gtest')
    gtest_incdir = gtest_proj.get_variable('gtest_incdir')

    subdir('tests')

endif

# This adds the clang format file to the build directory
configure_file(input : '.clang-format',
               output : '.clang-format',
	       copy: true)
run_target('format',
  command : ['clang-format','-i','-style=file',bootloader_sources])

# This regex excludes any sources from the third_party, tests, benchmarks 
# and gtest related files.
regex = '^((?!(third_party|tests|benchmarks|gtest)).)*$'
# This adds clang tidy support
configure_file(input : '.clang-tidy',
               output : '.clang-tidy',
               copy : true)
run_target('tidy',
  command : ['run-clang-tidy-13','-fix', '-j' , '8', 'files', regex ,'-format', '-p='+ meson.build_root()])