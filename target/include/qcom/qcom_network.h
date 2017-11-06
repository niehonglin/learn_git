/*
  * Copyright (c) 2013 Qualcomm Atheros, Inc..
  * All Rights Reserved.
  * Qualcomm Atheros Confidential and Proprietary.
  */

#ifndef __QCOM_NETWORK__
#define __QCOM_NETWORK__

#include <qcom/qcom_ota_api.h>

/* HTTP Server socket command MACROS*/
#define HTTP_UNKNOWN		-1
#define HTTP_SERVER_STOP    0
#define HTTP_SERVER_START   1
#define HTTPS_SERVER_STOP   2
#define HTTPS_SERVER_START  3

/* HTTP Server Get/Post socket MACROS */
#define HTTP_GET_METHOD     0
#define HTTP_POST_METHOD    1

#define ATH_MAX_ROUTES 3
#define IPV6_MAX_ROUTES 3

#define GET_TLV_TYPE(x,y)    (A_MEMCPY(&(y),(x),sizeof(A_UINT16)))
#define GET_TLV_LENGTH(x,y)  (A_MEMCPY(&(y),(x+2),sizeof(A_UINT16)))
#define GET_NEXT_TLV(x,y)    ((x)+ sizeof(A_UINT16) + sizeof(A_UINT16) + (y))
#define GET_TLV_VAL(x)       ((x)+ sizeof(A_UINT16) + sizeof(A_UINT16))

enum {
	/* Host command macros */
	HTTPC_CONNECT_CMD,
	HTTPC_GET_CMD,
	HTTPC_POST_CMD,
	HTTPC_DATA_CMD,
	HTTPC_DISCONNECT_CMD,
	HTTPC_CONNECT_SSL_CMD
};

typedef struct IPv4Route {
A_UINT32 reserved;
A_UINT32 address;
A_UINT32 mask;
A_UINT32 gateway;
A_UINT32 ifIndex;
A_UINT32 prot;
} IPV4_ROUTE_T;

typedef struct IPv4RouteLists {
    A_UINT32 rtcount;
    IPV4_ROUTE_T route[ATH_MAX_ROUTES];
}IPV4_ROUTE_LIST_T;

typedef PREPACK struct {
    A_UINT32    resp_type ;
    A_UINT32    sock_handle ; //handle to open socket
    A_UINT32    result ;
    A_UINT32    rt_count;
    IPV4_ROUTE_T  route[ATH_MAX_ROUTES] ;
} POSTPACK WMI_IP4ROUTE_RESPONSE_EVENT;

enum{
    IPV6_ROUTE_ADD,
    IPV6_ROUTE_DEL,
    IPV6_ROUTE_SHOW,
    IPV6_ROUTEPREFIX_SHOW
};

typedef enum route {
    ROUTE_ADD,
    ROUTE_DEL,
    ROUTE_SHOW,
    ROUTE_MAX
} ip_route;

typedef  struct sock_ipv6_route {
    A_UINT32 command ;
    A_UINT8  address[16] ;
    A_INT32  prefixlen ;
    A_UINT8  nexthop[16] ;
    A_UINT32 ifindex ;
    A_UINT32 prot ;
} IPV6_ROUTE_T;

typedef struct IPv6RouteLists {
    A_UINT32 rtcount;
    IPV6_ROUTE_T route[ATH_MAX_ROUTES];
}IPV6_ROUTE_LIST_T;

typedef  struct {
    A_UINT32    resp_type ;
    A_UINT32    sock_handle ; //handle to open socket
    A_UINT32    result ;
    A_UINT32    rt_count;
    IPV6_ROUTE_T  route[IPV6_MAX_ROUTES] ;
}  WMI_IP6ROUTE_RESPONSE_EVENT;

enum WI_POST_EVENT_TLV_TYPE{
    POST_TYPE_URI,
    POST_TYPE_NAME,
    POST_TYPE_VALUE
};

typedef struct {
    A_UINT32 numTLV;   /*Number of TLVs encapsulated in the event*/
    A_UINT8 data[0];   /*Start of TLV data*/
}HTTP_POST_EVENT_T;


