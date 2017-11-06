/*
  * Copyright (c) 2013-2015 Qualcomm Atheros, Inc..
  * All Rights Reserved.
  * Qualcomm Atheros Confidential and Proprietary.
  */

#include "qcom_common.h"
#include "qcom_gpio.h"
#include "qcom_cli.h"

/* define */
#define AIRKISS_CMD_DESC_PING "ping <host_ip> [ -c <count> -s <size> -i <interval> ]"
#define AIRKISS_CMD_DESC_WMICONFIG "wmiconfig --help\n\
          --version\n\
          --reset\n\
          --device <id>\n\
          --mode <station>\n\
          --channel <ch>\n\
          --p <pass>\n\
          --wpa <ver:1-WPA, 2-RSN> <ucipher:TKIP or CCMP> <mcipher:TKIP or CCMP>\n\
          --connect <ssid>\n\
          --disc\n\
          --pwrmode <mode:1=Power save, 0= Max Perf>\n\
          --ipconfig = Show IP parameters\n\
          --ipstatic <IP Address> <Subnet Mask> <Default Gateway>\n\
          --ipdhcp"

#define AIRKISS_CMD_DESC_IWCONFIG "iwconfig --scan [ssid]"
#define AIRKISS_CMD_DESC_AIRKISS "airkiss --start <start_channel> <end_channel> <dwell>\n\
        --stop"

/* external APIs */
extern A_INT32 Airkiss_CmdHandlerPing(A_INT32 argc, A_CHAR * argv[]);
extern A_INT32 Airkiss_CmdHandlerIwconfig(A_INT32 argc, A_CHAR * argv[]);
extern A_INT32 Airkiss_CmdHandlerWmiconfig(A_INT32 argc, A_CHAR * argv[]);
extern A_INT32 Airkiss_CmdHandlerAirkissConfig(A_INT32 argc, A_CHAR * argv[]);
    
/* internal APIs */
A_INT32 Airkiss_CmdPing(A_INT32 argc, A_CHAR *argv[]);
A_INT32 Airkiss_CmdWmiconfig(A_INT32 argc, A_CHAR *argv[]);
A_INT32 Airkiss_CmdIwconfig(A_INT32 argc, A_CHAR *argv[]);
A_INT32 Airkiss_AirkissConfig(A_INT32 argc, A_CHAR *argv[]);
    
console_cmd_t airkiss_cmds[] = {
    {.name = "ping",
     .description = AIRKISS_CMD_DESC_PING,
     .execute = Airkiss_CmdPing},
    {.name = "wmiconfig",
     .description = AIRKISS_CMD_DESC_WMICONFIG,
     .execute = Airkiss_CmdWmiconfig},
    {.name = "iwconfig",
     .description = AIRKISS_CMD_DESC_IWCONFIG,
     .execute = Airkiss_CmdIwconfig},
     {.name = "airkiss",
     .description = AIRKISS_CMD_DESC_AIRKISS,
     .execute = Airkiss_AirkissConfig},
};

A_INT32 airkiss_cmds_num = sizeof (airkiss_cmds) / sizeof (console_cmd_t);

A_INT32 Airkiss_CmdPing(A_INT32 argc, A_CHAR *argv[])
{
    return Airkiss_CmdHandlerPing(argc, argv);
}

A_INT32 Airkiss_CmdWmiconfig(A_INT32 argc, A_CHAR *argv[])
{
    return Airkiss_CmdHandlerWmiconfig(argc, argv);
}

A_INT32 Airkiss_CmdIwconfig(A_INT32 argc, A_CHAR *argv[])
{
    return Airkiss_CmdHandlerIwconfig(argc, argv);
}

A_INT32 Airkiss_AirkissConfig(A_INT32 argc, A_CHAR *argv[])
{
    return Airkiss_CmdHandlerAirkissConfig(argc, argv);
}

