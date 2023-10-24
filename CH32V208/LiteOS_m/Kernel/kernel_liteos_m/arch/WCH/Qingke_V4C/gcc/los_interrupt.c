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

#include <stdio.h>
#include <stdarg.h>
#include "los_arch.h"
#include "los_task.h"
#include "los_debug.h"
#include "ch32v20x.h"
#include "los_arch_context.h"
#include "los_arch_interrupt.h"

#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif /* __cplusplus */
#endif /* __cplusplus */

UINT32 g_intCount = 0;

STATIC UINT32 HwiUnmask(HWI_HANDLE_T hwiNum)
{
    if (hwiNum >= OS_HWI_MAX_NUM) {
        return OS_ERRNO_HWI_NUM_INVALID;
    }

    NVIC_EnableIRQ(hwiNum);

    return LOS_OK;
}

STATIC UINT32 HwiMask(HWI_HANDLE_T hwiNum)
{
    if (hwiNum >= OS_HWI_MAX_NUM) {
        return OS_ERRNO_HWI_NUM_INVALID;
    }

    NVIC_DisableIRQ(hwiNum);

    return LOS_OK;
}

STATIC UINT32 HwiSetPriority(HWI_HANDLE_T hwiNum, UINT8 priority)
{
    if (hwiNum >= OS_HWI_MAX_NUM) {
        return OS_ERRNO_HWI_NUM_INVALID;
    }

//    if (priority > OS_HWI_PRIO_HIGHEST || priority < OS_HWI_PRIO_LOWEST) {
//        return OS_ERRNO_HWI_PRIO_INVALID;
//    }

    NVIC_SetPriority(hwiNum, (UINT8)priority);

    return LOS_OK;
}

// LosExcInfo g_excInfo;
LITE_OS_SEC_TEXT_INIT VOID HalHwiInit(VOID)
{
    // already setup interrupt vectors
}

/*****************************************************************************
 Function    : ArchHwiCreate
 Description : create hardware interrupt
 Input       : hwiNum       --- hwi num to create
               hwiPrio      --- priority of the hwi
               hwiMode      --- hwi interrupt hwiMode, between vector or non-vector
               hwiHandler   --- hwi handler
               irqParam     --- set trig hwiMode of the hwi handler
                                Level Triggerred = 0
                                Postive/Rising Edge Triggered = 1
                                Negtive/Falling Edge Triggered = 3
 Output      : None
 Return      : LOS_OK on success or error code on failure
 *****************************************************************************/
UINT32 ArchHwiCreate(HWI_HANDLE_T hwiNum,
                     HWI_PRIOR_T hwiPrio,
                     HWI_MODE_T hwiMode,
                     HWI_PROC_FUNC hwiHandler,
                     HwiIrqParam *irqParam)
{
    if (hwiNum > SOC_INT_MAX)
    {
        return OS_ERRNO_HWI_NUM_INVALID;
    }
//    if (mode > ECLIC_VECTOR_INTERRUPT){
//        return OS_ERRNO_HWI_MODE_INVALID;
//    }
//    if (arg > ECLIC_NEGTIVE_EDGE_TRIGGER){
//        return OS_ERRNO_HWI_ARG_INVALID;
//    }

    NVIC_SetPriority(hwiNum, (UINT8)hwiPrio);
    NVIC_EnableIRQ(hwiNum);

    return LOS_OK;
}

/*****************************************************************************
 Function    : ArchHwiDelete
 Description : Delete hardware interrupt
 Input       : hwiNum   --- hwi num to delete
               irqParam --- param of the hwi handler
 Return      : LOS_OK on success or error code on failure
 *****************************************************************************/
LITE_OS_SEC_TEXT UINT32 ArchHwiDelete(HWI_HANDLE_T hwiNum, HwiIrqParam *irqParam)
{
    NVIC_DisableIRQ(hwiNum);
    return LOS_OK;
}

/* ****************************************************************************
 Function    : HalHwiDefaultHandler
 Description : default handler of the hardware interrupt
 Input       : None
 Output      : None
 Return      : None
 **************************************************************************** */
LITE_OS_SEC_TEXT_INIT VOID HalHwiDefaultHandler(VOID)
{
    PRINT_ERR("default handler\n");
    while (1)
    {
    }
}

/* ****************************************************************************
 Function    : HalDisplayTaskInfo
 Description : display the task list
 Input       : None
 Output      : None
 Return      : None
 **************************************************************************** */
VOID HalDisplayTaskInfo(VOID)
{
    TSK_INFO_S taskInfo;
    UINT32 index;
    UINT32 ret;

    PRINTK("ID  Pri    Status     name \r\n");
    PRINTK("--  ---    ---------  ----\r\n");

    for (index = 0; index < LOSCFG_BASE_CORE_TSK_LIMIT; index++)
    {
        ret = LOS_TaskInfoGet(index, &taskInfo);
        if (ret != LOS_OK)
        {
            continue;
        }
        PRINTK("%d    %d     %s      %s \r\n",
               taskInfo.uwTaskID, taskInfo.usTaskPrio, OsConvertTskStatus(taskInfo.usTaskStatus), taskInfo.acName);
    }
    return;
}

/* ****************************************************************************
 Function    : HalUnalignedAccessFix
 Description : Unaligned acess fixes are not supported by default
 Input       : None
 Output      : None
 Return      : None
 **************************************************************************** */
WEAK UINT32 HalUnalignedAccessFix(UINTPTR mcause, UINTPTR mepc, UINTPTR mtval, VOID *sp)
{
    /* Unaligned acess fixes are not supported by default */
    PRINTK("Unaligned acess fixes are not support by default!\r\n");
    return LOS_NOK;
}

__attribute__((section(".highcode")))
VOID ArchIntEnter(VOID)
{
    g_intCount += 1;
}

__attribute__((section(".highcode")))
VOID ArchIntExit(VOID)
{
    g_intCount -= 1;
}

__attribute__((section(".highcode")))
UINT32 ArchIsIntActive(VOID)
{
    return (g_intCount > 0);
}

HwiControllerOps g_archHwiOps = {
    .enableIrq      = HwiUnmask,
    .disableIrq     = HwiMask,
    .setIrqPriority = HwiSetPriority,
};

#ifdef __cplusplus
#if __cplusplus
}
#endif /* __cplusplus */
#endif /* __cplusplus */
