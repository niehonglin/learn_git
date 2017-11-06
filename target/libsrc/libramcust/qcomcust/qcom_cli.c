/*
 * Copyright (c) 2011 Qualcomm Atheros, Inc.
 * All Rights Reserved.
 * Qualcomm Atheros Confidential and Proprietary.
 */

#include <osapi.h>
#include "console_api.h"
#include "qcom_cli.h"


static struct console_apis console;

void
console_setup(void)
{
    CONSOLE_CONFIG console_settings;

    //A_ALLOCRAM_INIT(0, 0);
    // gpio_install_int_dispatcher();
    SERP_MODULE_INSTALL();
    SERP_Init();
    UARTSERP_HW_MODULE_INSTALL();

    //CONSOLE_MODULE_INSTALL();
    console_module_install(&console);

    /* use all defaults */
    A_MEMZERO(&console_settings, sizeof (console_settings));

    /* because the EEPROM dump is rather large, make sure output buffer is sized appropriately */
    console_settings.output_buffer_size = 4096;
    console_settings.baud = 115200;
    console_settings.flags |= CONSOLE_CONFIG_FLAGS_AWAKE_FOR_READ;
#ifdef FPGA
    console_settings.phys_uart_id = 1;
    console_settings.logical_port_name = (A_CHAR *) "UART1";
#else
    console_settings.phys_uart_id = 0;
    console_settings.logical_port_name = (A_CHAR *) "UART0";
#endif

    //console_init(&console_settings);
    console._init(&console_settings);

    //console_start();
    console._start();
}

void
console_reg_cmds(console_cmd_t * cmds, int cmds_num)
{
    console._register_commands((CONSOLE_COMMAND *) cmds, cmds_num);
}
