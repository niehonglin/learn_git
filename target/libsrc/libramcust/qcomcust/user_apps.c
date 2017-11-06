/*
 * Copyright (c) 2013 Qualcomm Atheros, Inc..
 * All Rights Reserved.  
 * Qualcomm Atheros Confidential and Proprietary.
 */

#include <osapi.h>
#include "qcom/wmi_cdr.h"
#include "qcom/wifi_api.h"
#include "threadxdmn_api.h"
#include "qcom/qcom_event.h"
#include "qcom/qcom_system.h"
#include "qcom/qcom_wlan.h"

//extern int qcom_event_dispatch(void *pContext,    /* app supplied context */
//                            A_UINT16 EventID,  /* event ID */
//                            A_UINT16 info, /* the info param passed to WMI_SendEvent */
//                            A_UINT8 * pEvtBuffer,  /* pointer to event data, if needed */
//                            int Length);  /* length of event data, if needed */
extern A_UINT8 gNumOfWlanDevices;

int user_bgrs_htc = 0;

WMI_FILTER_ACTION
qcom_event_filter_cb_forstart(void *pContext,   /* app supplied context */
                      A_UINT16 EventID, /* event ID */
                      A_UINT16 info,    /* the info param passed to WMI_SendEvent */
                      A_UINT8 * pEvtBuffer, /* pointer to event data, if needed */
                      int Length)
{   
    A_UINT8 device_id = info & 0xFF;

    if (WMI_READY_EVENTID == EventID)
    {
        if (device_id < WLAN_NUM_OF_DEVICES) {
            extern int cdr_ready_flag;
            cdr_ready_flag = 1;
        }
    }
    return WMI_EVT_FILTER_DISCARD;  //WMI_EVT_FILTER_ALLOW; //WMI_EVT_FILTER_DISCARD;
}

void
qcom_sta_register_callbacks()
{
#if defined(AR6002_REV72)
    /* Attach a low-level background service */
    extern void _qcom_HTC_Housekeeping(void *args_ptr, A_UINT32 args_len);
    user_bgrs_htc = A_BGR_ATTACH_LOW(_qcom_HTC_Housekeeping, NULL);
#endif
    /* Add a new wmi event filter */
    WMI_RegisterEventFilter(qcom_event_filter_cb_forstart, NULL, TRUE);

    return;
}

/* must be called in thread context */
void
user_pre_init(void)
{
    int i;
    
#if defined(FPGA)
    /*for hostless*/
    //extern A_BOOT_INFO AR6K_boot_info;
    //extern A_BOOL AR6K_host_present;
    //AR6K_boot_info.interconnect_type = A_HOST_IC_NONE; //A_HOST_IC_NONE;
	/* the event using CDR mechanism will use this variable */
    //AR6K_host_present = 0;
#endif
    qcom_ready();

   	CDR_wmi_dev_init();
    cdr_wmi_event_handler_init(QCOM_EVENT_API_FN(_qcom_event_dispatch));
    /* Add a new wmi event filter */
    WMI_RegisterEventFilter(qcom_event_filter_cb_forstart, NULL, FALSE);
#if 0
    extern WMI_FILTER_ACTION _qcom_host_less_event_filter(void *pContext,  /* app supplied context */
                                                       A_UINT16 EventID,    /* event ID */
                                                       A_UINT16 info,   /* the info param passed to WMI_SendEvent */
                                                       A_UINT8 * pEvtBuffer,    /* pointer to event data, if needed */
                                                       int Length);
    WMI_RegisterEventFilter(_qcom_host_less_event_filter, NULL, TRUE);
#endif

    /* set default keep alive interval 60s */
    for (i = 0; i < gNumOfWlanDevices; i++) {
        #define KEEP_ALIVE_INTERVAL_DEFAULT 60
        qcom_set_keep_alive(i, KEEP_ALIVE_INTERVAL_DEFAULT);
    }
    
    return;
}


