/********************************** (C) COPYRIGHT *******************************
 * File Name          : Main.c
 * Author             : WCH
 * Version            : V1.0
 * Date               : 2020/08/06
 * Description        : rt-thread nano移植例程，使用硬件压栈，中断嵌套可选，中断函数不再使用修饰
 *                      __attribute__((interrupt("WCH-Interrupt-fast")))，
 *                      中断函数直接按照普通函数定义，只使用HIGHCODE修饰即可。
 *********************************************************************************
 * Copyright (c) 2021 Nanjing Qinheng Microelectronics Co., Ltd.
 * Attention: This software (modified or not) and binary are used for 
 * microcontroller manufactured by Nanjing Qinheng Microelectronics.
 *******************************************************************************/

#include "CH58x_common.h"
#include <rtthread.h>
#include "config.h"
#include "HAL.h"
#include "peripheral.h"
#include "rthw.h"

ALIGN(RT_ALIGN_SIZE)
static char task1_stack[512];
static struct rt_thread task1_thread;

/*********************************************************************
 * @fn      task1_entry
 *
 * @brief   task1任务函数
 *
 * @return  none
 */
void task1_entry(void *parameter)
{
    while(1)
    {
        rt_kprintf("task1\r\n");
        rt_thread_delay(1000);
    }
}

ALIGN(RT_ALIGN_SIZE)
static char task2_stack[512];
static struct rt_thread task2_thread;

/*********************************************************************
 * @fn      task2_entry
 *
 * @brief   task2任务函数
 *
 * @return  none
 */
void task2_entry(void *parameter)
{
    while(1)
    {
        rt_kprintf("task2\r\n");
        rt_thread_delay(800);
    }
}

ALIGN(RT_ALIGN_SIZE)
static char task3_stack[512];
static struct rt_thread task3_thread;
static rt_sem_t gpioa_sem = RT_NULL;

/*********************************************************************
 * @fn      task3_entry
 *
 * @brief   task3任务函数
 *
 * @return  none
 */
void task3_entry(void *parameter)
{
    gpioa_sem = rt_sem_create("gpioa sem", 0, RT_IPC_FLAG_FIFO);
    if(gpioa_sem != NULL)
    {
        GPIOA_ModeCfg(GPIO_Pin_12, GPIO_ModeIN_PU);
        GPIOA_ITModeCfg(GPIO_Pin_12, GPIO_ITMode_FallEdge);
        PFIC_EnableIRQ(GPIO_A_IRQn);
        while(1)
        {
            rt_sem_take(gpioa_sem, RT_WAITING_FOREVER); /* 等待信号量 */
            rt_kprintf("gpioa sem get\r\n");
        }
    }
    else
    {
        rt_kprintf("sem create failed\r\n");
    }
}

ALIGN(RT_ALIGN_SIZE)
static char tmos_stack[1024];
static struct rt_thread tmos_thread;

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
    rt_base_t level;

    level = rt_hw_interrupt_disable();

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
    CH58X_BLEInit();
    HAL_Init();
    GAPRole_PeripheralInit();
    Peripheral_Init();
    rt_kprintf("%08x,%08x\n", PFIC->FIADDRR[2], PFIC->FIADDRR[3]);
    PFIC_DisableFastINT2();
    PFIC_DisableFastINT3();     /* 蓝牙使用了快速中断，所以需要将其失能，lib库中真实中断处理函数为BB_IRQLibHandler LLE_IRQLibHandler */
    rt_kprintf("%08x,%08x\n", PFIC->FIADDRR[2], PFIC->FIADDRR[3]);
   
    rt_hw_interrupt_enable(level);
    
    rt_kprintf("tmos ble start\n");
    while (1)
    {
        TMOS_SystemProcess();
    }
}

/*********************************************************************
 * @fn      main
 *
 * @brief   主函数
 *
 * @note    main is one of threads in rt-thread.
 *
 * @return  none
 */
int main()
{
    rt_enter_critical();

    rt_thread_init(&task1_thread,
                   "task1",
                   task1_entry,
                   RT_NULL,
                   &task1_stack[0],
                   sizeof(task1_stack),
                   4, 20);
    rt_thread_startup(&task1_thread);

    rt_thread_init(&task2_thread,
                   "task2",
                   task2_entry,
                   RT_NULL,
                   &task2_stack[0],
                   sizeof(task2_stack),
                   4, 20);
    rt_thread_startup(&task2_thread);

    rt_thread_init(&task3_thread,
                   "task3",
                   task3_entry,
                   RT_NULL,
                   &task3_stack[0],
                   sizeof(task3_stack),
                   4, 20);
    rt_thread_startup(&task3_thread);

    rt_thread_init(&tmos_thread,
                   "tmos",
                   tmos_task,
                   RT_NULL,
                   &tmos_stack[0],
                   sizeof(tmos_stack),
                   RT_THREAD_PRIORITY_MAX - 2, 20);
    rt_thread_startup(&tmos_thread);

    rt_exit_critical();

    return 0;
}

void msh_test_print(void)
{
    rt_kprintf("this is a test for msh.\n");
}
MSH_CMD_EXPORT(msh_test_print, this is a msh test);

/*********************************************************************
 * @fn      GPIOA_IRQHandler
 *
 * @brief   GPIOA_IRQHandler.
 *
 * @param   none
 *
 * @return  none
 */
__HIGH_CODE
void GPIOA_IRQHandler(void)
{
    /* 本函数可以作为在本工程rt-thread nano中的中断函数写法示例 */
    uint16_t flag;

    flag = GPIOA_ReadITFlagPort();
    if((flag & GPIO_Pin_12) != 0)
    {
        rt_sem_release(gpioa_sem);  /* 释放信号量 */
    }
    GPIOA_ClearITFlagBit(flag); /* 清除中断标志 */
}