#ifdef LIBCUST_INDIRECT_ACCESS
typedef struct  {

	void
	(* _qcom_ipconfig)(A_UINT8 device_id, int mode, A_UINT32 * ip_address, A_UINT32 * sn_mask, A_UINT32 * gateway);
	struct hostent *
	(* _qcom_dns_get)(A_UINT32 mode, A_UINT32 domain, char *hostname);
	A_STATUS
	(* _qcom_ip_address_set)(A_UINT8 device_id, A_UINT32 address, A_UINT32 submask, A_UINT32 gateway);
	A_STATUS
	(* _qcom_ip_address_get)(A_UINT8 device_id, A_UINT32 *paddress, A_UINT32 *psubmask, A_UINT32 *pgateway);
#if 0 //def IP_V6
	A_STATUS
	(* _qcom_ip6_address_get)(A_UINT8 device_id, IP6_ADDR_T *v6Global, IP6_ADDR_T *v6Link, IP6_ADDR_T *v6DefGw,IP6_ADDR_T *v6GlobalExtd, A_INT32 *LinkPrefix,       A_INT32 *GlbPrefix, A_INT32 *DefgwPrefix, A_INT32 *GlbPrefixExtd);
#else
	A_STATUS
	(* _qcom_ip6_address_get)(A_UINT8 device_id, A_UINT8 *v6Global, A_UINT8 *v6Link, A_UINT8 *v6DefGw,A_UINT8 *v6GlobalExtd, A_INT32 *LinkPrefix,       A_INT32 *GlbPrefix, A_INT32 *DefgwPrefix, A_INT32 *GlbPrefixExtd);
#endif

	A_STATUS
	(* _qcom_dns_server_address_get)(A_UINT32 pdns[], A_UINT32* number);
	A_STATUS
	(* _qcom_ip_ping)(A_UINT32 host, A_UINT32 size, A_UINT32 count, A_UINT32 interval);
#if 1 //def IP_V6
	A_STATUS
	(* _qcom_ip_ping6)(A_UINT8* host, A_UINT32 size, A_UINT32 count, A_UINT32 interval);
	A_STATUS (* _qcom_ip6config_router_prefix)(A_UINT8 device_id, A_UINT8 *addr,A_INT32 prefix_length,A_INT32 prefix_lifetime,A_INT32 valid_lifetime);
#endif
	A_STATUS
	(* _qcom_dhcpc_enable)(A_UINT8 device_id, A_BOOL enable);

	A_STATUS
	(* _qcom_dhcps_enable)(A_UINT8 device_id, A_BOOL enable);
	A_STATUS
	(* _qcom_dhcps_set_pool)(A_UINT8 device_id, A_UINT32 startip, A_UINT32 endip, A_UINT32 leasetime);
	A_STATUS
	(* _qcom_dhcps_get_pool)(A_UINT8 device_id, A_UINT32 *pstartip, A_UINT32 *pendip, A_UINT32 *pleasetime);
//#ifdef BRIDGE_ENABLED
	A_STATUS
	(* _qcom_bridge_mode_enable)(A_UINT16 bridgemode);
//#endif
	A_STATUS
	(* _qcom_dnsc_enable)(A_BOOL enable);

	A_STATUS
	(* _qcom_dnsc_add_server_address)(A_UINT8 *ipaddress, A_UINT8 type);
	A_STATUS
	(* _qcom_dnsc_del_server_address)(A_UINT8 *ipaddress, A_UINT8 type);
	A_STATUS
	(* _qcom_dnsc_get_host_by_name)(A_CHAR *pname, A_UINT32 *pipaddress);
	A_STATUS
	(* _qcom_dnss_enable)(A_BOOL enable);
	A_STATUS
	(* _qcom_dns_local_domain)(A_CHAR* local_domain);

	A_STATUS
	(* _qcom_dns_entry_create)(A_CHAR* hostname, A_UINT32 ipaddress);
	A_STATUS
	(* _qcom_dns_entry_delete)(A_CHAR* hostname, A_UINT32 ipaddress);
	A_STATUS
	(* _qcom_dns_6entry_create)(A_CHAR* hostname, A_UINT8* ip6addr);
	A_STATUS
	(* _qcom_dns_6entry_delete)(A_CHAR* hostname, A_UINT8* ip6addr);
	A_STATUS
	(* _qcom_dnsc_get_host_by_name2)(A_CHAR *pname, A_UINT32 *pipaddress, A_INT32 domain, A_UINT32 mode);

//#if defined HTTP_ENABLED
	A_STATUS
	(* _qcom_http_server)(A_INT32 enable);
	A_STATUS
	(* _qcom_http_server_method)(A_INT32 cmd,A_UINT8 *pagename, A_UINT8 *objname, A_INT32 objtype, A_INT32 objlen, A_UINT8 *value);
	A_STATUS
	(* _qcom_http_client_method)(A_UINT32 cmd, A_UINT8 *url, A_UINT8 *data, A_UINT8 *value);
//#endif
	void
	(* _qcom_wmi_event_socket_process)(void *pevent_buff);

    A_STATUS (*_qcom_ping)(A_UINT32 host, A_UINT32 size);
    A_STATUS (*_qcom_ping6)(A_UINT8* host, A_UINT32 size);
    A_STATUS (*_qcom_set_ping_id)(A_UINT32 PingId);
    A_STATUS (*_qcom_dhcps_release_pool)(A_UINT8 device_id);
    A_STATUS (*_qcom_tcp_set_exp_backoff)(A_UINT8 device_id, A_INT32 retry);
    A_STATUS (*_qcom_ip4_route)(A_UINT8 device_id, A_UINT32 cmd, A_UINT32 *addr, A_UINT32 *subnet, A_UINT32* gw, A_UINT32* ifindex,
        IPV4_ROUTE_LIST_T* rtlist);
    A_STATUS (*_qcom_ip6_route)(A_UINT8 device_id, A_UINT32 cmd, A_UINT8 *ip6addr, A_UINT32 *prefixLen, A_UINT8* gw, A_UINT32* ifindex,IPV6_ROUTE_LIST_T* rtlist);
    A_STATUS (*_qcom_ota_upgrade)(A_UINT8 device_id,A_UINT32 ip_addr, const A_CHAR *source,A_UINT8 mode,A_UINT8 preserve_last,A_UINT8 protocol,A_UINT32 *resp_code,A_UINT32 *length);
    A_STATUS (*_qcom_read_ota_area)(A_UINT32 offset,A_UINT32 size,A_UINT8 *buffer,A_UINT32 *ret_len);
    A_STATUS (*_qcom_ota_done)(A_UINT32 good_image);
    A_STATUS (*_qcom_set_hostname)(A_UINT8 device_id,A_CHAR *pstr);
	A_STATUS (*_qcom_tcp_conn_timeout)(A_UINT32 timeout_val);
    A_STATUS (*_qcom_http_set_post_cb)(void* cxt, void *callback);
    A_STATUS (* _qcom_dns_set_timeout)(int timeout);
    QCOM_OTA_STATUS_CODE_t (*_qcom_ota_session_start)(A_UINT32 flags, A_UINT32 partition_index);	
    A_UINT32 (*_qcom_ota_partition_get_size)(void);
    QCOM_OTA_STATUS_CODE_t (*_qcom_ota_partition_erase)(void);
    QCOM_OTA_STATUS_CODE_t (*_qcom_ota_parse_image_hdr)(A_UINT8 *header,A_UINT32 *offset);
    QCOM_OTA_STATUS_CODE_t (*_qcom_ota_partition_verify_checksum)(void);
    A_INT32 (*_qcom_ota_partition_read_data)(A_UINT32 offset, A_UINT8 *buf,A_UINT32 size);
    QCOM_OTA_STATUS_CODE_t (*_qcom_ota_partition_write_data)(A_UINT32 offset, A_UINT8 *buf,A_UINT32 size);
    QCOM_OTA_STATUS_CODE_t (*_qcom_ota_session_end)(A_UINT32 good_image);
    QCOM_OTA_STATUS_CODE_t (*_qcom_ota_partition_format)(A_UINT32 partition_index);
    A_STATUS (*_qcom_dhcps_register_cb)(A_UINT8 device_id,void *callback);
    A_STATUS (*_qcom_dhcpc_register_cb)(A_UINT8 device_id,void *callback);
    A_STATUS (*_qcom_set_ipv6_status)(A_UINT8 device_id,A_UINT8 flag);
		A_STATUS (*_qcom_dhcpc_set_timeout)(A_UINT8 device_id,A_UINT32 timeout);



}QCOM_NETWORK_API_INDIRECTION_TABLE;

