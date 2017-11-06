/*
  * Copyright (c) 2015 Qualcomm Atheros, Inc..
  * All Rights Reserved.
  * Qualcomm Atheros Confidential and Proprietary.
  */

#include "qcom_common.h"
//#include "swat_wmiconfig_common.h"

#include "swat_parse.h"
#include "swat_wmiconfig_misc.h"
#include "swat_wmiconfig_network.h"
//#include "swat_wmiconfig_pwm.h"
//#include "swat_wmiconfig_security.h"
#include "swat_wmiconfig_wlan.h"


#include "swat_parse.h"
#include "qcom/socket_api.h"
#include "swat_bench_core.h"
#include "swat_bench_test.h"
//#include "swat_wmiconfig_p2p.h"
#include "qcom/qcom_ssl.h"
#include "qcom/qcom_event.h"

extern int atoul(char *buf);
extern SSL_ROLE_T ssl_role;
A_UINT8 currentDeviceId = 0;


A_INT32
swat_wmiconfig_connect_handle(A_INT32 argc, A_CHAR * argv[])
{
    if (!A_STRCMP(argv[1], "--connect")) {
        if (argc == 3) {
            swat_wmiconfig_connect_ssid(currentDeviceId, (char *)argv[2]);
        } else {
            A_INT8 ssid[32];
            A_UINT8 i, j;

            j = 0;
            A_MEMSET(ssid, 0, 32);
            for (i=2; i<argc; i++)
            {
                if ((j + strlen(argv[i])) > 32)
                {
                    SWAT_PTF("ERROR: ssid > 32\n");
                    return SWAT_ERROR;
                }
                else
                {
                    do
                    {
                        ssid[j] = *argv[i];
                        j++;
                    } while (*argv[i]++);
                }

				ssid[j-1] = 0x20; //space
            }
			ssid[j-1] = 0;

            swat_wmiconfig_connect_ssid(currentDeviceId, (char *) ssid);
        }

        return SWAT_OK;
    }

	if (!A_STRCMP(argv[1], "--connect2"))
		{
		swat_wmiconfig_connect_ssid_2(currentDeviceId, (char *)argv[2]);
		return SWAT_OK;
		}

	if (!A_STRCMP(argv[1], "--adhoc"))
		{

        SWAT_PTF("Disabling Power Save\n");

        qcom_power_set_mode(0, MAX_PERF_POWER);

		swat_wmiconfig_connect_adhoc(currentDeviceId, (char *)argv[2]);

		return SWAT_OK;
		}

    if (!A_STRCMP(argv[1], "--disc")) {
        swat_wmiconfig_connect_disc(currentDeviceId);
        return SWAT_OK;

    }

	 if (!A_STRCMP(argv[1], "--stacountry")) {
	 	A_CHAR country_code[3];
        if (3 == argc){
            country_code[0] = argv[2][0];
            country_code[1] = argv[2][1];
            country_code[2] = 0x20;

            /* Removed country code QAPI. However, retain this application
             * API so as to not break CST scripts/tests
             */
            swat_wmiconfig_sta_country_code_set(currentDeviceId, country_code);
        }else{
            SWAT_PTF("Invalid paramter format\n");
            return SWAT_ERROR;
        }
        return SWAT_OK;

    }

    return SWAT_NOFOUND;
}

#if 0

