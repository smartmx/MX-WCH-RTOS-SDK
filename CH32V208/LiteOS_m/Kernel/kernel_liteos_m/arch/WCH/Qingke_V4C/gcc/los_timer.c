/*
 * Copyright (c) 2023, smartmx <smartmx@qq.com>
 *
 * SPDX-License-Identifier: Apache-2.0
 *
 * Change Logs:
 * Date           Author       Notes
 * 2023-10-23     smartmx      the first version
 *
 */

#include "los_timer.h"
#include "los_config.h"
#include "los_tick.h"
#include "los_reg.h"
#include "los_sched.h"
#include "core_riscv.h"
#include "los_arch_interrupt.h"
#include "los_arch_timer.h"


#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif /* __cplusplus */
#endif /* __cplusplus */

#define configKERNEL_INTERRUPT_PRIORITY         0

//#define SYSTICK_TICK_CONST  (OS_SYS_CLOCK / LOSCFG_BASE_CORE_TICK_PER_SECOND)

static HWI_PROC_FUNC systick_handler;

extern UINT32 g_intCount;

__attribute__((interrupt("WCH-Interrupt-fast")))
__attribute__((section(".highcode")))
void SysTick_Handler(void)
{
    ArchIntEnter();
    /* Do systick handler registered in HalTickStart. */
    if ((void *)systick_handler != NULL)
    {
        systick_handler();
    }
    SysTick->SR = 0;
    ArchIntExit();
}

extern void SW_Handler(void);


STATIC UINT32 SysTickStart(HWI_PROC_FUNC handler);
STATIC UINT64 SysTickReload(UINT64 nextResponseTime);
STATIC UINT64 SysTickCycleGet(UINT32 *period);
STATIC VOID SysTickLock(VOID);
STATIC VOID SysTickUnlock(VOID);

STATIC ArchTickTimer g_archTickTimer =
{
    .freq = 0,
    .irqNum = SysTicK_IRQn,
    .periodMax = LOSCFG_BASE_CORE_TICK_RESPONSE_MAX,
    .init = SysTickStart,
    .getCycle = SysTickCycleGet,
    .reload = SysTickReload,
    .lock = SysTickLock,
    .unlock = SysTickUnlock,
    .tickHandler = NULL,
};

STATIC UINT32 SysTickStart(HWI_PROC_FUNC handler)
{
    g_archTickTimer.freq = OS_SYS_CLOCK;
    g_cyclesPerTick = OS_SYS_CLOCK / LOSCFG_BASE_CORE_TICK_PER_SECOND;

    SetVTFIRQ((uint32_t)SW_Handler, Software_IRQn, 0, ENABLE);
    SetVTFIRQ((uint32_t)SysTick_Handler, SysTicK_IRQn, 1, ENABLE);

    NVIC_SetPriority(SysTicK_IRQn, 0xf0);
    NVIC_SetPriority(Software_IRQn, 0xf0);
    NVIC_EnableIRQ(SysTicK_IRQn);
    NVIC_EnableIRQ(Software_IRQn);

    systick_handler = handler;

    SysTick->CTLR = 0;
    SysTick->SR = 0;
    SysTick->CMP = g_cyclesPerTick - 1;
    SysTick->CNT = 0;
    SysTick->CTLR = 0xf;

    return LOS_OK;
}

STATIC UINT64 SysTickReload(UINT64 nextResponseTime)
{
    SysTick->CTLR &= ~(1 << 0);
    SysTick->CMP  = nextResponseTime - 1;
    SysTick->CNT  = 0;
    SysTick->SR  = 0;
    NVIC_ClearPendingIRQ(SysTicK_IRQn);
    SysTick->CTLR |= (1 << 0);
    return nextResponseTime;
}

STATIC UINT64 SysTickCycleGet(UINT32 *period)
{
    UINT64 ticks;
    UINT32 intSave = LOS_IntLock();
    ticks = SysTick->CNT;
    *period = SysTick->CMP;

    LOS_IntRestore(intSave);
    return ticks;
}

STATIC VOID SysTickLock(VOID)
{
    SysTick->CTLR &= ~(1 << 0);
}

STATIC VOID SysTickUnlock(VOID)
{
    SysTick->CTLR |= (1 << 0);
}

ArchTickTimer *ArchSysTickTimerGet(VOID)
{
    return &g_archTickTimer;
}

UINT32 ArchEnterSleep(VOID)
{
    __WFI();

    return LOS_OK;
}

#ifdef __cplusplus
#if __cplusplus
}
#endif /* __cplusplus */
#endif /* __cplusplus */
