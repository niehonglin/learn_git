/*
 * Copyright (c) 2015 Qualcomm Atheros, Inc.
 * All Rights Reserved.
 * Qualcomm Atheros Confidential and Proprietary.
 * $ATH_LICENSE_TARGET_C$
 */
#include "qcom_common.h"
#include "threadx/tx_api.h"
#include "qcom_cli.h"
#include "threadxdmn_api.h"
#include "qcom_wps.h"
#include "qcom_gpio.h"
#include "qcom_gpio_interrupts.h"

#define BYTE_POOL_SIZE (2*1024 + 256 )
#define AIRKISS_THREAD_STACK_SIZE (2 * 1024 )   
#define AIRKISS_SHELL_EVENT_FLAG  0x01

TX_THREAD airkiss_main_thread;
TX_EVENT_FLAGS_GROUP airkiss_shell_event;

typedef struct shell_gpio_info_s {
    unsigned int pin_num;
    unsigned int num_configs;
    gpio_pin_peripheral_config_t    *configs;
} airkiss_gpio_info_t;

void Airkiss_GpioConfig(void)
{
    A_UINT32    i = 0;

    /* Currently only one dynamic configuration is good enough as GPIO pin 2 
     * is used by both UART and I2S. UART configuration is already provided 
     * as part of tunable input text and only I2S configuration needs to be 
     * added here. None of GPIO configurations are added here as this demo 
     * doesnt use GPIO configurations. If customer applications requires it, 
     * it can be added here */
    airkiss_gpio_info_t    pin_configs[] = {
        { 2, 1, (gpio_pin_peripheral_config_t [1]) { { 5, 0x80001800 } } },
    };

    for (i = 0; i < sizeof(pin_configs)/sizeof(airkiss_gpio_info_t); i++)
    {
        if (A_OK != qcom_gpio_add_alternate_configurations(pin_configs[i].pin_num, 
                        pin_configs[i].num_configs, pin_configs[i].configs))
        {
            A_PRINTF("qcom_add_config failed for pin %d\n", pin_configs[i].pin_num);
        }
    }

    return;
}


/*
  * The name of this function is fixed.
  * It will be called by console_serial_event_callback to wake up the shell task.
  */
void wakeup_task_execute_cli_cmd(void)
{
    UINT status;

    status = tx_event_flags_set(&airkiss_shell_event, AIRKISS_SHELL_EVENT_FLAG, TX_OR);
    /* Check status.  */
    if (status != TX_SUCCESS) {
        QCOM_DEBUG_PRINTF("Failed to wake up shell task.\n");
    }

    return;
}

void Airkiss_ShellTask(void)
{
    void _console_process_char(void);
    UINT status;
    ULONG actual_flags;

    while (1) {
        status = tx_event_flags_get(&airkiss_shell_event, AIRKISS_SHELL_EVENT_FLAG, TX_OR_CLEAR,
                                    &actual_flags, TX_WAIT_FOREVER);
        /* Check status.  */
        if (status != TX_SUCCESS) {
            QCOM_DEBUG_PRINTF("Failed to take event, 0x%x\n", status);
            qcom_thread_msleep(10000);
            continue;
        }

        _console_process_char();
    }
}

void Airkiss_Entry(ULONG which_thread)
{
    #define PRINTF_ENBALE 1
    extern void user_pre_init(void);
    user_pre_init();
    qcom_enable_print(PRINTF_ENBALE);
	

    extern console_cmd_t airkiss_cmds[];

    extern int airkiss_cmds_num;

    console_setup();

    console_reg_cmds(airkiss_cmds, airkiss_cmds_num);

    /* Enable WPS and register event handler */
    /* THIS IS NOT THE RIGHT PLACE FOR THIS REGISTRATION. THIS API
     * SHOULD BE CALLED ALONG WITH qcom_wps_enable() WHEN enable is
     * set to 1.*/
    //qcom_wps_register_event_handler(qcom_wps_event_process_cb, NULL);

    /* Add all the alternate configurations for each GPIO pin */
    Airkiss_GpioConfig();

    Airkiss_ShellTask();
    /* Never returns */
}

void Airkiss_Main(void)
{   
    CHAR *stackPointer;
    
    stackPointer = qcom_mem_alloc(AIRKISS_THREAD_STACK_SIZE);

    tx_event_flags_create(&airkiss_shell_event, "airkiss event");

    tx_thread_create(&airkiss_main_thread, "airkiss thread", Airkiss_Entry,
                 0, stackPointer, AIRKISS_THREAD_STACK_SIZE, 16, 16, 4, TX_AUTO_START);

#if defined(AR6002_REV74)
    cdr_threadx_thread_init();
#endif
}

