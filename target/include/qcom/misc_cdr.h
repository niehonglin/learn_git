/*
  * Copyright (c) 2013 Qualcomm Atheros, Inc..
  * All Rights Reserved.
  * Qualcomm Atheros Confidential and Proprietary.
  */
  
#ifndef __MISC_CDR_H__
#define __MISC_CDR_H__

#include "cdr.h"

typedef struct {
    void *event;
    int completed;
} cdr_misc_completed_t;

typedef struct {
    unsigned int cmd;
    int dev_id;
    int ret;
} wps_init_priv_t;

typedef struct {
    unsigned int cmd;
    char *manufacture;
    char *model_name;
    char *model_num;
    char *serial_num;
    char *dev_name;
}wps_atrrs_t;

typedef struct {
    unsigned int cmd;
    int dev_id;
    wps_atrrs_t wps_atrrs;
    int ret;
}wps_atrrs_priv_t;

typedef struct {
    unsigned int cmd;
    int dev_id;
    int ret;
} wps_cancel_priv_t;

typedef struct {
    unsigned int cmd;
    int enable;
    int ret;
} dbg_print_priv_t;

#if 0
int cdr_wps_init(int dev_id);
int cdr_wps_set_attrs(int dev_id, wps_atrrs_t *pwps_attrs);
int cdr_wps_cancel(int dev_id);

#endif
void cdr_wps_set_attrs_handle(struct cdr_req_s *cdr_req);
void cdr_wps_init_request_handle(struct cdr_req_s *cdr_req);
void cdr_wps_cancel_request_handle(struct cdr_req_s *cdr_req);
void cdr_ready_handler(struct cdr_req_s *cdr_req);
void cdr_dbg_print_handle(struct cdr_req_s *cdr_req);


typedef struct  {

	void (* _cdr_misc_common_completed)(void *arg, struct cdr_req_s *cdr_req);
	void (* _bgr_misc_cdr_initiate)(void *request, unsigned int req_length);
	int (* _cdr_wps_init)(int dev_id);
	int (* _cdr_wps_set_attrs)(int dev_id, wps_atrrs_t *pwps_attrs);
	int (* _cdr_wps_cancel)(int dev_id);
	int (* _cdr_dbg_print)(int enable);
}PHOST_MISC_API_INDIRECTION_TABLE;

extern PHOST_MISC_API_INDIRECTION_TABLE phostMiscApiIndirectionTable;
#define PHOST_MISC_API_FN(fn) phostMiscApiIndirectionTable.fn

#define cdr_misc_common_completed(_a, _r) \
    PHOST_MISC_API_FN(_cdr_misc_common_completed((_a), (_r)))

#define bgr_misc_cdr_initiate(_r, _l) \
    PHOST_MISC_API_FN(_bgr_misc_cdr_initiate((_r), (_l)))

#define cdr_wps_init(_d) \
    PHOST_MISC_API_FN(_cdr_wps_init((_d)))

#define cdr_wps_set_attrs(_d, _a) \
    PHOST_MISC_API_FN(_cdr_wps_set_attrs((_d), (_a)))

#define cdr_wps_cancel(_d) \
    PHOST_MISC_API_FN(_cdr_wps_cancel((_d)))

#define cdr_dbg_print(_d) \
    PHOST_MISC_API_FN(_cdr_dbg_print((_d)))
    
#endif

