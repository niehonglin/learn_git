/*
  * Copyright (c) 2013-2015 Qualcomm Atheros, Inc..
  * All Rights Reserved.
  * Qualcomm Atheros Confidential and Proprietary.
  */

#include "qcom_common.h"
#include "swat_wmiconfig_common.h"
#include "qcom_gpio.h"
#include "swat_parse.h"

#if defined(OTA_ENABLE)

#include "qcom_ota_api.h"
A_UINT8 ota_initted=0;
TX_THREAD ota_thread;
TX_BYTE_POOL ota_pool;
A_CHAR *ota_thread_stack_pointer;
#endif

extern qcom_timer_t shell_timer;

extern int v6_enabled;
// wmiconfig
A_INT32 cmd_wmi_config(A_INT32 argc, A_CHAR *argv[]);

// iwconfig scan
A_INT32 cmd_iwconfig_scan(A_INT32 argc, A_CHAR *argv[]);

// bench test
A_INT32 cmd_bench_tx_test(A_INT32 argc, A_CHAR *argv[]);
A_INT32 cmd_bench_rx_test(A_INT32 argc, A_CHAR *argv[]);
A_INT32 cmd_bench_quit(A_INT32 argc, A_CHAR *argv[]);
A_INT32 cmd_bench_mode(A_INT32 argc, A_CHAR *argv[]);
// ping
A_INT32 cmd_ping(A_INT32 argc, A_CHAR *argv[]);
A_INT32 cmd_ping6(A_INT32 argc, A_CHAR *argv[]);

// ota
#if defined(OTA_ENABLE)

A_INT32 cmd_ota_fw_upgrade(A_INT32 argc, A_CHAR *argv[]);
A_INT32 cmd_ota_daemon_start(A_INT32 argc, A_CHAR *argv[]);
A_INT32 cmd_ota_daemon_stop(A_INT32 argc, A_CHAR *argv[]);
#endif

#if 1 //(defined(REV74_TEST_ENV1) || defined (AR6002_REV74))
extern int cmd_i2c_test(int argc, char *argv[]);
#if 1//def SWAT_I2S_TEST
extern int cmd_i2s_test(int argc, char *argv[]);
#endif
#endif

int cmd_sleep(int argc, char *argv[]);

console_cmd_t cust_cmds[] = {
#ifdef SWAT_PING
    {.name = (A_UCHAR *) "ping",
     .description = (A_UCHAR *) "ping",
     .execute = cmd_ping},
#ifdef IP_V6
    {.name = (A_UCHAR *) "ping6",
     .description = (A_UCHAR *) "ping6",
     .execute = cmd_ping6},
#endif
#endif

    /* Used to Auto Test */
#if defined(SWAT_WMICONFIG)
    {.name = (A_UCHAR *) "wmiconfig",
     .description = (A_UCHAR *) "wmiconfig cmd args",
     .execute = cmd_wmi_config},
    {.name = (A_UCHAR *) "iwconfig",
     .description = (A_UCHAR *) "scan for APs and specified SSID",
     .execute = cmd_iwconfig_scan},
#endif

#if defined(SWAT_BENCH)
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

    /* {.name = (A_UCHAR *) "benchdbg", */
     /* .description = (A_UCHAR *) "dbg the bench test", */
     /* .execute = swat_benchdbg_handle}, */
#endif

#ifdef OTA_ENABLE
    {.name = (A_UCHAR *) "ota_fw_upgrade",
     .description = (A_UCHAR *) "ota_fw_upgrade <OTA-server-ip> <image-name> <protocol>",
     .execute = cmd_ota_fw_upgrade},
    {.name = (A_UCHAR *) "ota_upgrade_monitor_start",
     .description = (A_UCHAR *) "Start demo OTA deamon",
     .execute = cmd_ota_daemon_start},
    {.name = (A_UCHAR *) "ota_upgrade_monitor_stop",
     .description = (A_UCHAR *) "Stop demo OTA deamon",
     .execute = cmd_ota_daemon_stop},
#endif

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

#ifdef SWAT_I2C
/* i2c */

  {.name = (A_UCHAR *) "i2c",
     .description = (A_UCHAR *) "i2c read/write",
     .execute = cmd_i2c_test},
#endif  
#ifdef SWAT_I2S
  {.name = (A_UCHAR *) "i2s",
     .description = (A_UCHAR *) "i2s test",
     .execute = cmd_i2s_test},
#endif


#ifdef ENABLE_HTTPS_CLIENT  
  {.name = (A_UCHAR *) "httpsc",
     .description = (A_UCHAR *) "https client",
     .execute = https_client_handler},
#endif
#ifdef ENABLE_HTTPS_SERVER 
  {.name = (A_UCHAR *) "httpss",
     .description = (A_UCHAR *) "https server",
     .execute = https_server_handler},
#endif

};

