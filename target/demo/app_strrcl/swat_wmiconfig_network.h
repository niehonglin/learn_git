/*
  * Copyright (c) 2015 Qualcomm Atheros, Inc..
  * All Rights Reserved.
  * Qualcomm Atheros Confidential and Proprietary.
  */

#ifndef _SWAT_WMICONFIG_NETWORK_H_
#define _SWAT_WMICONFIG_NETWORK_H_

#define CERT_HEADER_LEN sizeof(CERT_HEADER_T)

#define SSL_INBUF_SIZE               6000
#define SSL_OUTBUF_SIZE              3500
typedef struct
{
    A_CHAR id[4];
    A_UINT32 length;
    A_UINT8 data[0];
} CERT_HEADER_T;

void swat_wmiconfig_set_ping_id(A_UINT8 device_id,A_UINT32 PingId);

void swat_wmiconfig_ipstatic(A_UINT8 device_id, A_CHAR* pIpAddr, A_CHAR* pNetMask, A_CHAR* pGateway);

void swat_wmiconfig_ipdhcp(A_UINT8 device_id);

void swat_wmiconfig_ipconfig(A_UINT8 device_id);

void swat_wmiconfig_dns(A_UINT8 device_id, char* name);

void swat_wmiconfig_dns2(A_UINT8 device_id, char* name, int af, A_UINT32 mode);

#ifdef BRIDGE_ENABLED
void swat_wmiconfig_bridge_mode_enable(A_UINT8 device_id);
#endif

void swat_wmiconfig_dns_enable(A_UINT8 device_id, char* enable);

void swat_wmiconfig_dns_svr_add(A_UINT8 device_id, char* ipaddr);

void swat_wmiconfig_dns_svr_del(A_UINT8 device_id, char* ipaddr);
void swat_wmiconfig_dnss_enable(A_UINT8 device_id, char* enable);
void swat_wmiconfig_dns_domain(A_UINT8 device_id, char* local_name);
void swat_wmiconfig_dns_entry_add(A_UINT8 device_id, char* hostname, char* addr);
void swat_wmiconfig_dns_entry_del(A_UINT8 device_id, char* hostname, char* addr);

void swat_wmiconfig_dhcp_pool(A_UINT8 device_id, A_CHAR *pStartaddr, A_CHAR *pEndaddr, A_UINT32 leasetime);
void swat_wmiconfig_set_hostname(A_UINT8 device_id, A_CHAR *host_name);

void swat_wmiconfig_http_server(A_UINT8 device_id, int enable);

void swat_wmiconfig_sntp_enable(A_UINT8 device_id, char* enable);

void swat_wmiconfig_sntp_srv(A_UINT8 device_id, char* add_del,char* srv_addr);

void swat_wmiconfig_sntp_zone(A_UINT8 device_id, char* utc,char* dse_en_dis);

void swat_wmiconfig_sntp_get_time(A_UINT8 device_id);

void swat_wmiconfig_sntp_show_config(A_UINT8 device_id);

void swat_wmiconfig_sntp_get_time_of_day(A_UINT8 device_id);

A_CHAR * _inet_ntoa(A_UINT32 ip);
A_UINT32 _inet_addr(A_CHAR *str);

A_INT32 ssl_get_cert_handler(A_INT32 argc, char* argv[]);
A_INT32 ssl_start(A_INT32 argc, char *argv[]);
A_INT32 ssl_stop(A_INT32 argc, char *argv[]);
A_INT32 ssl_config(A_INT32 argc, char *argv[]);
A_INT32 ssl_add_cert(A_INT32 argc,char *argv[]);
A_INT32 ssl_store_cert(A_INT32 argc,char *argv[]);
A_INT32 ssl_delete_cert(A_INT32 argc,char *argv[]);
A_INT32 ssl_list_cert(A_INT32 argc,char *argv[]);

A_INT32 https_client_handler(A_INT32 argc,char *argv[]);
A_INT32 https_server_handler(A_INT32 argc,char *argv[]);
A_INT32 swat_ipv4_route(A_UINT8 device_id,A_INT32 argc, char* argv[]);
A_INT32 swat_ipv6_route(A_UINT8 device_id, A_INT32 argc, char* argv[]);
A_INT32 swat_ota_upgrade(A_UINT8 device_id,A_INT32 argc,char *argv[]);
A_INT32 swat_ota_read(A_INT32 argc,char *argv[]);
A_INT32 swat_ota_done();
#endif
