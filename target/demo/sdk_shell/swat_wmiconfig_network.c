/*
  * Copyright (c) 2013 Qualcomm Atheros, Inc..
  * All Rights Reserved.
  * Qualcomm Atheros Confidential and Proprietary.
  */

#include "qcom_common.h"
#include "swat_wmiconfig_common.h"
#include "swat_bench_core.h"
#include "qcom/socket_api.h"
#include "qcom/select_api.h"

#define ADD_SRVR_ADDR 1
#define DEL_SRVR_ADDR 2
#define SSL_FILENAME_LEN   20

A_UINT32 sBASE_AUTO_IP_ADDRESS = AUTOIP_BASE_ADDR;
A_UINT32 sMAX_AUTO_IP_ADDRESS = AUTOIP_MAX_ADDR;

//extern A_STATUS qcom_ip6_address_get(A_UINT8 device_id, IP6_ADDR_T *v6Global, IP6_ADDR_T *v6Link, IP6_ADDR_T *v6DefGw,IP6_ADDR_T *v6GlobalExtd, A_INT32 *LinkPrefix,
//		      A_INT32 *GlbPrefix, A_INT32 *DefgwPrefix, A_INT32 *GlbPrefixExtd);
extern char * print_ip6(IP6_ADDR_T * addr, char * str);

extern	int Inet6Pton(char * src, void * dst);
extern char * swat_strchr(char *str, char chr);

void
swat_wmiconfig_set_ping_id(A_UINT8 device_id,A_UINT32 PingId)
{
   SWAT_PTF("Change the ping ID to be : %d\n", PingId);
   qcom_set_ping_id(PingId);
   return;
}

A_UINT32
_inet_addr(A_CHAR *str)
{
    A_UINT32 ipaddr;
    A_UINT32 data[4];
    A_UINT32 ret;

    ret = A_SSCANF(str, "%3d.%3d.%3d.%3d", data, data + 1, data + 2, data + 3);
    if (ret < 0) {
        return 0;
    } else {
        ipaddr = data[0] << 24 | data[1] << 16 | data[2] << 8 | data[3];
    }
    return ipaddr;
}

char str[20];

A_CHAR *
_inet_ntoa(A_UINT32 ip)
{
    A_MEMSET(str, 0, sizeof (str));
    qcom_sprintf(str, "%u.%u.%u.%u",
            (unsigned) (ip >> 24),
            (unsigned) ((ip >> 16) & 0xff), (unsigned) ((ip >> 8) & 0xff), (unsigned) (ip & 0xff));
    return str;
}


    void
swat_wmiconfig_ipstatic(A_UINT8 device_id, A_CHAR * pIpAddr, A_CHAR * pNetMask, A_CHAR * pGateway)
{
    A_UINT32 address;
    A_UINT32 submask;
    A_UINT32 gateway;

    address = _inet_addr(pIpAddr);
    submask = _inet_addr(pNetMask);
    gateway = _inet_addr(pGateway);

    qcom_ipconfig(device_id, IP_CONFIG_SET, &address, &submask, &gateway);
}

    void
swat_wmiconfig_ipdhcp(A_UINT8 device_id)
{
    A_UINT32 param;
    qcom_ipconfig(device_id, IP_CONFIG_DHCP, &param, &param, &param);
}

    void
swat_wmiconfig_ipauto(A_UINT8 device_id, A_CHAR * pIpAddr)
{
    A_UINT32 param;
	A_UINT32 address = 0;
	
	if(pIpAddr != NULL)
	{
		address = _inet_addr(pIpAddr);
                if(address < sBASE_AUTO_IP_ADDRESS || address > sMAX_AUTO_IP_ADDRESS)
		{
			return;
		}
	}
    qcom_ipconfig(device_id, IP_CONFIG_AUTO, &address, &param, &param);
}
    void
swat_wmiconfig_dns(A_UINT8 device_id, A_CHAR *name)
{
    A_UINT32 ipAddress;
    if (qcom_dnsc_get_host_by_name(name, &ipAddress)== A_OK) {
        A_CHAR *ipaddr;
        ipaddr = _inet_ntoa(ipAddress);
        //SWAT_PTF("Get IP address of host %s\n", name);
        SWAT_PTF("%s has address %s\n", name, (char *)_inet_ntoa(ipAddress));
    }
    //else {
    //    SWAT_PTF("The IP of host %s is not gotten\n", name);
    //}
    return;
}

void
swat_wmiconfig_dns2(A_UINT8 device_id, A_CHAR * name, A_INT32 domain, A_UINT32 mode)
{
    IP6_ADDR_T ip6Addr;
    char ip6buf[48];
    char *ip6ptr =  NULL;
    A_UINT32 ipAddress;
    A_CHAR *ipaddr;
    A_STATUS ret;

   ret = qcom_dnsc_get_host_by_name2(name, (A_UINT32*)(&ip6Addr), domain,  mode);
   if (ret != A_OK){
        //SWAT_PTF("The IP of host %s is not gotten\n", name);
        return;
   }

   //SWAT_PTF("Get IP address of host %s\n", name);
   if (domain == AF_INET6){
        ip6ptr = print_ip6(&ip6Addr, ip6buf);
        SWAT_PTF("%s has address %s\n", name, ip6buf);
    } else {
        memcpy(&ipAddress, &ip6Addr, 4);
        ipAddress = ntohl(ipAddress);
        ipaddr = _inet_ntoa(ipAddress);
        SWAT_PTF("%s has address %s\n", name, ipaddr);
    }

    return;
}

    void
swat_wmiconfig_dns_enable(A_UINT8 device_id, A_CHAR *enable)
{
    int flag;
    if (!strcmp(enable, "start"))
        flag = 1;
    else if (!strcmp(enable, "stop"))
        flag = 0;
    else {
        SWAT_PTF("input paramenter should be start or stop!\n");
        return;
    }
    /* 1: enable; 0:diable; */
    qcom_dnsc_enable(flag);
    return;
}

    void
swat_wmiconfig_dns_svr_add(A_UINT8 device_id, A_CHAR *ipaddr)
{
    A_UINT32 ip = 0;
    A_UINT8 dnsSerIp[16];
    A_MEMSET(dnsSerIp, 0, 16);
    if(!Inet6Pton(ipaddr, dnsSerIp)){
        qcom_dnsc_add_server_address(dnsSerIp, AF_INET6);
        return;
    } else if (0 != (ip = _inet_addr(ipaddr))){
        A_MEMCPY(dnsSerIp, (char*)&ip, 4);
        qcom_dnsc_add_server_address(dnsSerIp, AF_INET);
        return;
    } else {
        SWAT_PTF("input ip addr is not valid!\n");
    }

    return;
}

void
swat_wmiconfig_set_hostname(A_UINT8 device_id, A_CHAR *host_name)
{
    SWAT_PTF("Change the DHCP hostname to be : %s\n", host_name);
    qcom_set_hostname(0,host_name);
    return;
}

    void
swat_wmiconfig_dns_svr_del(A_UINT8 device_id, A_CHAR *ipaddr)
{
    A_UINT32 ip = 0;
    A_UINT8 dnsSerIp[16];
    A_MEMSET(dnsSerIp, 0, 16);
    if(!Inet6Pton(ipaddr, dnsSerIp)){
        qcom_dnsc_del_server_address(dnsSerIp, AF_INET6);
        return;
    } else if (0 != (ip = _inet_addr(ipaddr))){
        A_MEMCPY(dnsSerIp, (char*)&ip, 4);
        qcom_dnsc_del_server_address(dnsSerIp, AF_INET);
        return;
    } else {
        SWAT_PTF("input ip addr is not valid!\n");
    }

    return;
}
    void
swat_wmiconfig_dnss_enable(A_UINT8 device_id, A_CHAR *enable)
{
    int flag;
    if (!strcmp(enable, "start"))
        flag = 1;
    else if (!strcmp(enable, "stop"))
        flag = 0;
    else {
        SWAT_PTF("input paramenter should be start or stop!\n");
        return;
    }
    qcom_dnss_enable(flag);
    return;
}
	void
swat_wmiconfig_dns_domain(A_UINT8 device_id, A_CHAR *local_name)
{
    qcom_dns_local_domain(local_name);
	SWAT_PTF("set DNS local domain name as %s\n", local_name);
    return;
}

    void
swat_wmiconfig_dns_entry_add(A_UINT8 device_id, A_CHAR* hostname, A_CHAR *ipaddr)
{
    A_UINT32 ip = 0;
	A_UINT8 ipv6[16];
	A_MEMSET(ipv6, 0, 16);

	if(!Inet6Pton(ipaddr, ipv6)) {
		SWAT_PTF("input ip addr is V6!\n");
		qcom_dns_6entry_create(hostname, ipv6);
		return;
	}
	else if(0 != (ip = _inet_addr(ipaddr))){
        SWAT_PTF("input ip addr is V4!\n");
		qcom_dns_entry_create(hostname, ip);
        return;
    }
	else {
        SWAT_PTF("input ip addr is not valid!\n");
        return;
	}

}
    void
swat_wmiconfig_dns_entry_del(A_UINT8 device_id, A_CHAR* hostname, A_CHAR *ipaddr)
{
    A_UINT32 ip = 0;
	A_UINT8 ipv6[16];
	A_MEMSET(ipv6, 0, 16);

    if(!Inet6Pton(ipaddr, ipv6)) {
		SWAT_PTF("input ip addr is V6!\n");
		qcom_dns_6entry_delete(hostname, ipv6);
		return;
	}
	else if (0 != (ip = _inet_addr(ipaddr))) {
        SWAT_PTF("input ip addr is V4!\n");
		qcom_dns_entry_delete(hostname, ip);
        return;
    }
	else {
        SWAT_PTF("input ip addr is not valid!\n");
        return;
	}
}

void swat_wmiconfig_dns_set_timeout(A_UINT8 device_id, int timeout)
{
    qcom_param_set(
            device_id,
            QCOM_PARAM_GROUP_NETWORK,
            QCOM_PARAM_GROUP_NETWORK_DNS_TIMEOUT_SECS,
            &timeout,
            4,
            TRUE);
}

