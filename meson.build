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


inc = include_directories('src', 'linkerscripts') 
bootloader_sources = []

subdir('src')

# TODO Later only run tests in subproject
if meson.can_run_host_binaries()
# if not meson.is_subproject() and meson.can_run_host_binaries()
    gtest_proj = subproject('gtest')
    gtest_incdir = gtest_proj.get_variable('gtest_incdir')

    subdir('tests')

endif

