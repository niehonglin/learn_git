/*
  * Copyright (c) 2013 Qualcomm Atheros, Inc..
  * All Rights Reserved.
  * Qualcomm Atheros Confidential and Proprietary.
  */

#include "qcom_common.h"
#include "swat_wmiconfig_common.h"
#include "qcom_pwm.h"
#include "qcom_nvram.h"

void
swat_wmiconfig_power_mode_set(A_UINT8 device_id, A_UINT32 powerMode)
{
	 qcom_power_set_mode(device_id, powerMode);
}

void
swat_wmiconfig_pmparams(A_UINT8 device_id, A_UINT16 idlePeriod,
                        A_UINT16 psPollNum, A_UINT16 dtimPolicy,
                        A_UINT16 tx_wakeup_policy, A_UINT16 num_tx_to_wakeup,
                        A_UINT16 ps_fail_event_policy)
{

     qcom_power_set_parameters(device_id, idlePeriod, psPollNum, dtimPolicy, tx_wakeup_policy, 
                                 num_tx_to_wakeup, ps_fail_event_policy); 
}

void
swat_wmiconfig_suspenable(A_UINT8 device_id)
{
    /* 1: enable */
    /* 0: disable */
     qcom_suspend_enable(1);
}

void
swat_wmiconfig_suspflag_get(A_UINT8 device_id, A_UINT8 *flag)
{
  qcom_suspend_restore_flag_get(flag);
}

void
swat_wmiconfig_suspstart(A_UINT32 susp_time)
{
     qcom_nvram_init(); 	/* sometimes this function is not called, and fini call will crash */
	 qcom_nvram_fini(); //power down flash before go to suspend mode
     if(qcom_suspend_start(susp_time) == A_ERROR){
         SWAT_PTF("Store recall failed\n");
     }
}
void 
swat_wmiconfig_pwm_start(A_UINT32 flag)
{
	qcom_pwm_control(flag);
}
void
swat_wmiconfig_pwm_clock(A_UINT32 clock)
{
	qcom_pwm_set_clock(clock);
}
void
swat_wmiconfig_pwm_duty(A_UINT32 duty)
{
	qcom_pwm_set_duty_cycle(duty);
}
void
swat_wmiconfig_pwm_dump(void)
{
//	extern void qcom_pwm_dump(void);
	qcom_pwm_dump();
}
void
swat_wmiconfig_pwm_set(A_UINT32 freq, A_UINT32 duty)
{
	qcom_pwm_set(freq, duty);
}

void
swat_wmiconfig_pwm_sw_start()
{
    qcom_pwm_sw_start();
}
void
swat_wmiconfig_pwm_sw_stop()
{
	qcom_pwm_sw_stop();
}
void
swat_wmiconfig_pwm_sw_config(A_INT32 id, A_UINT32 freq, A_UINT32 duty)
{
	qcom_pwm_sw_config(id, freq, duty);
}