A_INT32
swat_wmiconfig_wep_handle(A_INT32 argc, A_CHAR * argv[])
{

    A_UINT32 key_index;
	A_UINT32 mode;

    if (!A_STRCMP(argv[1], "--wepkey")) {

        if (argc == 4) {
            A_SSCANF(argv[2], "%d", &key_index);

            if ((key_index > 4) || (key_index < 1)) {
                SWAT_PTF("key_index: 1-4 \n");
                return SWAT_ERROR;
            }

            swat_wmiconfig_wep_key_set(currentDeviceId, argv[3], (A_CHAR) key_index);
        } else {
            SWAT_PTF("	  --wepkey <key_index> <key>\n");
            return SWAT_ERROR;
        }

        return SWAT_OK;
    }

    if (!A_STRCMP(argv[1], "--wep")) {

        if (argc == 4) {
            A_SSCANF(argv[2], "%d", &key_index);
            if ((key_index > 4) || (key_index < 1)) {
                SWAT_PTF("key_index: 1-4 \n");
                return SWAT_ERROR;
            }
			if (!A_STRCMP(argv[3], "open"))
			{
			  mode = 0;
			}
			else if (!A_STRCMP(argv[3], "shared"))
			{
			  mode = 1;
			}
            	       else if (!A_STRCMP(argv[3], "auto"))
			{
			  mode = 2;
			}
			else
			{
			  SWAT_PTF("<open> or <shared> \n");
			  return SWAT_ERROR;
			}
            /* support open mode now*/
            swat_wmiconfig_wep_key_index_set(currentDeviceId, key_index, mode);
        }else if (argc == 3){
        	A_SSCANF(argv[2], "%d", &key_index);
            if ((key_index > 4) || (key_index < 1)) {
                SWAT_PTF("key_index: 1-4 \n");
                return SWAT_ERROR;
            }
			swat_wmiconfig_wep_key_index_set(currentDeviceId, key_index, 2);
    	}
		else {
            SWAT_PTF("	  --wep <def_keyix> <mode>\n");
            return SWAT_ERROR;
        }
        return SWAT_OK;
    }

		return SWAT_NOFOUND;

}
#endif

#if 0

A_INT32
swat_wmiconfig_wpa_handle(A_INT32 argc, A_CHAR * argv[])
{
    if (!A_STRCMP(argv[1], "--p")) {
        if (3 == argc) {
            swat_wmiconfig_wep_passowrd_set(currentDeviceId, argv[2]);
        } else {
            SWAT_PTF("	  wmiconfig --p <passphrase>\n");
            return SWAT_ERROR;
        }

        return SWAT_OK;
    }
#if !defined(REV74_TEST_ENV2)
    if (!A_STRCMP(argv[1], "--wpa")) {
        if (argc == 5) {
            swat_wmiconfig_wpa_set(currentDeviceId, argv[2], argv[3], argv[4]);
        } else {
            SWAT_PTF("	  --wpa <ver> <ucipher> <mcipher>\n");
            return SWAT_ERROR;
        }
        return SWAT_OK;
    }
#endif
    return SWAT_NOFOUND;
}
#endif


//#if defined(SWAT_BENCH)