extern QCOM_NETWORK_API_INDIRECTION_TABLE qcomNetworkApiIndirectionTable;
#define QCOM_NETWORK_API_FN(fn) qcomNetworkApiIndirectionTable.fn

#else

void
_qcom_ipconfig(A_UINT8 device_id, int mode, A_UINT32 * ip_address, A_UINT32 * sn_mask, A_UINT32 * gateway);
struct hostent *
_qcom_dns_get(A_UINT32 mode, A_UINT32 domain, char *hostname);
A_STATUS
_qcom_ip_address_set(A_UINT8 device_id, A_UINT32 address, A_UINT32 submask, A_UINT32 gateway);
A_STATUS
_qcom_ip_address_get(A_UINT8 device_id, A_UINT32 *paddress, A_UINT32 *psubmask, A_UINT32 *pgateway);
#if 1//def IP_V6
#if 0
A_STATUS
_qcom_ip6_address_get(A_UINT8 device_id, IP6_ADDR_T *v6Global, IP6_ADDR_T *v6Link, IP6_ADDR_T *v6DefGw,IP6_ADDR_T *v6GlobalExtd, A_INT32 *LinkPrefix,
		      A_INT32 *GlbPrefix, A_INT32 *DefgwPrefix, A_INT32 *GlbPrefixExtd);
