/**
 ******************************************************************************
 * @file    Templates/stm32f7xx_it.c
 * @author  MCD Application Team
 * @brief   Main Interrupt Service Routines.
 *          This file provides template for all exceptions handler and
 *          peripherals interrupt service routine.
 ******************************************************************************
 * @attention
 *
 * Copyright (c) 2016 STMicroelectronics.
 * All rights reserved.
 *
 * This software is licensed under terms that can be found in the LICENSE file
 * in the root directory of this software component.
 * If no LICENSE file comes with this software, it is provided AS-IS.
 *
 ******************************************************************************
 */

/* Includes ------------------------------------------------------------------*/
#include "stm32f7xx_it.h"
#include "main.h"

/** @addtogroup STM32F7xx_HAL_Examples
 * @{
 */

/** @addtogroup Templates
 * @{
 */

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
/* Private function prototypes -----------------------------------------------*/
/* Private functions ---------------------------------------------------------*/

/******************************************************************************/
/*            Cortex-M7 Processor Exceptions Handlers                         */
/******************************************************************************/

/**
 * @brief   This function handles NMI exception.
 * @param  None
 * @retval None
 */
void NMI_Handler(void) { }

/**
 * @brief  This function handles Hard Fault exception.
 * @param  None
 * @retval None
 */
#define SCB_HFSR (*((volatile uint32_t*)0xE000ED2C))  // Hard Fault Status Register
#define SCB_CFSR (*((volatile uint32_t*)0xE000ED28))  // Configurable Fault Status Register
#define SCB_BFSR (*((volatile uint32_t*)0xE000ED29))  // BusFault Status Register
#define SCB_BFAR (*((volatile uint32_t*)0xE000ED38))  // Bus Fault Address Register
#define SCB_MMFSR (*((volatile uint32_t*)0xE000ED28)) // MemManage Status Register
#define SCB_MMFAR (*((volatile uint32_t*)0xE000ED34)) // MemManage Fault Address Register

void HardFault_Handler(void) __attribute__((naked));

void HardFault_Handler(void)
{
    __asm volatile("TST lr, #4 \n" // Check EXC_RETURN to determine if MSP or PSP was used
                   "ITE EQ \n"
                   "MRSEQ r0, MSP \n"         // Move Main Stack Pointer (MSP) into r0
                   "MRSNE r0, PSP \n"         // Otherwise, move Process Stack Pointer (PSP) into r0
                   "B HardFault_Handler_C \n" // Branch to C handler
    );
}

void HardFault_Handler_C(uint32_t* stack_pointer)
{
    uint32_t r0   = stack_pointer[0];
    uint32_t r1   = stack_pointer[1];
    uint32_t r2   = stack_pointer[2];
    uint32_t r3   = stack_pointer[3];
    uint32_t r12  = stack_pointer[4];
    uint32_t lr   = stack_pointer[5]; // Link register
    uint32_t pc   = stack_pointer[6]; // Program counter (faulting instruction)
    uint32_t xPSR = stack_pointer[7]; // xPSR (status register)

    // printf("\n[Hard Fault]\n");
    // printf("R0  = 0x%08lX\n", r0);
    // printf("R1  = 0x%08lX\n", r1);
    // printf("R2  = 0x%08lX\n", r2);
    // printf("R3  = 0x%08lX\n", r3);
    // printf("R12 = 0x%08lX\n", r12);
    // printf("LR  = 0x%08lX\n", lr);
    // printf("PC  = 0x%08lX  <-- Possible cause\n", pc);
    // printf("xPSR = 0x%08lX\n", xPSR);
    //
    // printf("\nFault Status Registers:\n");
    // printf("HFSR = 0x%08lX\n", SCB_HFSR);
    // printf("CFSR = 0x%08lX\n", SCB_CFSR);

    if (SCB_CFSR & 0x80) { }
    if (SCB_CFSR & 0x1) { }

    while (1); // Halt execution
}

/**
 * @brief  This function handles Memory Manage exception.
 * @param  None
 * @retval None
 */
void MemManage_Handler(void)
{
    /* Go to infinite loop when Memory Manage exception occurs */
    while (1) { }
}

/**
 * @brief  This function handles Bus Fault exception.
 * @param  None
 * @retval None
 */
void BusFault_Handler(void)
{
    /* Go to infinite loop when Bus Fault exception occurs */
    while (1) { }
}

/**
 * @brief  This function handles Usage Fault exception.
 * @param  None
 * @retval None
 */
void UsageFault_Handler(void)
{
    /* Go to infinite loop when Usage Fault exception occurs */
    while (1) { }
}

/**
 * @brief  This function handles Debug Monitor exception.
 * @param  None
 * @retval None
 */
void DebugMon_Handler(void) { }

/**
 * @brief  This function handles SysTick Handler.
 * @param  None
 * @retval None
 */
extern void xPortSysTickHandler(void);
void        SysTick_Handler(void)
{
    xPortSysTickHandler();
    HAL_IncTick();
}

/******************************************************************************/
/*                 STM32F7xx Peripherals Interrupt Handlers                   */
/*  Add here the Interrupt Handler for the used peripheral(s) (PPP), for the  */
/*  available peripheral interrupt handler's name please refer to the startup */
/*  file (startup_stm32f7xx.s).                                               */
/******************************************************************************/

/**
 * @brief  This function handles PPP interrupt request.
 * @param  None
 * @retval None
 */
/*void PPP_IRQHandler(void)
{
}*/

/**
 * @}
 */

/**
 * @}
 */
