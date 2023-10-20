/********************************** (C) COPYRIGHT *******************************
 * File Name          : Main.c
 * Author             : WCH
 * Version            : V1.0
 * Date               : 2023/10/20
 * Description        : rt-thread nano移植例程，使用硬件压栈，中断嵌套可选，中断函数不再使用修饰
 *                      __attribute__((interrupt("WCH-Interrupt-fast")))，
 *                      中断函数直接按照普通函数定义，只使用HIGHCODE修饰即可。
 *********************************************************************************
 * Copyright (c) 2021 Nanjing Qinheng Microelectronics Co., Ltd.
 * Attention: This software (modified or not) and binary are used for
 * microcontroller manufactured by Nanjing Qinheng Microelectronics.
 *******************************************************************************/

#include "debug.h"
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
        gpio_int_init();
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
    rt_base_t level;

    level = rt_hw_interrupt_disable();
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

    rt_hw_interrupt_enable(level);
    rt_kprintf("tmos ble start\n");
    Main_Circulation();
}

void msh_test_print(void)
{
    rt_kprintf("this is a test for msh.\n");
}
MSH_CMD_EXPORT(msh_test_print, this is a msh test);

/*******************************************************************************
 * Function Name  : main
 * Description    : 主函数
 * Input          : None
 * Output         : None
 * Return         : None
 *******************************************************************************/
int main(void)
{

    NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);

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

}

void EXTI1_IRQHandler(void)
{
    /* 本函数可以作为在本工程rt-thread nano中的中断函数写法示例 */
    if(EXTI_GetITStatus(EXTI_Line1) != RESET)
    {
        rt_sem_release(gpioa_sem);  /* 释放信号量 */
        EXTI->INTFR = EXTI_Line1;         /* EXTI_ClearITPendingBit(EXTI_Line1); Clear Flag */
    }
}
/******************************** endfile @ main ******************************/
