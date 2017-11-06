/*
  * Copyright (c) 2015 Qualcomm Atheros, Inc..
  * All Rights Reserved.
  * Qualcomm Atheros Confidential and Proprietary.
  */

#include "qcom_common.h"
#include "swat_wmiconfig_common.h"

char* cmd[] = {
	    "Usage: wmiconfig [<command>]",
        "Commands:",
        "--version",
        "--reset",
        "",
        "--device <id>",
        "   <id>: 0 - device 0",
        "         1 - device 1",
        "WPA Configuration:",
        "--p <passphrase>",
        "--wpa <ver> <ucipher> <mcipher>",
        "--connect <ssid>",
        "   where <p>      : Passphrase for WPA",
        "         <ver>    : 1-WPA, 2-RSN",
        "         <ucipher>: TKIP or CCMP",
        "         <mcipher>: TKIP or CCMP",
        "         <ssid>   : SSID of network",
        "",
        "WEP Configuration:",
        "--wepkey <key_index> <key>",
        "--wep <def_keyix> <mode>",
        "   where <key_index>: Entered WEP key index",
        "         <key>      : WEP key",
        "         <def_keyix>: Default WEP key index",
        "         <mode>     : open or shared",
        "",
#if defined(SWAT_WMICONFIG_WPS)
        "WPS Configuration:",
        "--wps <connect> <mode>",
        "   where <connect>: 0 - No attempt to connect after wps",
        "                  : 1 - Attempt to connect after wps",
        "         <mode>   : pin or push [pin:<=8 characters]",
        "",
#endif
        "--disc = Disconect from AP",
        "--wmode <mode> = Set mode <a,b,g,n>",
        "--pwrmode <mode> = Set power mode 1=Power save, 0= Max Perf",
        "--channel <ch> = Set channel hint 1-13",
        "--listen <int> = Set listen interval",
        "--mode <ap <hidden> <wps> | station>",
        "--scanctrl <0|1> <0|1> = Control firmware scan behavior. Disable/enable foreground OR background scanning",
        "--setscanpara <max_act_ch_dwell_time_ms> <pas_act_chan_dwell_time_ms> <fg_start_period(in secs)> <fg_end_period (in secs)> <bg_period (in secs)> ",
        "              <short_scan_ratio> <scan_ctrl_flags>  <min_active_chan_dwell_time_ms> <max_act_scan_per_ssid> <max_dfs_ch_act_time_in_ms>",
        "   set max dwell time in ms (0=reset value)",
        "   pass chan dwell time ms (0=reset value)",
        "   fg scan start period in sec (0=reset value, 65535=disable)",
        "   fg scan end period in sec(0=reset value)", 
        "   bg scan period (0=disable,65535=disable)",
        "   short scan ratio (default=3)", 
        "   scan control flags (default=47)", 
        "   min active chan dwell time in ms", 
        "   max scan per ssid",
        "   max time dfs chan active in ms ",
        "--setscan <forceFgScan> <homeDwellTimeInMs> <forceScanIntervalInMs> <scanType> ",
        "          <numChannels> [<channel> <channel>... upto numChannels] ",
        "   set force fg scan", 
        "   home chan dwell time in ms (0=default)", 
        "   force scan interval ms", 
        "   scan type as long or short scan", 
        "   numchannels to scan (0=no channels provided)",
        "   [channel1MHz, channel2MHz, .... upto channelNMhz]",
        "--allow_aggr <tx_tid_mask> <rx_tid_mask> Enables aggregation based on the provided bit mask where each bit represents a TID valid TID's are 0-7",
        "",
        "AP Configuration:",
        "--ap bconint <>   = set ap beacon interval",
        "--ap country <>   = set ap country code",
        "--ap inact <minutes>   = set ap inactive times",
        "",
        "benchmode v4|v6   = set IPv4/IPv6 mode",
        "DNS Client:",
        "--ip_gethostbyname [<host name>]  = resolve hostname",
#ifdef IP_V6
        "--ip_resolve_hostname [<host name> <domain_type]  = resolve hostname for domain_type (ipv4 =2, ipv6 =3)",
        "--ip_gethostbyname2 [<host name> <domain_type]  = resolve hostname for domain_type (ipv4 =2 ipv6 =3)",
#endif
        "--ip_dns_client [<start/stop>] 	= Start/Stop the DNS Client",
        "--ip_dns_server_addr <ip addr> 	= Address of the DNS Server",
        "--ip_dns_server [<start/stop>] 	= Start/Stop the DNS Server",
        "--ip_dns_local_domain [<domain name>] 	= set local domain name of DNS Server",
        "--ip_dns_entry_create [<domain name> <ip addr>] 	= create a DNS entry of DNS Server",
        "--ip_dns_entry_delete [<domain name> <ip addr>] 	= delete a DNS entry of DNS Server",
        "--ip_dns_delete_server_addr <ip addr>	   = Address of the DNS Server to be deleted",
        "",
#ifdef SWAT_SSL
        "--ssl_start server|client = start SSL as either server or client",
        "--ssl_stop server|client = stop SSL server or client",
        "--ssl_config server|client [protocol <protocol>] [time 0|1] [domain 0|<name>] [cipher <cipher>] = configure SSL server or client",
        "   where protocol <protocol> = select protocol: SSL3, TLS1.0, TLS1.1, TLS1.2",
        "         time 0|1            = disable|enable certificate time validation (optional)",
        "         domain 0|<name>     = disable|enable validation of peer's domain name against <name>",
        "         alert 0|1           = disable|enable sending of SSL alert if certificate validation fails.",
        "         cipher <cipher>     = select <cipher> (enum name) suite to use, can be repeated 8 times (optional)",
        "--ssl_add_cert server|client  certificate|calist [<name>] = add a certificate or CA list to either SSL server or client.",
        "   where <name> = name of file to load from FLASH. A default certificate or CA list will be added if <name> is omitted.",
        "--ssl_store_cert <name> = store a certificate or CA list in FLASH with <name>",
        "--ssl_delete_cert [<name>] = delete the certificate or CA list with <name> from FLASH.",
        "--ssl_list_cert  = list the names of the certificates and CA lists stored in FLASH.",

#endif    
        "IP Configuration:",
#ifdef BRIDGE_ENABLED
        "--ipbridgemode = Enable the bridge mode",
#endif
        "--ipconfig = Show IP parameters",
        "--ipstatic <IP Address> <Subnet Mask> <Default Gateway> = Set static IP parameters",
        "--ipdhcp = Run DHCP client",
        "--ipdhcppool <start ipaddr> <End ipaddr> <Lease time> = set ip dhcp pool",
        "--ip6rtprfx <prefix> <prefixlen> <prefix_lifetime> <valid_lifetime> = set ipv6 router prefix",
        "",
        "--pmparams    = Set Power management parameters",
        "       --idle <msec>                              Idle period",
        "       --np   < >                                 PS Poll Number",
        "       --dp <1=Ignore 2=Normal 3=Stick 4=Auto>    DTIM Policy",
        "       --txwp <1=wake 2=do not wake>              Tx Wakeup Policy",
        "       --ntxw < >                                 Number of TX to Wakeup",
        "       --psfp <1=send fail event 2=Ignore event>  PS Fail Event Policy",
        "--pwrmode <0 | 1>   = set power mode 1=Power save, 0= Max Perf",
        "--rssi   = prints Link Quality (SNR)",
        "--settxpower <>   = Set transmit power 1-17 dbM",
        "--suspend    = Enable device suspend mode(before connect command)",
        "--suspstart <msec>    = Suspend device for specified time in milliseconds",
        "--driver [mode]",
        "  	mode = down - Sets the driver State 0 unloads the driver",
        "  	       up   - Reloads the driver",
#ifdef SWAT_WMICONFIG_SNTP
        "--ip_sntp_client [<start/stop]  = SNTP Client start or stop",
        "--ip_sntp_srvr [<add/delete>] [server name]  = Add/Delete SNTP Server name",
        "--ip_sntp_get_time  = get the current Time stamp from SNTP server",
        "--ip_sntp_get_time_of_day  = Command to Display the seconds",
        "--ip_sntp_zone [<UTC+/-min:hr] dse [enable/disable]  = Cient zone modify & en/dis day-light-saving",
        "--ip_show_sntpconfig  = Displays the SNTP server address name",
#endif
        "--promisc <start/stop>  [mode]= Disable/Enable wlan promiscurous test",
        "  where <start/stop> = <0-stop 1-start>",
        "		<mode> = <1-normal mode 2-externd mode 3-externd mode under connection>, it's optional,default value is 1"
#ifdef ENABLE_HTTP_SERVER
        "--ip_http_server [<start/stop> <port>]  = Start/Stop the HTTP server",
        "--ip_http_post <page name>  <obj_name> <obj_type> <obj_len> <obj_value>  = Post/Update the object in HTML page",
        "   where <page name>  = page name to update",
        "         <obj_name>   = object name to update",
        "         <obj_type>   = <1-Bool 2-Integer 3-String>",
        "         <obj_len>    = Length of object<1-Bool 4-Integer Length-String>",
        "         <obj_value>  = Object value",
        "--ip_http_get <page name>  <obj_name>  = Get the object in HTML page",
        "   where <page name>  = page name to get",
        "         <obj_name>   = object name to get",
#endif

#ifdef ENABLE_HTTP_CLIENT
        "--ip_http_client [<connect/get/post/query/disc> <data1> <data2>]",
        "   where <connect> - Used to connect to HTTP server",
        "     <data1> - Hostname or IPaddress of http server",
        "     <data2> - port no of http server(optional)",
        "   where <get>     - Used to get a page from HTTP server",
        "     <data1> - Page name to retrieve",
        "   where <post>    - Used to post to HTTP server",
        "     <data1> - URL to post",
        "   where <query>   - Used to update a variable for get/post",
        "     <data1> - Name of variable",
        "     <data2> - value of variable",
        "   where <disc>    - Disconnect the HTTP server and close the HTTP client session.",
#endif
        "--wdt [<0/1> <timeout>]",
        "   0- Disable Watchdog, timeout should be 0",
        "   1- Disable Watchdog, must provide timeout",
        "   timeout- watchdog timeout in seconds",
        "--wdttest [timeout]",
        "   timeout- time is sec to block the CPU",
        "--setmainttimeout [timeout in msecs]",
        " --print <enable>",
        "   0|1- Disable/Enable firmware prints",
        " --activeptn <partition_index>",
        "   1-N: N is depend on flash size, the maxnum is 7"
   };


