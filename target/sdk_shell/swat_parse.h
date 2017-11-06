#ifndef _SWAT_PARSE_H_
#define _SWAT_PARSE_H_

//#include "swat_interface.h"
#include "qcom_internal.h"
/*----------------------- Module Macro Define ------------------------*/
#define SWAT 1
#define SWAT_WMICONFIG 1
#define SWAT_WMICONFIG_SOFTAP 1
#define SWAT_WMICONFIG_WEP 1
#define SWAT_WMICONFIG_WPA 1

#ifdef AR6002_REV74

/*Followign are enabled for REV74 and FPGA*/
//#define OTA_ENABLE 1
//#define ENABLE_HTTP_SERVER 1 
//#define ENABLE_HTTP_CLIENT 1 
//#define SWAT_WMICONFIG_SNTP   1
#define SWAT_BENCH 1
#define KF_ROM_1_1_2_INICHE 1
#define SWAT_WAKEUP_MGR 1
#define SWAT_SLEEP 1

//#define SWAT_WMICONFIG_P2P 1
//#define SWAT_WMICONFIG_WPS 1

//#define SWAT_SSL 
//#define ENABLE_HTTPS_SERVER 0 
//#define ENABLE_HTTPS_CLIENT 0

#else   //AR6002_REV74

#if defined (REV74_TEST_ENV1) 

#define SWAT_BENCH 1
#define KF_ROM_1_1_2_INICHE 1
#define SWAT_BENCH_RAW 1 
//#define SWAT_I2S_TEST
#define SWAT_WAKEUP_MGR 1
#define SWAT_SLEEP 1

#endif


#if defined (REV74_TEST_ENV2) 

#define SWAT_WMICONFIG_P2P 1
#define SWAT_WMICONFIG_WPS 1
#undef SWAT_WMICONFIG_WEP
#undef SWAT_BENCH 
#undef KF_ROM_1_1_2_INICHE
#undef SWAT_BENCH_RAW
#define SWAT_BENCH_RAW 0
#undef SWAT_I2S_TEST
#undef SWAT_WAKEUP_MGR
#undef SWAT_SLEEP
#endif

#if defined (REV74_TEST_ENV5)
/*SNTP,IPv6,Strrcl in TE5*/
#define SWAT_WMICONFIG_SNTP  1 

#endif//REV74_TEST_ENV5

#if defined (REV74_TEST_ENV3)

/*SNTP, HTTP, OTA together are only enabled in TE3*/
#define SWAT_WMICONFIG_SNTP  1 
#define ENABLE_HTTP_SERVER 1 
#define ENABLE_HTTP_CLIENT 1 
#define OTA_ENABLE 1
#define SWAT_WAKEUP_MGR 1
#define SWAT_SLEEP 1
#undef SWAT_BENCH_RAW
#define SWAT_BENCH_RAW 0
#undef SWAT_I2S_TEST
#define SWAT_BENCH 1
#define KF_ROM_1_1_2_INICHE 1

#endif //REV74_TEST_ENV3
#if defined (REV74_TEST_ENV4)
#undef SWAT_BENCH_RAW
#define SWAT_BENCH_RAW 0
//#undef SWAT_WMICONFIG_SOFTAP 
#undef SWAT_WMICONFIG_WEP 
//#undef SWAT_WMICONFIG_WPA 
#undef SWAT_I2S_TEST
#undef SWAT_WAKEUP_MGR 
#undef SWAT_SLEEP 
//#define SWAT_SSL
#endif
#if defined (REV74_TEST_ENV6) 

#define SWAT_BENCH 1
#define KF_ROM_1_1_2_INICHE 1
#define SWAT_BENCH_RAW 1
#define SWAT_I2S_TEST
#undef SWAT_SLEEP
#undef SWAT_WAKEUP_MGR
#undef SWAT_WMICONFIG_SOFTAP
#undef SWAT_WMICONFIG_WEP
#undef SWAT_WMICONFIG_WPA


#endif
#endif //AR6002_REV74