#else
A_STATUS
_qcom_ip6_address_get(A_UINT8 device_id, A_UINT8 *v6Global, A_UINT8 *v6Link, A_UINT8 *v6DefGw,A_UINT8 *v6GlobalExtd, A_INT32 *LinkPrefix,
		      A_INT32 *GlbPrefix, A_INT32 *DefgwPrefix, A_INT32 *GlbPrefixExtd);

#endif

#endif

A_STATUS
_qcom_dns_server_address_get(A_UINT32 pdns[], A_UINT32* number);
A_STATUS
_qcom_ip_ping(A_UINT32 host, A_UINT32 size, A_UINT32 count, A_UINT32 interval);
#if 1 //def IP_V6
A_STATUS
_qcom_ip_ping6(A_UINT8* host, A_UINT32 size, A_UINT32 count, A_UINT32 interval);
A_STATUS _qcom_ip6config_router_prefix(A_UINT8 device_id, A_UINT8 *addr,A_INT32 prefix_length,A_INT32 prefix_lifetime,A_INT32 valid_lifetime);
#endif
A_STATUS
_qcom_dhcpc_enable(A_UINT8 device_id, A_BOOL enable);

A_STATUS
_qcom_dhcps_enable(A_UINT8 device_id, A_BOOL enable);
A_STATUS
_qcom_dhcps_set_pool(A_UINT8 device_id, A_UINT32 startip, A_UINT32 endip, A_UINT32 leasetime);
A_STATUS
_qcom_dhcps_get_pool(A_UINT8 device_id, A_UINT32 *pstartip, A_UINT32 *pendip, A_UINT32 *pleasetime);
#ifdef BRIDGE_ENABLED
A_STATUS
_qcom_bridge_mode_enable(A_UINT16 bridgemode);
#endif
A_STATUS
_qcom_dnsc_enable(A_BOOL enable);

A_STATUS
_qcom_dnsc_add_server_address(A_UINT8 *ipaddress, A_UINT8 type);
A_STATUS
_qcom_dnsc_del_server_address(A_UINT8 *ipaddress, A_UINT8 type);
A_STATUS
_qcom_dnsc_get_host_by_name(A_CHAR *pname, A_UINT32 *pipaddress);
A_STATUS
_qcom_dnss_enable(A_BOOL enable);
A_STATUS
_qcom_dns_local_domain(A_CHAR* local_domain);

A_STATUS
_qcom_dns_entry_create(A_CHAR* hostname, A_UINT32 ipaddress);
A_STATUS
_qcom_dns_entry_delete(A_CHAR* hostname, A_UINT32 ipaddress);
A_STATUS
_qcom_dns_6entry_create(A_CHAR* hostname, A_UINT8* ip6addr);
A_STATUS
_qcom_dns_6entry_delete(A_CHAR* hostname, A_UINT8* ip6addr);
A_STATUS
_qcom_dnsc_get_host_by_name2(A_CHAR *pname, A_UINT32 *pipaddress, A_INT32 domain, A_UINT32 mode);

#if defined HTTP_ENABLED
A_STATUS
_qcom_http_server(A_INT32 enable);
A_STATUS
_qcom_http_server_method(A_INT32 cmd,A_UINT8 *pagename, A_UINT8 *objname, A_INT32 objtype, A_INT32 objlen, A_UINT8 *value);
A_STATUS
_qcom_http_client_method(A_UINT32 cmd, A_UINT8 *url, A_UINT8 *data, A_UINT8 *value);
#endif
void
_qcom_wmi_event_socket_process(void *pevent_buff);

A_STATUS _qcom_ping(A_UINT32 host, A_UINT32 size);
A_STATUS _qcom_ping6(A_UINT8* host, A_UINT32 size);
A_STATUS _qcom_set_ping_id(A_UINT32 PingId);
A_STATUS _qcom_dhcps_release_pool(A_UINT8 device_id);
A_STATUS _qcom_tcp_set_exp_backoff(A_UINT8 device_id, A_INT32 retry);

A_STATUS _qcom_ip4_route(A_UINT8 device_id, A_UINT32 cmd, A_UINT32 *addr, A_UINT32 *subnet, A_UINT32* gw, A_UINT32* ifindex,
        IPV4_ROUTE_LIST_T* rtlist);
A_STATUS _qcom_ip6_route(A_UINT8 device_id, A_UINT32 cmd, A_UINT8 *ip6addr, A_UINT32 *prefixLen, A_UINT8* gw, A_UINT32* ifindex,
        IPV6_ROUTE_LIST_T* rtlist);

