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

#ifndef _MX_CH58X_GPIO_H_
#define _MX_CH58X_GPIO_H_

/* GPIO Px register */
#define _R32_Px_DIR(N)              R32_P##N##_DIR
#define _R32_Px_PIN(N)              R32_P##N##_PIN
#define _R32_Px_OUT(N)              R32_P##N##_OUT
#define _R32_Px_CLR(N)              R32_P##N##_CLR
#define _R32_Px_PU(N)               R32_P##N##_PU
#define _R32_Px_PD_DRV(N)           R32_P##N##_PD_DRV
#define _R16_Px_INT_EN(N)           R16_P##N##_INT_EN
#define _R16_Px_INT_MODE(N)         R16_P##N##_INT_MODE
#define _R16_Px_INT_IF(N)           R16_P##N##_INT_IF
#define _GPIO_PIN_x(N)              GPIO_Pin_##N
#define _GPIO_x_IRQN(N)             GPIO_##N##_IRQn

#define R16_Px_INT_EN(N)            _R16_Px_INT_EN(N)       // RW, GPIO PA interrupt enable
#define R16_Px_INT_MODE(N)          _R16_Px_INT_MODE(N)     // RW, GPIO PA interrupt mode: 0=level action, 1=edge action
#define R16_Px_INT_IF(N)            _R16_Px_INT_IF(N)       // RW1, GPIO PA interrupt flag
#define R32_Px_DIR(N)               _R32_Px_DIR(N)          //RW,GPIOPxI/Odirection:0=in,1=out
#define R32_Px_PIN(N)               _R32_Px_PIN(N)          //RO,GPIOPxinput
#define R32_Px_OUT(N)               _R32_Px_OUT(N)          //RW,GPIOPxoutput
#define R32_Px_CLR(N)               _R32_Px_CLR(N)          //WZ,GPIOPxclearoutput:0=keep,1=clear
#define R32_Px_PU(N)                _R32_Px_PU(N)           //RW,GPIOPxpullupresistanceenable
#define R32_Px_PD_DRV(N)            _R32_Px_PD_DRV(N)       //RW,PxpulldownforinputorPxdrivingcapabilityforoutput
#define GPIO_PIN_x(N)               _GPIO_PIN_x(N)
#define GPIO_x_IRQN(N)              _GPIO_x_IRQN(N)

#define _GPIOx_IRQHandler(N)        GPIO##N##_IRQHandler
#define GPIOx_IRQHandler(N)         _GPIOx_IRQHandler(N)

/* GPIO操作宏，如果需要在中断使用GPIO，则程序内的打开中断后的GPIO操作应该改为原子操作 */
/* 宏定义中参数为PORT的，需要填入端口，GPIOA，则填入A，而且只能操作一个GPIO端口 */
/* 宏定义中参数为PIN的，需要填入原GPIO的宏，GPIO_Pin_0，则填入GPIO_Pin_0，而且可以操作多个PIN。也可以使用GPIO_Pin_x(0) 宏扩展为GPIO_Pin_0 */

/* 读取引脚电平 */
#define GPIO_READ_PORT_PIN(PORT, PIN)                   ((R32_Px_PIN(PORT)) & (PIN))

/* 设置引脚电平为高 */
#define GPIO_SETBITS(PORT, PIN)                         do{(R32_Px_OUT(PORT)) = ((R32_Px_OUT(PORT)) | (PIN));}while(0)

/* 设置引脚电平为低 */
#define GPIO_RESETBITS(PORT, PIN)                       do{(R32_Px_OUT(PORT)) = ((R32_Px_OUT(PORT)) & (~(PIN)));}while(0)

/* 翻转 GPIO电平 */
#define GPIO_INVERSEBITS(PORT, PIN)                     do{(R32_Px_OUT(PORT)) = ((R32_Px_OUT(PORT)) ^ (PIN));}while(0)

/* 设置GPIO上拉 - R32_Px_PU */
#define GPIO_SET_PU(PORT, PIN)                          do{(R32_Px_PU(PORT)) = ((R32_Px_PU(PORT)) | (PIN));}while(0)