void
swat_wmiconfig_ipconfig(A_UINT8 device_id)
{
    A_UINT8 macAddr[6];
    A_MEMSET(&macAddr, 0, sizeof (macAddr));
    A_UINT32 ipAddress;
    A_UINT32 submask;
    A_UINT32 gateway;
    A_UINT32 dns[3];
	A_UINT32 number=0;
	  A_UINT32 i;
#ifdef IP_V6
    IP6_ADDR_T v6Global;
    IP6_ADDR_T v6GlobalExtd;
    IP6_ADDR_T v6LinkLocal;
    IP6_ADDR_T v6DefGw;
    char ip6buf[48];
    char *ip6ptr =  NULL;
    A_INT32 LinkPrefix = 0;
    A_INT32 GlobalPrefix = 0;
    A_INT32 DefGwPrefix = 0;
    A_INT32 GlobalPrefixExtd = 0;
#endif

    qcom_mac_get(device_id, (A_UINT8 *) & macAddr);
    qcom_ipconfig(device_id, IP_CONFIG_QUERY, &ipAddress, &submask, &gateway);
	A_MEMSET(dns,0,3*sizeof(A_UINT32));
    qcom_dns_server_address_get(dns, &number);

    SWAT_PTF(" mac addr = %x:%x:%x:%x:%x:%x\n",
            macAddr[0], macAddr[1], macAddr[2], macAddr[3], macAddr[4], macAddr[5]);
    SWAT_PTF("IP:");
    IPV4_ORG_PTF(ipAddress);
    SWAT_PTF("\n");
    ORG_PTR(" Mask:");
    IPV4_PTF(submask);
    SWAT_PTF("\n");
    SWAT_PTF(", Gateway:");
    IPV4_PTF(gateway);
    SWAT_PTF("\n");
    SWAT_PTF(", Dns:");
    for(i=0;i<number;i++){
        IPV4_PTF(dns[i]);
        SWAT_PTF(";");
    }
    SWAT_PTF("\n");
#ifdef IP_V6
    qcom_ip6_address_get(device_id,(A_UINT8*) &v6Global,(A_UINT8*)&v6LinkLocal,(A_UINT8*)&v6DefGw,(A_UINT8*)&v6GlobalExtd,&LinkPrefix,&GlobalPrefix,&DefGwPrefix,&GlobalPrefixExtd);
    if(v6LinkLocal.addr[0] )
	{
        ip6ptr = print_ip6(&v6LinkLocal,ip6buf);
	    if(ip6ptr)
        {
             if(LinkPrefix)
                 printf("Link-local IPv6 Address ..... : %s/%d\n", ip6ptr,LinkPrefix);
	         else
                 printf("Link-local IPv6 Address ..... : %s\n", ip6ptr);
	    }
        ip6ptr = NULL;
        ip6ptr = print_ip6(&v6Global,ip6buf);
        if(ip6ptr)
        {
            if(GlobalPrefix)
	            printf("Global IPv6 Address ......... : %s/%d\n", ip6ptr,GlobalPrefix);
  	        else
  	            printf("Global IPv6 Address ......... : %s\n", ip6ptr);
	    }
	    ip6ptr = NULL;
        ip6ptr = print_ip6(&v6DefGw, ip6buf);
	    if(ip6ptr)
	    {
	      if(DefGwPrefix)
	      printf("Default Gateway  ............. : %s/%d\n", ip6ptr,DefGwPrefix);
	      else
	      printf("Default Gateway  ............. : %s\n", ip6ptr);

	    }
            ip6ptr = NULL;
            ip6ptr = print_ip6(&v6GlobalExtd,ip6buf);
            if(ip6ptr)
            {
              if(GlobalPrefixExtd)
	      printf("Global IPv6 Address 2 ......... : %s/%d\n", ip6ptr,GlobalPrefixExtd);
	      else
	      printf("Global IPv6 Address 2 ......... : %s\n", ip6ptr);
	    }

	}
#endif
}

    void
swat_wmiconfig_dhcp_pool(A_UINT8 device_id, A_CHAR * pStartaddr, A_CHAR * pEndaddr, A_UINT32 leasetime)
{
    A_UINT32 startaddr, endaddr;

    startaddr = _inet_addr(pStartaddr);
    endaddr = _inet_addr(pEndaddr);

    qcom_dhcps_set_pool(device_id, startaddr, endaddr, leasetime);
}

#ifdef SWAT_WMICONFIG_SNTP
void swat_wmiconfig_sntp_enable(A_UINT8 device_id, char* enable)
{
    int flag;
    if(!strcmp(enable, "start"))
    {
        flag = 1;
    }
    else if(!strcmp(enable, "stop"))
        flag = 0;
    else
    {
        SWAT_PTF("input parameter should be start or stop!\n");
        return;
    }

    qcom_enable_sntp_client(flag);
    return;
}

void swat_wmiconfig_sntp_srv(A_UINT8 device_id, char* add_del,char* srv_addr)
{

    int flag;
    if(!strcmp(add_del,"add"))
        flag = ADD_SRVR_ADDR;
    else if(!strcmp(add_del,"delete"))
        flag = DEL_SRVR_ADDR;
    else
    {
        SWAT_PTF("input parameter should be add or delete!\n");
        return;
    }

    qcom_sntp_srvr_addr(flag,srv_addr);
    return;
}

void swat_wmiconfig_sntp_zone(A_UINT8 device_id, char* utc,char* dse_en_dis)
{
    int flag;
    int hour = 0,min = 0;
    int add_sub;
    char hr[3],mn[3],parsing_hour_min[10];

    if(strlen(utc) > 9){
         SWAT_PTF("Error : Invalid UTC string. Valid string(UTC+hour:min).Eg UTC+05:30\n\r");
         return;
     }

    if(strlen(dse_en_dis) > 8){
         SWAT_PTF("Error : Invalid DSE string.Valid string enable/disable\n\r");
         return;
      }

    strcpy(parsing_hour_min,utc);

    // UTC+xx:xx / UTC-xx:xx
    if(strlen(parsing_hour_min) != 9){
       SWAT_PTF("Error : UTC time format should be UTC+XX:XX or UTC-XX:XX\n");
       SWAT_PTF("Hour from 00 to -12/+13, minute should be 0, 30 or 45\n\r");
       return;
    }

    hr[0] = parsing_hour_min[4];
    hr[1] = parsing_hour_min[5];
    hr[2] = '\0';
    hour  = (hr[0] - '0')*10+ (hr[1] - '0');
    mn[0] = parsing_hour_min[7];
    mn[1] = parsing_hour_min[8];
    mn[2] = '\0';
    min   = (mn[0] - '0')*10+ (mn[1] - '0');

    if( (min!=0) && (min!=30) && (min!=45) ){
       SWAT_PTF("Error : UTC time offset in minutes should be 0, 30 or 45\n\r");
       return;
    }

    // valid time zone : -12,-11,...,+13
    if(parsing_hour_min[3] == '+'){
         add_sub = TRUE; // time is to be added
         if((hour>14)||((14==hour)&&(min>0))){
            SWAT_PTF("Error : UTC time offset in hour from -12 to +14\n\r");
            return;
         }
      }
    else if(parsing_hour_min[3] == '-'){
         add_sub = FALSE; // time is to be subtracted
         if((hour>12)||((12==hour)&&(min>0))){
            SWAT_PTF("Error : UTC time offset in hour from -12 to +13\n\r");
            return;
         }
      }
    else{
         SWAT_PTF("Error : Only + / - operation is allowed\n\r");
         return;
        }


    if(!strcmp(dse_en_dis,"enable"))
        flag = TRUE;
    else if(!strcmp(dse_en_dis,"disable"))
        flag = FALSE;
    else
    {
        SWAT_PTF("DSE(day light saving) input parameter should be enable or disable!\n");
        return;
    }

    qcom_sntp_zone(hour,min,add_sub,flag);
    return;
}

void swat_wmiconfig_sntp_get_time(A_UINT8 device_id)
{
    tSntpTime time;
    char *months[12] = {"Jan","Feb","Mar","Apr","May","Jun","Jul","Aug","Sep","Oct","Nov","Dec"};
    char *Day[7] = {"Sun","Mon","Tue","Wed","Thu","Fri","Sat"};

    qcom_sntp_get_time(device_id, &time);

    SWAT_PTF("\nRTC Timestamp : %s %s %d ,%d %d:%d:%d\n\n\r", Day[time.wday],
                months[time.mon], time.yday,
                time.year, time.hour,
                time.min, time.Sec);

    return;
}

void swat_wmiconfig_sntp_show_config(A_UINT8 device_id)
{
    SNTP_QUERY_SRVR_ADDRESS srvAddr;
    int i;

    qcom_sntp_query_srvr_address(device_id,&srvAddr);

    for(i = 0;i < MAX_SNTP_SERVERS;i++)
    {
        if(srvAddr.SntpAddr[i].resolve != DNS_NAME_NOT_SPECIFIED)
        {
           SWAT_PTF("SNTP SERVER NAME %s\n",srvAddr.SntpAddr[i].addr);
        }
    }
    return;
}

void swat_wmiconfig_sntp_get_time_of_day(A_UINT8 device_id)
{
   tSntpTM time;
    qcom_sntp_get_time_of_day(device_id, &time);
    SWAT_PTF("Total seconds = %d\n",time.tv_sec);

    return;
}
#endif

/*Callback function called by PHOST when a client posts to HTTP server*/
void swat_http_post_callback(void* cxt, void* buf)
{
    HTTP_POST_EVENT_T* ev = (HTTP_POST_EVENT_T*)buf;
    int numTLV;
    unsigned char* data;

    if(!buf)
        return;

    numTLV = ev->numTLV;
    data = ev->data;

    while(numTLV){
        short type;
        short length;
        unsigned char* val;

        /*Parse through all TLVs*/
        GET_TLV_TYPE(data, type);
        GET_TLV_LENGTH(data, length);
        val = GET_TLV_VAL(data);
        switch(type){
            case POST_TYPE_URI:
                SWAT_PTF("URI: %s\n",val); 
                break;
            case POST_TYPE_NAME:
                SWAT_PTF("NAME: %s\n",val); 
                break;
            case POST_TYPE_VALUE: 
                SWAT_PTF("VALUE: %s\n",val); 
                break;
            default:
                SWAT_PTF("Invalid TLV\n");
                break;

        }
        data = GET_NEXT_TLV(data, length);
        numTLV--;
    }
}

void swat_wmiconfig_http_server(A_UINT8 device_id, int enable)
{
#ifdef HTTP_ENABLED
    int res;
    res = qcom_http_server(enable);
    if(enable == 1){
        if(res == 0){
            SWAT_PTF("HTTP SERVER start sucessful\n");
            /*Server started, set callback for post events*/
            qcom_http_set_post_cb(NULL, swat_http_post_callback);
        }else{
            SWAT_PTF("HTTP SERVER start failed\n");
        }
    }else if(enable == 0){
        if(res == 0){
            SWAT_PTF("HTTP SERVER stop sucessful\n");
        }else{
            SWAT_PTF("HTTP SERVER stop failed\n");
        }
    }else if(enable == 3){
        if(res == 0){
            SWAT_PTF("HTTPS SERVER start sucessful\n");
            /*Server started, set callback for post events*/
            qcom_http_set_post_cb(NULL, swat_http_post_callback);
        }else{
            SWAT_PTF("HTTPS SERVER start failed\n");
        }
    }else if(enable == 2){
        if(res == 0){
            SWAT_PTF("HTTPS SERVER stop sucessful\n");
        }else{
            SWAT_PTF("HTTPS SERVER stop failed\n");
        }
    }

#endif
}

#ifdef BRIDGE_ENABLED

//extern A_STATUS qcom_bridge_mode_enable(A_UINT16 bridgemode);

void
swat_wmiconfig_bridge_mode_enable(A_UINT8 device_id)
{
	qcom_bridge_mode_enable(1);

    return;
}
#endif


