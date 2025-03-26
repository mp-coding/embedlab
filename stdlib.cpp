/**
 * stdlib
 * Created on: 21/03/2025
 * Author mateuszpiesta
 * Company: mprogramming
 */

#include "stdlib/mutex"

#include <FreeRTOS.h>
#include <semphr.h>
#include <cmsis_gcc.h>

#include <system_error>

namespace {
    bool is_irq() { return (__get_xPSR() & (1 << 7)) != 0; }

    std::system_error make_syserr(const int err) { return {std::error_code {err, std::generic_category()}}; }
} // namespace

namespace std {
    template <bool recursive> mutex_helper<recursive>::mutex_helper() noexcept
    {
        static_assert(sizeof storage >= sizeof(StaticSemaphore_t));
        const auto s = new (&storage) StaticSemaphore_t();
        if constexpr (recursive) {
            handle = xSemaphoreCreateRecursiveMutexStatic(s);
        } else {
            handle = xSemaphoreCreateMutexStatic(s);
        }
    }
    template <bool recursive> mutex_helper<recursive>::~mutex_helper() = default;

    template <bool recursive> void mutex_helper<recursive>::lock()
    {
        if (is_irq()) { return; }
        if (xTaskGetSchedulerState() == taskSCHEDULER_NOT_STARTED) { return; }

        if constexpr (recursive) {
            if (const auto result = xSemaphoreTakeRecursive(static_cast<SemaphoreHandle_t>(handle), portMAX_DELAY); not result) { throw make_syserr(ENOMEM); }
        } else {
            if (const auto result = xSemaphoreTake(static_cast<SemaphoreHandle_t>(handle), portMAX_DELAY); not result) { throw make_syserr(ENOMEM); }
        }
    }
    template <bool recursive> void mutex_helper<recursive>::unlock()
    {
        if (is_irq()) { return; }
        if (xTaskGetSchedulerState() == taskSCHEDULER_NOT_STARTED) { return; }

        if constexpr (recursive) {
            if (const auto result = xSemaphoreGiveRecursive(static_cast<SemaphoreHandle_t>(handle)); not result) { throw make_syserr(ENOMEM); }
        } else {
            if (const auto result = xSemaphoreGive(static_cast<SemaphoreHandle_t>(handle)); not result) { throw make_syserr(ENOMEM); }
        }
    }
    template <bool recursive> bool mutex_helper<recursive>::try_lock()
    {
        if (is_irq()) { return true; }
        if (xTaskGetSchedulerState() == taskSCHEDULER_NOT_STARTED) { return true; }

        if constexpr (recursive) { return xSemaphoreTakeRecursive(static_cast<SemaphoreHandle_t>(handle), 0); }
        return xSemaphoreTake(static_cast<SemaphoreHandle_t>(handle), 0);
    }

    template class mutex_helper<true>;
    template class mutex_helper<false>;

} // namespace std
