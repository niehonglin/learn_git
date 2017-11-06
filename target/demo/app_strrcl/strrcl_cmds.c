/*
  * Copyright (c) 2015 Qualcomm Atheros, Inc..
  * All Rights Reserved.
  * Qualcomm Atheros Confidential and Proprietary.
  */

#include "qcom_common.h"

#include "swat_parse.h"
#include "swat_wmiconfig_misc.h"
#include "swat_wmiconfig_network.h"
#include "swat_wmiconfig_wlan.h"

#include "qcom_gpio.h"
#include "qcom_app_strrcl.h"
#include "swat_bench_core.h"
#include "swat_bench_test.h"

#define  PROTOCOL_TCP	0
#define  PROTOCOL_UDP   1
#define  PROTOCOL_RAW   2
#define  PROTOCOL_SSL   3

extern int v6_enabled;

extern A_BOOL  flag_strrcl_request;
extern A_BOOL  flag_strrcl_start;
extern A_UINT32 strrcl_time_ms;

extern A_BOOL  app_resume_done;
extern A_UINT8  *app_strrcl_store();
extern A_UINT32 buf_strrcl_len;

// bench test
A_INT32 cmd_wmi_config(A_INT32 argc, A_CHAR *argv[]);
A_INT32 cmd_bench_tx_test(A_INT32 argc, A_CHAR *argv[]);
A_INT32 cmd_bench_rx_test(A_INT32 argc, A_CHAR *argv[]);
A_INT32 cmd_bench_quit(A_INT32 argc, A_CHAR *argv[]);
A_INT32 cmd_bench_mode(A_INT32 argc, A_CHAR *argv[]);
A_INT32 cmd_ap_connect(int argc, char *argv[]);
A_INT32 cmd_ipdhcp(int argc, char *argv[]);
A_INT32 cmd_ping(A_INT32 argc, A_CHAR *argv[]);

int cmd_sleep(int argc, char *argv[]);

console_cmd_t app_strrcl_cmds[] = {
    {.name = (A_UCHAR *) "ping",
     .description = (A_UCHAR *) "ping",
     .execute = cmd_ping},
    {.name = (A_UCHAR *) "wmiconfig",
     .description = (A_UCHAR *) "wmiconfig cmd args",
     .execute = cmd_wmi_config},
    {.name = (A_UCHAR *) "benchtx",
     .description = (A_UCHAR *) "run the traffic transmit test",
     .execute = cmd_bench_tx_test},
    {.name = (A_UCHAR *) "benchrx",
     .description = (A_UCHAR *) "run the traffic receive test",
     .execute = cmd_bench_rx_test},
    {.name = (A_UCHAR *) "benchquit",
     .description = (A_UCHAR *) "quit the bench test",
     .execute = cmd_bench_quit},
     {.name = (A_UCHAR *) "benchmode",
     .description = (A_UCHAR *) "set benchmode v4/v6",
     .execute = cmd_bench_mode},

#ifdef SWAT_SLEEP
    {.name = (A_UCHAR *) "sleep",
     .description = (A_UCHAR *) "sleep num_seconds",
     .execute = cmd_sleep},
#endif
#ifdef SWAT_SSL
  {.name = (A_UCHAR *) "getcert",
     .description = (A_UCHAR *) "get certificate",
     .execute = ssl_get_cert_handler},
#endif
};


A_INT32 app_strrcl_cmds_num = sizeof (app_strrcl_cmds) / sizeof (console_cmd_t);


A_INT32 cmd_bench_tx_test(A_INT32 argc, A_CHAR *argv[])
{
    return swat_benchtx_handle(argc, argv);
}

A_INT32 cmd_bench_rx_test(A_INT32 argc, A_CHAR *argv[])
{
    return swat_benchrx_handle(argc, argv);
}

