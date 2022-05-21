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


if not meson.is_subproject() and meson.can_run_host_binaries()
    gtest_proj = subproject('gtest')
    # gtest_main_dep = gtest_proj.get_variable('gtest_main_dep')
    # gtest_dep = gtest_proj.get_variable('gtest_dep')
    gtest_incdir = gtest_proj.get_variable('gtest_incdir')

    subdir('tests')

endif

# gtest = subproject('gtest')
# gtest_dep = dependency('gtest', fallback : ['gtest', 'gtest_dep'])
# thread_dep = dependency('threads')


