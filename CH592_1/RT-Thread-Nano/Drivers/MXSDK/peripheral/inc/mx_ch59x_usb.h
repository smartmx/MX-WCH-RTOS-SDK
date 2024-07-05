/*
 * Copyright (c) 2024, smartmx <smartmx@qq.com>
 *
 * SPDX-License-Identifier: Apache-2.0
 *
 * Change Logs:
 * Date           Author       Notes
 * 2024-03-25     smartmx      the first version
 *
 */

#ifndef _MX_CH59X_USB_H_
#define _MX_CH59X_USB_H_

#define _R32_USB_EPx_CTRL(N)        R32_USB_EP##N##_CTRL
#define _R16_UEPx_DMA(N)            R16_UEP##N##_DMA
#define _R8_UEPx_T_LEN(N)           R8_UEP##N##_T_LEN
#define _R8_UEPx_CTRL(N)            R8_UEP##N##_CTRL
#define _RB_UEPx_TX_EN(N)           RB_UEP##N##_TX_EN
#define _RB_UEPx_RX_EN(N)           RB_UEP##N##_RX_EN
#define _RB_UEPx_BUF_MOD(N)         RB_UEP##N##_BUF_MOD

#define R32_USB_EPx_CTRL(N)         _R32_USB_EPx_CTRL(N)
#define R16_UEPx_DMA(N)             _R16_UEPx_DMA(N)
#define R8_UEPx_T_LEN(N)            _R8_UEPx_T_LEN(N)
#define R8_UEPx_CTRL(N)             _R8_UEPx_CTRL(N)
#define RB_UEPx_TX_EN(N)            _RB_UEPx_TX_EN(N)
#define RB_UEPx_RX_EN(N)            _RB_UEPx_RX_EN(N)
#define RB_UEPx_BUF_MOD(N)          _RB_UEPx_BUF_MOD(N)

#endif /* _MX_CH59X_USB_H_ */