A_INT32
swat_benchtx_handle(A_INT32 argc, A_CHAR * argv[])
{
    A_INT32 ret = 0;
    A_UINT32 aidata[4];
    A_UINT32 ipAddress = 0;
    A_UINT32 local_ipAddress = 0;
    A_UINT32 port = 0;
    A_UINT32 pktSize = 0;
    A_UINT32 seconds = 0;
    A_UINT32 numOfPkts = 0;
    A_UINT32 mode = 0;
    A_UINT32 protocol = 0;
    A_UINT32 delay = 0;
    A_UINT32 ip_hdr_incl = 0;
    IP6_ADDR_T ip6Address;

    if (((void *) 0 == argv) || (argc < 8)) {
        goto ERROR;
    }

    A_MEMSET(&ip6Address,0,sizeof(IP6_ADDR_T));
    /* IP Address */
    if(v6_enabled){
        Inet6Pton(argv[1], &ip6Address);
    }else{
    ret = swat_sscanf(argv[1], "%3d.%3d.%3d.%3d", &aidata[0], &aidata[1], &aidata[2], &aidata[3]);
    if (ret < 0) {
       goto ERROR;
    }

    }
    ipAddress = (aidata[0] << 24) | (aidata[1] << 16) | (aidata[2] << 8) | aidata[3];

    /* Port */
    port = swat_atoi(argv[2]);

    /* TCP or UDP or raw */
    if ((0 != swat_strcmp(argv[3], "tcp"))
        && (0 != swat_strcmp(argv[3], "udp")) && (0 != swat_strcmp(argv[3],"raw"))
        &&(0 != swat_strcmp(argv[3], "ssl")))
    {
       goto ERROR;
    }
    ret = swat_strcmp(argv[3], "tcp");
    if (0 == ret) {
        protocol = 0;
    }
    ret = swat_strcmp(argv[3], "udp");
    if (0 == ret) {
        protocol = 1;
    }
#ifdef SWAT_BENCH_RAW
    ret = swat_strcmp(argv[3], "raw");
    if (0 == ret)
    {
        if(argc >= 9){
            /* IP Address */
            ret = swat_sscanf(argv[8], "%3d.%3d.%3d.%3d", &aidata[0], &aidata[1], &aidata[2], &aidata[3]);
            if (ret < 0)
            {
                goto ERROR;
            }
            local_ipAddress = (aidata[0] << 24) | (aidata[1] << 16) | (aidata[2] << 8) | aidata[3];
        }
        protocol = 2;

        if((port == 6) || (port == 17) || (port < 0) || (port > 255))
        {
           goto ERROR;
        }

    }

    if(argc == 10)
    {
    ret = swat_strcmp(argv[9],"ip_hdr_inc");
    if(0 == ret)
        {
               ip_hdr_incl = 1;
        }
    else{
               goto ERROR;
        }
    }
#endif /*SWAT_BENCH_RAW*/
#ifdef SWAT_SSL
    if(swat_strcmp(argv[3], "ssl") == 0){

    if (ssl_role)
    {
        printf("ERROR: busy.\n");
        goto ERROR;
    }
    if (ssl_inst[SSL_CLIENT_INST].sslCtx == NULL || ssl_inst[SSL_CLIENT_INST].role != SSL_CLIENT)
    {
       printf("ERROR: SSL client not stated (Use 'wmiconfig --ssl_start client' first).\n");
       goto ERROR;
    }
    ssl_role = SSL_CLIENT;
    protocol = TEST_PROT_SSL;
    }
#endif

    /* Packet Size */
    pktSize = swat_atoi(argv[4]);
    if ((0 == pktSize) || (pktSize > 1400)) {
       goto ERROR;
    }

    /* Test Mode */
    mode = swat_atoi(argv[5]);
    if ((0 != mode)
        && (1 != mode)) {
        goto ERROR;
    }
    if (0 == mode) {
        seconds = swat_atoi(argv[6]);
        if (0 == seconds) {
            goto ERROR;
        }
    }
    if (1 == mode) {
        numOfPkts = swat_atoi(argv[6]);
        if (0 == numOfPkts) {
            goto ERROR;
        }
    }

    /* Do nothing */
    flag_new_console_task = 1;
    delay = swat_atoi(argv[7]);
    swat_bench_tx(ipAddress, ip6Address, port, protocol, pktSize, mode, seconds, numOfPkts, local_ipAddress, ip_hdr_incl, delay);

    return 0;
ERROR:
        SWAT_PTF("Invalid/missing param\n");
        SWAT_PTF("benchtx <Tx IP> <port> <protocol tcp/udp> <size> <test mode> <packets|time> <delay>\r\n");
        SWAT_PTF("Ex: benchtx 192.168.1.100 6000 tcp 64 1 1000 0\r\n");
        SWAT_PTF("Parameters:\r\n");
        SWAT_PTF("1 <Tx IP>       : IP address (v4 or v6)\r\n");
        SWAT_PTF("2 <port>        : Listening Port for tcp/udp\r\n");
        SWAT_PTF("3 <protocol>    : tcp/udp/ssl\r\n");
        SWAT_PTF("4 <size>        : Packet size in bytes\r\n");
        SWAT_PTF("5 <test mode>   : 0:Time 1:Packets\r\n");
        SWAT_PTF("6 <packets|time>: Seconds or Packets number\r\n");
        SWAT_PTF("7 <delay>       : Always 0\r\n");
#ifdef SWAT_BENCH_RAW
        SWAT_PTF("benchtx <Tx IP> <prot> <raw> <msg size> <test mode> <number of packets | time (sec)> <delay in msec> <local IP> [ip_hdr_inc*]\r\n");
#endif
        return -1;

}

