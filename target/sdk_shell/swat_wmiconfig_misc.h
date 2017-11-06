/*
  * Copyright (c) 2013 Qualcomm Atheros, Inc..
  * All Rights Reserved.
  * Qualcomm Atheros Confidential and Proprietary.
  */

#ifndef _SWAT_WMICONFIG_MISC_H_
#define _SWAT_WMICONFIG_MISC_H_

void swat_wmiconfig_help();

void swat_wmiconfig_version(void);

void swat_wmiconfig_reset();

void swat_wmiconfig_active_partition(A_UINT32 partition_index);

void swat_wmiconfig_prom_start(A_UINT32 seconds);

void swat_wmiconfig_prom_stop();

void swat_wmiconfig_ht40_flag_set(A_UINT8 flag);

void swat_wmiconfig_promiscuous_test(A_BOOL promiscuous, A_UINT8 mode);

void swat_wmiconfig_maint_timeout_set(A_UINT32 *timeout,A_UINT32 param_size);

#endif