/*Network Part */
#define SWAT_WMICONFIG_IP 1
/*Misc or Other Part */
#define SWAT_WMICONFIG_MISC 1

#define SWAT_PING 1
#define IP_RAW         1
#define ETH_RAW        2 
#define ATH_ETH_P_PAE  0x888E

#define SWAT_OK 1
#define SWAT_ERROR 2
#define SWAT_NOFOUND 0
/* Timer */
#define swat_time_get_ms    time_ms
#define swat_time_init      qcom_timer_init
#define swat_time_start     qcom_timer_start
#define swat_time_stop      qcom_timer_stop
#define swat_time_delete    qcom_timer_delete

#define WAKE_MGR_WAKE_EVENT_TIMER       0x00000001
#define WAKE_MGR_WAKE_EVENT_GPIO30_HIGH 0x00000002
#define WAKE_MGR_WAKE_EVENT_GPIO30_LOW  0x00000004
#define WAKE_MGR_WAKE_EVENT_GPIO31_HIGH 0x00000008
#define WAKE_MGR_WAKE_EVENT_GPIO31_LOW  0x00000010

/* Memory */
#define swat_mem_malloc     qcom_mem_alloc
#define swat_mem_cpy        A_MEMCPY
#define swat_mem_set        A_MEMSET
#define swat_mem_free       qcom_mem_free
/* String */
#define swat_atoi           atoi
#define swat_sscanf         A_SSCANF
#define swat_strcmp         A_STRCMP
#define swat_strcpy         A_STRCPY
/* Task */
#define swat_task_create    qcom_task_start
#define swat_task_delete    qcom_task_exit
/* TCP/IP */
#define swat_ping_request qcom_ping_request

/* Socket */
#define swat_accept         qcom_accept
#define swat_bind           qcom_bind
#define swat_close      	qcom_socket_close
#define swat_connect        qcom_connect
#define swat_setsockopt     qcom_setsockopt
#define swat_getsockopt     qcom_getsockopt
#define swat_listen         qcom_listen
#define swat_recv           qcom_recv
#define swat_recvfrom       qcom_recvfrom
#define swat_send           qcom_send
#define swat_sendto         qcom_sendto
#define swat_socket         qcom_socket
#define swat_select         qcom_select
#define swat_setsockopt     qcom_setsockopt
#define swat_getsockopt     qcom_getsockopt
#define swat_fd_zero        FD_ZERO
#define swat_fd_set         FD_SET
#define swat_fd_isset       FD_ISSET


// to fix later, xiny
//#define A_ATOUL atoi
#define A_ATOUL

#if defined(SWAT_BENCH)
extern void swat_bench_quit_config();
extern void swat_bench_rx_test(A_UINT32 protocol, A_UINT32 port, A_UINT32 localIpAddress, A_UINT32 mcastIpAddress,IP6_ADDR_T* lIp6, IP6_ADDR_T* mIp6,
                               A_UINT32 ip_hdr_inc, A_UINT32 rawmode);
extern void swat_bench_tx_test(A_UINT32 ipAddress,
                               IP6_ADDR_T ip6Address,  
                                    A_UINT32 port,
                                    A_UINT32 protocol,
                                    A_UINT32 pktSize,
                                    A_UINT32 mode,
                                    A_UINT32 seconds,
                                    A_UINT32 numOfPkts,
                                    A_UINT32 local_ipAddress,
                                    A_UINT32 ip_hdr_inc,
                                    A_UINT32 delay);


extern A_INT32 swat_benchtx_handle(A_INT32 argc, A_CHAR *argv[]);
extern A_INT32 swat_benchrx_handle(A_INT32 argc, A_CHAR *argv[]);
extern A_INT32 swat_benchquit_handle(A_INT32 argc, A_CHAR *argv[]);
extern A_INT32 swat_benchdbg_handle(A_INT32 argc, A_CHAR *argv[]);
extern void swat_bench_dbg();

#endif

#if defined(SWAT_WMICONFIG)

extern A_INT32 swat_wmiconfig_handle(A_INT32 argc, A_CHAR *argv[]);
extern A_INT32 swat_wmiconfig_connect_handle(A_INT32 argc, A_CHAR *argv[]);