A_INT32 cmd_bench_quit(A_INT32 argc, A_CHAR *argv[])
{
     return swat_benchquit_handle(argc, argv);
}
A_INT32 cmd_bench_mode(A_INT32 argc, A_CHAR *argv[])
{
     if (argc != 2) {
         goto error;				 
     }

     if(strcmp(argv[1],"v4") == 0){
         v6_enabled = 0;
     }else if(strcmp(argv[1],"v6")== 0){
         v6_enabled = 1;
     }else{
         goto error;
     }

     return 0;
		 
error:
     SWAT_PTF("invalid arguments : enter v4/v6 \n");
     return 1;
}

int
cmd_sleep(int argc, char *argv[])
{
    int sleep_seconds;

    if (argc != 2) {
        return 0;
    }

   sleep_seconds = atoi(argv[1]);

   qcom_gpio_apply_peripheral_configuration(QCOM_PERIPHERAL_ID_UART_0, TRUE);
   tx_thread_sleep(sleep_seconds * TXQC_TICK_PER_SEC);
   qcom_gpio_apply_peripheral_configuration(QCOM_PERIPHERAL_ID_UART_0, FALSE);
}

int 
cmd_ap_connect(int argc, char *argv[])
{
   swat_wmiconfig_connect_ssid(currentDeviceId, (char *)argv[2]);
}

int
cmd_ipdhcp(int argc, char *argv[])
{
   swat_wmiconfig_ipdhcp(currentDeviceId);
   return SWAT_OK;
}

void
swat_wmiconfig_suspenable(A_UINT8 device_id)
{
    /* 1: enable */
    /* 0: disable */
     qcom_suspend_enable(1);
}


A_STATUS
swat_wmiconfig_suspstart(A_UINT32 susp_time)
{
     A_STATUS status;

     status = qcom_suspend_start(susp_time);
     if(status == A_ERROR){
         SWAT_PTF("Store recall failed\n");
     }
     return status;
}

A_INT32 cmd_ping(A_INT32 argc, A_CHAR *argv[])
{
    return swat_ping_handle(argc, argv,0);
}

A_INT32
cmd_wmi_config(A_INT32 argc, A_CHAR * argv[])
{
    if (argc == 1) {
        swat_wmiconfig_information(currentDeviceId);
        return 0;
    }

    if (strcmp(argv[1],"--connect")== 0)
    {
	cmd_ap_connect(argc, argv);
        return SWAT_OK;
    }
    else if (strcmp(argv[1],"--ipdhcp")== 0)
    {
        cmd_ipdhcp(argc, argv);
        return SWAT_OK;
    }
    else if (strcmp(argv[1],"--suspend")== 0)
    {
        swat_wmiconfig_suspenable(currentDeviceId);
        return SWAT_OK;
    }
    else if (strcmp(argv[1],"--suspstart")== 0)
    {
        A_UINT32 susp_time;
        A_UINT32 wifiMode;
        A_UINT8  *pbuf;
        A_STATUS status;

        swat_wmiconfig_devmode_get(currentDeviceId, &wifiMode);
        if (wifiMode == 0) {
            SWAT_PTF("NOT supported \n");
            return SWAT_ERROR;
        }

        if (argc == 3) {
            flag_strrcl_start = 1;

            susp_time = swat_atoi(argv[2]);
            pbuf = app_strrcl_store(buf_strrcl_len);
            if (pbuf == NULL)
            {
               flag_strrcl_start = 0;
               return SWAT_ERROR;
            }

            status = qcom_strrcl_store(STRRCL_APP_ID0, pbuf, buf_strrcl_len);
            if (status != A_OK)
            {
               flag_strrcl_start = 0;
               swat_buffer_free(&pbuf);
               return SWAT_ERROR;
            }
            
            status = swat_wmiconfig_suspstart(susp_time);
            if (status != A_OK)
            {
               flag_strrcl_start = 0;
               swat_buffer_free(&pbuf);
               return SWAT_ERROR;
            }
        } else {
            SWAT_PTF("    --suspstart <time>\n");
            return SWAT_ERROR;
        }

        return SWAT_OK;
     }
}

