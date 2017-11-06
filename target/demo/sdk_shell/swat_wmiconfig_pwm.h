/*
  * Copyright (c) 2013 Qualcomm Atheros, Inc..
  * All Rights Reserved.
  * Qualcomm Atheros Confidential and Proprietary.
  */

#ifndef _SWAT_WMICONFIG_PWR_H_
#define _SWAT_WMICONFIG_PWR_H_

void swat_wmiconfig_power_mode_set(A_UINT8 device_id, A_UINT32 powerMode);

void swat_wmiconfig_pmparams(A_UINT8 device_id, A_UINT16 idlePeriod,
                 A_UINT16 psPollNum, A_UINT16 dtimPolicy,
                 A_UINT16 tx_wakeup_policy, A_UINT16 num_tx_to_wakeup,
                 A_UINT16 ps_fail_event_policy);

void swat_wmiconfig_suspenable(A_UINT8 device_id);

void swat_wmiconfig_suspflag_get(A_UINT8 device_id, A_UINT8 *flag);

void swat_wmiconfig_suspstart(A_UINT32 susp_time);

void swat_wmiconfig_suspstart(A_UINT32 susp_time);

void swat_wmiconfig_pwm_start(A_UINT32 flag);


void swat_wmiconfig_pwm_clock(A_UINT32 clock);
void swat_wmiconfig_pwm_duty(A_UINT32 duty);
void swat_wmiconfig_pwm_dump(void);
void swat_wmiconfig_pwm_set(A_UINT32 freq, A_UINT32 duty);


void swat_wmiconfig_pwm_sw_start(void);
void swat_wmiconfig_pwm_sw_config(A_INT32 id, A_UINT32 freq, A_UINT32 duty);
void swat_wmiconfig_pwm_sw_stop(void);


#endif

