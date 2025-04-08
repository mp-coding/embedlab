elfsize = find_program('elf-size-analyze', required : false)

if elfsize.found()

    firmware = meson.project_build_root() + '/embedlab'

    program_cmd = [
        elfsize,
        '-t', 'arm-none-eabi-',
        '-w', '120',
        '-HaFo',
        '@INPUT@',
    ]

    custom_target('size',
                  command : program_cmd,
                  input : firmware,
                  output : 'out.txt',
                  depends : embedlab,
                  build_by_default : false,
    )
else
    message('Missing elf-size-analyze, \'size\' target is not available')
endif