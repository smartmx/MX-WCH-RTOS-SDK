/********************************** (C) COPYRIGHT *******************************
 * File Name          : main.c
 * Author             : WCH
 * Version            : V1.1
 * Date               : 2022/05/10
 * Description        : kernel_liteos_m例程，使用硬件压栈，禁用中断嵌套，中断函数不再使用修饰
 *                      __attribute__((interrupt("WCH-Interrupt-fast")))，
 *                      中断函数直接按照普通函数定义，只使用HIGHCODE修饰即可。
 * Copyright (c) 2021 Nanjing Qinheng Microelectronics Co., Ltd.
 * SPDX-License-Identifier: Apache-2.0
 *******************************************************************************/

/******************************************************************************/
/* 头文件包含 */
#include "CH58x_common.h"
#include "config.h"
#include "peripheral.h"
#include "HAL.h"
#include "stdarg.h"
#include "los_tick.h"
#include "los_task.h"
#include "los_config.h"
#include "los_interrupt.h"
#include "los_debug.h"
#include "los_compiler.h"
#include "los_sem.h"
#include "mx_sdk.h"

static UINT32 g_semId;

__attribute__((aligned(4))) uint32_t MEM_BUF[BLE_MEMHEAP_SIZE / 4];
#if (defined (BLE_MAC)) && (BLE_MAC == TRUE)
const uint8_t MacAddr[6] = {0x84, 0xC2, 0xE4, 0x03, 0x32, 0x22};
#endif

/*********************************************************************
 * @fn      tmos_task
 *
 * @brief   tmos program.
 *
 * @param  *pvParameters - Parameters point of task2
 *
 * @return  none
 */
__HIGH_CODE
void tmos_task(void *pvParameters)
{
    UINT32 int_value;
    int_value = LOS_IntLock();
    CH58X_BLEInit();
    HAL_Init();
    GAPRole_PeripheralInit();
    Peripheral_Init();

#ifdef CH58xBLE_ROM
    /* 固定库需要手动修改中断向量表 */
    do
    {
        extern uint32_t _BB_IRQHandler_base[1];
        extern uint32_t _LLE_IRQHandler_base[1];
        _BB_IRQHandler_base[0] = (uint32_t)BB_IRQLibHandler;
        _LLE_IRQHandler_base[0] = (uint32_t)LLE_IRQLibHandler;
    } while (0);
#endif

    PRINT("%08x,%08x\n", PFIC->FIADDRR[2], PFIC->FIADDRR[3]);
    PFIC_DisableFastINT2();
    PFIC_DisableFastINT3();     /* 蓝牙使用了快速中断，所以需要将其失能，lib库中真实中断处理函数为BB_IRQLibHandler LLE_IRQLibHandler */
    PRINT("%08x,%08x\n", PFIC->FIADDRR[2], PFIC->FIADDRR[3]);
    LOS_IntRestore(int_value);

    PRINT_INFO("tmos ble start\n");
    while (1)
    {
        TMOS_SystemProcess();
    }
}

/*********************************************************************
 * @fn      taskSampleEntry2
 *
 * @brief   taskSampleEntry2 program.
 *
 * @return  none
 */
__HIGH_CODE
VOID taskSampleEntry2(VOID)
{
    while(1) {
      LOS_TaskDelay(10000);
      PRINT("taskSampleEntry2 running,task2 SP:%08x\n",__get_SP());
    }
}

/*********************************************************************
 * @fn      taskSampleEntry1
 *
 * @brief   taskSampleEntry1 program.
 *
 * @return  none
 */
__HIGH_CODE
VOID taskSampleEntry1(VOID)
{
    while(1) {
      LOS_TaskDelay(2000);
      PRINT("taskSampleEntry1 running,task1 SP:%08x\n",__get_SP());
    }
}

/*********************************************************************
 * @fn      taskIntSem
 *
 * @brief   taskIntSem program.
 *
 * @return  none
 */
__HIGH_CODE
VOID taskIntSem(VOID)
{
    if(LOS_BinarySemCreate(0, &g_semId) == LOS_OK)
    {
        GPIOA_ModeCfg(GPIO_Pin_12, GPIO_ModeIN_PU);
        GPIOA_ITModeCfg(GPIO_Pin_12, GPIO_ITMode_FallEdge);
        PFIC_EnableIRQ(GPIO_A_IRQn);
        while (1)
        {
            if(LOS_SemPend(g_semId, LOS_WAIT_FOREVER) == LOS_OK)
            {
                PRINT("sem get ok\n");
            }
            else
            {
                PRINT("sem get failed\n");
            }
        }
    }
    else
    {
        PRINT("sem create failed\n");
    }
}

