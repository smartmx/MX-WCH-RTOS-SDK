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

#include "los_task.h"
#include "los_memory.h"
#include "los_timer.h"
#include "los_sched.h"
#include "los_interrupt.h"
#include "ch32v20x.h"

#include "los_arch_context.h"
#include "los_arch_interrupt.h"
#include "los_arch_timer.h"
#include "riscv_bits.h"
#include "riscv_encoding.h"

extern VOID HalHwiInit(VOID);

#define ALIGN_DOWN(size, align)         ((size) & ~((align) - 1))

#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif /* __cplusplus */
#endif /* __cplusplus */

LITE_OS_SEC_TEXT_INIT VOID ArchInit(VOID)
{
    HalHwiInit();
}

LITE_OS_SEC_TEXT_MINOR VOID ArchSysExit(VOID)
{
    LOS_IntLock();
    while (1)
    {
    }
}

LITE_OS_SEC_TEXT_INIT VOID *ArchTskStackInit(UINT32 taskID, UINT32 stackSize, VOID *topStack)
{
    UINT32 index;
    UINT8 *stk = 0;
    TaskContext  *context = NULL;

    /* initialize the task stack, write magic num to stack top */
    *((UINT32 *)(topStack)) = OS_TASK_MAGIC_WORD;

    stk = ((UINT8 *)topStack) + stackSize + sizeof(STACK_TYPE);
    stk = (UINT8 *)ALIGN_DOWN((uintptr_t)stk, REGBYTES);
    context = (TaskContext *)(stk - sizeof(TaskContext));

    for (index = 1; index < sizeof(TaskContext) / sizeof(STACK_TYPE); index ++)
    {
        ((STACK_TYPE *)context)[index] = OS_TASK_STACK_INIT;
    }
    context->ra      = (STACK_TYPE)ArchSysExit;
    context->a0      = (STACK_TYPE)taskID;
    context->epc     = (STACK_TYPE)OsTaskEntry;

    return (VOID *)context;
}

extern LosTask g_losTask;
LITE_OS_SEC_TEXT_INIT UINT32 ArchStartSchedule(VOID)
{
    UINT32 ret;

    (VOID)LOS_IntLock();

    OsSchedStart();

    HalStartToRun();

    return LOS_OK; /* never return */
}

__attribute__((section(".highcode")))
VOID ArchTaskSchedule(VOID)
{
//    PRINT("sw\r\n");
    NVIC_SetPendingIRQ(Software_IRQn);
}

__attribute__((section(".highcode")))
VOID HalTaskSwitch(VOID)
{
    OsSchedTaskSwitch();
    /* Set newTask to runTask */
    g_losTask.runTask = g_losTask.newTask;
}

#ifdef __cplusplus
#if __cplusplus
}
#endif /* __cplusplus */
#endif /* __cplusplus */
