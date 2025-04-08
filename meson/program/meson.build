openocd = find_program('openocd', required : false)

if openocd.found()

    firmware = meson.project_build_root() + '/embedlab'

    program_cmd = [
        openocd,
        '-f', 'interface/stlink.cfg',
        '-f', 'target/stm32f7x.cfg',
        '-c', 'init',
        '-c', 'reset halt',
        '-c', 'flash write_image erase ' + firmware,
        '-c', 'reset run',
        '-c', 'shutdown'
    ]

    custom_target('program',
                  command : program_cmd,
                  input : firmware,
                  output : 'flashed.log',
                  depends : embedlab,
                  capture : false,
                  build_by_default : false
    )
else
    message('Missing openocd, \'program\' target is not available')
endif