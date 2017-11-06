/*
 * Copyright (c) 2015 Qualcomm Atheros, Inc.
 * All Rights Reserved.
 * Qualcomm Atheros Confidential and Proprietary.
 */

#ifndef  __QCOM_APP_STRRCL_H

#define  __QCOM_APP_STRRCL_H

#include  "dsetid.h"

enum {
	STRRCL_APP_ID0=0,
	STRRCL_APP_ID1,
	STRRCL_APP_ID2,
	STRRCL_APP_ID3,
	STRRCL_APP_ID4,
        MAX_STRRCL_APP_ID = MAX_STORE_RECALL_APP_ID
};

typedef struct {
    A_STATUS  (*_qcom_strrcl_store)(A_UINT32 id, A_UINT8 *pbuf, A_UINT32 len);
    A_STATUS  (*_qcom_strrcl_recall)(A_UINT32 id, A_UINT8 *pbuf, A_UINT32 buff_size, A_UINT32 *pActualLen);
    A_BOOL    (*_qcom_strrcl_in_recall)();
} APP_STRRCL_API_INDIRECTION_TABLE;


extern APP_STRRCL_API_INDIRECTION_TABLE pAppStrrclApiIndirectionTable;

#define APP_STRRCL_API_FN(fn) pAppStrrclApiIndirectionTable.fn


#if  !defined(AR6002_REV74)
#define  qcom_strrcl_store(id,p,l) \
		APP_STRRCL_API_FN(_qcom_strrcl_store)((id),(p),(l))

#define  qcom_strrcl_recall(id,p,l,ll) \
                APP_STRRCL_API_FN(_qcom_strrcl_recall)((id),(p), (l), (ll))

#define  qcom_strrcl_in_recall() \
                APP_STRRCL_API_FN(_qcom_strrcl_in_recall)()

#else
    A_STATUS  _qcom_strrcl_store(A_UINT32 id, A_UINT8 *pbuf, A_UINT32 len);
    A_STATUS  _qcom_strrcl_recall(A_UINT32 id, A_UINT8 *pbuf, A_UINT32 buff_size, A_UINT32 *pActualLen);
    A_BOOL    _qcom_strrcl_in_recall();

#define  qcom_strrcl_store(id, p, l) _qcom_strrcl_store((id), (p), (l))
#define  qcom_strrcl_recall(id, p, l, ll)  _qcom_strrcl_recall((id), (p), (l), (ll))
#define  qcom_strrcl_in_recall()  _qcom_strrcl_in_recall()

#endif

#endif

