
spi_sources = files('spi/hal_spi.cpp', 'spi/hw_hal_spi.cpp')

bootloader_common_sources = files('startup.cpp', 'bootloader.cpp', 'call_constructors.cpp', 'weak_irq_handlers.cpp')


# Platform dependent 
bootloader_armv7_sources = files('platforms' / 'armv7' / 'start_app.cpp')
bootloader_x64_sources = files('platforms' / 'x64' / 'start_app.cpp')

linker_args = []

if meson.is_cross_build()
	  linker_args = ['-T' + meson.current_source_dir() / 'platforms' / 'armv7' / 'bootloader.ld']
    bootloader_sources += bootloader_common_sources + bootloader_armv7_sources + spi_sources
else
    bootloader_sources += bootloader_common_sources + bootloader_x64_sources + spi_sources
endif

bootloader_exe = executable('bootloader',
          sources : bootloader_sources,
          link_args: linker_args,
          include_directories: inc
          )

bootloader_lib = static_library('bootloader_lib',
                        bootloader_sources,
                        include_directories : inc)