/************************************************************************
* NAME: ipv4_route
*
* DESCRIPTION: Add, delete or show IPv4 route table
*               wmiconfig --ipv4route add <addr> <mask> <gw>
*               wmiconfig --ipv4route del <addr> <mask> <ifindex>
*               wmiconfig --ipv4route show
************************************************************************/
A_INT32 swat_ipv4_route(A_UINT8 device_id,A_INT32 argc, char* argv[])
{ /* Body */
    A_INT32         error = A_OK;
    A_UINT32        ifIndex, addr, mask, gw;
    A_UINT32        command;
    IPV4_ROUTE_LIST_T routelist;

    do
    {
        if(argc < 3)
        {
            error = A_ERROR;
            break;
        }
        
        if(strcmp(argv[2], "add") == 0)
        {
            if(argc < 7)
            {
                error = A_ERROR;
                break;
            }
            
            command = ROUTE_ADD;
            addr=_inet_addr(argv[3]);
            mask=_inet_addr(argv[4]);
            gw=_inet_addr(argv[5]);
            ifIndex = atoi(argv[6]);
        }
        else if(strcmp(argv[2], "del") == 0)
        {
            if(argc < 6)
            {
                error = A_ERROR;
                break;
            }
            
            command = ROUTE_DEL;
            addr=_inet_addr(argv[3]);
            mask=_inet_addr(argv[4]);
            ifIndex = atoi(argv[5]);
        }
        else if(strcmp(argv[2], "show") == 0)
        {
            command = ROUTE_SHOW;
        }
        else
        {
            //printf("Unknown Command \"%s\"\n", argv[2]);
            error = A_ERROR;
            break;
        }

        if(A_OK == qcom_ip4_route(device_id, command, &addr, &mask, &gw, &ifIndex, &routelist))
        {
            if(command == ROUTE_SHOW)
            {
                int i = 0;
                    printf(" %d\n..IPaddr.......mask.........nexthop...iface..\n",routelist.rtcount);
                    for(i = 0; i < routelist.rtcount; i++)
                    {
                        printf("%s  ",_inet_ntoa(routelist.route[i].address));
                        printf("%s  ",_inet_ntoa(routelist.route[i].mask));
                        printf("%s  ",_inet_ntoa(routelist.route[i].gateway));
                        printf("%d  \n",routelist.route[i].ifIndex - 1);
                    }
            }
        }
        else
        {
            printf("Route %s failed\n", (command == 0)?"addition":((command ==1)?"deletion":"show"));
        }
    }while(0);

    if(A_ERROR == error)
    {
        printf ("USAGE: wmiconfig --ipv4route add <address> <mask> <gw>\n");
        printf ("       wmiconfig --ipv4route del <address> <mask> <ifIndex>\n");
        printf ("       wmiconfig --ipv4route show\n");
    }
    
    return error;
} /* Endbody */


/************************************************************************
* NAME: ipv6_route
*
* DESCRIPTION: Add, delete or show IPv4 route table
*               wmiconfig --ipv6route add <addr> <prfx len> <gw>
*               wmiconfig --ipv6route del <addr> <ifindex>
*               wmiconfig --ipv6route show
************************************************************************/
A_INT32 swat_ipv6_route(A_UINT8 device_id, A_INT32 argc, char* argv[])
{ /* Body */
    A_INT32         error = A_OK;
    A_UINT32        ifIndex, prefixLen;
    A_UINT8         ip6addr[16];
    A_UINT8         gateway[16];
    A_UINT32        command;
    IPV6_ROUTE_LIST_T routelist;

    do
    {
        if(argc < 3)
        {
            printf("incomplete params\n");
            error = A_ERROR;
            break;
        }

        if(strcmp(argv[2], "show") != 0)
        {
            error=Inet6Pton(argv[3],ip6addr);
            if(error!=0)
            {
                printf("Invalid IP address\n");
                error = A_ERROR;
                break;
            }

            prefixLen = atoi(argv[4]);
        }
        if(strcmp(argv[2], "add") == 0)
        {
            command = ROUTE_ADD;
            //ifIndex = atoi(argv[6]);

            error=Inet6Pton(argv[5],gateway);
            if(error!=0)
            {
                printf("Invalid Gateway address\n");
                error = A_ERROR;
                break;
            }
        }
        else if(strcmp(argv[2], "del") == 0)
        {
            command = ROUTE_DEL;
            ifIndex = atoi(argv[5]);
        }
        else if(strcmp(argv[2], "show") == 0)
        {
            command = ROUTE_SHOW;
        }
        else
        {
            printf("Unknown Command \"%s\"\n", argv[2]);
            error = A_ERROR;
            break;
        }

        if(A_OK == qcom_ip6_route(device_id, command, ip6addr, &prefixLen, gateway, &ifIndex, &routelist))
        {
            if(command == ROUTE_SHOW)
            {
                int i = 0;
                char ip_str[48];
                {
                    printf("\n..IPaddr.......prefixLen.........nexthop...iface..\n");
                    for(i = 0; i < routelist.rtcount; i++)
                    {
                        printf("%s  %d ", inet6_ntoa((char *)(&routelist.route[i].address), (char *)ip_str),
                               routelist.route[i].prefixlen);
                        printf("%s  %d\n", inet6_ntoa((char *)(&routelist.route[i].nexthop), (char *)ip_str),
                               routelist.route[i].ifindex);
                    }
                }
            }
        }
        else
        {
            printf("Route %s failed\n", (command == 0)?"addition":((command ==1)?"deletion":"show"));
        }
    }while(0);

    if (error == A_ERROR)
    {
        printf ("USAGE: wmiconfig --ipv6_route add <address> <prefixLen> <gw>\n");
        printf ("       wmiconfig --ipv6_route del <address> <prefixLen> <ifIndex>\n");
        printf ("       wmiconfig --ipv6_route show\n");
    }
    return error;
} /* Endbody */

A_INT32 swat_set_ipv6_status(A_UINT8 device_id, char* status){
    int flag;
    if (!strcmp(status, "enable"))
     flag = 1;
    else if (!strcmp(status, "disable"))
     flag = 0;
    else {
    SWAT_PTF("input paramenter should be enable or disable!\n");
     return A_ERROR;
    }
    qcom_set_ipv6_status(device_id,flag);
    return A_OK;
}

A_INT32 swat_tcp_conn_timeout(A_INT32 argc,char *argv[]){

    A_UINT32 timeout_val;
    if(argc < 2)
      {
          SWAT_PTF(" Incomplete Params.\n");
          SWAT_PTF("e.g wmiconfig --settcptimeout <val in sec.>\n");
          return A_ERROR;
     }

    timeout_val = atoi(argv[2]);
    qcom_tcp_conn_timeout(timeout_val);
    return A_OK;
}

#ifdef SWAT_SSL
#include "qcom_ssl.h"
#include "qcom/socket_api.h"
#include "qcom/select_api.h"


SSL_INST ssl_inst[MAX_SSL_INST];
A_UINT8 *ssl_cert_data_buf;
A_UINT16 ssl_cert_data_buf_len;

A_INT32 ssl_get_cert_handler(A_INT32 argc, char* argv[])
{
    A_INT32 res = A_ERROR;
//    DNC_CFG_CMD dnsCfg;
//    DNC_RESP_INFO dnsRespInfo;
    struct sockaddr_in hostAddr;
    A_UINT32 socketHandle = 0, ipAddress = 0;
    int reqLen;
    CERT_HEADER_T *req;
    CERT_HEADER_T *header;
    A_UINT8 *buf;
    int certNameLen, numRead = 0, index, port = 1443;
    char *host, *certName, *flashName = NULL;
    q_fd_set sockSet,master;
    struct timeval tmo;

    // Free certificate buffer if allocated
    if (ssl_cert_data_buf)
    {
      mem_free(ssl_cert_data_buf);
    }
    ssl_cert_data_buf_len = 0;


    // Parse the arguments
    if(argc < 3)
    {
        if (argc > 1)
        {
          printf("Incomplete parameters\n");
        }
        printf("Usage: %s <name> <host> -p <port -s <fname>\n", argv[0]);
        printf("  <name>  = Name of the certificate or CA list file to retrieve\n");
        printf("  <host>  = Host name or IP address of certificate server\n");
        printf("  <port>  = Optional TCP port number\n");
        printf("  <fname> = Optional file name used if certificate is stored in FLASH\n");
        return A_ERROR;
    }
    certName = argv[1];
    host = argv[2];
    for(index = 3; index < argc ; index++)
    {
        if(argv[index][0] == '-')
        {
            switch(argv[index][1])
            {
            case 'p':
                index++;
                port = atoi(argv[index]);
                break;
            case 's':
                index++;
                flashName = argv[index];
                if (strlen(flashName) >= SSL_FILENAME_LEN)
                {
                    printf("ERROR: the length of certificate file name to be stored should less than %d\n", SSL_FILENAME_LEN);
                    return A_ERROR;
                }
                break;
            default:
                printf("Unknown option: %s\n", argv[index]);
                return A_ERROR;
            }
        }
    }

    do
    {
#if 0
        // resolve the IP address of the certificate server
        if (0 == ath_inet_aton(host, &dnsRespInfo.ipaddrs_list[0]))
        {
            if (strlen(host) >= sizeof(dnsCfg.ahostname))
            {
                printf("ERROR: host name too long\n");
                break;
            }
            strcpy((char*)dnsCfg.ahostname, host);
            dnsCfg.domain = ATH_AF_INET;
            dnsCfg.mode =  RESOLVEHOSTNAME;
            if (A_OK != custom_ip_resolve_hostname(handle, &dnsCfg, &dnsRespInfo))
            {
                printf("ERROR: Unable to resolve server name\r\n");
                break;
            }
        }
#endif
        ipAddress = _inet_addr(host);
        // Create socket
        if((socketHandle = swat_socket(AF_INET, SOCK_STREAM, 0)) == A_ERROR)
        {
            printf("ERROR: Unable to create socket\n");
            break;
        }

        // Connect to certificate server
        memset(&hostAddr, 0, sizeof(hostAddr));
        hostAddr.sin_addr.s_addr = htonl(ipAddress);//dnsRespInfo.ipaddrs_list[0];
        hostAddr.sin_port = htons(port);
        hostAddr.sin_family = AF_INET;
        res = swat_connect( socketHandle,(struct sockaddr *)(&hostAddr), sizeof(hostAddr));
        if(res != A_OK)
        {
            printf("ERROR: Connection failed (%d).\n", res);
            break;
        }

        // Build and send request
        certNameLen = strlen(certName);
        reqLen = CERT_HEADER_LEN + certNameLen;
        req = (CERT_HEADER_T*) mem_alloc(reqLen);

        if (req == NULL)
        {
            printf("ERROR: Out of memory.\n");
            break;
        }
        req->id[0] = 'C';
        req->id[1] = 'R';
        req->id[2] = 'T';
        req->id[3] = '0';
        req->length = HTONL(certNameLen);
        memcpy(&req->data[0], certName, certNameLen);
        res = swat_send(socketHandle, (char*)req, reqLen, 0);
        mem_free(req);
        if (res < 0 )
        {
            printf("ERROR: send error = %d\n", res);
            break;
        }
    } while (0);

    // Read the response
    swat_fd_zero(&master);
    swat_fd_set(socketHandle, &master);
    tmo.tv_sec	= 10;
    tmo.tv_usec = 0;
    if((buf = mem_alloc(1500)) == NULL){
        SWAT_PTF("Buf MALLOC ERR\n");
        return A_ERROR;
    }
    do
    {
        sockSet = master;
        res = swat_select(socketHandle + 1, &sockSet, NULL, NULL, &tmo);
         if (0 != res) {
            if (swat_fd_isset(socketHandle, &sockSet)) {
            res = swat_recv(socketHandle, (char*)buf, 1500, 0);
            printf("RX: %d\n", res);
            if (res > 0)
            {
                if (ssl_cert_data_buf_len == 0)
                {
                    if (buf[0] != 'C' || buf[1] != 'R' || buf[2] != 'T')
                    {
                        printf("ERROR: Bad MAGIC received in header\n");
                        break;
                    }
                    header = (CERT_HEADER_T*)buf;
                    header->length =  NTOHL(header->length);
                    if (header->length == 0)
                    {
                        break;
                    }
                    ssl_cert_data_buf = mem_alloc(header->length);
                    if(ssl_cert_data_buf == NULL)
                    {
                        printf("ERROR: Out of memory error\n");
                        res = A_ERROR;
                        break;
                    }
                    ssl_cert_data_buf_len = header->length;
                    res -= 8;
                    memcpy(ssl_cert_data_buf, header->data, res);
                    numRead = res;
                }
                else
                {
                    if (res + numRead <= ssl_cert_data_buf_len)
                    {
                        memcpy(&ssl_cert_data_buf[numRead], buf, res);
                        numRead += res;
                        res = ssl_cert_data_buf_len;
                    }
                    else
                    {
                        printf("ERROR: read failed\n");
                        res = A_ERROR;
                        break;
                    }
                }
            }
            else
            {
                printf("ERROR: no response\n");
                res = A_ERROR;
                break;
            }
            }
        }
    } while (numRead < ssl_cert_data_buf_len);
    mem_free(buf);
    if (socketHandle)
    {
        qcom_socket_close( socketHandle);
    }
    if (res == ssl_cert_data_buf_len && res != 0)
    {
        printf("Received %d bytes from %s:%d\n", ssl_cert_data_buf_len, host, port);

        if (flashName != NULL)
        {
            // store certificate in FLASH
            if (A_OK == qcom_SSL_storeCert(flashName, ssl_cert_data_buf, ssl_cert_data_buf_len))
            {
                printf("'%s' is stored in FLASH\n", flashName);
            }
            else
            {
                printf("ERROR: failed to store in %s\n", flashName);
                res = A_ERROR;
            }
        }
    }
/*
    if (ssl_cert_data_buf)
    {
      mem_free(ssl_cert_data_buf);
    }
    ssl_cert_data_buf_len = 0;
*/
    return res;
}