/* 取消GPIO上拉 - R32_Px_PU*/
#define GPIO_RESET_PU(PORT, PIN)                        do{(R32_Px_PU(PORT)) = ((R32_Px_PU(PORT)) & (~(PIN)));}while(0)

/* GPIO方向输出 - R32_Px_DIR */
#define GPIO_SET_DIR(PORT, PIN)                         do{(R32_Px_DIR(PORT)) = ((R32_Px_DIR(PORT)) | (PIN));}while(0)

/* GPIO方向输入 - R32_Px_DIR */
#define GPIO_RESET_DIR(PORT, PIN)                       do{(R32_Px_DIR(PORT)) = ((R32_Px_DIR(PORT)) & (~(PIN)));}while(0)

/* GPIO输入下拉，输出20ma - R32_Px_DIR */
#define GPIO_SET_PD_DRV(PORT, PIN)                      do{(R32_Px_PD_DRV(PORT)) = ((R32_Px_PD_DRV(PORT)) | (PIN));}while(0)

/* GPIO输入不下拉，输出5ma - R32_Px_DIR */
#define GPIO_RESET_PD_DRV(PORT, PIN)                    do{(R32_Px_PD_DRV(PORT)) = ((R32_Px_PD_DRV(PORT)) & (~(PIN)));}while(0)

/* 设置GPIO为浮空输入 */
#define GPIO_MODECFG_IN_FLOATING(PORT, PIN)             do{GPIO_RESET_PD_DRV(PORT, PIN);GPIO_RESET_PU(PORT, PIN);GPIO_RESET_DIR(PORT, PIN);}while(0)

/* 设置GPIO为上拉输入 */
#define GPIO_MODECFG_IN_PU(PORT, PIN)                   do{GPIO_RESET_PD_DRV(PORT, PIN);GPIO_SET_PU(PORT, PIN);GPIO_RESET_DIR(PORT, PIN);}while(0)

/* 设置GPIO为下拉输入 */
#define GPIO_MODECFG_IN_PD(PORT, PIN)                   do{GPIO_SET_PD_DRV(PORT, PIN);GPIO_RESET_PU(PORT, PIN);GPIO_RESET_DIR(PORT, PIN);}while(0)

/* 设置GPIO为5ma输出 */
#define GPIO_MODECFG_OUT_PP_5MA(PORT, PIN)              do{GPIO_RESET_PD_DRV(PORT, PIN);GPIO_SET_DIR(PORT, PIN);}while(0)

/* 设置GPIO为20ma输出 */
#define GPIO_MODECFG_OUT_PP_20MA(PORT, PIN)             do{GPIO_SET_PD_DRV(PORT, PIN);GPIO_SET_DIR(PORT, PIN);}while(0)

/* 设置GPIO低电平中断 */
#define GPIO_ITMODECFG_LOWLEVEL(PORT, PIN)              do{R16_Px_INT_MODE(PORT) &= ~(PIN);R32_Px_CLR(PORT) |= (PIN);R16_Px_INT_IF(PORT) = (PIN);R16_Px_INT_EN(PORT) |= (PIN);}while(0)

/* 设置GPIO高电平中断 */
#define GPIO_ITMODECFG_HIGHLEVEL(PORT, PIN)             do{R16_Px_INT_MODE(PORT) &= ~(PIN);R32_Px_OUT(PORT) |= (PIN);R16_Px_INT_IF(PORT) = (PIN);R16_Px_INT_EN(PORT) |= (PIN);}while(0)

/* 设置GPIO下降沿中断 */
#define GPIO_ITMODECFG_FALLEDGE(PORT, PIN)              do{R16_Px_INT_MODE(PORT) |= (PIN);R32_Px_CLR(PORT) |= (PIN);R16_Px_INT_IF(PORT) = (PIN);R16_Px_INT_EN(PORT) |= (PIN);}while(0)

/* 设置GPIO上升沿中断 */
#define GPIO_ITMODECFG_RISEEDGE(PORT, PIN)              do{R16_Px_INT_MODE(PORT) |= (PIN);R32_Px_OUT(PORT) |= (PIN);R16_Px_INT_IF(PORT) = (PIN);R16_Px_INT_EN(PORT) |= (PIN);}while(0)

#endif /* _MX_CH58X_GPIO_H_ */
