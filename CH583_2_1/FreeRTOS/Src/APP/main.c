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
#include "CH58x_common.h"
#include "FreeRTOS.h"
#include "task.h"
#include "config.h"
#include "peripheral.h"
#include "HAL.h"
#include "semphr.h"
#include "stdarg.h"
#include "mx_sdk.h"

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
__HIGH_CODE
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
__HIGH_CODE
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
__HIGH_CODE
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

/*********************************************************************
 * @fn      task3_task
 *
 * @brief   task3 program.
 *
 * @param  *pvParameters - Parameters point of task3
 *
 * @return  none
 */
__HIGH_CODE
void task3_task(void *pvParameters)
{
    xBinarySem = xSemaphoreCreateBinary();
    if(xBinarySem != NULL)
    {
        GPIOA_ModeCfg(GPIO_Pin_12, GPIO_ModeIN_PU);
        GPIOA_ITModeCfg(GPIO_Pin_12, GPIO_ITMode_FallEdge);
        PFIC_EnableIRQ(GPIO_A_IRQn);
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
    portDISABLE_INTERRUPTS();
    
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
    PRINT("%08x,%08x\n", PFIC->FIADDRR[2], PFIC->FIADDRR[3]);
    PFIC_DisableFastINT2();
    PFIC_DisableFastINT3();     /* 蓝牙使用了快速中断，所以需要将其失能，lib库中真实中断处理函数为BB_IRQLibHandler LLE_IRQLibHandler */
    PRINT("%08x,%08x\n", PFIC->FIADDRR[2], PFIC->FIADDRR[3]);
    
    portENABLE_INTERRUPTS();
    
    App_Printf("tmos ble start\n");
    while (1)
    {
        TMOS_SystemProcess();
    }
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
#if(defined(DCDC_ENABLE)) && (DCDC_ENABLE == TRUE)
    PWR_DCDCCfg(ENABLE);
#endif
    SetSysClock(CLK_SOURCE_PLL_60MHz);
#if(defined(HAL_SLEEP)) && (HAL_SLEEP == TRUE)
    GPIOA_ModeCfg(GPIO_Pin_All, GPIO_ModeIN_PU);
    GPIOB_ModeCfg(GPIO_Pin_All, GPIO_ModeIN_PU);
#endif

#ifdef DEBUG
    mx_debug_uart_init(115200);
#endif

    PRINT("start.\n");
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

__HIGH_CODE
void GPIOA_IRQHandler(void)
{
    /* 本函数可以作为在本工程FreeRTOS中的中断函数写法示例 */
    uint16_t flag;
    portBASE_TYPE xHigherPriorityTaskWoken;
    flag = GPIOA_ReadITFlagPort();
    if((flag & GPIO_Pin_12) != 0)
    {
        xSemaphoreGiveFromISR(xBinarySem, &xHigherPriorityTaskWoken);
        portYIELD_FROM_ISR(xHigherPriorityTaskWoken);   //根据需要发起切换请求
    }
    GPIOA_ClearITFlagBit(flag); //清除中断标志
}

/******************************** endfile @ main ******************************/
