# Inject FreeRTOS configuration
add_project_arguments('-I' + meson.current_source_dir() + '/freertos_config', language : ['c', 'cpp'])

subdir('third-party/stm32cubef7')
subdir('third-party/tlsf')
subdir('third-party/FreeRTOS-Kernel-11.2.0')

_inc = [
    '.',
]

board = library(
    'board',
    sources : [
        'sdcard_blockdev.cpp',
        'syscalls.cpp',
        'sysheap.cpp',
        'stdout.cpp',
        'stdlib.cpp',
        'critical_section.cpp',
        'board.cpp',
    ],
    include_directories : _inc,
    dependencies : [freertos_dep, stm32hal_dep, tlsf_dep, evfs.get_variable('evfs_dep')]
)

board_dep = declare_dependency(link_whole : board, include_directories : _inc, dependencies : [freertos_dep])