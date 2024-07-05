/*
 * Copyright (c) 2024, smartmx <smartmx@qq.com>
 *
 * SPDX-License-Identifier: Apache-2.0
 *
 * Change Logs:
 * Date           Author       Notes
 * 2024-04-02     smartmx      the first version
 *
 */

#include "mx_sdk.h"

#ifdef DEBUG

void mx_debug_uart_init(uint32_t baudrate)
{
    uint32_t x;

    GPIO_SETBITS(MX_DEBUG_UART_GPIO_PORT, GPIO_PIN_x(MX_DEBUG_UART_GPIO_PIN));
    GPIO_MODECFG_OUT_PP_5MA(MX_DEBUG_UART_GPIO_PORT, GPIO_PIN_x(MX_DEBUG_UART_GPIO_PIN));
#if MX_DEBUG_UART_GPIO_PIN_REMAP
    R16_PIN_ALTERNATE |= RB_PIN_UART(MX_DEBUG_UART);
#endif

    x = 10 * GetSysClock() / 8 / baudrate;
    x = (x + 5) / 10;
    R16_UARTx_DL(MX_DEBUG_UART) = (uint16_t)x;

    R8_UARTx_FCR(MX_DEBUG_UART) = (2 << 6) | RB_FCR_TX_FIFO_CLR | RB_FCR_RX_FIFO_CLR | RB_FCR_FIFO_EN; // FIFO打开，触发点4字节
    R8_UARTx_LCR(MX_DEBUG_UART) = RB_LCR_WORD_SZ;
    R8_UARTx_IER(MX_DEBUG_UART) = RB_IER_TXD_EN;
    R8_UARTx_DIV(MX_DEBUG_UART) = 1;
}

void print_buf_hex8(uint8_t *data, uint32_t len)
{
    uint32_t slen = 0;
    while(len > slen)
    {
        PRINT("%02x ", data[slen]);
        slen++;
    }
    PRINT("\n");
}

void print_buf_hex16(uint16_t *data, uint32_t len)
{
    uint32_t slen = 0;
    while(len > slen)
    {
        PRINT("%04x ", data[slen]);
        slen++;
    }
    PRINT("\n");
}

void print_buf_hex32(uint32_t *data, uint32_t len)
{
    uint32_t slen = 0;
    while(len > slen)
    {
        PRINT("%08x ", data[slen]);
        slen++;
    }
    PRINT("\n");
}

#endif