A_INT32 ssl_start(A_INT32 argc, char *argv[])
{
    SSL_INST *ssl;
    SSL_ROLE_T role;

    if(argc < 3)
    {
       printf("ERROR: Incomplete params\n");
       return A_ERROR;
    }

    if (0 == strcmp(argv[2], "server"))
    {
        ssl = &ssl_inst[SSL_SERVER_INST];
        role = SSL_SERVER;
    }
    else if (0 == strcmp(argv[2], "client"))
    {
        ssl = &ssl_inst[SSL_CLIENT_INST];
        role = SSL_CLIENT;
    }
    else
    {
        printf("ERROR: Invalid parameter: %s\n", argv[2]);
        return A_ERROR;
    }

    if (ssl->sslCtx != NULL)
    {
        printf("ERROR: one SSL context only!\n");
        return A_ERROR;
    }

    // Create SSL context
    memset(ssl, 0, sizeof(SSL_INST));
    ssl->role = role;
    ssl->sslCtx = qcom_SSL_ctx_new(role, SSL_INBUF_SIZE, SSL_OUTBUF_SIZE, 0);
    if (ssl->sslCtx == NULL)
    {
        printf("ERROR: Unable to create SSL context\n");
        return A_ERROR;
    }

    // Reset config struct
    memset(&ssl->config, 0, sizeof(SSL_CONFIG));

    // Done
    printf("SSL %s started\n", argv[2]);
    return A_OK;
}

A_INT32 ssl_stop(A_INT32 argc, char *argv[])
{
    SSL_INST *ssl;
    A_INT32 res = A_OK;
    SSL_ROLE_T role;

    if(argc < 3)
    {
       printf("ERROR: Incomplete params\n");
       return A_ERROR;
    }

    if (0 == strcmp(argv[2], "server"))
    {
        ssl = &ssl_inst[SSL_SERVER_INST];
        role = SSL_SERVER;
    }
    else if (0 == strcmp(argv[2], "client"))
    {
        ssl = &ssl_inst[SSL_CLIENT_INST];
        role = SSL_CLIENT;
    }
    else
    {
        printf("ERROR: Invalid parameter: %s\n", argv[2]);
        return A_ERROR;
    }

    if (ssl->sslCtx == NULL || role != ssl->role)
    {
        printf("ERROR: SSL %s not started\n", argv[2]);
        return A_ERROR;
    }

    if (ssl->ssl)
    {
        qcom_SSL_shutdown(ssl->ssl);
        ssl->ssl = NULL;
    }

    if (ssl->sslCtx)
    {
        qcom_SSL_ctx_free(ssl->sslCtx);
        ssl->sslCtx = NULL;
    }

    printf("SSL %s stopped\n", argv[2]);
    return res;
}


A_INT32 ssl_config(A_INT32 argc, char *argv[])
{
    SSL_INST *ssl;
    A_INT32 res;
    int argn, cipher_count = 0;

    if(argc < 3)
    {
       printf("ERROR: Incomplete params\n");
    }

    if (0 == strcmp(argv[2], "server"))
    {
        ssl = &ssl_inst[SSL_SERVER_INST];
    }
    else if (0 == strcmp(argv[2], "client"))
    {
        ssl = &ssl_inst[SSL_CLIENT_INST];
    }
    else
    {
        printf("ERROR: Invalid parameter: %s\n", argv[2]);
        return A_ERROR;
    }

    if (ssl->sslCtx == NULL)
    {
        printf("ERROR: SSL %s not started\n", argv[2]);
        return A_ERROR;
    }

    argn = 3;
    ssl->config_set = 0;
    while (argn < argc)
    {
        if (argn == argc-1)
        {
            printf("ERROR: Incomplete params\n");
            return A_ERROR;
        }

        if (0 == strcmp("protocol", argv[argn]))
        {
            // Setting of protocol option is supported for SSL client only
            if (strcmp(argv[2], "client"))
            {
                printf("ERROR: Protocol option is not supported for %s\n", argv[2]);
                return A_ERROR;
            }
            argn++;
            if (0 == strcmp("SSL3", argv[argn]))
            {
                ssl->config.protocol = SSL_PROTOCOL_SSL_3_0;
            }
            else if (0 == strcmp("TLS1.0", argv[argn]))
            {
                ssl->config.protocol = SSL_PROTOCOL_TLS_1_0;
            }
            else if (0 == strcmp("TLS1.1", argv[argn]))
            {
                ssl->config.protocol = SSL_PROTOCOL_TLS_1_1;
            }
            else if (0 == strcmp("TLS1.2", argv[argn]))
            {
                ssl->config.protocol = SSL_PROTOCOL_TLS_1_2;
            }
            else
            {
                printf("ERROR: Invalid protocol: %s\n", argv[argn]);
                return A_ERROR;
            }
        }

        if (0 == strcmp("time", argv[argn]))
        {
            argn++;
            if (0 == strcmp("1", argv[argn]))
            {
                ssl->config.verify.timeValidity = 1;
            }
            else if (0 == strcmp("0", argv[argn]))
            {
                ssl->config.verify.timeValidity = 0;
            }
            else
            {
                printf("ERROR: Invalid option: %s\n", argv[argn]);
                return A_ERROR;
            }
        }

        if (0 == strcmp("alert", argv[argn]))
        {
            argn++;
            if (0 == strcmp("1", argv[argn]))
            {
                ssl->config.verify.sendAlert = 1;
            }
            else if (0 == strcmp("0", argv[argn]))
            {
                ssl->config.verify.sendAlert = 0;
            }
            else
            {
                printf("ERROR: Invalid option: %s\n", argv[argn]);
                return A_ERROR;
            }
        }

        if (0 == strcmp("domain", argv[argn]))
        {
            argn++;
            if (0 == strcmp("0", argv[argn]))
            {
                ssl->config.verify.domain = 0;
                ssl->config.matchName[0] = '\0';
            }
            else
            {
                ssl->config.verify.domain = 1;
                if (strlen(argv[argn]) >= sizeof(ssl->config.matchName))
                {
                    printf("ERROR: %s too long (max %d chars)\n", argv[argn], sizeof(ssl->config.matchName));
                    return A_ERROR;
                }
                strcpy(ssl->config.matchName, argv[argn]);
            }
        }

        if (0 == strcmp("cipher", argv[argn]))
        {
            argn++;
            if (cipher_count == 0)
            {
                memset(ssl->config.cipher, 0, sizeof(ssl->config.cipher));
            }
            if (cipher_count == SSL_CIPHERSUITE_LIST_DEPTH)
            {
                printf("ERROR: Too many cipher options %s (max %d)\n", argv[argn], SSL_CIPHERSUITE_LIST_DEPTH);
                return A_ERROR;
            }
            if (0 == strcmp("TLS_RSA_WITH_AES_256_GCM_SHA384", argv[argn]))
            {
                ssl->config.cipher[cipher_count] = TLS_RSA_WITH_AES_256_GCM_SHA384;
            }
            else if (0 == strcmp("TLS_RSA_WITH_AES_256_CBC_SHA256", argv[argn]))
            {
                ssl->config.cipher[cipher_count] = TLS_RSA_WITH_AES_256_CBC_SHA256;
            }
            else if (0 == strcmp("TLS_RSA_WITH_AES_256_CBC_SHA", argv[argn]))
            {
                ssl->config.cipher[cipher_count] = TLS_RSA_WITH_AES_256_CBC_SHA;
            }
            else if (0 == strcmp("TLS_RSA_WITH_AES_128_GCM_SHA256", argv[argn]))
            {
                ssl->config.cipher[cipher_count] = TLS_RSA_WITH_AES_128_GCM_SHA256;
            }
            else if (0 == strcmp("TLS_RSA_WITH_AES_128_CBC_SHA256", argv[argn]))
            {
                ssl->config.cipher[cipher_count] = TLS_RSA_WITH_AES_128_CBC_SHA256;
            }
            else if (0 == strcmp("TLS_RSA_WITH_AES_128_CBC_SHA", argv[argn]))
            {
                ssl->config.cipher[cipher_count] = TLS_RSA_WITH_AES_128_CBC_SHA;
            }
            else if (0 == strcmp("TLS_RSA_WITH_3DES_EDE_CBC_SHA", argv[argn]))
            {
                ssl->config.cipher[cipher_count] = TLS_RSA_WITH_3DES_EDE_CBC_SHA;
            }
            else
            {
                printf("ERROR: Invalid protocol: %s\n", argv[argn]);
                return A_ERROR;
            }
            cipher_count++;
        }

        argn++;
    }

	ssl->config_set = 1;

    if (ssl->ssl != NULL)
    {

        /*
		        We should not create an SSL connection object.
        		We should create it after TCP connection, it's already done in tcp_bench_rx and tcp_bench_tx
        		If create too early, it will cause some other configure issue, such as we can't set CA list after the connection object created.
        	*/
       	/*
		        ssl->ssl = qcom_SSL_new(ssl->sslCtx);
		        if (ssl->ssl == NULL)
		        {
		            printf("ERROR: SSL configure failed (Unable to create SSL context)\n");
		            return A_ERROR;;
		        }
	 	*/
	    // configure the SSL connection
	    res = qcom_SSL_configure(ssl->ssl, &ssl->config);
	    if (res < A_OK)
	    {
	        printf("ERROR: SSL configure failed (%d)\n", res);
	        return A_ERROR;
	    }
	    printf("SSL %s configuration changed\n", argv[2]);
	}
    return A_OK;
}


