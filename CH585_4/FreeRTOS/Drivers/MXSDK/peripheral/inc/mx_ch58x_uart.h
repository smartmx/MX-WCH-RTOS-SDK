/*
 * Copyright (c) 2025, smartmx <smartmx@qq.com>
 *
 * SPDX-License-Identifier: Apache-2.0
 *
 * Change Logs:
 * Date           Author       Notes
 * 2025-02-19     smartmx      the first version
 *
 */

#ifndef _MX_CH58X_UART_H_
#define _MX_CH58X_UART_H_

/* UARTx register */
#define _R32_UARTx_CTRL(N)              R32_UART##N##_CTRL
#define _R8_UARTx_MCR(N)                R8_UART##N##_MCR
#define _R8_UARTx_IER(N)                R8_UART##N##_IER
#define _R8_UARTx_FCR(N)                R8_UART##N##_FCR
#define _R8_UARTx_LCR(N)                R8_UART##N##_LCR
#define _R32_UARTx_STAT(N)              R32_UART##N##_STAT
#define _R8_UARTx_IIR(N)                R8_UART##N##_IIR
#define _R8_UARTx_LSR(N)                R8_UART##N##_LSR
#define _R8_UARTx_MSR(N)                R8_UART##N##_MSR
#define _R32_UARTx_FIFO(N)              R32_UART##N##_FIFO
#define _R8_UARTx_RBR(N)                R8_UART##N##_RBR
#define _R8_UARTx_THR(N)                R8_UART##N##_THR
#define _R8_UARTx_RFC(N)                R8_UART##N##_RFC
#define _R8_UARTx_TFC(N)                R8_UART##N##_TFC
#define _R32_UARTx_SETUP(N)             R32_UART##N##_SETUP
#define _R16_UARTx_DL(N)                R16_UART##N##_DL
#define _R8_UARTx_DLL(N)                R8_UART##N##_DLL
//#define _R8_UARTx_DLM(N)                R8_UART##N##_DLM
#define _R8_UARTx_DIV(N)                R8_UART##N##_DIV
#define _R8_UARTx_ADR(N)                R8_UART##N##_ADR

/* UARTx register */
#define R32_UARTx_CTRL(N)               _R32_UARTx_CTRL(N)      //RW,UARTxcontrol
#define R8_UARTx_MCR(N)                 _R8_UARTx_MCR(N)        //RW,UARTxmodemcontrol
#define R8_UARTx_IER(N)                 _R8_UARTx_IER(N)        //RW,UARTxinterruptenable
#define R8_UARTx_FCR(N)                 _R8_UARTx_FCR(N)        //RW,UARTxFIFOcontrol
#define R8_UARTx_LCR(N)                 _R8_UARTx_LCR(N)        //RW,UARTxlinecontrol
#define R32_UARTx_STAT(N)               _R32_UARTx_STAT(N)      //RO,UARTxstatus
#define R8_UARTx_IIR(N)                 _R8_UARTx_IIR(N)        //RO,UARTxinterruptidentification
#define R8_UARTx_LSR(N)                 _R8_UARTx_LSR(N)        //RO,UARTxlinestatus
#define R8_UARTx_MSR(N)                 _R8_UARTx_MSR(N)        //RO,UARTxmodemstatus
#define R32_UARTx_FIFO(N)               _R32_UARTx_FIFO(N)      //RW,UARTxdataorFIFOport
#define R8_UARTx_RBR(N)                 _R8_UARTx_RBR(N)        //RO,UARTxreceiverbuffer,receivingbyte
#define R8_UARTx_THR(N)                 _R8_UARTx_THR(N)        //WO,UARTxtransmitterholding,transmittalbyte
#define R8_UARTx_RFC(N)                 _R8_UARTx_RFC(N)        //RO,UARTxreceiverFIFOcount
#define R8_UARTx_TFC(N)                 _R8_UARTx_TFC(N)        //RO,UARTxtransmitterFIFOcount
#define R32_UARTx_SETUP(N)              _R32_UARTx_SETUP(N)     //RW,UARTxsetup
#define R16_UARTx_DL(N)                 _R16_UARTx_DL(N)        //RW,UARTxdivisorlatch
#define R8_UARTx_DLL(N)                 _R8_UARTx_DLL(N)        //RW,UARTxdivisorlatchLSBbyte
//#define R8_UARTx_DLM(N)                 _R8_UARTx_DLM(N)        //RW,UARTxdivisorlatchMSBbyte
#define R8_UARTx_DIV(N)                 _R8_UARTx_DIV(N)        //RW,UARTxpre-divisorlatchbyte,onlylow7bit,from1to0/128
#define R8_UARTx_ADR(N)                 _R8_UARTx_ADR(N)        //RW,UARTxslaveaddress:0xFF=disable,other=enable

#define _UARTx_IRQN(N)                  UART##N##_IRQn
#define UARTx_IRQN(N)                   _UARTx_IRQN(N)

#define _RB_PIN_UART(N)                 RB_PIN_UART##N
#define RB_PIN_UART(N)                  _RB_PIN_UART(N)

#define _UARTx_IRQHandler(N)            UART##N##_IRQHandler
#define UARTx_IRQHandler(N)             _UARTx_IRQHandler(N)

#endif /* _MX_CH58X_UART_H_ */
