/**
 * critical_section
 * Created on: 22/03/2025
 * Author mateuszpiesta
 * Company: mprogramming
 */

#include "osal/critical_section.hpp"

#include <FreeRTOS.h>

namespace osal {

    void critical_section::lock() { portENTER_CRITICAL(); }
    void critical_section::unlock() { portEXIT_CRITICAL(); }
    bool critical_section::try_lock()
    {
        portENTER_CRITICAL();
        return true;
    }
} // namespace osal