/*
 * Copyright (c) 2015 Qualcomm Atheros, Inc.
 * All Rights Reserved.
 * Qualcomm Atheros Confidential and Proprietary.
 * $ATH_LICENSE_TARGET_C$
 */

#include "qcom_common.h"
#include "qcom_app_strrcl.h"
#include "qcom_event.h"
#include "swat_parse.h"

extern A_UINT8 buf_strrcl[];
extern A_UINT32  buf_strrcl_len;
extern A_BOOL    recall_flag;
extern int cdr_ready_flag;

extern TX_EVENT_FLAGS_GROUP  *pwmi_ready_event_flags;

TX_EVENT_FLAGS_GROUP event_flags_0;
TX_EVENT_FLAGS_GROUP event_flags_wmi_ready;

static qcom_timer_t shell_timer;
extern void qcom_msleep(int ms);
extern int qcom_task_start(void (*fn) (unsigned int), unsigned int arg, int stk_size, int tk_ms);
void
task_execute_cli_cmd()
{
    void _console_process_char(void);
    UINT status;
    ULONG actual_flags;
    A_UINT32   actual_len;
    A_UINT8    *pbuf_strrcl;

    /* task resource recycle */
    void qcom_task_timer_handler(unsigned int alarm, void *data);
    qcom_timer_init(&shell_timer, qcom_task_timer_handler, NULL, 3 * 1000, PERIODIC);
    qcom_timer_start(&shell_timer);

    status = tx_event_flags_create(&event_flags_0, "event flags 0");
    //A_PRINTF("create event 0x%x.\n", status);
    status = qcom_strrcl_in_recall();

    if ((status != 0) && (!swat_strrcl_recall_done()))
    {
#if 1
        while (swat_strrcl_recall_done() != TRUE)
        {
            qcom_thread_msleep(1);
        }
#else
        qcom_thread_msleep(1000);
#endif

        pbuf_strrcl = swat_mem_malloc(buf_strrcl_len);
        if (pbuf_strrcl != NULL)
        {
           recall_flag = qcom_strrcl_recall(STRRCL_APP_ID0, pbuf_strrcl, buf_strrcl_len, &actual_len);
           if (recall_flag == A_OK)
           {
              qcom_printf("data recall\n");
              app_strrcl_recall(pbuf_strrcl, actual_len);
           }
           else
           {
              qcom_printf("recall failed");
           }
           swat_buffer_free(&pbuf_strrcl);
        }
    }

    while (1) {
        status = tx_event_flags_get(&event_flags_0, 0x1, TX_OR_CLEAR,
                                    &actual_flags, TX_WAIT_FOREVER);
        /* Check status.  */
        if (status != TX_SUCCESS) {
            QCOM_DEBUG_PRINTF("take event failed, 0x%x\n", status);
            qcom_thread_msleep(10000);
            continue;
        }

        _console_process_char();
    }
}

void
wakeup_task_execute_cli_cmd()
{
    UINT status;

    status = tx_event_flags_set(&event_flags_0, 0x1, TX_OR);
    /* Check status.  */
    if (status != TX_SUCCESS) {
        QCOM_DEBUG_PRINTF("wake up cli failed.\n");
    }

}

void
start_task_exec_cli_cmd(ULONG index)
{
    UINT status;
    //A_PRINTF("start task to execute cli command.\n");
    /* added by xzm 2013-7-9 */
    status = tx_event_flags_create(&event_flags_0, "event flags 0");
    //A_PRINTF("create event 0x%x.\n", status);
    qcom_task_start(task_execute_cli_cmd, index, 3072, 50);
}