#if defined(SWAT_WMICONFIG_WEP)
extern A_INT32 swat_wmiconfig_wep_handle(A_INT32 argc, A_CHAR *argv[]);
#endif
#if defined(SWAT_WMICONFIG_WPA)
extern A_INT32 swat_wmiconfig_wpa_handle(A_INT32 argc, A_CHAR *argv[]);
#endif
#if defined(SWAT_WMICONFIG_WPS)
extern A_INT32 swat_wmiconfig_wps_handle(A_INT32 argc, A_CHAR *argv[]);
#endif
#if defined(SWAT_WMICONFIG_P2P)
extern A_INT32 swat_wmiconfig_p2p_handle(A_INT32 argc, A_CHAR *argv[]);
#endif
#if defined(SWAT_WMICONFIG_MISC)
extern A_INT32 swat_wmiconfig_misc_handle(A_INT32 argc, A_CHAR *argv[]);
#endif
#if defined(SWAT_WMICONFIG_SOFTAP)
extern A_INT32 swat_wmiconfig_softAp_handle(A_INT32 argc, A_CHAR *argv[]);
#endif
#if defined(SWAT_WMICONFIG_IP)
extern A_INT32 swat_wmiconfig_ip_handle(A_INT32 argc, A_CHAR *argv[]);
#endif
extern A_INT32 swat_iwconfig_scan_handle(A_INT32 argc, A_CHAR *argv[]);
#endif

#if defined(SWAT_PING)
extern A_INT32 swat_ping_handle(A_INT32 argc, A_CHAR *argv[], A_CHAR v6);
#endif
extern A_INT32 swat_wmiconfig_misc_ext_handle(A_INT32 argc, A_CHAR *argv[]);

typedef struct httpc_params
{
    A_UINT32          command;
    A_UINT8           url[256];
    A_UINT8           data[128];
}HTTPC_PARAMS;

extern A_INT32
swat_http_server(A_INT32 argc, A_CHAR * argv[]);
extern A_INT32
swat_http_server_get(A_INT32 argc, A_CHAR * argv[]);
extern A_INT32
swat_http_server_post(A_INT32 argc, A_CHAR * argv[]);

extern A_INT32 httpc_command_parser(A_INT32 argc, A_CHAR* argv[]);

extern void httpc_method(HTTPC_PARAMS *p_httpc);
extern A_INT32 swat_ip6_set_router_prefix(A_INT32 argc,char *argv[]);
//extern int app_printf(const char * format, ...);

extern A_UINT8 currentDeviceId;

typedef struct _console_cmd {
    struct _console_cmd *next;
    A_UCHAR    *name;                  /* command name */
    A_UCHAR    *description;           /* optional description */
    A_INT32       (*execute)(A_INT32 argc, A_CHAR *argv[]);  /* execute callback */
    void      *context;               /* context to pass to callback */
} console_cmd_t;

void
swat_time();

#define HTONS htons
#define HTONL htonl
#define NTOHL ntohl
#define NTOHS ntohs

#define ORG_PTR(fmt,args...) do{if(1) {qcom_printf(fmt,##args);}}while(0)
#define SWAT_PTF(fmt,args...) do{if(1) {swat_time();qcom_printf(fmt,##args);}}while(0)
#define SWAT_PTF_NO_TIME(fmt,args...) do{if(1) {qcom_printf(fmt,##args);}}while(0)

#define IPV4_PTF(addr) SWAT_PTF("%d.%d.%d.%d", (addr) >> 24 & 0xFF, (addr) >> 16 & 0xFF, (addr) >> 8 & 0xFF, (addr) & 0xFF)
#define IPV4_ORG_PTF(addr) ORG_PTR("%d.%d.%d.%d", (addr) >> 24 & 0xFF, (addr) >> 16 & 0xFF, (addr) >> 8 & 0xFF, (addr) & 0xFF)

/*Stack type definination*/
 
#define CALC_STREAM_NUMBER 4
#define CALC_STREAM_NUMBER_INVALID 0xFF


#endif