A_INT32
swat_benchrx_handle(A_INT32 argc, A_CHAR * argv[])
{
    A_INT32 ret = 0;
    A_UINT32 aidata[4];
    A_UINT32 ipAddress = 0;
    A_UINT32 localIp = 0;
    A_UINT32 mcastIp = 0;
    A_UINT32 port = 0;
    A_UINT32 protocol = 0;
    A_UINT32 ip_hdr_inc = 0;
    A_UINT32 rawmode = 0;
    IP6_ADDR_T localIp6;
    IP6_ADDR_T mcastIp6;
    A_UINT8 mcastEnabled = 0;

    if (((void *) 0 == argv) || (argc > 5) || (argc < 3)) {
        goto ERROR;
    }

    A_MEMSET(&localIp6,0,sizeof(IP6_ADDR_T));
    A_MEMSET(&mcastIp6,0,sizeof(IP6_ADDR_T));

    /* TCP or UDP */
    if ((0 != swat_strcmp(argv[1], "tcp"))
        && (0 != swat_strcmp(argv[1], "udp"))
        && (0 != swat_strcmp(argv[1], "raw"))
        && (0 != swat_strcmp(argv[1], "ssl")))
    {
        goto ERROR;
    }
    ret = swat_strcmp(argv[1], "tcp");
    if (0 == ret) {
        protocol = 0;
    }
    ret = swat_strcmp(argv[1], "udp");
    if (0 == ret) {
        protocol = 1;
    }

#ifdef SWAT_BENCH_RAW

    ret = swat_strcmp(argv[1], "raw");
    if (0 == ret)
    {
        protocol = 2;
        ret = swat_strcmp(argv[2],"eapol");
        if(ret == 0)
                rawmode = ETH_RAW;
        else
                rawmode = IP_RAW;
    }
#endif
#ifdef SWAT_SSL
    if(swat_strcmp("ssl", argv[1]) == 0)
    {
        if (ssl_role)
        {
            printf("ERROR: busy.\n");
            goto ERROR;
        }
        if (ssl_inst[SSL_SERVER_INST].sslCtx == NULL || ssl_inst[SSL_SERVER_INST].role != SSL_SERVER)
        {
           printf("ERROR: SSL server not stated (Use 'wmiconfig --ssl_start server' first).\n");
           goto ERROR;
        }
        protocol = TEST_PROT_SSL;
        ssl_role = SSL_SERVER;
    }
#endif
    /* Port */
    port = swat_atoi(argv[2]);
    if(rawmode == ETH_RAW)
    {
        port = ATH_ETH_P_PAE;
    }

    /* IP Check */
    if (4 == argc) {
        if(strcasecmp("ip_hdr_inc",argv[3]) == 0)
		{
		    ip_hdr_inc = 1;
		}else{
            ret = swat_sscanf(argv[3], "%3d.%3d.%3d.%3d", &aidata[0], &aidata[1], &aidata[2], &aidata[3]);
            if (ret < 0) {
                goto ERROR;
            }
            ipAddress = (aidata[0] << 24) | (aidata[1] << 16) | (aidata[2] << 8) | aidata[3];

            /* Check Multicast IP Or Local IP */
            if ((aidata[0] & 0xf0) == 0xE0) //224.xxx.xxx.xxx - 239.xxx.xxx.xxx
            {
                mcastIp = ipAddress;
            } else {
                localIp = ipAddress;
            }
        }
    }

    if (5 <= argc)
    {
        ret = swat_strcmp(argv[4],"ip_hdr_inc");
        if(ret == 0)
        {
#ifdef SWAT_BENCH_RAW
            ip_hdr_inc = 1;
            ret = swat_sscanf(argv[3], "%3d.%3d.%3d.%3d", &aidata[0], &aidata[1], &aidata[2], &aidata[3]);
            if (ret < 0)
            {
                goto ERROR;
            }
            localIp = (aidata[0] << 24) | (aidata[1] << 16) | (aidata[2] << 8) | aidata[3];
#endif
         }else{
#ifdef SWAT_BENCH_RAW
            if(argc == 6){
                ret = swat_strcmp(argv[5],"ip_hdr_inc");
                if(ret == 0)
                {
                    ip_hdr_inc = 1;
                }
            }
#endif
            if(v6_enabled){
                Inet6Pton(argv[3], &mcastIp6);
                Inet6Pton(argv[4], &localIp6);
            }else{
            ret = swat_sscanf(argv[3], "%3d.%3d.%3d.%3d", &aidata[0], &aidata[1], &aidata[2], &aidata[3]);
            if (ret < 0)
            {
                   goto ERROR;
            }
            mcastIp = (aidata[0] << 24) | (aidata[1] << 16) | (aidata[2] << 8) | aidata[3];

            if((aidata[0] & 0xf0) != 0xE0)
            {
                  goto ERROR;
            }

            ret = swat_sscanf(argv[4], "%3d.%3d.%3d.%3d", &aidata[0], &aidata[1], &aidata[2], &aidata[3]);
            if (ret < 0)
            {
                  goto ERROR;
            }
            localIp = (aidata[0] << 24) | (aidata[1] << 16) | (aidata[2] << 8) | aidata[3];
        }
            mcastEnabled = 1;
        }
    }/* argc >= 5 */

    if(mcastEnabled)
        swat_bench_rx_test(protocol, port, localIp, mcastIp, &localIp6, &mcastIp6, ip_hdr_inc, rawmode);
    else
        swat_bench_rx_test(protocol, port, localIp, mcastIp, NULL, NULL, ip_hdr_inc, rawmode);

    return 0;
ERROR:
    SWAT_PTF("Incorrect/missing param\n");
    SWAT_PTF("benchrx <protocol> <port> <multicast IP*> <local IP*>\r\n");
    SWAT_PTF("Ex: benchrx tcp 6000 192.168.1.100 \r\n");
    SWAT_PTF("Parameters:\r\n");
    SWAT_PTF("1 <protocol>    : tcp/udp/ssl\r\n");
    SWAT_PTF("2 <port>        : Listening Port\r\n");
    SWAT_PTF("3 <multicast IP>: Multicast IP Address\r\n");
    SWAT_PTF("4 <local IP>    : Local IP Address\r\n");
#ifdef SWAT_BENCH_RAW
    SWAT_PTF("benchrx <protocol> <port> <multicast IP*> <local IP*> [ip_hdr_inc]\r\n");
#endif /*SWAT_BENCH_RAW*/

    return -1;
}

