/********************************** (C) COPYRIGHT *******************************
 * File Name          : SLEEP.c
 * Author             : WCH
 * Version            : V1.2
 * Date               : 2022/01/18
 * Description        : ˯�����ü����ʼ��
 *********************************************************************************
 * Copyright (c) 2021 Nanjing Qinheng Microelectronics Co., Ltd.
 * Attention: This software (modified or not) and binary are used for 
 * microcontroller manufactured by Nanjing Qinheng Microelectronics.
 *******************************************************************************/

/******************************************************************************/
/* ͷ�ļ����� */
#include "HAL.h"


pfnLowPowerGapProcessCB_t LowPowerGapProcess;
/*******************************************************************************
 * @fn          CH58x_LowPower
 *
 * @brief       ����˯��
 *
 * @param       time  - ���ѵ�ʱ��㣨RTC����ֵ��
 *
 * @return      state.
 */
__HIGH_CODE
uint32_t CH58x_LowPower(uint32_t time)
{
#if(defined(HAL_SLEEP)) && (HAL_SLEEP == TRUE)
    volatile uint32_t i;
    uint32_t time_tign, time_sleep, time_curr;
    unsigned long irq_status;

    // ��ǰ����
    if (time <= WAKE_UP_RTC_MAX_TIME) {
        time_tign = time + (RTC_MAX_COUNT - WAKE_UP_RTC_MAX_TIME);
    } else {
        time_tign = time - WAKE_UP_RTC_MAX_TIME;
    }

    SYS_DisableAllIrq(&irq_status);




    time_curr = RTC_GetCycle32k();
    // ���˯��ʱ��
    if (time_tign < time_curr) {
        time_sleep = time_tign + (RTC_MAX_COUNT - time_curr);
    } else {
        time_sleep = time_tign - time_curr;
    }

    // ��˯��ʱ��С����С˯��ʱ���������˯��ʱ�䣬��˯��
    if ((time_sleep < SLEEP_RTC_MIN_TIME) || 
        (time_sleep > SLEEP_RTC_MAX_TIME)) {
        SYS_RecoverIrq(irq_status);
        return 2;
    }

    RTC_SetTignTime(time_tign);
    R8_RTC_FLAG_CTRL = (RB_RTC_TMR_CLR | RB_RTC_TRIG_CLR);

#if(DEBUG == Debug_UART0) // ʹ���������������ӡ��Ϣ��Ҫ�޸����д���
    while((R8_UART0_LSR & RB_LSR_TX_ALL_EMP) == 0)
    {
        __nop();
    }
#endif
    // LOW POWER-sleepģʽ
    if(!(R8_RTC_FLAG_CTRL&RB_RTC_TRIG_FLAG))
    {
        uint8_t x32Mpw;

        LowPower_Sleep_WFE(RB_PWR_RAM32K | RB_PWR_RAM96K | RB_PWR_EXTEND);

        // �л�32M����
        x32Mpw = R8_XT32M_TUNE;
        x32Mpw = (x32Mpw & 0xfc) | 0x03; // 150%�����
        sys_safe_access_enable();
        R8_XT32M_TUNE = x32Mpw;
        sys_safe_access_disable();

        if(!(R8_RTC_FLAG_CTRL & RB_RTC_TRIG_FLAG)) //��RTC����
        {
            // ע���ʱ32M����ȴ��ȶ���Ҳ��ִ��һЩʱ��Ҫ�󲻸ߵĴ���
            DelayUs(1400);
            SetSysClock( SYSCLK_FREQ );
            SYS_RecoverIrq(irq_status);
            return 0;
        }

        R8_RTC_FLAG_CTRL = (RB_RTC_TMR_CLR | RB_RTC_TRIG_CLR);
        RTC_SetTignTime(time);
        sys_safe_access_enable();
        R8_HFCK_PWR_CTRL |= RB_CLK_XT32M_KEEP;
        sys_safe_access_disable();
        if(!(R8_RTC_FLAG_CTRL&RB_RTC_TRIG_FLAG)) //��RTC����
        {
            LowPower_Halt_WFE();

        }



        // �ָ�ʱ��
        SetSysClock( SYSCLK_FREQ );
        R8_RTC_FLAG_CTRL = (RB_RTC_TMR_CLR | RB_RTC_TRIG_CLR);
        SYS_RecoverIrq(irq_status);
        HSECFG_Current(HSE_RCur_100); // ��Ϊ�����(�͹��ĺ�����������HSEƫ�õ���)
        return 0;
    }
    SYS_RecoverIrq(irq_status);
#endif
    return 3;
}

/*******************************************************************************
 * @fn      LowPowerGapProcess_Register
 *
 * @brief   ע��͹��Ļ��Ѽ�϶ִ�лص�
 *
 * @param   None.
 *
 * @return  None.
 */
void LowPowerGapProcess_Register(pfnLowPowerGapProcessCB_t cb)
{
    if((uint32_t)cb & 0x20000000)
    {
        LowPowerGapProcess = cb;
    }
}

/*******************************************************************************
 * @fn      HAL_SleepInit
 *
 * @brief   ����˯�߻��ѵķ�ʽ   - RTC���ѣ�����ģʽ
 *
 * @param   None.
 *
 * @return  None.
 */
void HAL_SleepInit(void)
{
#if(defined(HAL_SLEEP)) && (HAL_SLEEP == TRUE)
    sys_safe_access_enable();
    R8_SLP_WAKE_CTRL |= RB_SLP_RTC_WAKE; // RTC����
    sys_safe_access_disable();
    sys_safe_access_enable();
    R8_RTC_MODE_CTRL |= RB_RTC_TRIG_EN;  // ����ģʽ
    sys_safe_access_disable();
    PFIC_EnableIRQ(RTC_IRQn);
#endif
}