#include "cert-kingfisher.inc"
extern const A_UINT8 sharkSslRSACertKingfisher[1016];
A_INT32 ssl_add_cert(A_INT32 argc,char *argv[])
{
    SSL_INST *ssl;
    SSL_CERT_TYPE_T type;
    char *name = NULL;

    if(argc < 4)
    {
       printf("Incomplete params\n");
       return A_ERROR;
    }

    if (0 == strcmp(argv[2], "server"))
    {
        ssl = &ssl_inst[SSL_SERVER_INST];
        if (ssl->sslCtx == NULL)
        {
            printf("ERROR: SSL server not started\n");
            return A_ERROR;
        }
    }
    else if (0 == strcmp(argv[2], "client"))
    {
        ssl = &ssl_inst[SSL_CLIENT_INST];
        if (ssl->sslCtx == NULL)
        {
            printf("ERROR: SSL client not started\n");
            return A_ERROR;
        }
    }
    else
    {
        printf("ERROR: Invalid parameter: %s\n", argv[2]);
        return A_ERROR;
    }

    if (0 == strcmp("certificate", argv[3]))
    {
        type = SSL_CERTIFICATE;
    }
    else if (0 == strcmp("calist", argv[3]))
    {
        type = SSL_CA_LIST;
    }
    else
    {
        printf("ERROR: Invalid parameter: %s\n", argv[3]);
        return A_ERROR;
    }

    if (argc > 4)
    {
        name = argv[4];
    }

    // Load/add certificate
    if (name != NULL)
    {
        if (qcom_SSL_loadCert(ssl->sslCtx, type, name) < A_OK)
        {
            printf("ERROR: Unable to load %s from FLASH\n" , name);
            return A_ERROR;
        }
        printf("%s loaded from FLASH\n", name);
    }
    else
    {
        A_UINT8 *cert = ssl_cert_data_buf;
        A_UINT16 cert_len = ssl_cert_data_buf_len;
        if (type == SSL_CERTIFICATE)
        {
            if (cert_len == 0)
            {
                // Load the default certificate
                printf("Note: using the default certificate\n");
                cert = (A_UINT8*)sharkSslRSACertKingfisher;
                cert_len = sizeof(sharkSslRSACertKingfisher);
            }
            if (qcom_SSL_addCert(ssl->sslCtx, cert, cert_len) < A_OK)
            {
                printf("ERROR: Unable to add certificate\n");
                return A_OK;
            }
            printf("Certificate added to SSL %s\n",ssl->role == SSL_SERVER?"server":"client" );
        }
        else
        {
            if (cert_len == 0)
            {
                // Load the default CA list
                //printf("Note: using the default CA list\n");
                //cert = (A_UINT8*)ssl_default_calist;
                //cert_len = ssl_default_calist_len;
                printf("Note: CA list is NULL\n");
	            return A_ERROR;		
            }
            if (qcom_SSL_setCaList(ssl->sslCtx, cert, cert_len)< A_OK)
            {
                printf("ERROR: Unable to set CA list\n");
                return A_ERROR;
            }
            printf("CA list added to SSL %s\n",ssl->role == SSL_SERVER?"server":"client" );
        }
	}
    return A_OK;
}


A_INT32 ssl_store_cert(A_INT32 argc,char *argv[])
{
    char *name;

    if(argc < 3)
    {
       printf("Incomplete params\n");
       return A_ERROR;
    }
    name = argv[2];

    if (ssl_cert_data_buf_len == 0)
    {
        printf("ERROR: no certificate data.\nHint: Use the wmiconfig --ssl_get_cert to read a certificate from a certificate server.\n");
        return A_ERROR;
    }
    if (strlen(name) >= SSL_FILENAME_LEN)
    {
        printf("ERROR: failed to store %s, the length of certificate file name should less than %d\n", name, SSL_FILENAME_LEN);
        return A_ERROR;
    }
    if (A_OK == qcom_SSL_storeCert(name, ssl_cert_data_buf, ssl_cert_data_buf_len))
    {
        printf("%s is stored in FLASH\n", name);
        return A_OK;
    }
    else
    {
        printf("ERROR: failed to store %s\n", name);
        return A_ERROR;
    }
}

A_INT32 ssl_delete_cert(A_INT32 argc,char *argv[])
{
    char *name;

    if(argc < 3)
    {
        ssl_cert_data_buf_len = 0;
        printf("Deleted the certificate data stored in RAM.\n");
        return A_OK;
    }
    name = argv[2];

    if (A_OK == qcom_SSL_storeCert(name, NULL, 0))
    {
        printf("Deleted %s from FLASH\n", name);
        return A_OK;
    }
    else
    {
        printf("ERROR: failed to delete %s\n", name);
        return A_ERROR;
    }
}


A_INT32 ssl_list_cert(A_INT32 argc,char *argv[])
{
    SSL_FILE_NAME_LIST *fileNames;
    A_INT32 i, numFiles, namesLen = sizeof(SSL_FILE_NAME_LIST); // 10 files

    fileNames = mem_alloc(namesLen);
    if (fileNames == NULL)
    {
        printf("ALLOC ERROR\n");
        return A_ERROR;
    }

    numFiles = qcom_SSL_listCert(fileNames);
    if (numFiles < 0)
    {
        printf("ERROR: failed to list files (%d)\n", numFiles);
        mem_free(fileNames);
        return A_ERROR;
    }

    printf("%d %s stored in FLASH\n", numFiles, numFiles == 1 ? "file" : "files");
    for (i=0; i<numFiles; i++)
    {
        char str[21];
        if (fileNames->name[i][0])
        {
            strncpy(str, (char*)fileNames->name[i], sizeof(str)-1);
            str[sizeof(str)-1] = '\0';
            printf("%s\n", str);
        }
    }
    mem_free(fileNames);
    return A_OK;
}

#endif

#ifdef SWAT_OTA
A_INT32 swat_ota_upgrade(A_UINT8 device_id,A_INT32 argc, char* argv[])
{
    A_UINT32 addr;
    A_CHAR filename[100];
    A_UINT8 mode;
    A_UINT8 preserve_last;
    A_UINT8 protocol;
    A_UINT32 resp_code,length;
    A_UINT32 partition_index = 0;

    if(argc < 7)
    {
        SWAT_PTF("Incomplete params\n");
        SWAT_PTF("Usage: wmiconfig --ota_upgrade <ip> <filename> <mode> <preseve_last> <protocol> [partition_index]\n");
        return A_ERROR;
    }

    if (argc == 8) {
        partition_index = atoi(argv[7]);
        if (partition_index < 1 || partition_index > 7) {
            SWAT_PTF("Error : Invalid Partition index [1 - N, N is depend on flash size, the maxnum is 7]\n\r");
            return A_ERROR;
        }
    }

    addr=_inet_addr(argv[2]);

    if(strlen(argv[3]) >= 100)
    {
        SWAT_PTF("Error : Invalid Filename Length\n");
        return A_ERROR;
    }

    strcpy(filename,argv[3]);
    mode= atoi(argv[4]);

    if(mode > 2)
    {
        SWAT_PTF("Error : Invalid Mode [valid values are 0 or 1 ]\n\r");
        return A_ERROR;
    }

    preserve_last= atoi(argv[5]);

    if(preserve_last > 2)
    {
        SWAT_PTF("Error : Invalid preserve_last [valid values are 0 or 1 ]\n\r");
        return A_ERROR;
    }

    protocol= atoi(argv[6]);

    if(protocol > 2)
    {
        SWAT_PTF("Error: Invalid protocol [valid values are 0 or 1 ]\n\r");
        return A_ERROR;
    }
    
    if (argc == 7)
        qcom_ota_upgrade(device_id,addr,filename,mode,preserve_last,protocol,&resp_code,&length);

    extern A_STATUS _qcom_ota_upgrade_partition(A_UINT8 device_id,A_UINT32 ip_addr, const A_CHAR *source,
                    A_UINT8 mode,A_UINT8 preserve_last,A_UINT8 protocol,
                    A_UINT32 *resp_code,A_UINT32 *length, A_UINT32 partition_index);
    if (argc == 8)
        _qcom_ota_upgrade_partition(device_id,addr,filename,mode,preserve_last,protocol,&resp_code,&length, partition_index);

    if(resp_code!=A_OK){
       SWAT_PTF("OTA Download Failed, ERR=%d",resp_code);
       return A_ERROR;
   }else{
       SWAT_PTF("OTA Download Successful Image Size:%d\n ",length);
   }


}

#define OTA_READ_BUF_SIZE	256

A_INT32 swat_ota_read(A_INT32 argc,char *argv[])
{
    A_ULONG offset;
    A_ULONG len, total_read = 0;
    A_UCHAR *data;
    A_INT32 error = 0;
    A_UINT32 ret_len=0;

    if(argc < 4)
    {
        SWAT_PTF(" Incomplete Params\nUsage: wmiconfig --ota_read <offset> <size>!\n");
        SWAT_PTF("e.g wmiconfig --ota_read 100 20 to read 20 bytes from and offset 100 w.r.t start of the image!\n");
        return A_ERROR;
    }

    data = mem_alloc(OTA_READ_BUF_SIZE);
    if (!data) {
    	SWAT_PTF("Insufficient memory\n");
    	return A_ERROR;
    }

    offset = atoi(argv[2]);
    len = atoi(argv[3]);

    SWAT_PTF("Partition data:\n");

    while (total_read < len) {
        A_UINT32 index;
        A_UINT32 bytes_to_read;

        if (len - total_read > OTA_READ_BUF_SIZE) {
        	bytes_to_read = OTA_READ_BUF_SIZE;
        } else {
        	bytes_to_read = len - total_read;
        }

    	error = qcom_read_ota_area(offset, bytes_to_read, data, &ret_len);

        if (error) {
        	SWAT_PTF("OTA Read Error: %d\n",error);
        	break;
        }

        for (index = 0; index < ret_len; index++) {
            if ((index & 15) == 0) {
                SWAT_PTF("\n");
            }
            SWAT_PTF("%02x ", data[index]);
        }

        total_read += ret_len;
        offset += ret_len;
    }

    mem_free(data);
    return error;
}

A_INT32 swat_ota_done(A_INT32 argc,char *argv[])
{
	A_UINT32 resp;
#ifndef USE_QCOM_REL_3_3_API 
	A_UINT32 good_image;
	if (argc < 3) {
		SWAT_PTF("Invalid Params\nUsage: wmiconfig --ota_done <good_image>\n");
		return A_ERROR;
	}

	good_image = atoi(argv[2]);

	resp = qcom_ota_done(good_image ? 1 : 0);
#else
	resp = qcom_ota_done();
#endif
	if (resp == 1) {
		SWAT_PTF("OTA Success");
	} else {
		SWAT_PTF("OTA Err:%d\n", resp);
	}

	return resp;
}

#define SWAT_OTA_CUST

