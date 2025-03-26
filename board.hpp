/**
 * board
 * Created on: 18/03/2025
 * Author mateuszpiesta
 * Company: mprogramming
 */

#pragma once

#include <system_error>

namespace board {
    std::error_code init();
    std::error_code deinit();
} // namespace board
