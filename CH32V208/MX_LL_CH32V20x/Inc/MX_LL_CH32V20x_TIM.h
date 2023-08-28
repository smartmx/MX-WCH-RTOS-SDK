/*
 * Copyright (c) 2023, smartmx <smartmx@qq.com>
 *
 * SPDX-License-Identifier: Apache-2.0
 *
 * Change Logs:
 * Date           Author       Notes
 * 2023-08-28     smartmx      the first version
 *
 */

#ifndef _MX_LL_CH32V20X_TIM_H_
#define _MX_LL_CH32V20X_TIM_H_

/* 个人常用的一些宏定义改写汇总 */

#define _TIM_CHxCVR(X)                  CH##X##CVR
#define _TIM_IT_CCx(X)                  TIM_IT_CC##X
#define _TIMx_IRQn(X)                   TIM##X##_IRQn
#define _TIMx_IRQ_DEF(X)                void TIM##X##_IRQHandler(void)
#define _TIMx(X)                        TIM##X

#define TIM_CHxCVR(X)                   _TIM_CHxCVR(X)
#define TIM_IT_CCx(X)                   _TIM_IT_CCx(X)
#define TIMx_IRQn(X)                    _TIMx_IRQn(X)
#define TIMx_IRQ_DEF(X)                 _TIMx_IRQ_DEF(X)
#define TIMx(X)                         _TIMx(X)

#endif  /* _MX_LL_CH32V20X_TIM_H_ */