#ifdef SWAT_OTA_CUST
A_INT32 swat_ota_cust(A_INT32 argc,char *argv[])
{
   A_UINT32 len, i, offset;
   A_UINT32 cmd, rtn;
   if( argc < 3 )
   	return A_ERROR;
   cmd = atoi(argv[2]);

   switch(cmd) {
   	case 0:
		//ota session start
		{
                   A_UINT32 flags, partition_indx;
	           if (argc < 5) {
		         SWAT_PTF("Invalid Params\nUsage: wmiconfig --ota_cust 0 <flags> <partition_index>\n");
		         return A_ERROR;
	           }
	           flags = atoi(argv[3]);
	           partition_indx = atoi(argv[4]);
	           rtn = qcom_ota_session_start(flags, partition_indx);
		}
		break;		
  	case 1:
		//ota partition get size
              rtn =  qcom_ota_partition_get_size();
		break;
  	case 2:
		//ota partition erase
		rtn = qcom_ota_partition_erase();
		break;
  	case 3:
		//ota partion parse image hdr
             {
                  #define HDR_LEN 24
                  A_UINT8 buf[HDR_LEN];
                  A_UINT32 data0;				  
		    if( (argc < 4) || (strlen(argv[3]) != HDR_LEN*2) ) {
		         SWAT_PTF("Invalid Params\nUsage: wmiconfig --ota_cust 3 <xx...xx>\n");
		         return A_ERROR;
	           }			
		    for(i = 0; i < HDR_LEN;i++ ) {
		   	 A_SSCANF(&argv[3][i*2], "%2x", &data0);
			 buf[i] = (A_UINT8) data0;
		    }	 
                  rtn = qcom_ota_parse_image_hdr(buf, &offset);
                  SWAT_PTF("offset: %d\n", offset);
  		}
	  	break;
  	case 4:
		//ota partition write data
		{
                   #define BUF_LEN 50
                   A_UINT8 buf[BUF_LEN];
                   A_UINT32 data0;				   

		     if(argc < 6)
                   {
                         SWAT_PTF(" Incomplete Params\nUsage: wmiconfig --ota_cust 4 <offset> <len> <xx...xx>\n");
                         return A_ERROR;
                    }

                    offset = atoi(argv[3]);
                    len = atoi(argv[4]);
			if( len > BUF_LEN) {
    	                  SWAT_PTF("data must be less than 50 bytes here \n");
    	                  return A_ERROR;
			}
			if( strlen(argv[5])  != len*2) {
    	                  SWAT_PTF("data format is not correct \n");
    	                  return A_ERROR;
			}

		      for(i = 0; i < len;i++ ) {
		   	   A_SSCANF(&argv[5][i*2], "%2x", &data0);
			   buf[i] = (A_UINT8) data0;
		     }
		     qcom_ota_partition_write_data(offset, buf, len, &rtn);
		}
		break;
  	case 5:
		//ota partition verify checksum
		rtn = qcom_ota_partition_verify_checksum();
                if (rtn != QCOM_OTA_OK)
                {
                  SWAT_PTF("Checksum verification failed:%d\n", rtn);
                  return A_ERROR;
                }
		break;
  	case 6:
		//ota read
	{
             A_ULONG offset;
             A_ULONG len, total_read = 0;
            A_UCHAR *data;
            A_INT32 error = A_OK;
            A_UINT32 ret_len=0;

            if(argc < 5)
            {
                SWAT_PTF(" Incomplete Params\nUsage: wmiconfig --ota_custom 6 <offset> <size>!\n");
                SWAT_PTF("e.g wmiconfig --ota_cust 6 100 20 to read 20 bytes from offset 100 w.r.t start of the image!\n");
                return A_ERROR;
            }

            data = mem_alloc(OTA_READ_BUF_SIZE);
            if (!data) {
            	SWAT_PTF("Insufficient memory\n");
            	return A_ERROR;
            }

            offset = atoi(argv[3]);
            len = atoi(argv[4]);

            SWAT_PTF("Partition data:\n");

            while (total_read < len) {
                A_UINT32 index;
                A_UINT32 bytes_to_read;

                if (len - total_read > OTA_READ_BUF_SIZE) {
                	bytes_to_read = OTA_READ_BUF_SIZE;
                } else {
                	bytes_to_read = len - total_read;
                }

                 if ( -1 == (ret_len = qcom_ota_partition_read_data(offset, data, bytes_to_read)) ) {
                 	 SWAT_PTF("OTA Read Error\n");
		 	 error = A_ERROR;		
                      break;
                 }

                 for (index = 0; index < ret_len; index++) {
                     if ((index & 15) == 0) {
                         SWAT_PTF("\n");
                     }
                     SWAT_PTF("%02x ", data[index]);
                 }
        
                 total_read += ret_len;
                 offset += ret_len;
             }

             mem_free(data);
             return A_OK;	
	}
		break;
		
  	case 7:
		//ota session end
		{
                   A_UINT32 good_image;
	           if (argc < 4) {
		         SWAT_PTF("Invalid Params\nUsage: wmiconfig --ota_cust 7 <good_image>\n");
		         return A_ERROR;
	           }
	           good_image = atoi(argv[3]);
	           rtn = qcom_ota_session_end(good_image ? 1 : 0);
		}
		break;
		
   	}

       SWAT_PTF("%d\n", rtn);
	return A_OK;
}

A_INT32 swat_ota_format(A_INT32 argc,char *argv[])
{
    A_STATUS rtn;
    A_UINT32 indx;

    if (argc < 3) {
         SWAT_PTF("Invalid Params\nUsage: wmiconfig --ota_format <partition_index>\n");
	  return A_ERROR;
    }

    indx = atoi(argv[2]); 
    rtn = qcom_ota_partition_format(indx);
    if (rtn == QCOM_OTA_OK) {
        SWAT_PTF("OTA Success");
    } else {
        SWAT_PTF("OTA Err:%d\n", rtn);
    }

    return A_OK;
}

/*****************************************************************************************************************************/
/*                                                                                                                           */
/*       OTA_FTP                                                                                                             */
/*                                                                                                                           */
/*****************************************************************************************************************************/
#define OTA_FTP_DATA_PORT                        1068
#define OTA_FTP_BUF_SIZE                           1024
#define OTA_FTP_INCOMING_CONNECT_TIMEOUT         20

/*
 * OTA FTP Status codes
 */
typedef enum {
        QCOM_OTA_ERR_FTP_CONNECT_FAIL = 2000,
        QCOM_OTA_ERR_FTP_BIND_FAIL,
        QCOM_OTA_ERR_FTP_PEER_CLOSED,
        QCOM_OTA_ERR_FTP_SEND_COMMAND,
        QCOM_OTA_ERR_FTP_LISTEN_DATA_PORT,
        QCOM_OTA_ERR_FTP_DATA_CONNECTION_TIMEOUT,
        QCOM_OTA_ERR_FTP_ACCEPT_DATA_CONNECT,
        QCOM_OTA_ERR_FTP_FILE_NOT_COMPLETE,
        QCOM_OTA_ERR_FTP_SYST,
        QCOM_OTA_ERR_FTP_LOGIN_INCORRECT,
        QCOM_OTA_ERR_FTP_SET_TYPE,
        QCOM_OTA_ERR_FTP_SET_PORT,
        QCOM_OTA_ERR_FTP_FILE_NOT_FOUND,
       
} QCOM_OTA_FTP_STATUS_CODE_t;


//#define DEBUG_OTA_FTP_PRINTF 
#if defined(DEBUG_OTA_FTP_PRINTF)
#define OTA_FTP_D_PRINTF(args...) printf(args)
#else
#define OTA_FTP_D_PRINTF(args...)
#endif

           
typedef struct {
    A_UINT8  device_id;
    A_UINT32 data_sock;                /* data socket.*/
    A_UINT32 peer_sock; 
    A_UINT32 control_sock;             /* control socket.*/    
    A_UINT32 remote_ip_address;
    A_UINT32 local_ip_address;  
    A_UINT16 cmd_port;
    A_UINT16 data_port;
    A_UINT32 flags;
    A_UINT32 partition_index;
     A_UINT32 total_write;
} QCOM_OTA_FTP_SESSION_INFO;

A_INT32 qcom_ota_ftp_upgrade(A_UINT8 device_id, A_UINT32 ip_addr, A_UINT16 port, char *user_name, char *password, char *filename, A_UINT32 flags, A_UINT32 partition_index, A_UINT32 *size);
A_INT32 ota_ftp_init(A_UINT8 device_id);
A_INT32 ota_ftp_finit();
A_INT32 ota_ftp_open_control_sock(A_UINT32 ip_address, A_UINT16 port);
A_INT32 ota_ftp_close_control_sock();
A_INT32 ota_ftp_open_data_sock(A_UINT32 ip_address, A_UINT16 port);
A_INT32 ota_ftp_close_data_sock();
A_INT32 ota_ftp_send_cmd(char *cmd);
A_INT32 ota_ftp_recv_cmd(struct timeval tmo, int *resp_code);
A_INT32 ota_ftp_send_cmd_resp(char *cmd, int *resp_code);
A_INT32 ota_ftp_program_ota(A_UINT32 *size);

/* OTA FTP Global */
QCOM_OTA_FTP_SESSION_INFO *ota_ftp_sess = NULL;
A_UINT32 ota_cmd, ota_resp, ota_result;
static A_UINT16 ota_data_port= OTA_FTP_DATA_PORT;

A_UINT16 ota_get_data_port()
{
    ota_data_port++;
    if( ota_data_port >= 0x7fff )
	ota_data_port= OTA_FTP_DATA_PORT;	
    	
    return ota_data_port;
}

A_INT32 ota_ftp_init(A_UINT8 device_id)
{
     A_UINT32 addr = 0, mask = 0, gw = 0;

    OTA_FTP_D_PRINTF("FTP Init....\n");
    
    ota_ftp_sess = qcom_mem_alloc(sizeof(QCOM_OTA_FTP_SESSION_INFO));
    if (!ota_ftp_sess) {
	return A_ERROR;
    }
    memset(ota_ftp_sess, '\0', sizeof(QCOM_OTA_FTP_SESSION_INFO));

    ota_ftp_sess->device_id = device_id;
    ota_ftp_sess->control_sock = A_ERROR;
    ota_ftp_sess->peer_sock = A_ERROR;
    ota_ftp_sess->data_sock = A_ERROR;
	
    //get local ip address
    qcom_ipconfig(device_id, IP_CONFIG_QUERY, &addr, &mask, &gw);
    
    ota_ftp_sess->local_ip_address = addr;
    ota_ftp_sess->data_port = ota_get_data_port();
    
    if((ota_ftp_sess->control_sock = qcom_socket(AF_INET, SOCK_STREAM, 0)) == A_ERROR)
    {
      ota_ftp_sess->control_sock = A_ERROR;
	printf("ERROR: Unable to create ftp control socket\n");
	goto error_1;
    }

    if((ota_ftp_sess->data_sock = qcom_socket(AF_INET, SOCK_STREAM, 0)) == A_ERROR)
    {
       ota_ftp_sess->data_sock = A_ERROR;
	printf("ERROR: Unable to create ftp data socket\n");
	goto error_1;
    }

    return QCOM_OTA_OK;
error_1:
    ota_ftp_finit();
    return A_ERROR; 
}


A_INT32 ota_ftp_finit()
{
    ota_ftp_close_data_sock();
    ota_ftp_close_control_sock();

    /* Clean up and free all resources */
    if(ota_ftp_sess != NULL) {
        qcom_mem_free(ota_ftp_sess);
	ota_ftp_sess = NULL;
    }

    return QCOM_OTA_OK;  
}


