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

supported_systems = ['va41620', 'linux']

if host_machine.system() in supported_systems

    ### Prepare Linker Script Dependency
    # According to suggestion by phillipjohnston: https://github.com/mesonbuild/meson/issues/6888#issuecomment-607440358
    bootloader_linker_script_flags = []
    bootloader_linker_script_deps = []
    foreach entry : meson.get_cross_property('bootloader_linker_paths', [''])
        if entry != ''
            bootloader_linker_script_flags += '-L' + meson.source_root() / entry
        endif
    endforeach
    foreach entry : meson.get_cross_property('bootloader_linker_scripts', [''])
        if entry != ''
            bootloader_linker_script_flags += '-T' + entry
        endif
    endforeach
    foreach entry : meson.get_cross_property('bootloader_link_depends', [''])
        if entry != ''
            bootloader_linker_script_deps += meson.source_root() / entry
        endif
    endforeach

    #####    Collect Source Files and decide on include paths depending on decisions in src    #####
    # Will be evaluated in src directory, depending on crossfile
    bootloader_sources = []
    bootloader_generic_dependent_inc = include_directories('include/generic')
    bootloader_va41620_dependent_inc = include_directories('include/va41620')
    bootloader_va41620_dev_board_dependent_inc = include_directories('include/va41620_dev_board')
    bootloader_independent_inc = include_directories('src', 'include')
    bootloader_dependent_inc = []
    subdir('src')

    bootloader_inc = [bootloader_independent_inc, bootloader_dependent_inc]

    #####    Executable and Library    #####
    bootloader_exe = executable('bootloader',
            sources : bootloader_sources,
            link_args: bootloader_linker_script_flags,
            link_depends : bootloader_linker_script_deps,
            include_directories: bootloader_inc,
            )

    bootloader_lib = static_library('bootloader_lib',
                            bootloader_sources,
                            include_directories : bootloader_inc)

    bootloader = declare_dependency(
        link_with           : bootloader_lib,
        include_directories : bootloader_inc,
        dependencies        : []
    )

    #####    Tests    #####

    # Activate this to run tests even if this is a subproject
    # if meson.can_run_host_binaries()
    if not meson.is_subproject() and meson.can_run_host_binaries()
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
                    meson.current_source_dir() + '/.gitlab/utils/build_doxygen.sh',
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
                    meson.current_source_dir() + '/.gitlab/utils/clang-format.sh',
                    meson.current_source_dir() + '/.gitlab/utils/run-clang-format.py',
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
                    meson.current_source_dir() + '/.gitlab/utils/clang-tidy.sh',
                    '',
                    meson.current_build_dir()
                ],
            )
        endif
    endif
endif 