void
swat_wmiconfig_help()
{
   int i;
   
   for (i = 0; i < sizeof(cmd) /sizeof(cmd[0]); ++i) {
        SWAT_PTF("%s\n", cmd[i]);
	if (i%20 == 0)
        tx_thread_sleep(100);
   }
}


void swat_wmiconfig_maint_timeout_set(A_UINT32 *timeout,A_UINT32 param_size){

    qcom_param_set(currentDeviceId,QCOM_PARAM_GROUP_SYSTEM,QCOM_PARAM_GROUP_SYSTEM_SLEEP_MAINT_TIMEOUT_MSECS,timeout,param_size,TRUE);

}

void
swat_wmiconfig_version(void)
{
	A_CHAR date[20];
	A_CHAR time[20];
	A_CHAR ver[20];
	A_CHAR cl[20];
	qcom_firmware_version_get(date, time, ver, cl);


	SWAT_PTF("Host version        : Hostless\n");
    SWAT_PTF("Target version      : QCM\n");
    SWAT_PTF("Firmware version    : %s\n", ver);
    SWAT_PTF("Firmware changelist : %s\n", cl);
    SWAT_PTF("Interface version   : EBS\n");
    SWAT_PTF(" - built on %s %s\n", date, time);
}

void
swat_wmiconfig_reset()
{
    qcom_sys_reset();
}

