/*
 * Copyright (c) 2025, smartmx <smartmx@qq.com>
 *
 * SPDX-License-Identifier: Apache-2.0
 *
 * Change Logs:
 * Date           Author       Notes
 * 2025-02-21     smartmx      the first version
 *
 */

#ifndef _MX_CH58X_TIMER_H_
#define _MX_CH58X_TIMER_H_

#define _R32_TMRx_CONTROL(N)            R32_TMR##N##_CONTROL
#define _R8_TMRx_CTRL_MOD(N)            R8_TMR##N##_CTRL_MOD
#define _R8_TMRx_CTRL_DMA(N)            R8_TMR##N##_CTRL_DMA
#define _R8_TMRx_INTER_EN(N)            R8_TMR##N##_INTER_EN
//#define _R32_TMRx_STATUS(N)             R32_TMR##N##_STATUS
#define _R8_TMRx_INT_FLAG(N)            R8_TMR##N##_INT_FLAG
#define _R8_TMRx_FIFO_COUNT(N)          R8_TMR##N##_FIFO_COUNT
#define _R32_TMRx_COUNT(N)              R32_TMR##N##_COUNT
#define _R16_TMRx_COUNT(N)              R16_TMR##N##_COUNT
#define _R8_TMRx_COUNT(N)               R8_TMR##N##_COUNT
#define _R32_TMRx_CNT_END(N)            R32_TMR##N##_CNT_END
#define _R32_TMRx_FIFO(N)               R32_TMR##N##_FIFO
#define _R16_TMRx_FIFO(N)               R16_TMR##N##_FIFO
#define _R8_TMRx_FIFO(N)                R8_TMR##N##_FIFO
#define _R32_TMRx_DMA_NOW(N)            R32_TMR##N##_DMA_NOW
#define _R16_TMRx_DMA_NOW(N)            R16_TMR##N##_DMA_NOW
#define _R32_TMRx_DMA_BEG(N)            R32_TMR##N##_DMA_BEG
#define _R16_TMRx_DMA_BEG(N)            R16_TMR##N##_DMA_BEG
#define _R32_TMRx_DMA_END(N)            R32_TMR##N##_DMA_END
#define _R16_TMRx_DMA_END(N)            R16_TMR##N##_DMA_END

#define R32_TMRx_CONTROL(N)             _R32_TMRx_CONTROL(N)
#define R8_TMRx_CTRL_MOD(N)             _R8_TMRx_CTRL_MOD(N)
#define R8_TMRx_CTRL_DMA(N)             _R8_TMRx_CTRL_DMA(N)
#define R8_TMRx_INTER_EN(N)             _R8_TMRx_INTER_EN(N)
//#define R32_TMRx_STATUS(N)              _R32_TMRx_STATUS(N)
#define R8_TMRx_INT_FLAG(N)             _R8_TMRx_INT_FLAG(N)
#define R8_TMRx_FIFO_COUNT(N)           _R8_TMRx_FIFO_COUNT(N)
#define R32_TMRx_COUNT(N)               _R32_TMRx_COUNT(N)
#define R16_TMRx_COUNT(N)               _R16_TMRx_COUNT(N)
#define R8_TMRx_COUNT(N)                _R8_TMRx_COUNT(N)
#define R32_TMRx_CNT_END(N)             _R32_TMRx_CNT_END(N)
#define R32_TMRx_FIFO(N)                _R32_TMRx_FIFO(N)
#define R16_TMRx_FIFO(N)                _R16_TMRx_FIFO(N)
#define R8_TMRx_FIFO(N)                 _R8_TMRx_FIFO(N)
#define R32_TMRx_DMA_NOW(N)             _R32_TMRx_DMA_NOW(N)
#define R16_TMRx_DMA_NOW(N)             _R16_TMRx_DMA_NOW(N)
#define R32_TMRx_DMA_BEG(N)             _R32_TMRx_DMA_BEG(N)
#define R16_TMRx_DMA_BEG(N)             _R16_TMRx_DMA_BEG(N)
#define R32_TMRx_DMA_END(N)             _R32_TMRx_DMA_END(N)
#define R16_TMRx_DMA_END(N)             _R16_TMRx_DMA_END(N)

#define _TMRx_IRQN(N)                   TMR##N##_IRQn
#define TMRx_IRQN(N)                    _TMRx_IRQN(N)

#define _RB_PIN_TMR(N)                  RB_PIN_TMR##N
#define RB_PIN_TMR(N)                   _RB_PIN_TMR(N)

#define _TMRx_IRQHandler(N)             TMR##N##_IRQHandler
#define TMRx_IRQHandler(N)              _TMRx_IRQHandler(N)

#endif /* _MX_CH58X_TIMER_H_ */
