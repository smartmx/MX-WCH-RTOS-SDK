/********************************** (C) COPYRIGHT *******************************
 * File Name          : main.c
 * Author             : WCH
 * Version            : V1.1
 * Date               : 2022/05/10
 * Description        : FreeRTOS例程，使用硬件压栈，禁用中断嵌套，中断函数不再使用修饰
 *                      __attribute__((interrupt("WCH-Interrupt-fast")))，
 *                      中断函数直接按照普通函数定义，只使用HIGHCODE修饰即可。
 * Copyright (c) 2021 Nanjing Qinheng Microelectronics Co., Ltd.
 * SPDX-License-Identifier: Apache-2.0
 *******************************************************************************/

/******************************************************************************/
/* 头文件包含 */
#include "debug.h"
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

static UINT32 g_semId;

__attribute__((aligned(4))) uint32_t MEM_BUF[BLE_MEMHEAP_SIZE / 4];
#if (defined (BLE_MAC)) && (BLE_MAC == TRUE)
const uint8_t MacAddr[6] = {0x84, 0xC2, 0xE4, 0x03, 0x32, 0x22};
#endif

/*********************************************************************
 * @fn      Main_Circulation
 *
 * @brief   Main loop
 *
 * @return  none
 */
__attribute__((section(".highcode")))
__attribute__((noinline))
void Main_Circulation(void)
{
    while(1)
    {
        TMOS_SystemProcess();
    }
}

/*********************************************************************
 * @fn      tmos_task
 *
 * @brief   tmos program.
 *
 * @param  *pvParameters - Parameters point of task2
 *
 * @return  none
 */
void tmos_task(void *pvParameters)
{
    UINT32 int_value;
    int_value = LOS_IntLock();
    WCHBLE_Init();
    HAL_Init();
    GAPRole_PeripheralInit();
    Peripheral_Init();

#ifdef WCHBLE_ROM
    /* 固定库需要手动修改中断向量表 */
    do
    {
        extern uint32_t _BB_IRQHandler_base[1];
        _BB_IRQHandler_base[0] = (uint32_t)BB_IRQLibHandler;
    } while (0);
#endif
    NVIC_SetPriority(LLE_IRQn, 0xe0);   /* 必须保证不可抢断其他任务 */
    LOS_IntRestore(int_value);

    Main_Circulation();
}

/*********************************************************************
 * @fn      taskSampleEntry2
 *
 * @brief   taskSampleEntry2 program.
 *
 * @return  none
 */
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
VOID taskSampleEntry1(VOID)
{
    while(1) {
      LOS_TaskDelay(2000);
      PRINT("taskSampleEntry1 running,task1 SP:%08x\n",__get_SP());
    }
}

void gpio_int_init(void)
{
    GPIO_InitTypeDef GPIO_InitStructure = {0};
    EXTI_InitTypeDef EXTI_InitStructure = {0};
    NVIC_InitTypeDef NVIC_InitStructure = {0};

    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_1;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;
    GPIO_Init(GPIOB, &GPIO_InitStructure);

    /* 开启NSS下降沿中断 */
    GPIO_EXTILineConfig(GPIO_PortSourceGPIOB, GPIO_PinSource1);
    EXTI_InitStructure.EXTI_Line = EXTI_Line1;
    EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;
    EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Falling;
    EXTI_InitStructure.EXTI_LineCmd = ENABLE;
    EXTI_Init(&EXTI_InitStructure);

    NVIC_InitStructure.NVIC_IRQChannel = EXTI1_IRQn;
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 1;
    NVIC_InitStructure.NVIC_IRQChannelSubPriority = 2;
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
    NVIC_Init(&NVIC_InitStructure);

}

/*********************************************************************
 * @fn      taskIntSem
 *
 * @brief   taskIntSem program.
 *
 * @return  none
 */
VOID taskIntSem(VOID)
{
    if(LOS_BinarySemCreate(0, &g_semId) == LOS_OK)
    {
        gpio_int_init();
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

/*******************************************************************************
 * Function Name  : main
 * Description    : 主函数
 * Input          : None
 * Output         : None
 * Return         : None
 *******************************************************************************/
int main(void)
{
    unsigned int ret;

    NVIC_PriorityGroupConfig(NVIC_PriorityGroup_1);

#ifdef DEBUG
    USART_Printf_Init(115200);
    PRINT("start.\n");
#endif

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

void EXTI1_IRQHandler(void)
{
    /* 本函数可以作为在本工程LiteOS中的中断函数写法示例 */
    if(EXTI_GetITStatus(EXTI_Line1) != RESET)
    {
        LOS_SemPost(g_semId);
        EXTI->INTFR = EXTI_Line1;         /* EXTI_ClearITPendingBit(EXTI_Line1); Clear Flag */
    }
}
/******************************** endfile @ main ******************************/
