/**
 * stdout
 * Created on: 06/03/2025
 * Author mateuszpiesta
 * Company: mprogramming
 */

#pragma once

#include <system_error>
#include <cstdint>
#include <span>

namespace syscalls {
    std::error_code stdout_init();
    void            stdout_deinit();
    std::error_code stdout_write(const char* buffer);
    std::error_code stdout_write(const std::uint8_t* data, std::size_t len);
    std::error_code stdout_write(std::span<const char> data);
} // namespace syscalls
