 /********************************** (C) COPYRIGHT *******************************
 * File Name          : drv_usart.c
 * Author             : WCH
 * Version            : V1.0
 * Date               : 2022/04/26
 * Description
 *********************************************************************************
 * Copyright (c) 2021 Nanjing Qinheng Microelectronics Co., Ltd.
 * Attention: This software (modified or not) and binary are used for 
 * microcontroller manufactured by Nanjing Qinheng Microelectronics.
 *******************************************************************************/
#include "drv_usart.h"
#include "rtthread.h"
#include "rtdevice.h"
#include <string.h>

#define RT_UART_RINGBUF_SIZE        128
static struct rt_ringbuffer rt_uart_ringbuf;
ALIGN(RT_ALIGN_SIZE) uint8_t rt_uart_buffer[RT_UART_RINGBUF_SIZE];

int usart_init(void)
{
    GPIO_InitTypeDef  GPIO_InitStructure;
    USART_InitTypeDef USART_InitStructure;
    NVIC_InitTypeDef  NVIC_InitStructure = {0};

    rt_ringbuffer_init(&rt_uart_ringbuf, rt_uart_buffer, RT_UART_RINGBUF_SIZE);

    RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART1 | RCC_APB2Periph_GPIOA, ENABLE);

    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_9;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
    GPIO_Init(GPIOA, &GPIO_InitStructure);

    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
    GPIO_Init(GPIOA, &GPIO_InitStructure);

    USART_InitStructure.USART_BaudRate = 115200;
    USART_InitStructure.USART_WordLength = USART_WordLength_8b;
    USART_InitStructure.USART_StopBits = USART_StopBits_1;
    USART_InitStructure.USART_Parity = USART_Parity_No;
    USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
    USART_InitStructure.USART_Mode = USART_Mode_Tx | USART_Mode_Rx;
    USART_Init(USART1, &USART_InitStructure);

    USART_ITConfig(USART1, USART_IT_RXNE, ENABLE);

    NVIC_InitStructure.NVIC_IRQChannel = USART1_IRQn;
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;
    NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
    NVIC_Init(&NVIC_InitStructure);

    USART_Cmd(USART1, ENABLE);

    PRINT("U\n");

    return 0;
}
INIT_BOARD_EXPORT(usart_init);//add to board auto initial



// output
void rt_hw_console_output(const char *str)
{
    rt_size_t i = 0, size = 0;
    char a = '\r';

    size = rt_strlen(str);
    for (i = 0; i < size; i++)
    {
        if (*(str + i) == '\n')
        {
            while (USART_GetFlagStatus(USART1, USART_FLAG_TC) == RESET);
            USART1->DATAR = a;
        }
        while (USART_GetFlagStatus(USART1, USART_FLAG_TC) == RESET);
        USART1->DATAR = *(str + i);
    }
}

//input
char rt_hw_console_getchar(void)
{
    /* Note: the initial value of ch must < 0 */
    rt_uint8_t ch;

    if(rt_ringbuffer_getchar(&rt_uart_ringbuf, &ch) != 0)
    {
        return ch;
    }
    else
    {
        rt_thread_mdelay(10);
        return -1;
    }
    return ch;
}

void USART1_IRQHandler(void)
{
    u8 Res;
    if(USART_GetITStatus(USART1, USART_IT_RXNE) != RESET)
    {
        Res = USART1->DATAR;
        rt_ringbuffer_putchar(&rt_uart_ringbuf, Res);
    }
}