A_STATUS _qcom_ota_upgrade(A_UINT8 device_id,A_UINT32 ,const A_CHAR *source,
		A_UINT8 mode, A_UINT8 preserve_last, A_UINT8 protocol,
		A_UINT32 *resp_code, A_UINT32 *length);
A_STATUS _qcom_read_ota_area(A_UINT32 offset,A_UINT32 size, A_UINT8 *buffer,A_UINT32 *ret_len);
A_STATUS _qcom_ota_done(A_UINT32 good_image);
A_STATUS _qcom_set_hostname(A_UINT8 device_id,A_CHAR* pstr);
A_STATUS _qcom_tcp_conn_timeout(A_UINT32 timeout_val);
A_STATUS _qcom_http_set_post_cb(void* cxt, void *callback);
A_STATUS _qcom_dns_set_timeout(int timeout);
QCOM_OTA_STATUS_CODE_t _qcom_ota_session_start(A_UINT32 flags, A_UINT32 partition_index);
A_UINT32 _qcom_ota_partition_get_size(void);
QCOM_OTA_STATUS_CODE_t _qcom_ota_partition_erase(void);
QCOM_OTA_STATUS_CODE_t _qcom_ota_partition_verify_checksum(void);
QCOM_OTA_STATUS_CODE_t _qcom_ota_parse_image_hdr(A_UINT8 *header,A_UINT32 *offset);
A_INT32 _qcom_ota_partition_read_data(A_UINT32 offset, A_UINT8 *buf,A_UINT32 size);
QCOM_OTA_STATUS_CODE_t _qcom_ota_partition_write_data(A_UINT32 offset, A_UINT8 *buf,A_UINT32 size,A_UINT32 *ret_size);
QCOM_OTA_STATUS_CODE_t  _qcom_ota_session_end(A_UINT32 good_image);
QCOM_OTA_STATUS_CODE_t _qcom_ota_partition_format(A_UINT32 partition_index);
A_STATUS _qcom_dhcps_register_cb(A_UINT8 device_id,void *callback);
A_STATUS _qcom_dhcpc_register_cb(A_UINT8 device_id,void *callback);
A_STATUS _qcom_set_ipv6_status(A_UINT8 device_id,A_UINT8 flag);
 A_STATUS _qcom_dhcpc_set_timeout(A_UINT8 device_id,A_UINT32 timeout);

#define QCOM_NETWORK_API_FN(fn) fn


#endif



#define qcom_ipconfig(id, mode, ip, mask, gate) \
	QCOM_NETWORK_API_FN(_qcom_ipconfig((id), (mode), (ip), (mask), (gate)))

#define qcom_dns_get(mode, domain, name) \
	QCOM_NETWORK_API_FN(_qcom_dns_get((mode), (domain), (name)))

#define qcom_ip_address_set(id, ip, mask, gate) \
	QCOM_NETWORK_API_FN(_qcom_ip_address_set((id), (ip), (mask), (gate)))

#define qcom_ip_address_get(id, ip, mask, gate) \
	QCOM_NETWORK_API_FN(_qcom_ip_address_get((id), (ip), (mask), (gate)))


#if 1 //def IP_V6
#define qcom_ip6_address_get(id, global, link, gate, extd, linkPrefix, glbPrefix, gwPrefix, glbPrefixExtd) \
	QCOM_NETWORK_API_FN(_qcom_ip6_address_get((id), (global), (link), (gate), (extd), (linkPrefix), \
							(glbPrefix), (gwPrefix), (glbPrefixExtd)))
#endif

#define qcom_dns_server_address_get(dns, num) \
	QCOM_NETWORK_API_FN(_qcom_dns_server_address_get((dns), (num)))

/* The API qcom_ip_ping is obsolete , it need to use the new qcom_ping() instead */

#if 1 //def IP_V6

/* The API qcom_ip_ping6 is obsolete , it need to use the new qcom_ping6() instead */

#define qcom_ip6config_router_prefix(device_id, addr, len, prefixTime, validTime) \
	QCOM_NETWORK_API_FN(_qcom_ip6config_router_prefix((device_id), (addr), (len), (prefixTime), (validTime)))

#endif

#define qcom_dhcpc_enable(id, en) \
	QCOM_NETWORK_API_FN(_qcom_dhcpc_enable((id), (en)))

#define qcom_dhcps_enable(id, en) \
	QCOM_NETWORK_API_FN(_qcom_dhcps_enable((id), (en)))

#define qcom_dhcps_set_pool(id, sip, eip, time) \
	QCOM_NETWORK_API_FN(_qcom_dhcps_set_pool((id), (sip), (eip), (time)))

#define qcom_dhcps_get_pool(id, sip, eip, time) \
	QCOM_NETWORK_API_FN(_qcom_dhcps_get_pool((id), (sip), (eip), (time)))

