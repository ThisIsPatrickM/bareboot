project('Bootloader', ['c','cpp'],
  version : '0.0.1',
  default_options : [
        'warning_level=3',
        'cpp_std=c++2a',
        'optimization=s',
        'werror=true',
        'b_staticpic=false',
        'b_lto=true',
        'cpp_eh=none',
        'cpp_rtti=false',])

### Prepare Linker Script Dependency
# According to suggestion by phillipjohnston: https://github.com/mesonbuild/meson/issues/6888#issuecomment-607440358
linker_script_flags = []
linker_script_deps = []
foreach entry : meson.get_cross_property('linker_paths', [''])
	if entry != ''
		linker_script_flags += '-L' + meson.source_root() / entry
	endif
endforeach
foreach entry : meson.get_cross_property('linker_scripts', [''])
	if entry != ''
		linker_script_flags += '-T' + entry
	endif
endforeach
foreach entry : meson.get_cross_property('link_depends', [''])
	if entry != ''
		linker_script_deps += meson.source_root() / entry
	endif
endforeach

#####    Collect Source Files and additional includes to platform_parameters.h depending on target    #####
bootloader_sources = []
bootloader_inc = [include_directories('src', 'include')]
subdir('src')

#####    Executable and Library    #####
bootloader_exe = executable('bootloader',
          sources : bootloader_sources,
          link_args: linker_script_flags,
          link_depends : linker_script_deps,
          include_directories: bootloader_inc,
          )

bootloader_lib = static_library('bootloader_lib',
                        bootloader_sources,
                        include_directories : bootloader_inc)

bootloader = declare_dependency(
    link_with           : bootloader_lib,
    include_directories : bootloader_inc,
    dependencies        : [],
)

#####    Tests    #####

# TODO Later only run tests in subproject
if meson.can_run_host_binaries()
# if not meson.is_subproject() and meson.can_run_host_binaries()
    gtest_proj = subproject('gtest')
    subdir('tests')
endif

#####    Clang tidy target    #####



if not meson.is_subproject()

    #####    External Programs    #####
    bash = find_program('bash', required : false)
    python_interpreter = import('python').find_installation('python3', required : false)

    #####    Doxygen    #####
    doxygen = find_program('doxygen', required : false)
    if bash.found() and doxygen.found() and not meson.is_subproject()
        run_target('doxygen',
            command : [
                meson.current_source_dir() + '/utils/build_doxygen.sh',
                meson.current_source_dir() + '/Doxyfile',
                'PROJECT_NAME = ' + meson.project_name(),
                'PROJECT_NUMBER = ' + meson.project_version(),
                'INPUT += include / src/'
            ]
        )
    endif

    #####    clang-format    #####
    clang_format = find_program('clang-format', required : false)
    if bash.found() and python_interpreter.found() and clang_format.found() and not meson.is_subproject()
        run_target('check-clang-format',
            command : [
                meson.current_source_dir() + '/utils/clang-format.sh',
                meson.current_source_dir() + '/utils/run-clang-format.py',
                'include',
                'src',
            ]
        )
    endif

    #####    clang-tidy    #####
    clang_tidy = find_program('clang-tidy', required : false)
    gnu_parallel = find_program('parallel', required : false)
    if bash.found() and clang_tidy.found() and gnu_parallel.found() and not meson.is_subproject()
        run_target('check-clang-tidy',
            command : [
                meson.current_source_dir() + '/utils/clang-tidy.sh',
                '',
                meson.current_build_dir()
            ],
        )
    endif
endif



# This adds the clang format file to the build directory
# configure_file(input : '.clang-format',
#                output : '.clang-format',
# 	       copy: true)
# run_target('format',
#   command : ['clang-format','-i','-style=file',bootloader_sources])

# # This regex excludes any sources from the third_party, tests, benchmarks 
# # and gtest related files.
# regex = '^((?!(third_party|tests|benchmarks|gtest)).)*$'
# # This adds clang tidy support
# configure_file(input : '.clang-tidy',
#                output : '.clang-tidy',
#                copy : true)
# run_target('tidy',
#   command : ['run-clang-tidy-13','-fix', '-j' , '8', 'files', regex ,'-format', '-p='+ meson.build_root()])