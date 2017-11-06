/******************************************************************************
 * Copyright (c) 2013 Qualcomm Atheros, Inc.
 * All Rights Reserved.
 * Qualcomm Atheros Confidential and Proprietary.
 * $ATH_LICENSE_HOSTSDK0_C$
 ******************************************************************************/

#ifndef __QCOM_TIMER_H__
#define __QCOM_TIMER_H__

#ifdef __cplusplus
extern "C" {
#endif

#define A_MS_PER_SECOND      1000         /* Milliseconds */
#define A_US_PER_SECOND      1000000      /* Microseconds */
#define A_NS_PER_SECOND      1000000000   /* Nanoseconds  */

/* Timer API */
typedef enum {
    ONESHOT = 0,
    PERIODIC = 1,
} qcom_timer_type_e;

typedef struct {
    unsigned int timer[6];
    int timeout;
    int type;
    unsigned int arg;
} qcom_timer_t;

#if 0
/**
 * This function is used to initialize a timer.
 * @param qtimer                pointer to qcom_timer_t, used to save internal data of timer.
 * @param fn                    call back function for the timer.
 * @param timeout               timeout value for the timer, unit in milliseconds.
 * @param type                  ONESHOT or PERIODIC.
 * @return                      0 on success. else on error.
 */
int qcom_timer_init(qcom_timer_t* qtimer, void (*fn)(unsigned int, void *), void* arg, int timeout, qcom_timer_type_e type);
/**
 * This function is used to start a timer.
 * @param qtimer                pointer to qcom_timer_t returned from qcom_timer_init.
 * @return                      0 on success. else on error.
 */
int qcom_timer_start(qcom_timer_t* qtimer);
/**
 * This function is used to stop a timer.
 * @param qtimer                pointer to qcom_timer_t returned from qcom_timer_init.
 * @return                      0 on success. else on error.
 */
int qcom_timer_stop(qcom_timer_t* qtimer);
/**
 * This function is used to delete a timer.
 * @param qtimer                pointer to qcom_timer_t returned from qcom_timer_init.
 * @return                      0 on success. else on error.
 */
int qcom_timer_delete(qcom_timer_t* qtimer);
#endif


#ifdef LIBCUST_INDIRECT_ACCESS
typedef struct 
{
int (* _qcom_timer_init)(qcom_timer_t * qtimer, void (*fn) (unsigned int, void *), void *arg,
                    int timeout, qcom_timer_type_e type);
int (* _qcom_timer_start)(qcom_timer_t * qtimer);
int (* _qcom_timer_stop)(qcom_timer_t * qtimer);
int (* _qcom_timer_delete)(qcom_timer_t * qtimer);
void (* _sleep_timeout)(unsigned int nouse, void *data);
void (* _qcom_thread_msleep)(unsigned long ms);


}QCOM_TIMER_API_INDIRECTION_TABLE;


extern QCOM_TIMER_API_INDIRECTION_TABLE qcomTimerApiIndirectionTable;
#define QCOM_TIMER_API_FN(fn) qcomTimerApiIndirectionTable.fn

#else



int _qcom_timer_init(qcom_timer_t * qtimer, void (*fn) (unsigned int, void *), void *arg,
                    int timeout, qcom_timer_type_e type);
int _qcom_timer_start(qcom_timer_t * qtimer);
int _qcom_timer_stop(qcom_timer_t * qtimer);
int _qcom_timer_delete(qcom_timer_t * qtimer);
void _sleep_timeout(unsigned int nouse, void *data);
void _qcom_thread_msleep(unsigned long ms);

#define QCOM_TIMER_API_FN(fn) fn

#endif


#define qcom_timer_init(timer, fn, fn_arg, timeout, type) \
	QCOM_TIMER_API_FN(_qcom_timer_init((timer), (fn), (fn_arg), (timeout), (type)))
#define qcom_timer_start(timer) \
	QCOM_TIMER_API_FN(_qcom_timer_start(timer))
#define qcom_timer_stop(timer) \
	QCOM_TIMER_API_FN(_qcom_timer_stop(timer))
#define qcom_timer_delete(timer) \
	QCOM_TIMER_API_FN(_qcom_timer_delete(timer))
#define sleep_timeout(nouse, data) \
	QCOM_TIMER_API_FN(_sleep_timeout((nouse), (data)))

#if 0
#define qcom_thread_msleep(time) \
	QCOM_TIMER_API_FN(_qcom_thread_msleep(time))
#else
/*
  * Now 1 tick of threadx is 1ms.
  */
extern unsigned int  _tx_thread_sleep(unsigned long timer_ticks);
#define qcom_thread_msleep(time) \
	_tx_thread_sleep(time)
#endif




#ifdef __cplusplus
}
#endif

#endif