#ifdef BRIDGE_ENABLED
#define qcom_bridge_mode_enable(mode) \
	QCOM_NETWORK_API_FN(_qcom_bridge_mode_enable((mode)))
#endif

#define qcom_dnsc_enable(en) \
	QCOM_NETWORK_API_FN(_qcom_dnsc_enable((en)))

#define qcom_dnsc_add_server_address(ip, type) \
	QCOM_NETWORK_API_FN(_qcom_dnsc_add_server_address((ip), (type)))

#define qcom_dnsc_del_server_address(ip, type) \
	QCOM_NETWORK_API_FN(_qcom_dnsc_del_server_address((ip), (type)))

#define qcom_dnsc_get_host_by_name(name, addr) \
	QCOM_NETWORK_API_FN(_qcom_dnsc_get_host_by_name((name), (addr)))

#define qcom_dnss_enable(en) \
	QCOM_NETWORK_API_FN(_qcom_dnss_enable((en)))

#define qcom_dns_local_domain(domain) \
	QCOM_NETWORK_API_FN(_qcom_dns_local_domain(domain))

#define qcom_dns_entry_create(host, ip) \
	QCOM_NETWORK_API_FN(_qcom_dns_entry_create((host), (ip)))

#define qcom_dns_entry_delete(host, ip) \
	QCOM_NETWORK_API_FN(_qcom_dns_entry_delete((host), (ip)))

#define qcom_dns_6entry_create(host, ip) \
	QCOM_NETWORK_API_FN(_qcom_dns_6entry_create((host), (ip)))

#define qcom_dns_6entry_delete(host, ip) \
	QCOM_NETWORK_API_FN(_qcom_dns_6entry_delete((host), (ip)))

#define qcom_dnsc_get_host_by_name2(name, ip, domain, mode) \
	QCOM_NETWORK_API_FN(_qcom_dnsc_get_host_by_name2((name), (ip), (domain), (mode)))

#define qcom_http_server(en) \
	QCOM_NETWORK_API_FN(_qcom_http_server(en))

#define qcom_http_server_method(cmd, pagename, objname, type, len, val) \
	QCOM_NETWORK_API_FN(_qcom_http_server_method((cmd), (pagename), (objname), (type), (len), (val)))

#define qcom_http_client_method(cmd, url, data, val) \
	QCOM_NETWORK_API_FN(_qcom_http_client_method((cmd), (url), (data), (val)))

#define qcom_wmi_event_socket_process(buf) \
	QCOM_NETWORK_API_FN(_qcom_wmi_event_socket_process(buf))


#ifdef AR6002_REV74

A_STATUS _qcom_ping(A_UINT32 host, A_UINT32 size);
A_STATUS _qcom_ping6(A_UINT8* host, A_UINT32 size);
A_STATUS _qcom_set_ping_id(A_UINT32 PingId);
A_STATUS _qcom_dhcps_release_pool(A_UINT8 device_id);
A_STATUS _qcom_tcp_set_exp_backoff(A_UINT8 device_id, A_INT32 retry);

A_STATUS _qcom_ip4_route(A_UINT8 device_id, A_UINT32 cmd, A_UINT32 *addr, A_UINT32 *subnet, A_UINT32* gw, A_UINT32* ifindex,
        IPV4_ROUTE_LIST_T* rtlist);
A_STATUS _qcom_ip6_route(A_UINT8 device_id, A_UINT32 cmd, A_UINT8 *ip6addr, A_UINT32 *prefixLen, A_UINT8* gw, A_UINT32* ifindex,
        IPV6_ROUTE_LIST_T* rtlist);
A_STATUS _qcom_ota_upgrade(A_UINT8 device_id,A_UINT32 ip_addr, const A_CHAR *source,
		A_UINT8 mode,A_UINT8 preserve_last,A_UINT8 protocol,
		A_UINT32 *resp_code,A_UINT32*length);
A_STATUS _qcom_read_ota_area(A_UINT32 offset,A_UINT32 size, A_UINT8 *buffer,A_UINT32 *ret_len);
A_STATUS _qcom_ota_done(A_UINT32 good_image);
A_STATUS _qcom_set_hostname(A_UINT8 device_id,A_CHAR* pstr);
A_STATUS _qcom_tcp_conn_timeout(A_UINT32 timeout_val);
A_STATUS _qcom_http_set_post_cb(void* cxt, void *callback);
A_STATUS _qcom_dns_set_timeout(int timeout);
QCOM_OTA_STATUS_CODE_t _qcom_ota_session_start(A_UINT32 flags, A_UINT32 partition_index);
A_UINT32 _qcom_ota_partition_get_size(void);
QCOM_OTA_STATUS_CODE_t _qcom_ota_partition_erase(void);
QCOM_OTA_STATUS_CODE_t _qcom_ota_partition_verify_checksum(void);
QCOM_OTA_STATUS_CODE_t _qcom_ota_parse_image_hdr(A_UINT8 *header,A_UINT32 *offset);
A_INT32 _qcom_ota_partition_read_data(A_UINT32 offset, A_UINT8 *buf,A_UINT32 size);
QCOM_OTA_STATUS_CODE_t _qcom_ota_partition_write_data(A_UINT32 offset, A_UINT8 *buf,A_UINT32 size,A_UINT32 *ret_size);
QCOM_OTA_STATUS_CODE_t  _qcom_ota_session_end(A_UINT32 good_image);
QCOM_OTA_STATUS_CODE_t _qcom_ota_partition_format(A_UINT32 partition_index);