A_INT32 cust_cmds_num = sizeof (cust_cmds) / sizeof (console_cmd_t);


#ifdef SWAT_PING
A_INT32 cmd_ping(A_INT32 argc, A_CHAR *argv[])
{
    return swat_ping_handle(argc, argv,0);
}
A_INT32 cmd_ping6(A_INT32 argc, A_CHAR *argv[])
{
    return swat_ping_handle(argc, argv,1);
}

#endif

/* For Auto Test */
#if defined(SWAT_WMICONFIG)
A_INT32 cmd_wmi_config(A_INT32 argc, A_CHAR *argv[])
{
    return swat_wmiconfig_handle(argc, argv);
}

A_INT32 cmd_iwconfig_scan(A_INT32 argc, A_CHAR *argv[])
{
    return swat_iwconfig_scan_handle(argc, argv);
}
#endif

#if defined(SWAT_BENCH)
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

#endif


#if defined(OTA_ENABLE)


void
_ota_daemon_start(unsigned long which_thread){
    int resp_code,length;
    qcom_ota_daemon_start(currentDeviceId,&resp_code,&length);

}
A_INT32
cmd_ota_daemon_start(A_INT32 argc, A_CHAR *argv[])
{
    /*extern A_INT32 qcom_ota_daemon_start(void);*/
    int i;

	if(!ota_initted)
	{
        tx_byte_pool_create(&ota_pool,
                        "ota pool",
                        TX_POOL_CREATE_DYNAMIC,
                        OTA_POOL_SIZE);
		tx_byte_allocate(&ota_pool, (VOID **)&ota_thread_stack_pointer, OTA_STACK_SIZE, TX_NO_WAIT);
            
		ota_initted = 1;
	}
	else
	{
		
        tx_thread_delete(&ota_thread);
	}



    tx_thread_create(&ota_thread, "ota thread", _ota_daemon_start,
          i, ota_thread_stack_pointer, OTA_STACK_SIZE, 16, 16, 4, TX_AUTO_START);

	return 0;

    
}

A_INT32
cmd_ota_daemon_stop(A_INT32 argc, A_CHAR *argv[])
{

    qcom_ota_daemon_stop();
    tx_thread_terminate(&ota_thread);

}
#endif

int
cmd_sleep(int argc, char *argv[])
{
    int sleep_seconds;
    A_UINT32    peripheral_map = 0;
    A_STATUS    retVal = 0;

    if (argc != 2) {
        return 0;
    }

    sleep_seconds = atoi(argv[1]);

    retVal = qcom_gpio_apply_peripheral_configuration(QCOM_PERIPHERAL_ID_UART_0, 
                                                                TRUE);
    if (A_OK != retVal)
    {
        A_STATUS status = A_OK;

        /* Get the failure reason */
        status = qcom_gpio_peripheral_pin_conflict_check(QCOM_PERIPHERAL_ID_UART_0, 
                                                            &peripheral_map);
        SWAT_PTF("UART0 dis fail, map %x, status %d\n", peripheral_map, status);
    }
 
    qcom_timer_stop(&shell_timer);
  
    tx_thread_sleep(sleep_seconds * TXQC_TICK_PER_SEC);
   
    qcom_timer_start(&shell_timer);

    retVal = qcom_gpio_apply_peripheral_configuration(QCOM_PERIPHERAL_ID_UART_0, 
                                                                    FALSE);
    if (A_OK != retVal)
    {
        A_STATUS status = A_OK;

        /* Get the failure reason */
        status = qcom_gpio_peripheral_pin_conflict_check(QCOM_PERIPHERAL_ID_UART_0, 
                                                            &peripheral_map);
        SWAT_PTF("UART0 en fail, map %x, status %d\n", peripheral_map, status);
    }
}