A_INT32
swat_benchquit_handle(A_INT32 argc, A_CHAR * argv[])
{
    if (argc > 1) {
        SWAT_PTF("input benchquit\n");
        return -1;
    }

    swat_bench_quit_config();

#if 0
    qcom_msleep(1000);

    extern void qcom_task_kill_all(void);
    qcom_task_kill_all();

    extern void swat_socket_close_all(void);
    swat_socket_close_all();
#endif

    return 0;
}

A_INT32
swat_benchdbg_handle(A_INT32 argc, A_CHAR * argv[])
{

    /* swat_bench_dbg(); */
    return 0;
}

A_BOOL
swat_strrcl_recall_done()
{
    return qcom_event_strrcl_recall_done(currentDeviceId);
}

//#endif



void
swat_time()
{
		  #if 0
          cmnos_printf("[%dh%dm%ds:]", (swat_time_get_ms() / 1000) / 3600,
                 ((swat_time_get_ms() / 1000) % 3600) / 60, (swat_time_get_ms() / 1000) % 60);
		  #endif
}

extern A_INT32 isdigit(A_INT32 c);

A_INT32
inet_aton(const A_CHAR *name,
          /* [IN] dotted decimal IP address */
          A_UINT32 * ipaddr_ptr
          /* [OUT] binary IP address */
    )
{                               /* Body */
    A_INT8 ipok = FALSE;
    A_UINT32 dots;
    A_UINT32 byte;
    A_UINT32 addr;

    addr = 0;
    dots = 0;
    for (;;) {
        if (!isdigit(*name))
            break;

        byte = 0;
        while (isdigit(*name)) {
            byte *= 10;
            byte += *name - '0';
            if (byte > 255)
                break;
            name++;
        }                       /* Endwhile */

        if (byte > 255)
            break;

        addr <<= 8;
        addr += byte;
        if (*name == '.') {
            dots++;
            if (dots > 3)
                break;
            name++;
            continue;
        }
      if ((*name == '\0') && (dots == 3)) {
         ipok = TRUE;
      } /* Endif */
      break;
   } /* Endfor */
   if (!ipok) {
      return 0;
   } /* Endif */
   if (ipaddr_ptr) {
      *ipaddr_ptr = addr;
   } /* Endif */
   return -1;
}                               /* Endbody */

