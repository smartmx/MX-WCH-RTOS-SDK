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

#ifndef _MX_CH59X_DEBUG_H_
#define _MX_CH59X_DEBUG_H_

#ifdef DEBUG

/* 串口0相关定义 */
#if (DEBUG == 0)            /* 串口0默认引脚 */
#define MX_DEBUG_UART                   0
#define MX_DEBUG_UART_GPIO_PORT         B
#define MX_DEBUG_UART_GPIO_PIN          7
#define MX_DEBUG_UART_GPIO_PIN_REMAP    0
#elif ((DEBUG % 10) == 0)   /* 串口0重映射引脚 */
#define MX_DEBUG_UART                   0
#define MX_DEBUG_UART_GPIO_PORT         A
#define MX_DEBUG_UART_GPIO_PIN          14
#define MX_DEBUG_UART_GPIO_PIN_REMAP    1
#endif

/* 串口1相关定义 */
#if (DEBUG == 1)            /* 串口1默认引脚 */
#define MX_DEBUG_UART                   1
#define MX_DEBUG_UART_GPIO_PORT         A
#define MX_DEBUG_UART_GPIO_PIN          9
#define MX_DEBUG_UART_GPIO_PIN_REMAP    0
#elif ((DEBUG % 10) == 1)   /* 串口1重映射引脚 */
#define MX_DEBUG_UART                   1
#define MX_DEBUG_UART_GPIO_PORT         B
#define MX_DEBUG_UART_GPIO_PIN          13
#define MX_DEBUG_UART_GPIO_PIN_REMAP    1
#endif

/* 串口2相关定义 */
#if (DEBUG == 2)            /* 串口2默认引脚 */
#define MX_DEBUG_UART                   2
#define MX_DEBUG_UART_GPIO_PORT         A
#define MX_DEBUG_UART_GPIO_PIN          7
#define MX_DEBUG_UART_GPIO_PIN_REMAP    0
#elif ((DEBUG % 10) == 2)   /* 串口2重映射引脚 */
#define MX_DEBUG_UART                   2
#define MX_DEBUG_UART_GPIO_PORT         B
#define MX_DEBUG_UART_GPIO_PIN          23
#define MX_DEBUG_UART_GPIO_PIN_REMAP    1
#endif

/* 串口3相关定义 */
#if (DEBUG == 3)            /* 串口3默认引脚 */
#define MX_DEBUG_UART                   3
#define MX_DEBUG_UART_GPIO_PORT         A
#define MX_DEBUG_UART_GPIO_PIN          5
#define MX_DEBUG_UART_GPIO_PIN_REMAP    0
#elif ((DEBUG % 10) == 3)   /* 串口3重映射引脚 */
#define MX_DEBUG_UART                   3
#define MX_DEBUG_UART_GPIO_PORT         B
#define MX_DEBUG_UART_GPIO_PIN          21
#define MX_DEBUG_UART_GPIO_PIN_REMAP    1
#endif

extern void mx_debug_uart_init(uint32_t baudrate);

extern void print_buf_hex8(uint8_t *data, uint32_t len);

extern void print_buf_hex16(uint16_t *data, uint32_t len);

extern void print_buf_hex32(uint32_t *data, uint32_t len);

#endif

#endif /* _MX_CH59X_UART_H_ */
