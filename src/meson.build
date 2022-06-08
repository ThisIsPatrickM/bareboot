
bootloader_common_sources = files('bootloader.cpp', 'call_constructors.cpp')

bootloader_sources += bootloader_common_sources 


subdir('architecture')
subdir('microcontroller')


# TODO Move up to base meson file. Take care of all my scripts using src
bootloader_exe = executable('bootloader',
          sources : bootloader_sources,
          link_args: linker_args,
          include_directories: bootloader_inc
          )

bootloader_lib = static_library('bootloader_lib',
                        bootloader_sources,
                        include_directories : bootloader_inc)



