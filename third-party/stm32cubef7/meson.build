_inc = [
    'inc',
    'BSP/STM32746G-Discovery',
    'STM32F7xx_HAL_Driver/Inc',
    'STM32F7xx_HAL_Driver/Inc/Legacy',
    'CMSIS/Device/ST/STM32F7xx/Include',
    'CMSIS/Include',
]

compile_flags = [
    '-DSTM32F746NGHx',
    '-DUSE_HAL_DRIVER',
    '-DSTM32F746xx',
    '-DUSE_FULL_ASSERT',
    '-DDEBUG']

_compile_flags = [
    '-Og',
    '-Wno-unused-parameter',
    '-Wno-unused-variable',
]

add_project_link_arguments('-T' + meson.current_source_dir() + '/linker/linker.ld', language : ['c', 'cpp'])

stm32hal = library(
    'stm32hal',
    cpp_args : [compile_flags, _compile_flags],
    c_args : [compile_flags, _compile_flags],
    sources : [
        'stm32f7xx_hal_msp.c',
        'stm32f7xx_it.c',
        'system_stm32f7xx.c',
        'startup/startup_stm32f746xx.s',

        'BSP/STM32746G-Discovery/stm32746g_discovery.c',
        'BSP/STM32746G-Discovery/stm32746g_discovery_sd.c',

        'STM32F7xx_HAL_Driver/Src/stm32f7xx_hal.c',
        'STM32F7xx_HAL_Driver/Src/stm32f7xx_hal_cortex.c',
        'STM32F7xx_HAL_Driver/Src/stm32f7xx_hal_gpio.c',
        'STM32F7xx_HAL_Driver/Src/stm32f7xx_hal_rcc.c',
        'STM32F7xx_HAL_Driver/Src/stm32f7xx_hal_rcc_ex.c',
        'STM32F7xx_HAL_Driver/Src/stm32f7xx_hal_pwr.c',
        'STM32F7xx_HAL_Driver/Src/stm32f7xx_hal_pwr_ex.c',
        'STM32F7xx_HAL_Driver/Src/stm32f7xx_hal_i2c.c',
        'STM32F7xx_HAL_Driver/Src/stm32f7xx_hal_uart.c',
        'STM32F7xx_HAL_Driver/Src/stm32f7xx_hal_dma.c',
        'STM32F7xx_HAL_Driver/Src/stm32f7xx_hal_sd.c',
        'STM32F7xx_HAL_Driver/Src/stm32f7xx_ll_gpio.c',
        'STM32F7xx_HAL_Driver/Src/stm32f7xx_ll_pwr.c',
        'STM32F7xx_HAL_Driver/Src/stm32f7xx_ll_rcc.c',
        'STM32F7xx_HAL_Driver/Src/stm32f7xx_ll_sdmmc.c',
        'STM32F7xx_HAL_Driver/Src/stm32f7xx_ll_i2c.c',
        'STM32F7xx_HAL_Driver/Src/stm32f7xx_ll_usart.c',
        'STM32F7xx_HAL_Driver/Src/stm32f7xx_ll_dma.c',
    ],
    include_directories : _inc)

stm32hal_dep = declare_dependency(link_whole : stm32hal, include_directories : _inc, compile_args : compile_flags)