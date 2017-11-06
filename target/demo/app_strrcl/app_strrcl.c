/*
  * Copyright (c) 2015 Qualcomm Atheros, Inc..
  * All Rights Reserved.
  * Qualcomm Atheros Confidential and Proprietary.
  */

#include "qcom_common.h"
#include "threadx/tx_api.h"
#include "qcom_cli.h"
#include "threadxdmn_api.h"
#include "qcom_wps.h"
#include "qcom_app_strrcl.h"

TX_THREAD host_thread;
#ifdef REV74_TEST_ENV4

#define BYTE_POOL_SIZE (2*1024 + 128 )
#define PSEUDO_HOST_STACK_SIZE (2 * 1024 )   /* small stack for pseudo-Host thread */

#else

#define BYTE_POOL_SIZE (3*1024 + 256 )
#define PSEUDO_HOST_STACK_SIZE (3 * 1024 )   /* small stack for pseudo-Host thread */

#endif
TX_BYTE_POOL pool;

int app_strrcl_recall(A_UINT8 device_id, A_UINT8 *pbuf);
extern A_UINT8 buf_strrcl[];
extern A_UINT32  buf_strrcl_len;
extern A_UINT8 currentDeviceId;

void
app_strrcl_shell_host_entry(ULONG which_thread)
{
    #define PRINTF_ENABLE 1
    extern void user_pre_init(void);
    user_pre_init();
    qcom_enable_print(PRINTF_ENABLE);
	

    extern console_cmd_t app_strrcl_cmds[];

    extern int app_strrcl_cmds_num;

    extern void task_execute_cli_cmd();


    console_setup();

    console_reg_cmds(app_strrcl_cmds, app_strrcl_cmds_num);

    /* Enable WPS and register event handler */
    /* THIS IS NOT THE RIGHT PLACE FOR THIS REGISTRATION. THIS API
     * SHOULD BE CALLED ALONG WITH qcom_wps_enable() WHEN enable is
     * set to 1.*/

    task_execute_cli_cmd();
    /* Never returns */
}


void user_main(void)
{   
    extern void task_execute_cli_cmd();
    
    tx_byte_pool_create(&pool, "cdrtest pool", TX_POOL_CREATE_DYNAMIC, BYTE_POOL_SIZE);

    {
        CHAR *pointer;
        tx_byte_allocate(&pool, (VOID **) & pointer, PSEUDO_HOST_STACK_SIZE, TX_NO_WAIT);

        tx_thread_create(&host_thread, "cdrtest thread", app_strrcl_shell_host_entry,
                         0, pointer, PSEUDO_HOST_STACK_SIZE, 16, 16, 4, TX_AUTO_START);
    }

#if defined(AR6002_REV74)
    cdr_threadx_thread_init();
#endif
}

