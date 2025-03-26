/**
 * critical_section
 * Created on: 22/03/2025
 * Author mateuszpiesta
 * Company: mprogramming
 */

#pragma once

namespace osal {
    class critical_section {
    public:
        critical_section() noexcept = default;

        critical_section(const critical_section&)            = delete;
        critical_section& operator=(const critical_section&) = delete;

        using native_handle_type = void;
        void lock();
        void unlock();
        bool               try_lock();
    };
} // namespace osal
