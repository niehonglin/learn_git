/*
 * Copyright (c) 2013 Qualcomm Atheros, Inc.
 * All Rights Reserved.
 * Qualcomm Atheros Confidential and Proprietary.
 * $ATH_LICENSE_HOSTSDK0_C$
 */

#ifndef __QCOM_PWM_H__
#define __QCOM_PWM_H__
#if 0
/**
 *This function is to enable/disable PWM module
 *flag 1: enable and start PWM module with the default configuration.
 *The default configuration of PWM module is that:
 *      the clock is (1/256)*Crystal,
 *      the duty cycle is 0.
 *
 *flag 0: stop PWM module.
 *
 */
void qcom_pwm_control(A_UINT32 flag);

/**
 * This function is to set the clock frequence of PWM module. Clock source for PWM
 * module is Crystal.
 * The val should be a value from 0 to 255.
 * The clock frequence of PWM module is: Crystal/(val+1).
 */
A_STATUS qcom_pwm_set_clock(A_UINT32 val);

/**
 * This function is to set the duty cycle of PWM module.
 * The val should be a value from 0 to 255.
 */
A_STATUS qcom_pwm_set_duty_cycle(A_UINT32 val);


/**
 * This function is to configure the frequency and duty cycle of PWM output.
 * the divider is calcualted as divider = CORE_CLOCK * duty / (256 * frequency)
 */
A_STATUS qcom_pwm_set(A_INT32 freq, A_INT32 duty);


/**
 * This function is to create a SW PWM 
 * pin: 	the output GPIO pin number of this SW PWM
 * freq: 	the freqency of this SW PWM
 * duty:	the duty cycle of this SW PWM
 * return value: the created SW PWM id or -1 of error
 */
A_INT32 qcom_pwm_sw_start(void);
/**
 * This function is to delete a SW PWM
 */
A_STATUS qcom_pwm_sw_stop(void);

/**
 * This function is to configure a existing SW PWM 
 * id: 	the SW PWM id to be configured
 * pin: 	the output GPIO pin number of this SW PWM
 * freq: 	the freqency of this SW PWM
 * duty:	the duty cycle of this SW PWM
 */
A_STATUS qcom_pwm_sw_config(A_INT32 id, A_UINT32 freq, A_UINT32 duty);
#endif

#define PWM_SW_DEBUG 0

typedef struct PWM_SW_DB_s
{
  A_UINT32 pwm_sw_id;
  A_UINT32 pin;
  A_UINT32 pwm_sw_freq;
  A_UINT32 pwm_sw_duty;
  A_UINT32 cur_pull_type;
  A_UINT32 switch_time;
  A_UINT32 high_time;
  A_UINT32 low_time;
  A_UINT32 last_time;
#if 1 //in libromcust/qcom_pwm.c, these 2 fields are used.
//#ifdef PWM_SW_DEBUG
  A_UINT32 interval;
  A_UINT32 interval2;
#endif  
  struct PWM_SW_DB_s *next;
}PWM_SW_DB_t;



#ifdef LIBCUST_INDIRECT_ACCESS

typedef struct 
{
	 A_UINT32
	(* _pwm_now)(void);
	A_UINT32 (* _pwm_ListAdd)(PWM_SW_DB_t *list_node);
	
#ifdef AR6002_REV74
	int (* _qcom_pwm_get_gpio_val_of_pin)(A_UINT32 pin);
	void (* _qcom_pwm_select_pin)(A_UINT32 pin);
	void (* _qcom_pwm_deselect_pin)(A_UINT32 pin);
#endif
	
	A_STATUS (* _qcom_pwm_set_clock)(A_UINT32 val);
	A_STATUS (* _qcom_pwm_set_duty_cycle)(A_INT32 val);
	void  (* _qcom_pwm_start)(void);
	void  (* _qcom_pwm_stop)(void);
	void (* _qcom_pwm_control)(A_UINT32 flag);
	
	
	void (* _qcom_pwm_dump)(void);
	A_STATUS (* _qcom_pwm_set)(A_UINT32 freq, A_UINT32 duty);
	void (* _qcom_pwm_sw_timer)(A_UINT32 alarm, void *data);
	void (* _qcom_pwm_sw_init)();
	void (* _qcom_pwm_sw_start)(void);
	
	void (* _qcom_pwm_sw_stop)(void);
	A_STATUS (* _qcom_pwm_sw_config)(A_INT32 id, A_UINT32 freq, A_UINT32 duty);

}QCOM_PWM_API_INDIRECTION_TABLE;
extern QCOM_PWM_API_INDIRECTION_TABLE qcomPwmApiIndirectionTable;
#define QCOM_PWM_API_FN(fn) qcomPwmApiIndirectionTable.fn

