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
#include "FreeRTOS.h"
#include "task.h"
#include "config.h"
#include "peripheral.h"
#include "HAL.h"
#include "semphr.h"
#include "stdarg.h"

/*********************************************************************
 * GLOBAL TYPEDEFS
 */
#define TASK1_TASK_PRIO     5
#define TASK1_STK_SIZE      256     /* 因在任务中使用了APP_Printf所以栈空间必须要大于打印可能用到的最大栈空间 */
#define TASK2_TASK_PRIO     5
#define TASK2_STK_SIZE      256
#define TASK3_TASK_PRIO     configMAX_PRIORITIES - 2
#define TASK3_STK_SIZE      256

/* Global Variable */
TaskHandle_t Task1Task_Handler;
TaskHandle_t Task2Task_Handler;
TaskHandle_t Task3Task_Handler;
TaskHandle_t tmosTask_Handler;
SemaphoreHandle_t printMutex;
SemaphoreHandle_t xBinarySem;

/*********************************************************************
 * @fn      App_Printf
 *
 * @brief   printf can be used within freertos.
 *
 * @param  *fmt - printf params.
 *
 * @return  none
 */

void App_Printf(const char *fmt, ...)
{
    va_list   v_args;

    /* 互斥量操作，不可在中断中使用 */
    xSemaphoreTake(printMutex, portMAX_DELAY);

    va_start(v_args, fmt);
    vprintf_(fmt, v_args);
    va_end(v_args);

    xSemaphoreGive(printMutex);
}

/*********************************************************************
 * @fn      task1_task
 *
 * @brief   task1 program.
 *
 * @param  *pvParameters - Parameters point of task1
 *
 * @return  none
 */

void task1_task(void *pvParameters)
{
    while (1)
    {
        App_Printf("task1 entry 1\n");
        vTaskDelay(configTICK_RATE_HZ / 4);
        App_Printf("task1 entry 2\n");
        vTaskDelay(configTICK_RATE_HZ / 4);
    }
}

/*********************************************************************
 * @fn      task2_task
 *
 * @brief   task2 program.
 *
 * @param  *pvParameters - Parameters point of task2
 *
 * @return  none
 */

void task2_task(void *pvParameters)
{
    while (1)
    {
        App_Printf("task2 entry 1\n");
        vTaskDelay(configTICK_RATE_HZ / 2);
        App_Printf("task2 entry 2\n");
        vTaskDelay(configTICK_RATE_HZ / 2);
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
 * @fn      task3_task
 *
 * @brief   task3 program.
 *
 * @param  *pvParameters - Parameters point of task3
 *
 * @return  none
 */
void task3_task(void *pvParameters)
{
    xBinarySem = xSemaphoreCreateBinary();
    if(xBinarySem != NULL)
    {
        gpio_int_init();
        while (1)
        {
            if(xSemaphoreTake(xBinarySem, portMAX_DELAY) == pdTRUE)
            {
                App_Printf("task3 sem get ok\n");
            }
            else
            {
                App_Printf("task3 sem get failed\n");
            }
        }
    }
    else
    {
        App_Printf("task3 sem init failed\n");
    }
}

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
    portDISABLE_INTERRUPTS();
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
    portENABLE_INTERRUPTS();
    App_Printf("tmos ble start\n");
    Main_Circulation();
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

    NVIC_PriorityGroupConfig(NVIC_PriorityGroup_1);

#ifdef DEBUG
    USART_Printf_Init(115200);
    PRINT("start.\n");
#endif

    printMutex = xSemaphoreCreateMutex();
    if(printMutex == NULL)
    {
        PRINT("printMutex error\n");
        while(1);
    }

    /* create three task */
    xTaskCreate((TaskFunction_t)task3_task,
                (const char *)"task3",
                (uint16_t)TASK3_STK_SIZE,
                (void *)NULL,
                (UBaseType_t)TASK3_TASK_PRIO,
                (TaskHandle_t *)&Task3Task_Handler);

    xTaskCreate((TaskFunction_t)task2_task,
                (const char *)"task2",
                (uint16_t)TASK2_STK_SIZE,
                (void *)NULL,
                (UBaseType_t)TASK2_TASK_PRIO,
                (TaskHandle_t *)&Task2Task_Handler);

    xTaskCreate((TaskFunction_t)task1_task,
                (const char *)"task1",
                (uint16_t)TASK1_STK_SIZE,
                (void *)NULL,
                (UBaseType_t)TASK1_TASK_PRIO,
                (TaskHandle_t *)&Task1Task_Handler);

    xTaskCreate((TaskFunction_t)tmos_task,
                (const char *)"tmos_task",
                (uint16_t)configMINIMAL_STACK_SIZE * 2U,
                (void *)NULL,
                (UBaseType_t)tskIDLE_PRIORITY + 1,
                (TaskHandle_t *)&tmosTask_Handler);

    vTaskStartScheduler();
    while (1)
    {
        PRINT("shouldn't run at here!!\n");
    }

}

void EXTI1_IRQHandler(void)
{
    /* 本函数可以作为在本工程FreeRTOS中的中断函数写法示例 */
    portBASE_TYPE xHigherPriorityTaskWoken;

    if(EXTI_GetITStatus(EXTI_Line1) != RESET)
    {
        xSemaphoreGiveFromISR(xBinarySem, &xHigherPriorityTaskWoken);
        portYIELD_FROM_ISR(xHigherPriorityTaskWoken);   //根据需要发起切换请求
        EXTI->INTFR = EXTI_Line1;         /* EXTI_ClearITPendingBit(EXTI_Line1); Clear Flag */
    }
}
/******************************** endfile @ main ******************************/