A_STATUS _qcom_dhcps_register_cb(A_UINT8 device_id,void *callback);
A_STATUS _qcom_dhcpc_register_cb(A_UINT8 device_id,void *callback);
A_STATUS _qcom_set_ipv6_status(A_UINT8 device_id,A_UINT8 flag);
A_STATUS _qcom_dhcpc_set_timeout(A_UINT8 device_id,A_UINT32 timeout);

#define qcom_ping(host, size)  \
        _qcom_ping((host), (size))

#define qcom_ping6(host, size)  \
        _qcom_ping6((host), (size))

#define  qcom_set_ping_id(PingId)\
    _qcom_set_ping_id((PingId))

#define qcom_dhcps_release_pool(device_id)  \
    _qcom_dhcps_release_pool((device_id))

#define qcom_tcp_set_exp_backoff(device_id, retry)  \
    _qcom_tcp_set_exp_backoff((device_id), (retry))


#define qcom_ip4_route(device_id,cmd,addr,subnet,gw,ifindex,rtlist) \
    _qcom_ip4_route(device_id,cmd,addr,subnet,gw,ifindex,rtlist)

#define qcom_ip6_route(device_id, cmd, ip6addr, prefixLen, gw, ifindex, rtlist)  \
    _qcom_ip6_route((device_id), (cmd), (ip6addr), (prefixLen), (gw), (ifindex), (rtlist))

#define qcom_ota_upgrade(device_id,ip_addr,source,mode,preserve_last, protocol,resp_code,length)\
    _qcom_ota_upgrade((device_id),(ip_addr),(source),(mode),(preserve_last),( protocol),(resp_code),(length))


#define qcom_read_ota_area(offset,size,buffer,ret_len)\
    _qcom_read_ota_area((offset),(size),(buffer),(ret_len))

#define  qcom_ota_done(good_image)\
    _qcom_ota_done(good_image)

#define  qcom_set_hostname(device_id,pstr)\
    _qcom_set_hostname((device_id),(pstr))


#define qcom_tcp_conn_timeout(timeout_val)\
    _qcom_tcp_conn_timeout((timeout_val))

#define qcom_http_set_post_cb(cxt, callback)  \
    _qcom_http_set_post_cb((cxt), (callback))

#define qcom_dns_set_timeout(timeout_val)\
    _qcom_dns_set_timeout((timeout_val))

#define qcom_ota_session_start(flags, partition_index) \
    _qcom_ota_session_start((flags),(partition_index))

#define qcom_ota_partition_get_size() \
    _qcom_ota_partition_get_size()

#define qcom_ota_partition_erase() \
    _qcom_ota_partition_erase()

#define qcom_ota_parse_image_hdr(header,offset) \
    _qcom_ota_parse_image_hdr((header),(offset))

#define qcom_ota_partition_verify_checksum() \
    _qcom_ota_partition_verify_checksum()

#define qcom_ota_partition_read_data(offset,buf,size) \
     _qcom_ota_partition_read_data((offset),(buf),(size))

#define qcom_ota_partition_write_data(offset,buf,size,ret_size) \
     _qcom_ota_partition_write_data((offset),(buf),(size),(ret_size))

#define qcom_ota_session_end(good_image) \
     _qcom_ota_session_end((good_image))

#define qcom_ota_partition_format(partition_index) \
    _qcom_ota_partition_format((partition_index))

#define qcom_dhcps_register_cb(device_id,callback)\
   _qcom_dhcps_register_cb((device_id),(callback))

#define qcom_dhcpc_register_cb(device_id,callback)\
       _qcom_dhcpc_register_cb((device_id),(callback))

#define qcom_set_ipv6_status(device_id,flag)\
        _qcom_set_ipv6_status((device_id),(flag))

 #define qcom_dhcpc_set_timeout(device_id,flag)\
        _qcom_dhcpc_set_timeout((device_id),(flag))