A_INT32 ota_ftp_close_data_sock()
{
    if( ota_ftp_sess->data_sock != A_ERROR ) {
        qcom_socket_close(ota_ftp_sess->data_sock);
        ota_ftp_sess->data_sock = A_ERROR;
    }
    return QCOM_OTA_OK;
}

A_INT32 ota_ftp_open_data_sock(A_UINT32 ip_addr, A_UINT16 port)
{
    struct sockaddr_in local_addr;
    A_INT32 rtn = QCOM_OTA_OK;

    memset(&local_addr, 0, sizeof(local_addr));
    local_addr.sin_addr.s_addr =  htonl(ip_addr);
    local_addr.sin_port =  htons(port);
    local_addr.sin_family = AF_INET;

    OTA_FTP_D_PRINTF("connect ftp data port ip:%xh port:%d\n", ip_addr, port);
    
    /* bind to local port.*/
    if(qcom_bind( ota_ftp_sess->data_sock, (struct sockaddr*)(&local_addr), sizeof(local_addr)) == A_ERROR)
    {
	//printf("socket bind failed.\n");
	rtn = QCOM_OTA_ERR_FTP_BIND_FAIL;
    }
    
    return rtn;     
}

A_INT32 ota_ftp_close_control_sock()
{
    OTA_FTP_D_PRINTF("close ftp control connection\n");
  
    if( ota_ftp_sess->control_sock != A_ERROR ) {
        qcom_socket_close(ota_ftp_sess->control_sock);
        ota_ftp_sess->control_sock = A_ERROR;
    }
    return QCOM_OTA_OK;
}

A_INT32 ota_ftp_open_control_sock(A_UINT32 ip_addr, A_UINT16 port)
{
    struct sockaddr_in foreign_addr;
    A_INT32 rtn = QCOM_OTA_OK;

    OTA_FTP_D_PRINTF("FTP Connecting ip:%xh port:%d\n", ip_addr, port);
    
    ota_ftp_sess->remote_ip_address = ip_addr;
    ota_ftp_sess->cmd_port = port;

    memset(&foreign_addr, 0, sizeof(foreign_addr));
    foreign_addr.sin_addr.s_addr = htonl(ip_addr);
    foreign_addr.sin_port = htons(port);
    foreign_addr.sin_family = AF_INET;
			
    /* Connect to the server.*/
    if(qcom_connect( ota_ftp_sess->control_sock, (struct sockaddr*)(&foreign_addr), sizeof(foreign_addr)) == A_ERROR)
    {
	printf("Connection failed.\n");
	rtn = QCOM_OTA_ERR_FTP_CONNECT_FAIL;
    }
    
    return rtn;    
}

/* return: send length */
int ota_ftp_send_cmd(char *cmd)
{
    OTA_FTP_D_PRINTF("send: %s", cmd);
    if ( ota_ftp_sess->control_sock != A_ERROR)	
        return (qcom_send(ota_ftp_sess->control_sock, cmd, strlen(cmd),0));  
    return A_ERROR;
}

A_INT32 ota_ftp_recv_cmd(struct timeval to, int *resp_code)
{
#define  FTP_CMD_BUF_LEN 256
    struct timeval tmo;
    q_fd_set sockSet,master;	
    A_INT32 conn_sock, received = 0;
    A_INT32 rtn = QCOM_OTA_OK;
    char buffer[FTP_CMD_BUF_LEN];
	
    *resp_code = 0;

    FD_ZERO(&master);
    FD_SET(ota_ftp_sess->control_sock, &master);
    if (FD_ISSET(ota_ftp_sess->control_sock, &master) == 0)
        return QCOM_OTA_ERR_FTP_PEER_CLOSED;

    tmo.tv_sec = to.tv_sec;
    tmo.tv_usec =to.tv_usec;

    while(1) /* Receiving data.*/
    {
        sockSet = master;        /* Wait for Input */
        conn_sock = qcom_select(ota_ftp_sess->control_sock,&sockSet, NULL, NULL, &tmo);
        if(conn_sock !=0)
        {
            if(FD_ISSET( ota_ftp_sess->control_sock,&sockSet) )
            {
                /*Packet is available, receive it*/
                received = qcom_recv( ota_ftp_sess->control_sock, (char*)(&buffer[0]), FTP_CMD_BUF_LEN, 0);
                if( received <= 0 )
                {
                    /* Test ended, peer closed connection*/
                    rtn = QCOM_OTA_ERR_FTP_PEER_CLOSED;
                }
                break;
            	} else {
                    /* Test ended, peer closed connection*/
                    rtn = QCOM_OTA_ERR_FTP_PEER_CLOSED;
                    break;                 
            	}
        } else {
             /* Test ended, peer closed connection*/
            rtn = QCOM_OTA_ERR_FTP_PEER_CLOSED;
            break;
	     		
        }
    }

    /*Valid packet received*/
    if(rtn == QCOM_OTA_OK)
    {
        buffer[received] = '\0';
        OTA_FTP_D_PRINTF("recv: %s", buffer);
    
        buffer[3] = '\0';
        *resp_code = atoi(buffer);
    }
    
    return rtn; 
}

A_INT32 ota_ftp_send_cmd_resp(char *cmd, int *resp_code)
{
    struct timeval tmo;
    tmo.tv_sec = 1;
    tmo.tv_usec = 0;   	
    *resp_code = 0;
    if( ota_ftp_send_cmd(cmd) <= 0 )
      return QCOM_OTA_ERR_FTP_SEND_COMMAND;
    
    return(ota_ftp_recv_cmd(tmo, resp_code));
}


A_INT32 ota_ftp_program_ota(A_UINT32 *size)
{
    struct sockaddr_in foreign_addr;
    struct timeval tmo, to;
    q_fd_set sockSet,master;	
    A_INT32 conn_sock, received = 0;
    A_INT32 addr_len;
    A_UINT32 count, is_first, offset, img_offset, complete, len;
    A_UINT32 ret_size;
    A_INT32 rtn = QCOM_OTA_OK;
    int resp;
    char *buffer;
  
    *size = 0;
    /*Allocate buffer*/
    if((buffer = qcom_mem_alloc(OTA_FTP_BUF_SIZE)) == NULL)
    {
        OTA_FTP_D_PRINTF("Out of memory error\n");
        rtn = A_ERROR;
        goto error_1;
    }    

    FD_ZERO(&master);
    FD_SET(ota_ftp_sess->data_sock, &master);

    /* Listen. */
    OTA_FTP_D_PRINTF("Listening data socket\n");   
    if(qcom_listen( ota_ftp_sess->data_sock, 10) < 0)  
    {
       //chnage to non-blocking
       //printf("ERROR: Socket listen error.\r\n");
       // rtn = QCOM_OTA_ERR_FTP_LISTEN_DATA_PORT; 
       // goto error_1;
    }

    tmo.tv_sec = 10;
    tmo.tv_usec = 0;  
     /* block for 10sec or until a packet is received */
     sockSet = master;        /* Wait for Input */
     conn_sock = qcom_select(ota_ftp_sess->data_sock+1,&sockSet, NULL, NULL, &tmo);

    if( conn_sock <= 0 ) {	
        OTA_FTP_D_PRINTF("FTP incoming data connection timeout\n");   
        rtn = QCOM_OTA_ERR_FTP_DATA_CONNECTION_TIMEOUT;
        goto error_1;
    }

    /*Accept incoming connection*/
    OTA_FTP_D_PRINTF("Accept incoming data connection\n");   
    addr_len = sizeof(foreign_addr);
    if( (ota_ftp_sess->peer_sock = qcom_accept( ota_ftp_sess->data_sock, (struct sockaddr*)&foreign_addr, (int*)&addr_len)) < 0 )
    {
        rtn = QCOM_OTA_ERR_FTP_ACCEPT_DATA_CONNECT;
        goto error_2;
    }
    
    /* -----------------------------------------------------*/
    /* process FTP data                                   */
    /* -----------------------------------------------------*/
     printf("FTP Transfer File\n");	
	
    count = 0;    
    is_first = 1;
    img_offset = 0;
    complete = 0;
    ota_ftp_sess->total_write = 0;

    tmo.tv_sec = 1;
    tmo.tv_usec = 0;  

    FD_ZERO(&master);
    FD_SET(ota_ftp_sess->peer_sock, &master);

   if(FD_ISSET(ota_ftp_sess->peer_sock, &master)== 0)
   {
        rtn = QCOM_OTA_ERR_FTP_ACCEPT_DATA_CONNECT;
        goto error_2;
   }

    while( complete != 1 ) 
    {
        /*-----------------------------------------------------------*/
        /* Receiving data from FTP server.*/
        /* Copy the master list everytime before calling select() because select
         * modifies sockSet */
        sockSet = master;		
        conn_sock = qcom_select(ota_ftp_sess->peer_sock+1, &sockSet, NULL, NULL, &tmo);
        if(conn_sock != 0)
        {
            if (FD_ISSET(ota_ftp_sess->peer_sock, &sockSet)) {
           
                /*Packet is available, receive it*/
                received = qcom_recv( ota_ftp_sess->peer_sock, buffer, OTA_FTP_BUF_SIZE, 0);
                OTA_FTP_D_PRINTF("recv %dB\n", received);   

                if( received <=  0 )
                {
                    /* peer closed connection*/
                    to.tv_sec = 1;
                    to.tv_usec = 0;    
                    ota_ftp_recv_cmd(to, &resp);

                    if( resp != 0 ) {
                        //we have received the whole file
                        complete = 1;
                        if( resp == 226 )     //recv 226 Transfer complete
                       {
                            rtn = QCOM_OTA_OK;
                            OTA_FTP_D_PRINTF("Transfer Complete received\n");
                        } else {
                            //get error
                            OTA_FTP_D_PRINTF("Transfer not Complete:%d\n", resp);
                            rtn = QCOM_OTA_ERR_FTP_FILE_NOT_COMPLETE;
                       }
                    } else {
                        rtn = QCOM_OTA_ERR_FTP_PEER_CLOSED;
                    }
                    goto error_2;
                }

                /* handle data */
                offset = 0;
                len = received;
        
                if(is_first == 1) 
                {
                    //need parse image hdr
                    is_first = 0;   
                    if( (rtn = qcom_ota_parse_image_hdr((A_UINT8 *)buffer,&offset)) != QCOM_OTA_OK ) 
                    {
                        OTA_FTP_D_PRINTF("OTA Parse Image Hdr Error\n");
                        rtn = QCOM_OTA_ERR_IMAGE_HDR_INCORRECT;
                        goto error_2;  
                    }
                    OTA_FTP_D_PRINTF("OTA Parse Image Hdr: %d\n", offset);
                
                    len -=offset;
                }
            
                while(1)
                {
                     if((rtn = qcom_ota_partition_write_data(img_offset, (A_UINT8 *)&buffer[offset],len, &ret_size)) != QCOM_OTA_OK )
                     {
                         goto error_2;                    
                     }

			printf(".");
			
                     offset+=ret_size;
                     img_offset+=ret_size;
                     len -=ret_size;
                     if(len == 0)
                         break;
                }
                ota_ftp_sess->total_write += received;
            }else {
                    /* peer closed connection*/
                    rtn = QCOM_OTA_ERR_FTP_PEER_CLOSED;
                    goto error_2;
            }
        }        else  {            // receiving command
            to.tv_sec = 0;
            to.tv_usec = 500;    

            ota_ftp_recv_cmd(to, &resp);

            if( resp != 0 ) {
                //we have received the whole file
                complete = 1;
                if( resp == 226 )     //recv 226 Transfer complete
                {
                    rtn = QCOM_OTA_OK;
                    OTA_FTP_D_PRINTF("Transfer Complete received\n");
                    
                } else {
                    //get error
                    OTA_FTP_D_PRINTF("Transfer not Complete:%d\n", resp);
                    rtn = QCOM_OTA_ERR_FTP_FILE_NOT_COMPLETE;
                }
            }
        } 
    } /* while(  */
error_2:
    printf("\nTotal write: %d, rtn:%d\n", ota_ftp_sess->total_write, rtn);  
    if( ota_ftp_sess->peer_sock != A_ERROR ) {
        qcom_socket_close(ota_ftp_sess->peer_sock);
        ota_ftp_sess->peer_sock = A_ERROR;
    }  
    ota_ftp_close_data_sock();   
    *size = ota_ftp_sess->total_write;
error_1:
    if(buffer)
    	qcom_mem_free(buffer);
    return rtn; 
}

