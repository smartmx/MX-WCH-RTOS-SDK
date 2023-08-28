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

#ifndef _MX_LL_CH32V20X_DMA_H_
#define _MX_LL_CH32V20X_DMA_H_

#define _DMAx_CHANNELy(X,Y)             DMA##X##_Channel##Y
#define _DMAxy_IRQn(X,Y)                DMA##X##_Channel##Y##_IRQn
#define _DMAxy_IRQ_DEF(X,Y)             void DMA##X##_Channel##Y##_IRQHandler(void)
#define _DMA_TEIFx(X)                   DMA_TEIF##X
#define _DMA_HTIFx(X)                   DMA_HTIF##X
#define _DMA_TCIFx(X)                   DMA_TCIF##X
#define _DMA_GIFx(X)                    DMA_GIF##X
#define _DMAx_IT_TEy(X,Y)               DMA##X##_IT_TE##Y
#define _DMAx_IT_HTy(X,Y)               DMA##X##_IT_HT##Y
#define _DMAx_IT_TCy(X,Y)               DMA##X##_IT_TC##Y
#define _DMAx_IT_GLy(X,Y)               DMA##X##_IT_GL##Y
#define _DMAx_FLAG_TEy(X,Y)             DMA##X##_FLAG_TE##Y
#define _DMAx_FLAG_HTy(X,Y)             DMA##X##_FLAG_HT##Y
#define _DMAx_FLAG_TCy(X,Y)             DMA##X##_FLAG_TC##Y
#define _DMAx_FLAG_GLy(X,Y)             DMA##X##_FLAG_GL##Y
#define _DMAx(X)                        DMA##X

#define DMAx_CHANNELy(X,Y)              _DMAx_CHANNELy(X,Y)
#define DMAxy_IRQn(X,Y)                 _DMAxy_IRQn(X,Y)
#define DMAxy_IRQ_DEF(X,Y)              _DMAxy_IRQ_DEF(X,Y)
#define DMA_TEIFx(X)                    _DMA_TEIFx(X)
#define DMA_HTIFx(X)                    _DMA_HTIFx(X)
#define DMA_TCIFx(X)                    _DMA_TCIFx(X)
#define DMA_GIFx(X)                     _DMA_GIFx(X)
#define DMAx_IT_TEy(X,Y)                _DMAx_IT_TEy(X,Y)
#define DMAx_IT_HTy(X,Y)                _DMAx_IT_HTy(X,Y)
#define DMAx_IT_TCy(X,Y)                _DMAx_IT_TCy(X,Y)
#define DMAx_IT_GLy(X,Y)                _DMAx_IT_GLy(X,Y)
#define DMAx_FLAG_TEy(X,Y)              _DMAx_FLAG_TEy(X,Y)
#define DMAx_FLAG_HTy(X,Y)              _DMAx_FLAG_HTy(X,Y)
#define DMAx_FLAG_TCy(X,Y)              _DMAx_FLAG_TCy(X,Y)
#define DMAx_FLAG_GLy(X,Y)              _DMAx_FLAG_GLy(X,Y)
#define DMAx(X)                         _DMAx(X)

#endif  /* _MX_LL_CH32V20X_DMA_H_ */
