/**
 * stdout
 * Created on: 06/03/2025
 * Author mateuszpiesta
 * Company: mprogramming
 */

#include "stdout.hpp"

#include "stm32746g_discovery.h"
#include <cerrno>
#include <cstring>

namespace syscalls {

    UART_HandleTypeDef handle {};
    bool               is_initialized {};

    std::error_code stdout_init()
    {
        handle.Init.BaudRate   = 115200;
        handle.Init.WordLength = UART_WORDLENGTH_8B;
        handle.Init.StopBits   = UART_STOPBITS_1;
        handle.Init.Parity     = UART_PARITY_NONE;
        handle.Init.HwFlowCtl  = UART_HWCONTROL_NONE;
        handle.Init.Mode       = UART_MODE_RX | UART_MODE_TX;
        BSP_COM_Init(COM1, &handle);
        is_initialized = true;
        return {};
    }
    void stdout_deinit()
    {
        BSP_COM_DeInit(COM1, &handle);
        is_initialized = false;
    }
    std::error_code stdout_write(const char* buffer) { return stdout_write(reinterpret_cast<const std::uint8_t*>(buffer), std::strlen(buffer)); }
    std::error_code stdout_write(const std::uint8_t* data, std::size_t len)
    {
        if (not is_initialized) { return std::error_code {ENXIO, std::generic_category()}; }
        return HAL_UART_Transmit(&handle, data, len, 50) == HAL_OK ? std::error_code {} : std::error_code {EIO, std::generic_category()};
    }
    std::error_code stdout_write(std::span<const char> data) { return stdout_write(reinterpret_cast<const std::uint8_t*>(data.data()), data.size()); }
} // namespace syscalls
