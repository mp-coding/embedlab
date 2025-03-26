/**
 * board
 * Created on: 18/03/2025
 * Author mateuszpiesta
 * Company: mprogramming
 */

extern "C" {
#include "FreeRTOS.h"
}
#include "task.h"

#include "board.hpp"

#include "stdout.hpp"
#include "stm32746g_discovery.h"
#include <cinttypes>

namespace {
    /**
     * @brief  This function is executed in case of error occurrence.
     * @param  None
     * @retval None
     */
    void Error_Handler()
    {
        /* User may add here some code to deal with this error */
        while (1) { }
    }

    /**
     * @brief  CPU L1-Cache enable.
     * @param  None
     * @retval None
     */
    void CPU_CACHE_Enable()
    {
        /* Enable I-Cache */
        SCB_EnableICache();

        /* Enable D-Cache */
        SCB_EnableDCache();
    }

    /**
     * @brief  Configure the MPU attributes
     * @param  None
     * @retval None
     */
    void MPU_Config()
    {
        MPU_Region_InitTypeDef MPU_InitStruct;

        /* Disable the MPU */
        HAL_MPU_Disable();

        /* Configure the MPU as Strongly ordered for not defined regions */
        MPU_InitStruct.Enable           = MPU_REGION_ENABLE;
        MPU_InitStruct.BaseAddress      = 0x00;
        MPU_InitStruct.Size             = MPU_REGION_SIZE_4GB;
        MPU_InitStruct.AccessPermission = MPU_REGION_NO_ACCESS;
        MPU_InitStruct.IsBufferable     = MPU_ACCESS_NOT_BUFFERABLE;
        MPU_InitStruct.IsCacheable      = MPU_ACCESS_NOT_CACHEABLE;
        MPU_InitStruct.IsShareable      = MPU_ACCESS_SHAREABLE;
        MPU_InitStruct.Number           = MPU_REGION_NUMBER0;
        MPU_InitStruct.TypeExtField     = MPU_TEX_LEVEL0;
        MPU_InitStruct.SubRegionDisable = 0x87;
        MPU_InitStruct.DisableExec      = MPU_INSTRUCTION_ACCESS_DISABLE;

        HAL_MPU_ConfigRegion(&MPU_InitStruct);

        /* Enable the MPU */
        HAL_MPU_Enable(MPU_PRIVILEGED_DEFAULT);
    }

    /**
     * @brief  System Clock Configuration
     *         The system Clock is configured as follow :
     *            System Clock source            = PLL (HSE)
     *            SYSCLK(Hz)                     = 216000000
     *            HCLK(Hz)                       = 216000000
     *            AHB Prescaler                  = 1
     *            APB1 Prescaler                 = 4
     *            APB2 Prescaler                 = 2
     *            HSE Frequency(Hz)              = 25000000
     *            PLL_M                          = 25
     *            PLL_N                          = 432
     *            PLL_P                          = 2
     *            PLL_Q                          = 9
     *            VDD(V)                         = 3.3
     *            Main regulator output voltage  = Scale1 mode
     *            Flash Latency(WS)              = 7
     * @param  None
     * @retval None
     */
    void SystemClock_Config()
    {
        RCC_ClkInitTypeDef RCC_ClkInitStruct;
        RCC_OscInitTypeDef RCC_OscInitStruct;

        /* Enable HSE Oscillator and activate PLL with HSE as source */
        RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSE;
        RCC_OscInitStruct.HSEState       = RCC_HSE_ON;
        RCC_OscInitStruct.HSIState       = RCC_HSI_OFF;
        RCC_OscInitStruct.PLL.PLLState   = RCC_PLL_ON;
        RCC_OscInitStruct.PLL.PLLSource  = RCC_PLLSOURCE_HSE;
        RCC_OscInitStruct.PLL.PLLM       = 25;
        RCC_OscInitStruct.PLL.PLLN       = 432;
        RCC_OscInitStruct.PLL.PLLP       = RCC_PLLP_DIV2;
        RCC_OscInitStruct.PLL.PLLQ       = 9;
        if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK) { Error_Handler(); }

        /* activate the OverDrive to reach the 216 Mhz Frequency */
        if (HAL_PWREx_EnableOverDrive() != HAL_OK) { Error_Handler(); }

        /* Select PLL as system clock source and configure the HCLK, PCLK1 and PCLK2
           clocks dividers */
        RCC_ClkInitStruct.ClockType      = (RCC_CLOCKTYPE_SYSCLK | RCC_CLOCKTYPE_HCLK | RCC_CLOCKTYPE_PCLK1 | RCC_CLOCKTYPE_PCLK2);
        RCC_ClkInitStruct.SYSCLKSource   = RCC_SYSCLKSOURCE_PLLCLK;
        RCC_ClkInitStruct.AHBCLKDivider  = RCC_SYSCLK_DIV1;
        RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV4;
        RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV2;
        if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_7) != HAL_OK) { Error_Handler(); }
    }
} // namespace

extern "C" {
#ifdef USE_FULL_ASSERT

/**
 * @brief  Reports the name of the source file and the source line number
 *         where the assert_param error has occurred.
 * @param  file: pointer to the source file name
 * @param  line: assert_param error line source number
 * @retval None
 */
void assert_failed(uint8_t* file, uint32_t line)
{
    printf("assertion failed, file:%s, line:%" PRIu32, reinterpret_cast<const char*>(file), line);
    /* Infinite loop */
    while (1) { }
}
#endif

StaticTask_t xIdleTaskTCB {};
StackType_t  uxIdleTaskStack[configMINIMAL_STACK_SIZE] {};
void         vApplicationGetIdleTaskMemory(StaticTask_t** ppxIdleTaskTCBBuffer, StackType_t** ppxIdleTaskStackBuffer, configSTACK_DEPTH_TYPE* puxIdleTaskStackSize)
{
    /* If the buffers to be provided to the Idle task are declared inside this
       function then they must be declared static - otherwise they will be allocated on
        the stack and so not exists after this function exits. */

    /* Pass out a pointer to the StaticTask_t structure in which the Idle task's
       state will be stored. */
    *ppxIdleTaskTCBBuffer = &xIdleTaskTCB;

    /* Pass out the array that will be used as the Idle task's stack. */
    *ppxIdleTaskStackBuffer = uxIdleTaskStack;

    /* Pass out the size of the array pointed to by *ppxIdleTaskStackBuffer.
       Note that, as the array is necessarily of type StackType_t,
       configMINIMAL_STACK_SIZE is specified in words, not bytes. */
    *puxIdleTaskStackSize = configMINIMAL_STACK_SIZE;
}

void vApplicationStackOverflowHook(TaskHandle_t, char* pcTaskName) { printf("Stack overflow, task: %s\r\n", pcTaskName); }
}

namespace board {

    std::error_code init()
    {
        MPU_Config();
        CPU_CACHE_Enable();
        HAL_Init();
        /* Configure the System clock to have a frequency of 216 MHz */
        SystemClock_Config();

        BSP_LED_Init(LED1);

        return syscalls::stdout_init();
    }
    std::error_code deinit() { return {}; }
} // namespace board