#else

#if 0
LOCAL A_UINT32
_pwm_now(void);
#endif

A_UINT32 _pwm_ListAdd(PWM_SW_DB_t *list_node);
#ifdef AR6002_REV74
int _qcom_pwm_get_gpio_val_of_pin(A_UINT32 pin);
void _qcom_pwm_select_pin(A_UINT32 pin);
void _qcom_pwm_deselect_pin(A_UINT32 pin);
#endif

A_STATUS _qcom_pwm_set_clock(A_UINT32 val);
A_STATUS _qcom_pwm_set_duty_cycle(A_INT32 val);
void  _qcom_pwm_start(void);
void  _qcom_pwm_stop(void);
void _qcom_pwm_control(A_UINT32 flag);


void _qcom_pwm_dump(void);
A_STATUS _qcom_pwm_set(A_UINT32 freq, A_UINT32 duty);
void _qcom_pwm_sw_timer(A_UINT32 alarm, void *data);
void _qcom_pwm_sw_init();


void _qcom_pwm_sw_start(void);

void _qcom_pwm_sw_stop(void);
A_STATUS _qcom_pwm_sw_config(A_INT32 id, A_UINT32 freq, A_UINT32 duty);



#define QCOM_PWM_API_FN(fn) fn

#endif

#define pwm_now() \
	QCOM_PWM_API_FN(_pwm_now())
#define pwm_ListAdd(listNode) \
	QCOM_PWM_API_FN(_pwm_ListAdd(listNode))
#ifdef AR6002_REV74
#define qcom_pwm_get_gpio_val_of_pin(pin) \
	QCOM_PWM_API_FN(_qcom_pwm_get_gpio_val_of_pin(pin))
#define qcom_pwm_select_pin(pin) \
	QCOM_PWM_API_FN(_qcom_pwm_select_pin(pin))
#define qcom_pwm_deselect_pin(pin) \
	QCOM_PWM_API_FN(_qcom_pwm_deselect_pin(pin))
#endif
#define qcom_pwm_set_clock(val) \
	QCOM_PWM_API_FN(_qcom_pwm_set_clock(val))
#define qcom_pwm_set_duty_cycle(pin) \
	QCOM_PWM_API_FN(_qcom_pwm_set_duty_cycle(pin))
#define qcom_pwm_start() \
	QCOM_PWM_API_FN(_qcom_pwm_start())
#define qcom_pwm_stop() \
	QCOM_PWM_API_FN(_qcom_pwm_stop())
#define qcom_pwm_control(flag) \
	QCOM_PWM_API_FN(_qcom_pwm_control(flag))
#define qcom_pwm_dump() \
	QCOM_PWM_API_FN(_qcom_pwm_dump())
#define qcom_pwm_set(freq, duty) \
	QCOM_PWM_API_FN(_qcom_pwm_set((freq), (duty)))
#define qcom_pwm_sw_timer(alarm, data) \
	QCOM_PWM_API_FN(_qcom_pwm_sw_timer((alarm),(data)))
#define qcom_pwm_sw_init() \
	QCOM_PWM_API_FN(_qcom_pwm_sw_init())
#define qcom_pwm_sw_start() \
	QCOM_PWM_API_FN(_qcom_pwm_sw_start())
#define qcom_pwm_sw_stop() \
	QCOM_PWM_API_FN(_qcom_pwm_sw_stop())
#define qcom_pwm_sw_config(id, freq, duty) \
	QCOM_PWM_API_FN(_qcom_pwm_sw_config((id), (freq), (duty)))


#endif

