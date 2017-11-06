/*
  * Copyright (c) 2013 Qualcomm Atheros, Inc..
  * All Rights Reserved.
  * Qualcomm Atheros Confidential and Proprietary.
  */

#if defined(P2P_ENABLED)
extern void swat_wmiconfig_p2p(int argc, char *argv[]);

/*local var*/
typedef struct {
    A_CHAR wps_pin[WPS_PIN_LEN+1] ;
    A_UINT8 peer_addr[ATH_MAC_LEN] ;
    A_UINT8 wps_role ;
    A_UINT8 dialog_token;
    A_UINT16 dev_password_id;
        A_UINT8 wps_method;
    A_BOOL persistent;
} CMD_P2P_INFO;

/* P2P Event Queue Nodes */
typedef struct _CMD_P2P_EVENT_INFO_{
   struct _CMD_P2P_EVENT_INFO_ *nextEvent; // link to next event in the queue
   A_UINT8 device_id; 
   P2P_EVENT_ID event_id;
   A_UINT32 Length;
   A_UINT8 pBuffer[1]; //variable size buffer. Allocation based on the Length
} CMD_P2P_EVENT_INFO;

/* P2P Event Queue */
typedef struct {
  TX_MUTEX eventQueueMutex; //queue mutex
  struct _CMD_P2P_EVENT_INFO_ *pEventHead; //queue head
  struct _CMD_P2P_EVENT_INFO_ *pEventTail; // queue tail
} CMD_P2P_EVENT_LIST;
#endif