void
swat_wmiconfig_active_partition(A_UINT32 partition_index)
{
    extern A_STATUS qcom_active_partition(A_UINT32 partition_index);
    A_INT32 ret;
    ret = qcom_active_partition(partition_index);
    if (ret == A_OK) {
        SWAT_PTF("Activeptn OK, after reset partition %d will be up.\n", partition_index);
    } else if (ret == A_EINVAL) {
        SWAT_PTF("Activeptn fail, due to invalid partition index or the partition has no valid image.\n");
        SWAT_PTF("                partition index 1 -N, N is depend on flash size, the maxnum is 7.\n");
    } else if (ret == A_NO_MEMORY) {
        SWAT_PTF("Activeptn fail, due to no memory\n");
    } else {
        SWAT_PTF("Activeptn fail: ret %d\n", ret);
    }
}

void swat_application_frame_cb(A_UINT8 *pData, A_UINT16 length)
{
	A_UINT16 i,print_length,j=0;

	print_length = 32;
	SWAT_PTF_NO_TIME("frame (%d):\n", length);

	/* only print the first 64 bytes of each frame */
	if(length < print_length)
		print_length = length;

	for(i=0 ; i<print_length ; i++){
        //since printf("0x%02x") does not appear to work we brute force it
        if(pData[i] < 0x10){
            SWAT_PTF_NO_TIME("0x0%x, ", pData[i]);
        }else{
    		SWAT_PTF_NO_TIME("0x%x, ", pData[i]);
        }

		if(j++==7){
			j=0;
			SWAT_PTF_NO_TIME("\n");
		}
	}

	if(j){
		SWAT_PTF_NO_TIME("\n");
	}

	tx_thread_sleep(100);
}


void swat_wmiconfig_ht40_flag_set(A_UINT8 flag)
{
	qcom_set_ht40_flag(flag);
}

/*
When the callback function used under connected with AP, the less handle code, the better.
*/
void swat_application_frame_cb_ext(A_UINT8 *pData, A_UINT16 length, PROMISC_EXT_INFO_t* extinfo)
{
    /*Just print some info*/    
    SWAT_PTF_NO_TIME("frame (type=%02x,len=%d,rssi=%d)\n",pData[0],length,extinfo->rssi);
}

extern PROMISC_MODE  promiscMode;
void
swat_wmiconfig_promiscuous_test(A_BOOL promiscuousEn, A_UINT8 mode)
{
    if(mode == PROMISC_MODE_NORMAL){
        qcom_set_promiscuous_rx_cb((QCOM_PROMISCUOUS_CB)swat_application_frame_cb);
        qcom_set_promiscuous_rx_cb_ext(NULL);
    }
    else if(mode == PROMISC_MODE_NORMAL_EXT || mode == PROMISC_MODE_CONNECT_EXT){
        qcom_set_promiscuous_rx_cb_ext((QCOM_PROMISCUOUS_CB_EXT)swat_application_frame_cb_ext);
        qcom_set_promiscuous_rx_cb(NULL);
    }
    promiscMode = mode;
    qcom_promiscuous_enable(promiscuousEn);	
}