#else

#define qcom_ping(host, size)  \
    QCOM_NETWORK_API_FN(_qcom_ping((host), (size)))

#define qcom_ping6(host, size)  \
    QCOM_NETWORK_API_FN(_qcom_ping6((host), (size)))

#define qcom_set_ping_id(PingId)\
    QCOM_NETWORK_API_FN(_qcom_set_ping_id((PingId)))

#define qcom_dhcps_release_pool(device_id)  \
    QCOM_NETWORK_API_FN(_qcom_dhcps_release_pool((device_id)))

#define qcom_tcp_set_exp_backoff(device_id, retry)  \
    QCOM_NETWORK_API_FNQCOM_NETWORK_API_FN(_qcom_tcp_set_exp_backoff((device_id), (retry)))

#define qcom_ip4_route(device_id,cmd,addr,subnet,gw,ifindex,rtlist) \
    QCOM_NETWORK_API_FN(_qcom_ip4_route(device_id,cmd,addr,subnet,gw,ifindex,rtlist))

#define qcom_ip6_route(device_id, cmd, ip6addr, prefixLen, gw, ifindex, rtlist)  \
    QCOM_NETWORK_API_FN(_qcom_ip6_route((device_id), (cmd), (ip6addr), (prefixLen), (gw), (ifindex), (rtlist)))

#define qcom_ota_upgrade(device_id,ip_addr,source,mode,preserve_last, protocol,resp_code,length)\
        QCOM_NETWORK_API_FN(_qcom_ota_upgrade((device_id),(ip_addr),(source),(mode),(preserve_last),( protocol),(resp_code),(length)))

#define qcom_read_ota_area(offset,size,buffer,ret_len)\
       QCOM_NETWORK_API_FN( _qcom_read_ota_area((offset),(size),(buffer),(ret_len)))

#define qcom_ota_done(good_image)\
    QCOM_NETWORK_API_FN(_qcom_ota_done(good_image))

#define  qcom_set_hostname(device_id,pstr)\
        QCOM_NETWORK_API_FN(_qcom_set_hostname((device_id),(pstr)))

#define qcom_tcp_conn_timeout(timeout_val)\
        QCOM_NETWORK_API_FN(_qcom_tcp_conn_timeout((timeout_val)))

#define qcom_http_set_post_cb(cxt, callback)  \
        QCOM_NETWORK_API_FN(_qcom_http_set_post_cb((cxt), (callback)))

#define qcom_dns_set_timeout(timeout_val)\
        QCOM_NETWORK_API_FN(_qcom_dns_set_timeout((timeout_val)))

#define qcom_ota_session_start(flags, partition_index) \
	QCOM_NETWORK_API_FN(_qcom_ota_session_start((flags), (partition_index)))

#define qcom_ota_partition_get_size() \
      	QCOM_NETWORK_API_FN(_qcom_ota_partition_get_size())

#define qcom_ota_partition_erase() \
      	QCOM_NETWORK_API_FN(_qcom_ota_partition_erase())

#define qcom_ota_parse_image_hdr(header,offset) \
       QCOM_NETWORK_API_FN(_qcom_ota_parse_image_hdr((header),(offset)))

#define qcom_ota_partition_read_data(offset,buf,size) \
     QCOM_NETWORK_API_FN(_qcom_ota_partition_read_data((offset),(buf),(size)))

#define qcom_ota_partition_verify_checksum() \
      	QCOM_NETWORK_API_FN(_qcom_ota_partition_verify_checksum())

#define qcom_ota_partition_write_data(offset, buf,size,ret_size) \
        QCOM_NETWORK_API_FN(_qcom_ota_partition_write_data((offset),(buf),(size),(ret_size)))

#define qcom_ota_session_end(good_image) \
     QCOM_NETWORK_API_FN(_qcom_ota_session_end((good_image)))

#define qcom_ota_partition_format(partition_index) \
    QCOM_NETWORK_API_FN(_qcom_ota_partition_format((partition_index)))

#define qcom_dhcps_register_cb(device_id,callback)\
    QCOM_NETWORK_API_FN(_qcom_dhcps_register_cb((device_id),(callback)))

#define qcom_dhcpc_register_cb(device_id,callback)\
        QCOM_NETWORK_API_FN(_qcom_dhcpc_register_cb((device_id),(callback)))

#define qcom_set_ipv6_status(device_id,flag)\
            QCOM_NETWORK_API_FN(_qcom_set_ipv6_status((device_id),(flag)))


#define qcom_dhcpc_set_timeout(device_id,flag) \
            QCOM_NETWORK_API_FN(_qcom_dhcpc_set_timeout((device_id),(flag)))

#endif

#endif /* __QCOM_NETWORK__ */
