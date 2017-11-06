/*
  * Copyright (c) 2013-2015 Qualcomm Atheros, Inc..
  * All Rights Reserved.
  * Qualcomm Atheros Confidential and Proprietary.
  */

#include "qcom_common.h"
#include "threadx/tx_api.h"
#include "threadxdmn_api.h"
#include "ww_hlink.h"


//#define PSEUDO_UART_STACK_SIZE (4 * 1024)
#define PSEUDO_HOST_STACK_SIZE (13* 1024 )   /* small stack for pseudo-Host thread */
//#define PSEUDO_RSSI_STACK_SIZE (1 * 1024)

#define BYTE_POOL_SIZE (PSEUDO_HOST_STACK_SIZE + 256 )

TX_THREAD host_thread;

//TX_THREAD uart_thread;

//TX_THREAD rssi_thread;

TX_BYTE_POOL pool;

void user_main(void)
{   
    //extern void task_execute_cli_cmd();
    tx_byte_pool_create(&pool, "cdrtest pool", TX_POOL_CREATE_DYNAMIC, BYTE_POOL_SIZE);
/*
    {
        CHAR *uart_pointer;
        tx_byte_allocate(&pool, (VOID **) & uart_pointer, PSEUDO_UART_STACK_SIZE, TX_NO_WAIT);

        tx_thread_create(&uart_thread, "uart thread", uart_entry,
                         0, uart_pointer, PSEUDO_UART_STACK_SIZE, 16, 16, 4, TX_AUTO_START);
    }
*/
	
    {
        CHAR *pointer;
        tx_byte_allocate(&pool, (VOID **) & pointer, PSEUDO_HOST_STACK_SIZE, TX_NO_WAIT);

        tx_thread_create(&host_thread, "cdrtest thread", hlink_entry,
                         0, pointer, PSEUDO_HOST_STACK_SIZE, 16, 16, 4, TX_AUTO_START);
    }
/*
    {
        CHAR *rssi_pointer;
        tx_byte_allocate(&pool, (VOID **) & rssi_pointer, PSEUDO_RSSI_STACK_SIZE, TX_NO_WAIT);

        tx_thread_create(&rssi_thread, "rssi thread", rssi_entry,
                         0, rssi_pointer, PSEUDO_RSSI_STACK_SIZE, 16, 16, 4, TX_AUTO_START);
    }
*/
	
#if defined(AR6002_REV74)
    cdr_threadx_thread_init();
#endif
}


