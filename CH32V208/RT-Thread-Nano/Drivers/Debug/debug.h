/********************************** (C) COPYRIGHT  *******************************
 * File Name          : debug.h
 * Author             : WCH
 * Version            : V1.0.0
 * Date               : 2021/06/06
 * Description        : This file contains all the functions prototypes for UART
 *                      Printf , Delay functions.
 * Copyright (c) 2021 Nanjing Qinheng Microelectronics Co., Ltd.
 * SPDX-License-Identifier: Apache-2.0
 *******************************************************************************/
#ifndef __DEBUG_H
#define __DEBUG_H

#include "stdio.h"
#include "ch32v20x.h"
#include "printf/printf.h"

/* UART Printf Definition */
#define DEBUG_UART1    1
#define DEBUG_UART2    2
#define DEBUG_UART3    3

/* DEBUG UATR Definition */
#define DEBUG          DEBUG_UART1
//#define DEBUG   DEBUG_UART2
//#define DEBUG   DEBUG_UART3

void USART_Printf_Init(uint32_t baudrate);

void mDelayuS(uint16_t t);

void mDelaymS(uint16_t t);


#define Delay_Ms mDelaymS

#define Delay_Us mDelayuS

#if(DEBUG)
    #define PRINT(format, ...)    printf_(format, ##__VA_ARGS__)
#else
    #define PRINT(X...)
#endif
#endif