/*********************************************************************
 * @fn      taskSample
 *
 * @brief   taskSample program.
 *
 * @return  none
 */
UINT32 taskSample(VOID)
{
    UINT32  uwRet;
    UINT32 taskID1,taskID2,taskID3,taskIDIntSem;
    TSK_INIT_PARAM_S stTask={0};

    stTask.pfnTaskEntry = (TSK_ENTRY_FUNC)taskIntSem;
    stTask.uwStackSize  = LOSCFG_BASE_CORE_TSK_DEFAULT_STACK_SIZE;
    stTask.pcName       = "int sem test";
    stTask.usTaskPrio   = 6;/* high priority */
    uwRet = LOS_TaskCreate(&taskIDIntSem, &stTask);
    if (uwRet != LOS_OK) {
        PRINT("create int sem test failed\n");
    }

    stTask.pfnTaskEntry = (TSK_ENTRY_FUNC)taskSampleEntry1;
    stTask.uwStackSize  = LOSCFG_BASE_CORE_TSK_DEFAULT_STACK_SIZE;
    stTask.pcName       = "taskSampleEntry1";
    stTask.usTaskPrio   = 6;/* high priority */
    uwRet = LOS_TaskCreate(&taskID1, &stTask);
    if (uwRet != LOS_OK) {
        PRINT("create task1 failed\n");
    }

    stTask.pfnTaskEntry = (TSK_ENTRY_FUNC)taskSampleEntry2;
    stTask.uwStackSize  = LOSCFG_BASE_CORE_TSK_DEFAULT_STACK_SIZE;
    stTask.pcName       = "taskSampleEntry2";
    stTask.usTaskPrio   = 7;/* low priority */
    uwRet = LOS_TaskCreate(&taskID2, &stTask);
    if (uwRet != LOS_OK) {
        PRINT("create task2 failed\n");
    }

    stTask.pfnTaskEntry = (TSK_ENTRY_FUNC)tmos_task;
    stTask.uwStackSize  = 0x500;
    stTask.pcName       = "tmos_task";
    stTask.usTaskPrio   = OS_TASK_PRIORITY_LOWEST - 2;  /* only 1 high than idle task priority */
    uwRet = LOS_TaskCreate(&taskID3, &stTask);
    if (uwRet != LOS_OK) {
        PRINT("create tmos task failed\n");
    }

    return LOS_OK;
}

/*********************************************************************
 * @fn      main
 *
 * @brief   Main program.
 *
 * @return  none
 */
LITE_OS_SEC_TEXT_INIT int main(void)
{
    unsigned int ret;

#if (defined(DCDC_ENABLE)) && (DCDC_ENABLE == TRUE)
    PWR_DCDCCfg(ENABLE);
#endif
    SetSysClock(CLK_SOURCE_PLL_60MHz);
#if (defined(HAL_SLEEP)) && (HAL_SLEEP == TRUE)
    GPIOA_ModeCfg(GPIO_Pin_All, GPIO_ModeIN_PU);
    GPIOB_ModeCfg(GPIO_Pin_All, GPIO_ModeIN_PU);
#endif

#ifdef DEBUG
    mx_debug_uart_init(115200);
#endif
    PRINT("start.\n");

    PRINT("Program build on: %s, %s\n", __DATE__, __TIME__);

    ret = LOS_KernelInit();
    taskSample();
    if (ret == LOS_OK)
    {
        LOS_Start();
    }

    PRINT("shouldn't run at here!!!\r\n");
    while (1) {
        __asm volatile("nop");
    }

    return 0;

}

__HIGH_CODE
void GPIOA_IRQHandler(void)
{
    /* 本函数可以作为在本工程LiteOS中的中断函数写法示例 */
    uint16_t flag;

    flag = GPIOA_ReadITFlagPort();
    if((flag & GPIO_Pin_12) != 0)
    {
        LOS_SemPost(g_semId);
    }
    GPIOA_ClearITFlagBit(flag); //清除中断标志
}

/******************************** endfile @ main ******************************/