A_INT32 qcom_ota_ftp_upgrade(A_UINT8 device_id, A_UINT32 ip_addr, A_UINT16 port, char *user_name, char *password, char *filename, A_UINT32 flags, A_UINT32 partition_index, A_UINT32 *size)
{
   struct timeval tmo;
    A_INT32 rtn = A_OK;
    int resp;
    A_UINT32 partition_size, good_image = 0;
    char buf[256];
    
    OTA_FTP_D_PRINTF("Enter qcom_ota_ftp_upgrade\n");
    OTA_FTP_D_PRINTF("ota_ftp_update: ip:%xH, port:%d\n", ip_addr, port);
    OTA_FTP_D_PRINTF("                User:%s, Password:%s\n", user_name, password);
    OTA_FTP_D_PRINTF("                file:%s\n", filename);
    OTA_FTP_D_PRINTF("                flags:%d, partition_index:%d \n", flags, partition_index);   

    *size = 0;
    tmo.tv_sec = 2;
    tmo.tv_usec = 0;
	
    if( (flags & QCOM_OTA_TARGET_FIRMWARE_UPGRADE) == 0)
    {
       A_PRINTF("OTA FTP supports Target Firmware Upgrade only, check flags setting\n");
       return QCOM_OTA_ERR_UNSUPPORT_PROTOCOL; 
    }
    if( (rtn = qcom_ota_session_start(flags, partition_index)) != QCOM_OTA_OK )
    {
       OTA_FTP_D_PRINTF("OTA Session Start Fail\n");
       return rtn;
    }
    if( (rtn = qcom_ota_partition_erase()) != QCOM_OTA_OK )
    {
       OTA_FTP_D_PRINTF("OTA Partition Erase Fail\n");
       return rtn;
    }

    /* check partition size */
    partition_size = qcom_ota_partition_get_size();
    OTA_FTP_D_PRINTF("OTA Partition Get Size: %d\n",partition_size);
    if( partition_size == 0) {
        rtn = A_ERROR;
        return rtn;
    }	

    if( ota_ftp_init(device_id) != QCOM_OTA_OK )
    {
        OTA_FTP_D_PRINTF("Error : ota_ftp_init error\n");
	goto error_1;
    }

     printf("\nConnect to FTP Server...\n");
     //open control sock
    if( (rtn = ota_ftp_open_control_sock(ip_addr, port)) != QCOM_OTA_OK )
      goto error_1;
    
    //recev welcome
    if( (rtn = ota_ftp_recv_cmd(tmo, &resp)) != QCOM_OTA_OK )
    {
       goto error_1;
    }
    if( resp != 220 )
    {
       printf("FTP Welcome error: %d\n",resp);
       goto error_1;
    }
         
    //send user
    A_MEMSET(buf, 0, sizeof(buf));
    qcom_sprintf(buf, "USER %s\r\n", user_name);
    if( (rtn= ota_ftp_send_cmd_resp(buf,&resp)) != QCOM_OTA_OK )
    {
        goto error_1;
    }
    if( resp != 331 )
    {
        rtn = QCOM_OTA_ERR_FTP_LOGIN_INCORRECT;      
        printf("FTP USER error: %d",resp);
        goto error_1;
    }

    //send password
    A_MEMSET(buf, 0, sizeof(buf));
    qcom_sprintf(buf, "PASS %s\r\n", password);
    if( (rtn= ota_ftp_send_cmd_resp(buf,&resp)) != QCOM_OTA_OK )
    {
        goto error_1;
    }
    if( resp != 230 )
    {
       rtn = QCOM_OTA_ERR_FTP_LOGIN_INCORRECT;
       printf("FTP PASS error: %d\n",resp);
       goto error_1;
    }

    printf("FTP Login ...\n");

    //send syst
    if( (rtn= ota_ftp_send_cmd_resp("SYST\r\n",&resp)) != QCOM_OTA_OK )
    {
       goto error_1;
    }
    if( resp != 215 )
    {
       rtn = QCOM_OTA_ERR_FTP_SYST;
       printf("FTP SYST error: %d\n",resp);
       goto error_1;
    }
    
    //setup BINARY mode
    if( (rtn= ota_ftp_send_cmd_resp("TYPE I\r\n",&resp)) != QCOM_OTA_OK )
    {
       goto error_1;
    }
    if( resp != 200 )
    {
       rtn = QCOM_OTA_ERR_FTP_SET_TYPE;
       printf("FTP TYPE error: %d\n",resp);
       goto error_1;
    }
    
    //open data socket
   if( (rtn = ota_ftp_open_data_sock(ota_ftp_sess->local_ip_address, ota_ftp_sess->data_port)) != QCOM_OTA_OK ) 
           goto error_1;    

    //send port
    A_MEMSET(buf, 0, sizeof(buf));
    qcom_sprintf(buf, "PORT %d,%d,%d,%d,%d,%d\r\n", 
                                               (ota_ftp_sess->local_ip_address)>>24&0xff,
                                               (ota_ftp_sess->local_ip_address)>>16 &0xff,
                                               (ota_ftp_sess->local_ip_address)>>8 & 0xff, 
		                                 (ota_ftp_sess->local_ip_address)&0xff, 
                                               ota_ftp_sess->data_port/256,
                                               ota_ftp_sess->data_port%256);
    if( (rtn= ota_ftp_send_cmd_resp(buf,&resp)) != QCOM_OTA_OK )
    {
       goto error_1;
    }
    if( resp != 200 )
    {
       rtn = QCOM_OTA_ERR_FTP_SET_PORT;
       printf("FTP PORT error: %d\n",resp);
       goto error_1;
    }

    //send get file
    A_MEMSET(buf, 0, sizeof(buf));
    qcom_sprintf(buf, "RETR %s\r\n", filename);
    if( (rtn= ota_ftp_send_cmd_resp(buf,&resp)) != QCOM_OTA_OK )
    {
       goto error_1;
    }
    if( resp != 150 )
    {
       rtn = QCOM_OTA_ERR_FTP_FILE_NOT_FOUND;
       printf("FTP RETR error: %d\n",resp);
       goto error_1;
    }

	
   //get file and write ota
    rtn = ota_ftp_program_ota(size);
    if( rtn == QCOM_OTA_OK ) 
    {
        //we are done
        if( (rtn = qcom_ota_partition_verify_checksum()) == QCOM_OTA_OK )  
        {
            good_image = 1;        
            OTA_FTP_D_PRINTF("OTA Partition Verify Checksum is correct\n");
            
        } 
        else
        {
            OTA_FTP_D_PRINTF("OTA Partition Verify Checksum is NOT correct\n");
        }
    }

error_1:
    //send quit and recv goodbye
    ota_ftp_send_cmd_resp("QUIT\r\n", &resp);
    qcom_ota_session_end(good_image); 
    ota_ftp_finit(); 
    OTA_FTP_D_PRINTF("Leave qcom_ota_ftp_update %dl\n", rtn);
    return rtn;
}

A_INT32 swat_ota_ftp_upgrade(A_UINT8 device_id,A_INT32 argc, char *argv[])
{
    A_UINT32 ip_addr, flags, partition_index;
    A_UINT16 port;
    A_UINT32 resp_code;
    A_UINT32 size;    
 
    if(argc < 9)
    {
       printf("Incomplete params\n");
       printf("Usage: wmiconfig --ota_ftp_upgrade <ip> <port> <user name> <password> <filename> <flags> <partition_index> \n");
       return A_ERROR;
    }
    
    ip_addr = _inet_addr(argv[2]);    
    port=atoi(argv[3]);
    flags=atoi(argv[7]);
    partition_index=atoi(argv[8]);
 
    resp_code = qcom_ota_ftp_upgrade(device_id,ip_addr,port,argv[4],argv[5],argv[6],flags, partition_index, &size);
    if(A_OK != resp_code)
    {
        printf("OTA Image Download Failed ERR:%d\n",resp_code);
    } else {
        printf("OTA Image Download Completed successfully,Image Size:%d\n",size);
    }
    return A_OK;
}
  
#endif //define SWAT_OTA_CUST
#endif //define SWAT_OTA


void swat_wmiconfig_dhcps_success_cb(A_UINT8 *mac,A_UINT32 ipAddr){

    SWAT_PTF("DHCP Server issued Client MAC:%02x:%02x:%02x:%02x:%02x:%02x ,IP Addr:",mac[0],mac[1],mac[2],mac[3],mac[4],mac[5]);
    IPV4_ORG_PTF(ipAddr);
    SWAT_PTF("\n");

}


A_INT32 swat_wmiconfig_dhcps_cb_enable(A_UINT8 device_id,char * enable){

    if(!strcmp(enable, "start"))
    {
       qcom_dhcps_register_cb(device_id,swat_wmiconfig_dhcps_success_cb); 
    }
    else if(!strcmp(enable, "stop"))
    {
        qcom_dhcps_register_cb(device_id,NULL); 
    }
    else
     {
      SWAT_PTF("input parameter should be start or stop!\n");
      return A_ERROR;
     }

     return A_OK;
}

void swat_wmiconfig_dhcpc_success_cb(A_UINT32 ipAddr,A_UINT32 mask,A_UINT32 gw){

      SWAT_PTF("IP Addr:");
      IPV4_ORG_PTF(ipAddr);
      SWAT_PTF("\nMask :");
      IPV4_ORG_PTF(mask);
      SWAT_PTF("\nGW:");
      IPV4_ORG_PTF(gw);
      SWAT_PTF("\n");  
}

A_INT32 swat_wmiconfig_dhcpc_cb_enable(A_UINT8 device_id,char * enable){

      if(!strcmp(enable, "start"))
      {
           qcom_dhcpc_register_cb(device_id,swat_wmiconfig_dhcpc_success_cb); 
      }
      else if(!strcmp(enable, "stop"))
      {
           qcom_dhcpc_register_cb(device_id,NULL); 
      }
      else
      {
           SWAT_PTF("input parameter should be start or stop!\n");
          return A_ERROR;
      }
     return A_OK;
}