A_INT32
swat_ping_handle(A_INT32 argc, A_CHAR *argv[], A_CHAR v6)
{
    A_UINT32 hostaddr;
#ifdef IP_V6
    A_UINT8 host6addr[16];
#endif
    A_INT32 error;
    A_UINT32 count, size, interval;
    A_UINT32 i;

    if (argc < 2) {
        SWAT_PTF("Usage: %s <host_ip> [ -c <count> -s <size> -i <interval> ]\n", argv[0]);

        return -1;
    }

    if(v6){
#ifdef IP_V6
        error = Inet6Pton(argv[1], &host6addr);
#else
        return -1;
#endif
    }else{
    error = inet_aton(argv[1], &hostaddr);
    }

    if (!v6 && error != -1) {
        if (strlen(argv[1]) > 32) {
            SWAT_PTF("host name cannot be more then 32 Bytes\n");

            return -1;
        } else {
#if !defined (REV74_TEST_ENV2)
            if (A_OK != qcom_dnsc_get_host_by_name(argv[1], &hostaddr) ) {
                SWAT_PTF("Can't get IP addr by host name %s\n", argv[1]);
                return -1;
            } else {
                SWAT_PTF("Get IP addr %s by host name %s\n", (char *)_inet_ntoa(hostaddr), argv[1]);
            }
#endif
        }
    }

    if (v6 && error !=0 ) {
        if (strlen(argv[1]) > 32) {
            SWAT_PTF("host name cannot be more then 32 Bytes\n");

            return -1;
        } else {
#if !defined (REV74_TEST_ENV2)
	  if (A_OK != qcom_dnsc_get_host_by_name2(argv[1], (A_UINT32*)(host6addr), AF_INET6, 2)) {
                SWAT_PTF("Can't get IPv6 addr by host name %s\n", argv[1]);
                return -1;
            } else {
	        A_UINT8 ip6_str[48];
	        inet6_ntoa((char *)&host6addr,(char *)ip6_str);
                SWAT_PTF("Get IPv6 addr %s by host name %s\n", ip6_str, argv[1]);
            }
#endif
        }
    }

    count = 1;
    size = 64;
    interval = 0;

    if (argc > 2 || argc <= 8) {
        for (i = 2; i < argc; i += 2) {
            if (!A_STRCMP(argv[i], "-c")) {
                if ((i + 1) == argc) {
                    SWAT_PTF("Missing parameter\n");
                    return -1;
                }
                A_SSCANF(argv[i + 1], "%u", &count);
            } else if (!A_STRCMP(argv[i], "-s")) {
                if ((i + 1) == argc) {
                    SWAT_PTF("Missing parameter\n");
                    return -1;
                }
                A_SSCANF(argv[i + 1], "%u", &size);
            } else if (!A_STRCMP(argv[i], "-i")) {
                if ((i + 1) == argc) {
                    SWAT_PTF("Missing parameter\n");
                    return -1;
                }
                A_SSCANF(argv[i + 1], "%u", &interval);
            }
        }
    } else {
        SWAT_PTF("Usage: %s <host> [ -c <count> -s <size> -i <interval> ] \n", argv[0]);

        return -1;
    }

    if (size > 7000) {          /*CFG_PACKET_SIZE_MAX_TX */
        SWAT_PTF("Error: Invalid Parameter %s \n", argv[5]);
        return -1;
    }

    for (i = 0; i < count; i++) {
        if(v6){
#ifdef IP_V6
            if(qcom_ping6(host6addr, size) == A_OK){
                A_UINT8 ip6_str[48];
                inet6_ntoa((char *)&host6addr,(char *)ip6_str);
                A_PRINTF("Ping reply from %s : time<1ms\r\n",ip6_str);
            }else{
                A_PRINTF("Request timed out\r\n");
            }
#else
            return -1;
#endif
        }else{
            if(qcom_ping(hostaddr, size) == A_OK){
                A_PRINTF("Ping reply from %d.%d.%d.%d: time<1ms\r\n",
                    hostaddr >> 24 & 0xFF, hostaddr >> 16 & 0xFF, hostaddr >> 8 & 0xFF, hostaddr & 0xFF);

            }else{
                A_PRINTF("Request timed out\r\n");
            }
        }
        if ((count > 0) && (i < (count - 1))) {
            qcom_thread_msleep(interval); /*Sleep to wait for Reply packets */
        }

    }
    return 0;
}

