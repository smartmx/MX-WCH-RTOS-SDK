/********************************** (C) COPYRIGHT *******************************
 * File Name          : main.c
 * Author             : WCH
 * Version            : V1.0
 * Date               : 2024/08/13
 * Description        : FreeRTOS���̣�ʹ��Ӳ��ѹջ���ж�Ƕ�׿�ѡ���жϺ�������ʹ������
 *                      __attribute__((interrupt("WCH-Interrupt-fast")))��
 *                      �жϺ���ֱ�Ӱ�����ͨ�������壬ֻʹ��HIGHCODE���μ��ɡ�
 *********************************************************************************
 * Copyright (c) 2024 Nanjing Qinheng Microelectronics Co., Ltd.
 * Attention: This software (modified or not) and binary are used for 
 * microcontroller manufactured by Nanjing Qinheng Microelectronics.
 *******************************************************************************/

/******************************************************************************/
/* ͷ�ļ����� */
#include "mx_sdk.h"
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
#define TASK1_STK_SIZE      256     /* ����������ʹ����APP_Printf����ջ�ռ����Ҫ���ڴ�ӡ�����õ������ջ�ռ� */
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

    /* �������������������ж���ʹ�� */
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
    CH58x_BLEInit();
    HAL_Init();
    GAPRole_PeripheralInit();
    Peripheral_Init();
#ifdef CH58xBLE_ROM
    /* �̶�����Ҫ�ֶ��޸��ж������� */
    do
    {
        extern uint32_t _BB_IRQHandler_base[1];
        _BB_IRQHandler_base[0] = (uint32_t)BB_IRQLibHandler;
    } while (0);
#endif
    PRINT("%08x,%08x\n", PFIC->VTFADDR[2], PFIC->VTFADDR[3]);
    SetVTFIRQ((uint32_t)0, BLEB_IRQn, 2, DISABLE);
    SetVTFIRQ((uint32_t)0, BLEL_IRQn, 3, DISABLE);
    PRINT("%08x,%08x\n", PFIC->VTFADDR[2], PFIC->VTFADDR[3]);
    PFIC_SetPriority(BLEL_IRQn, 0xff);   /* ���뱣֤���������������� */
    portENABLE_INTERRUPTS();
    App_Printf("tmos ble start\n");
    Main_Circulation();
}

/*******************************************************************************
 * Function Name  : main
 * Description    : ������
 * Input          : None
 * Output         : None
 * Return         : None
 *******************************************************************************/
int main(void)
{
#if(defined(DCDC_ENABLE)) && (DCDC_ENABLE == TRUE)
    PWR_DCDCCfg(ENABLE);
#endif
    HSECFG_Capacitance(HSECap_18p);
    SetSysClock(SYSCLK_FREQ);
#if(defined(HAL_SLEEP)) && (HAL_SLEEP == TRUE)
    GPIOA_ModeCfg(GPIO_Pin_All, GPIO_ModeIN_PU);
    GPIOB_ModeCfg(GPIO_Pin_All, GPIO_ModeIN_PU);
#endif

#ifdef DEBUG
    mx_debug_uart_init(115200);
#endif
    PRINT("Program build on: %s, %s\n", __DATE__, __TIME__);
    PRINT("FreeRTOS %s start.\n", tskKERNEL_VERSION_NUMBER);
    
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
    /* ������������Ϊ�ڱ�����FreeRTOS�е��жϺ���д��ʾ�� */
    uint16_t flag;
    portBASE_TYPE xHigherPriorityTaskWoken;
    flag = GPIOA_ReadITFlagPort();
    if((flag & GPIO_Pin_12) != 0)
    {
        xSemaphoreGiveFromISR(xBinarySem, &xHigherPriorityTaskWoken);
        portYIELD_FROM_ISR(xHigherPriorityTaskWoken);   /* ������Ҫ�����л����� */
    }
    GPIOA_ClearITFlagBit(flag); /* ����жϱ�־ */
}

/******************************** endfile @ main ******************************/
