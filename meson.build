project('embedlab', 'c', 'cpp',
        version : '1.0.0',
        default_options : ['default_library=static', 'warning_level=2', 'cpp_std=c++23', 'c_std=c2x', 'b_pch=false',
                           'b_staticpic=false',
                           'b_pie=false',
                           'werror=true',
                           'build.werror=true'])

# Inject custom stdlib headers
add_global_arguments('-I../stdlib', language : ['c', 'cpp'])
# Inject missing system headers
add_global_arguments('-I../subprojects/evfs/adapters/sys', language : ['c', 'cpp'])
# Inject FreeRTOS configuration
add_project_arguments('-I../freertos_config', language : ['c', 'cpp'])

add_project_arguments('-fno-use-cxa-atexit', '-fno-rtti', language : 'cpp')
add_project_arguments(['-fdata-sections', '-ffunction-sections', '-fstack-usage'], language : ['c', 'cpp'])
add_project_link_arguments('-Wl,--gc-sections', language : ['c', 'cpp'])

subdir('third-party/stm32cubef7')
subdir('third-party/tlsf')
subdir('third-party/FreeRTOS-Kernel-11.2.0')


evfs = subproject('evfs')
lua = subproject('lua')

src = [
    'main.cpp',
    'sdcard_blockdev.cpp',
    'syscalls.cpp',
    'sysheap.cpp',
    'stdout.cpp',
    'stdlib.cpp',
    'critical_section.cpp',
    'board.cpp',
]

inc = []

executable('embedlab', src,
           include_directories : inc,
           dependencies : [
               libtlsf_dep,
               stm32hal_dep,
               freertos_dep,
               evfs.get_variable('evfs_dep'),
               evfs.get_variable('evfs_syscalls_newlib_dep'),
               lua.get_variable('lua_dep')],
)