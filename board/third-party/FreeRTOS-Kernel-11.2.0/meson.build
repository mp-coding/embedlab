#TODO:
# 1. Make port/mem configurable

src = [
    'croutine.c',
    'event_groups.c',
    'list.c',
    'queue.c',
    'stream_buffer.c',
    'tasks.c',
    'timers.c',
    'portable/GCC/ARM_CM7/r0p1/port.c',
    'portable/MemMang/heap_3.c'
]

_inc = [
    'include',
    'portable/GCC/ARM_CM7/r0p1/'
]
inc = _inc

libfreertos = library(
    'freertos',
    sources : src,
    include_directories : _inc)

freertos_dep = declare_dependency(link_with : libfreertos, include_directories : inc)