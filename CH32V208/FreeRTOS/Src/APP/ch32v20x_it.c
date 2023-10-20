/********************************** (C) COPYRIGHT *******************************
 * File Name          : ch32v20x_it.c
 * Author             : WCH
 * Version            : V1.0.0
 * Date               : 2022/06/16
 * Description        : Main Interrupt Service Routines.
 * Copyright (c) 2022 Nanjing Qinheng Microelectronics Co., Ltd.
 * SPDX-License-Identifier: Apache-2.0
 *******************************************************************************/

/*********************************************************************
 * INCLUDES
 */
#include "ch32v20x_it.h"
#include "CONFIG.h"

/*********************************************************************
 * LOCAL FUNCTIONS
 */
/* 这几个函数在本FreeRTOS例程中，在startup_ch32v20x_D8W.S中的_vector_base中，不从统一入口中断函数处理，所以需要声明  __attribute__((interrupt("WCH-Interrupt-fast")))*/
void NMI_Handler(void) __attribute__((interrupt("WCH-Interrupt-fast")));
void HardFault_Handler(void) __attribute__((interrupt("WCH-Interrupt-fast")));
void Break_Point_Handler(void) __attribute__((interrupt("WCH-Interrupt-fast")));

/*********************************************************************
 * @fn      NMI_Handler
 *
 * @brief   This function handles NMI exception.
 *
 * @return  None
 */
void NMI_Handler(void)
{
    PRINT("NMI\n");
    while (1)
    {
    }
}

/*********************************************************************
 * @fn      HardFault_Handler
 *
 * @brief   This function handles Hard Fault exception.
 *
 * @return  None
 */
void HardFault_Handler(void)
{
    PRINT("HardFault\n");
    while (1)
    {
    }
}

void Break_Point_Handler()
{
    PRINT("Break_Point\n");
    while (1)
    {
    }
}
