/**
 *  * @file
 *   */
/******************************************************************************
 *  Copyright (c) 2013 Qualcomm Atheros, Inc.
 *  All Rights Reserved.
 *  Qualcomm Atheros Confidential and Proprietary.
 *  $ATH_LICENSE_HOSTSDK0_C$
 * 
 *  Unless required by applicable law or agreed to in writing, software
 *  distributed under the license is distributed on an "AS IS" BASIS,
 *  WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 *  See the license for the specific language governing permissions and
 *  limitations under the license.
 *  ******************************************************************************/

#ifndef __QCOM_EVENT_H__
#define __QCOM_EVENT_H__




#ifdef LIBCUST_INDIRECT_ACCESS

typedef struct  {
    void (* _noise_floor_print)(void);
    void (* _app_fsm1)(unsigned char device_id, void *args_ptr, unsigned int args_len);
    void (* _my_wifi_conn_callback)(unsigned char device_id, int value);
    int (* _qcom_event_dispatch)(void *pContext,    /* app supplied context */
            unsigned short EventID,  /* event ID */
            unsigned short info, /* the info param passed to WMI_SendEvent */
            unsigned char * pEvtBuffer,  /* pointer to event data, if needed */
            int Length);  /* length of event data, if needed */
    void (* _user_app_init)(void);
    A_STATUS (*_qcom_p2p_event_process)(void *pContext,A_UINT16 EventID,A_UINT16 info,A_UINT8 * pEvtBuffer,int Length);
    void (*_qcom_p2p_connect_event_process)(void *pContext,A_UINT16 EventID,A_UINT16 info,A_UINT8 * pEvtBuffer,int Length);
    A_BOOL (*_qcom_event_strrcl_recall_done)(A_UINT8 device_id);
}QCOM_EVENT_API_INDIRECTION_TABLE;

extern QCOM_EVENT_API_INDIRECTION_TABLE qcomEventApiIndirectionTable;
#define QCOM_EVENT_API_FN(fn) qcomEventApiIndirectionTable.fn

#else
void _noise_floor_print(void);
void _app_fsm1(unsigned char device_id, void *args_ptr, unsigned int args_len);
void _my_wifi_conn_callback(unsigned char device_id, int value);
int _qcom_event_dispatch(void *pContext,    /* app supplied context */
        unsigned short EventID,  /* event ID */
        unsigned short info, /* the info param passed to WMI_SendEvent */
        unsigned char * pEvtBuffer,  /* pointer to event data, if needed */
        int Length);  /* length of event data, if needed */
void _user_app_init(void);
A_STATUS _qcom_p2p_event_process(void *pContext,A_UINT16 EventID,A_UINT16 info,A_UINT8 * pEvtBuffer,int Length);
void  _qcom_p2p_connect_event_process(void *pContext,A_UINT16 EventID,A_UINT16 info,A_UINT8 * pEvtBuffer,int Length);
    
#define QCOM_EVENT_API_FN(fn) fn

#endif

#define noise_floor_print() \
    QCOM_EVENT_API_FN(_noise_floor_print())

#define app_fsm1(id, ptr, len) \
    QCOM_EVENT_API_FN(_app_fsm1((id), (ptr), (len)))

#define my_wifi_conn_callback(id, value) \
    QCOM_EVENT_API_FN(_my_wifi_conn_callback((id), (value)))

#define qcom_event_dispatch(ctxt, id, info ,buf, len) \
    QCOM_EVENT_API_FN(_qcom_event_dispatch((ctxt), (id), (info) ,(buf), (len)))

#define user_app_init() \
    QCOM_EVENT_API_FN(_user_app_init())

#if defined(AR6002_REV74)
A_BOOL patch_qcom_event_strrcl_recall_done(A_UINT8 device_id);
#define qcom_event_strrcl_recall_done(id)  patch_qcom_event_strrcl_recall_done((id))
#else
#define qcom_event_strrcl_recall_done()  QCOM_EVENT_API_FN(_qcom_event_strrcl_recall_done())
#endif

#if !defined(AR6002_REV72) 
#define qcom_p2p_event_process(pContext,EventID,info, pEvtBuffer,Length)\
    QCOM_EVENT_API_FN(_qcom_p2p_event_process((pContext),(EventID),(info),(pEvtBuffer),(Length)))

#define qcom_p2p_connect_event_process(pContext,EventID,info, pEvtBuffer,Length)\
    QCOM_EVENT_API_FN(_qcom_p2p_connect_event_process((pContext),(EventID),(info),(pEvtBuffer),(Length)))

#else

#if defined(P2P_ENABLED)
extern A_STATUS _qcom_p2p_priv_event_process(void *pContext,    /* app supplied context */
                            A_UINT16 EventID,  /* event ID */
                            A_UINT16 info, /* the info param passed to WMI_SendEvent */
                              A_UINT8  * pEvtBuffer,  /* pointer to event data, if needed */
                            int Length);

extern void _qcom_p2p_priv_connect_event_process(void *pContext,    /* app supplied context */
                            A_UINT16 EventID,  /* event ID */
                            A_UINT16 info, /* the info param passed to WMI_SendEvent */
                              A_UINT8  * pEvtBuffer,  /* pointer to event data, if needed */
                            int Length);

#define qcom_p2p_connect_event_process(pContext,EventID,info, pEvtBuffer,Length)\
    _qcom_p2p_priv_connect_event_process((pContext),(EventID),(info),(pEvtBuffer),(Length))

#define qcom_p2p_event_process(pContext,EventID,info, pEvtBuffer,Length)\
        _qcom_p2p_priv_event_process((pContext),(EventID),(info),(pEvtBuffer),(Length))
#else
#define qcom_p2p_event_process(pContext,EventID,info, pEvtBuffer,Length)\
    wmi_p2p_event_handler(pContext, EventID, info, pEvtBuffer, Length) 

/*NOP in case of P2P*/
#define qcom_p2p_connect_event_process(pContext,EventID,info, pEvtBuffer,Length)



#endif





#endif


#endif
