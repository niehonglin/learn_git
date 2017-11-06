/*
  * Copyright (c) 2015 Qualcomm Atheros, Inc..
  * All Rights Reserved.
  * Qualcomm Atheros Confidential and Proprietary.
  */

#include "qcom_common.h"
#include "qcom_cli.h"
#include "qcom/socket_api.h"
#include "qcom/qcom_ssl.h"
#include "qcom/qcom_scan.h"
#include "qcom/qcom_gpio.h"
#include "qcom/qcom_internal.h"
#include "airkiss_cmds_parser.h"

/* external functions */
extern int atoul(char *buf);

/* external variable */
extern A_UINT8 qcom_sta_wlan_reconnect_start_flag[];
extern A_INT32 airkiss_cmds_num;
extern console_cmd_t airkiss_cmds[];

/* internal functions */
void Airkiss_CmdWmiconfigInformation(A_UINT8 deviceId);
void Airkiss_WmiconfigVersion(void);
void Airkiss_WmiconfigHelp(void);
void Airkiss_WmiconfigWlanDisc(A_UINT8 deviceId);
void Airkiss_WmiconfigIpConfig(A_UINT8 deviceId);
A_INT32 Airkiss_WmiconfigSetDevId(A_INT32 argc, A_CHAR * argv[]);
A_INT32 Airkiss_WmiconfigSetWlanMode(A_INT32 argc, A_CHAR * argv[]);
A_INT32 Airkiss_WmiconfigSetWpa(A_INT32 argc, A_CHAR * argv[]);
A_INT32 Airkiss_WmiconfigConnect(A_INT32 argc, A_CHAR * argv[]);
A_INT32 Airkiss_WmiconfigSetWlanPowerMode(A_INT32 argc, A_CHAR * argv[]);
A_INT32 Airkiss_WmiconfigIpStatic(A_INT32 argc, A_CHAR * argv[]);
A_INT32 Airkiss_Inet_aton(const A_CHAR *name, A_UINT32 * ipaddrPtr);
A_CHAR *Airkiss_Inet_ntoa(A_UINT32 ip);
A_UINT32 Airkiss_InetAddr(A_CHAR *str);

A_UINT8 currentDeviceId = 0;

A_INT32 Airkiss_CmdHandlerPing(A_INT32 argc, A_CHAR * argv[])
{
    A_UINT32 hostaddr;
    A_INT32 error;
    A_UINT32 count, size, interval;
    A_UINT32 i;

    if (argc < 2) {
        return AIRKISS_CMD_ERROR;
    }

    error = Airkiss_Inet_aton(argv[1], &hostaddr);

    if (error != -1) {
        if (strlen(argv[1]) > 32) {
            printf("host name cannot be more then 32 Bytes\n");

            return AIRKISS_CMD_ERROR;
        } else {
#if !defined (REV74_TEST_ENV2)
            if (A_OK != qcom_dnsc_get_host_by_name(argv[1], &hostaddr) ) {
                printf("Can't get IP addr by host name %s\n", argv[1]);
                return AIRKISS_CMD_ERROR;
            } else {
                printf("Get IP addr %s by host name %s\n", (char *)Airkiss_Inet_ntoa(hostaddr), argv[1]);
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
                    printf("Missing parameter\n");
                    return AIRKISS_CMD_ERROR;
                }
                
                A_SSCANF(argv[i + 1], "%u", &count);
            } else if (!A_STRCMP(argv[i], "-s")) {
                if ((i + 1) == argc) {
                    printf("Missing parameter\n");
                    return AIRKISS_CMD_ERROR;
                }
                
                A_SSCANF(argv[i + 1], "%u", &size);
            } else if (!A_STRCMP(argv[i], "-i")) {
                if ((i + 1) == argc) {
                    printf("Missing parameter\n");
                    return AIRKISS_CMD_ERROR;
                }
                A_SSCANF(argv[i + 1], "%u", &interval);
            }
        }
    } else {
        printf("Usage: %s <host> [ -c <count> -s <size> -i <interval> ] \n", argv[0]);
        return AIRKISS_CMD_ERROR;
    }

    if (size > 7000) {          /*CFG_PACKET_SIZE_MAX_TX */
        printf("Error: Invalid Parameter %s \n", argv[5]);
        return AIRKISS_CMD_ERROR;
    }

    for (i = 0; i < count; i++) {
        if(qcom_ping(hostaddr, size) == A_OK){
            printf("Ping reply from %d.%d.%d.%d: time<1ms\r\n",
                hostaddr >> 24 & 0xFF, hostaddr >> 16 & 0xFF, hostaddr >> 8 & 0xFF, hostaddr & 0xFF);

        }else{
            printf("Request timed out\r\n");
        }

        if ((count > 0) && (i < (count - 1))) {
            qcom_thread_msleep(interval); /*Sleep to wait for Reply packets */
        }

    }
    return AIRKISS_CMD_OK;
}

A_INT32 Airkiss_CmdHandlerIwconfig(A_INT32 argc, A_CHAR * argv[])
{
    A_UCHAR wifiState = 0;
    A_UINT16 count;
    A_CHAR* pssid;
    QCOM_BSS_SCAN_INFO* pOut;
    qcom_start_scan_params_t scanParams;
    
    qcom_get_state(currentDeviceId, &wifiState);

    if (argc == 2) {
        pssid = "";
    } else if (argc == 3) {
        pssid = argv[2];
    } else {
        A_CHAR ssid[32];
        A_UINT8 i, j;

        j = 0;
        A_MEMSET(ssid, 0, 32);
        for (i=2; i<argc; i++)
        {
            if ((j + strlen(argv[i])) > 32)
            {
                return AIRKISS_CMD_ERROR;
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
        pssid = ssid;
    }

    /*Set the SSID*/
    qcom_set_ssid(currentDeviceId, pssid);
    
    /*Start scan*/
    scanParams.forceFgScan  = 1;
    scanParams.scanType     = WMI_LONG_SCAN;
    scanParams.numChannels  = 0;
    scanParams.forceScanIntervalInMs = 1;
    scanParams.homeDwellTimeInMs = 0;       
    qcom_set_scan(currentDeviceId, &scanParams);

    /*Get scan results*/
    if(qcom_get_scan(currentDeviceId, &pOut, &count) == A_OK){
        QCOM_BSS_SCAN_INFO* info = pOut;
        int i,j;

        tx_thread_sleep(1000);
        for (i = 0; i < count; i++)
        {
            printf("ssid = ");
            {
                for(j = 0;j < info[i].ssid_len;j++)
                {
                    printf("%c",info[i].ssid[j]);
                }
                printf("\n");
            }
#if 1
            //tx_thread_sleep(10);
            printf("bssid = %.2x:%.2x:%.2x:%.2x:%.2x:%.2x\n",info[i].bssid[0],info[i].bssid[1],info[i].bssid[2],info[i].bssid[3],info[i].bssid[4],info[i].bssid[5]);

        //   tx_thread_sleep(10);
            printf("channel = %d\n",info[i].channel);

            printf("indicator = %d\n",info[i].rssi);

            printf("security = ");
            if(info[i].security_enabled){
                printf("\n");
                if(info[i].rsn_auth || info[i].rsn_cipher){
                    printf("RSN/WPA2= ");

                if(info[i].rsn_auth){
                    printf("{");
                    if(info[i].rsn_auth & SECURITY_AUTH_1X){
                        printf("802.1X ");
                    }
                    if(info[i].rsn_auth & SECURITY_AUTH_PSK){
                        printf("PSK ");
                    }
                    printf("}");
                }
                if(info[i].rsn_cipher){
                    printf("{");
                    if(info[i].rsn_cipher & ATH_CIPHER_TYPE_WEP){
                        printf("WEP ");
                    }
                    if(info[i].rsn_cipher & ATH_CIPHER_TYPE_TKIP){
                        printf("TKIP ");
                    }
                    if(info[i].rsn_cipher & ATH_CIPHER_TYPE_CCMP){
                        printf("AES ");
                    }
                    printf("}");
                }
                printf("\n");
            }
            if(info[i].wpa_auth || info[i].wpa_cipher){
                printf("WPA= ");

                if(info[i].wpa_auth){
                    printf("{");
                    if(info[i].wpa_auth & SECURITY_AUTH_1X){
                        printf("802.1X ");
                    }
                    if(info[i].wpa_auth & SECURITY_AUTH_PSK){
                        printf("PSK ");
                    }
                    printf("}");
                }

             if(info[i].wpa_cipher){
                    printf("{");
                    if(info[i].wpa_cipher & ATH_CIPHER_TYPE_WEP){
                        printf("WEP ");
                    }
                    if(info[i].wpa_cipher & ATH_CIPHER_TYPE_TKIP){
                        printf("TKIP ");
                    }
                    if(info[i].wpa_cipher & ATH_CIPHER_TYPE_CCMP){
                        printf("AES ");
                    }
                    printf("}");
                }
                printf("\n");
            }
            if(info[i].rsn_cipher == 0 && info[i].wpa_cipher == 0){
                printf("WEP \n");
            }
        }else{
            printf("NONE! \n");
        }
		if (i != (count-1))
		printf("\n");
#endif
    }
	printf("\nshell> ");
    }
    
    return AIRKISS_CMD_OK;
}

A_INT32 Airkiss_CmdHandlerWmiconfig(A_INT32 argc, A_CHAR * argv[])
{
    A_INT32 ret = AIRKISS_CMD_OK;

    if (argc == 1) {
        Airkiss_CmdWmiconfigInformation(currentDeviceId);
        return AIRKISS_CMD_OK;
    }

    if (!A_STRCMP(argv[1], "--version")) {
        Airkiss_WmiconfigVersion();
    }
    else if (!A_STRCMP(argv[1], "--reset")) {
        qcom_sys_reset();
    }
    else if (!A_STRCMP(argv[1], "--device")) {
	 ret = Airkiss_WmiconfigSetDevId(argc, argv);
    }
    else if (!A_STRCMP(argv[1], "--mode")) {
        ret = Airkiss_WmiconfigSetWlanMode(argc, argv);
    }
    else if (!A_STRCMP(argv[1], "--channel")) {
        A_INT32 data;
        
        if (argc != 3) {
            return AIRKISS_CMD_ERROR;
        }
 
        A_SSCANF(argv[2], "%d", &data);
        if ((data < 1) || (data > 165))
        {
            printf("invalid channel: should range between 1-165\n");
            return AIRKISS_CMD_ERROR;
        }
        qcom_set_channel(currentDeviceId, (A_UINT32) data);
    }
    else if (!A_STRCMP(argv[1], "--p")) {
        if (argc != 3) {
            return AIRKISS_CMD_ERROR;
        }

        qcom_sec_set_passphrase(currentDeviceId, argv[2]);
    }
    else if (!A_STRCMP(argv[1], "--wpa")) {
        ret = Airkiss_WmiconfigSetWpa(argc, argv);
    }
    else if (!A_STRCMP(argv[1], "--connect")) {
        ret = Airkiss_WmiconfigConnect(argc, argv);
    }
    else if (!A_STRCMP(argv[1], "--disc")) {
        Airkiss_WmiconfigWlanDisc(currentDeviceId);
    }
    else if (!A_STRCMP(argv[1], "--pwrmode")) {
        ret = Airkiss_WmiconfigSetWlanPowerMode(argc, argv);
    }
    else if (!A_STRCMP(argv[1], "--ipconfig")) {
        Airkiss_WmiconfigIpConfig(currentDeviceId);
    }
    else if (!A_STRCMP(argv[1], "--ipstatic")) {
        ret = Airkiss_WmiconfigIpStatic(argc, argv);
    }
    else if (!A_STRCMP(argv[1], "--ipdhcp")) {
        A_UINT32 param;
        qcom_ipconfig(currentDeviceId, IP_CONFIG_DHCP, &param, &param, &param);
    }
    else {
        Airkiss_WmiconfigHelp();
    }

    return ret;
}

A_INT32 Airkiss_CmdHandlerAirkissConfig(A_INT32 argc, A_CHAR * argv[])
{
    static int AirkissCfgState = 0;
    
    extern void smartconfig_task_start(int start_channel_id, int stop_channel_id, int channel_dwell_time);
    extern void smartconfig_task_stop(void);

    if (argc < 2) {
        return AIRKISS_CMD_ERROR;
    }

    if (A_STRCMP(argv[1],"--start") == 0) {

        if (argc < 5) {
            return AIRKISS_CMD_ERROR;
        }
        
        if (0 == AirkissCfgState) {
            int startCh;
            int stopCh;
            int dwell;
            
            AirkissCfgState = 1;
            startCh = atoi(argv[2]);
            stopCh = atoi(argv[3]);
            dwell = atoi(argv[4]);
            
            smartconfig_task_start(startCh, stopCh, dwell);
        }
    }
    else {
        AirkissCfgState = 0;
        smartconfig_task_stop();
    }

    return AIRKISS_CMD_OK;
}

A_INT32 Airkiss_WmiconfigIpStatic(A_INT32 argc, A_CHAR * argv[])
{
    A_UINT32 address;
    A_UINT32 submask;
    A_UINT32 gateway;

    if (argc < 5) {
        return AIRKISS_CMD_ERROR;
    }
    
    address = Airkiss_InetAddr(argv[2]);
    submask = Airkiss_InetAddr(argv[3]);
    gateway = Airkiss_InetAddr(argv[4]);

    qcom_ipconfig(currentDeviceId, IP_CONFIG_SET, &address, &submask, &gateway);

    return AIRKISS_CMD_OK;
}

void Airkiss_WmiconfigIpConfig(A_UINT8 deviceId)
{
    A_UINT8 macAddr[6];
    A_UINT32 ipAddress;
    A_UINT32 submask;
    A_UINT32 gateway;
    A_UINT32 dns[3];
    A_UINT32 number=0;
    A_UINT32 i;

    qcom_mac_get(deviceId, (A_UINT8 *) & macAddr);
    qcom_ipconfig(deviceId, IP_CONFIG_QUERY, &ipAddress, &submask, &gateway);
    
    A_MEMSET(dns,0,3*sizeof(A_UINT32));
    qcom_dns_server_address_get(dns, &number);

    printf(" mac addr = %x:%x:%x:%x:%x:%x\n",
            macAddr[0], macAddr[1], macAddr[2], macAddr[3], macAddr[4], macAddr[5]);
    printf("IP:");
    IPV4_PTF(ipAddress);
    printf("\n");
    printf(" Mask:");
    IPV4_PTF(submask);
    printf("\n");
    printf(", Gateway:");
    IPV4_PTF(gateway);
    printf("\n");
    printf(", Dns:");
    for(i=0;i<number;i++){
        IPV4_PTF(dns[i]);
        printf(";");
    }
    printf("\n");

    return;
}

A_INT32 Airkiss_WmiconfigSetWlanPowerMode(A_INT32 argc, A_CHAR * argv[])
{
    A_INT32 data;
    
    if (argc < 3) {
        return AIRKISS_CMD_ERROR;
    }

    A_SSCANF(argv[2], "%d", &data);

    if (data == 0) {
        qcom_power_set_mode(currentDeviceId, 2);   //MAX_PERF_POWER
    } else if (data == 1) {
        A_UINT32 wifiMode;
        qcom_op_get_mode(currentDeviceId, &wifiMode);
        if (wifiMode == 1) {
            printf("Setting REC Power is not allowed MODE_AP \n");
            return AIRKISS_CMD_ERROR;
        }

        qcom_power_set_mode(currentDeviceId, 1);   //REC_POWER
    } else {
        return AIRKISS_CMD_ERROR;
    }

    return AIRKISS_CMD_OK;
}

A_INT32 Airkiss_WmiconfigSetWpa(A_INT32 argc, A_CHAR * argv[])
{
    int authMode, encyptMode;

    if (argc != 5) {
        return AIRKISS_CMD_ERROR;
    }

    if (!A_STRCMP(argv[2], "1")) {
        authMode = WLAN_AUTH_WPA_PSK;
    }
    else if (!A_STRCMP(argv[2], "2")) {
        authMode = WLAN_AUTH_WPA2_PSK;
    }
    else {
        printf("invaid version, should be 1 or 2\n");
        return AIRKISS_CMD_ERROR;
    }

    if (!A_STRCMP(argv[3], argv[4])) {
        if (!A_STRCMP(argv[3], "TKIP")) {
            encyptMode = WLAN_CRYPT_TKIP_CRYPT;
        }
        else if (!A_STRCMP(argv[3], "CCMP")) {
            encyptMode = WLAN_CRYPT_AES_CRYPT;
        }
        else {
            printf("invaid uchipher mcipher, should be TKIP or CCMP\n");
            return AIRKISS_CMD_ERROR;
        }
    }
    else
    {
        printf("invaid uchipher mcipher, should be same\n");
        return AIRKISS_CMD_ERROR;
    }
    
    qcom_sec_set_auth_mode(currentDeviceId, authMode);
    qcom_sec_set_encrypt_mode(currentDeviceId, encyptMode);

    return AIRKISS_CMD_OK;
}

A_INT32 Airkiss_WmiconfigSetWlanMode(A_INT32 argc, A_CHAR * argv[])
{
    if(argc <= 2)
    {
        printf("invalid parameters\n");
        return AIRKISS_CMD_ERROR;
    }
    if (!A_STRCMP((A_CHAR *) argv[2], "ap"))
    {
        printf("Disabling PowerSave\n");
        qcom_power_set_mode(currentDeviceId,MAX_PERF_POWER);
    }
    else if (!A_STRCMP((A_CHAR *) argv[2], "station"))
    {
        printf("Enabling PowerSave\n");
        qcom_power_set_mode(currentDeviceId,REC_POWER);
    }


    if (3 == argc) {
        A_UINT32 devMode;
        
        if (!A_STRCMP((A_CHAR *) argv[2], "ap"))
        {
            devMode = QCOM_WLAN_DEV_MODE_AP;
        }
        else if (!A_STRCMP((A_CHAR *) argv[2], "station"))
        {
            devMode = QCOM_WLAN_DEV_MODE_STATION;
        }
        else
        {
            printf("unkown mode\n");
            return AIRKISS_CMD_ERROR;
        }

        qcom_op_set_mode(currentDeviceId, devMode);
    } 
    else {
        return AIRKISS_CMD_ERROR;
    }

    return AIRKISS_CMD_OK;
}

A_INT32 Airkiss_WmiconfigSetDevId(A_INT32 argc, A_CHAR * argv[])
{
    if (argc == 3) {
        extern A_UINT8 gNumOfWlanDevices;
        if (gNumOfWlanDevices > 1) {
            currentDeviceId = atoi(argv[2]);
            if(currentDeviceId < gNumOfWlanDevices){
                printf("configure device switch to %d\n", currentDeviceId);
                return AIRKISS_CMD_OK;
            }
            else{
                printf("Max device id is %d\n", gNumOfWlanDevices-1);
                return AIRKISS_CMD_ERROR;
            }
        }
        else {
            printf("Board is working on single device mode.\n");
            return AIRKISS_CMD_OK;
        }
    }
    
    return AIRKISS_CMD_ERROR;
}

void Airkiss_WmiconfigVersion(void)
{
    A_CHAR date[20];
    A_CHAR time[20];
    A_CHAR ver[20];
    A_CHAR cl[20];
    
    qcom_firmware_version_get(date, time, ver, cl);
    printf("Host version        : Hostless\n");
    printf("Target version      : QCM\n");
    printf("Firmware version    : %s\n", ver);
    printf("Firmware changelist : %s\n", cl);
    printf("Interface version   : EBS\n");
    printf(" - built on %s %s\n", date, time);

    return;
}

void Airkiss_CmdWmiconfigInformation(A_UINT8 deviceId)
{
    A_UINT8 chipState;
    A_UINT8 phyMode;
    A_UINT16 channel = 0;
    A_UINT8 macAddr[6];
    A_CHAR ssid[33];
    A_UINT32 powerMode;
    QCOM_WLAN_DEV_MODE devMode = QCOM_WLAN_DEV_MODE_INVALID;
    A_CHAR *modeDesc[] = { "Station", "AP", "AD-HOC", "Unknown" };

    A_MEMSET(&macAddr, 0, sizeof (macAddr));
    qcom_mac_get(deviceId, (A_UINT8 *) & macAddr);

    /* Mode */
    qcom_op_get_mode(deviceId, (A_UINT32 *)(&devMode));

    /* SSID */
    A_MEMSET(ssid, 0, sizeof(ssid));
    qcom_get_state(deviceId, &chipState);

    if (chipState == 4) {//K_WLAN_LINK_STATE_CONNECTED_STATE
        qcom_get_ssid(deviceId, ssid);
        printf("SSID         =   %s\n", ssid);
    }
    else {
        printf("un-connected\n");
    }

    /* PHY MODE */
    qcom_get_phy_mode(deviceId, &phyMode);
	
    if (phyMode == QCOM_11A_MODE) {
        printf("Phy Mode     =   a\n");
    } else if (phyMode == QCOM_11B_MODE) {
        printf("Phy Mode     =   b\n");
    } else if (phyMode == QCOM_11G_MODE) {
        printf("Phy Mode     =   g\n");
    } else if (phyMode == QCOM_11N_MODE) {
        printf("Phy Mode     =   n\n");
    } else if (phyMode == QCOM_HT40_MODE) {
        printf("Phy Mode     =   ht40\n");
    } else {
        printf("Phy Mode     =   mixed\n");
    }

    /* Power Mode */
    qcom_power_get_mode(deviceId, &powerMode); 
    if (powerMode == 1)         //REC_POWER
    {
        printf("Power Mode   =   Power Save\n");
    } else if (powerMode == 2)  //MAX_PERF_POWER
    {
        printf("Power Mode   =   Max Perf\n");
    } else {
        printf("Power Mode   =   Power Save\n"); //default is in sleep mode
    }
    printf("Mac Addr     =   %x:%x:%x:%x:%x:%x\n",
             macAddr[0], macAddr[1], macAddr[2], macAddr[3], macAddr[4], macAddr[5]);
    printf("Mode         =   %s\n", modeDesc[devMode]);

    /* Channel */
    qcom_get_channel(deviceId, &channel);
    if (channel >= 2412) {
        channel = (((channel - 2412) / 5) + 1);
    }
    printf("Channel      =   %d\n", channel);
    
    {
      A_UINT32 size;
      A_UINT8 macCnt;
      A_UINT8 macReap;
      A_UINT8 htcCnt; 
      A_UINT8 htcReap;
      A_UINT8 fwCnt;
      A_UINT8 fwReap;
      A_UINT8 freeCnt;
      A_UINT32 htcGet;
      A_UINT32 htcPut;
      
      qcom_free_heap_size_get(&size);
      qcom_buffer_info_get(&macCnt, &macReap, &htcCnt, &htcReap, &fwCnt, 
                           &fwReap, &freeCnt, &htcGet, &htcPut);
      
      printf("Heapsize     =   %d\n", size);
      printf("Buffer : Mac (%d+%d), HTC (%d+%d), FW (%d+%d), Free %d, tx (alloc %d, free %d)\n",  \
             macCnt, macReap, htcCnt, htcReap, fwCnt, fwReap, freeCnt, htcGet, htcPut);
    }

    {
        int i;
        unsigned char *pointTemp;
        printf("board data addr: 0x%08X:\n", *((unsigned int *)0x428854));
        pointTemp = (unsigned char *)(*((unsigned int *)0x428854));
        for (i = 0; i < 16; i++)
        {
            printf("%02X ", pointTemp[i]);
        }
        printf("\n");
    }
}

void Airkiss_WmiconfigWlanConnect(A_UINT8 deviceId, A_CHAR * ssid)
{
    A_UINT32 devMode;
	
    printf("Setting SSID to %s \n",(char *)ssid);

    /*Set callback*/
    //qcom_set_connect_callback(deviceId, wifi_conn_callback);

    qcom_op_get_mode(deviceId, &devMode);
    if (QCOM_WLAN_DEV_MODE_AP != devMode) {
        qcom_op_set_mode(deviceId, QCOM_WLAN_DEV_MODE_STATION); // station
    }
    
    qcom_set_ssid(deviceId, ssid);

    qcom_commit(deviceId);

    return;
}

void Airkiss_WmiconfigWlanDisc(A_UINT8 deviceId)
{
    qcom_disconnect(deviceId);

    if (qcom_sta_wlan_reconnect_start_flag[deviceId] == 1) {
        qcom_sta_reconnect_stop(deviceId);
    }
}

A_INT32 Airkiss_WmiconfigConnect(A_INT32 argc, A_CHAR * argv[])
{
    if (!A_STRCMP(argv[1], "--connect")) {
        if (argc == 3) {
            Airkiss_WmiconfigWlanConnect(currentDeviceId, (char *)argv[2]);
        } else {
            A_INT8 ssid[32];
            A_UINT8 i, j;

            j = 0;
            A_MEMSET(ssid, 0, 32);
            for (i=2; i<argc; i++)
            {
                if ((j + strlen(argv[i])) > 32)
                {
                    return AIRKISS_CMD_ERROR;
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

            Airkiss_WmiconfigWlanConnect(currentDeviceId, (char *) ssid);
        }

        return AIRKISS_CMD_OK;
    }

    return AIRKISS_CMD_NOFOUND;
}

void Airkiss_WmiconfigHelp(void)
{
   int i;
   
   for (i = 0; i < airkiss_cmds_num; i++) {
        /* printf can only print 256bytes */
        A_PRINTF("%s\n", airkiss_cmds[i].description);
        tx_thread_sleep(1);
   }

   return;
}

extern A_INT32 isdigit(A_INT32 c);

A_INT32 Airkiss_Inet_aton(const A_CHAR *name, A_UINT32 * ipaddrPtr)
{

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

   if (ipaddrPtr) {
      *ipaddrPtr = addr;
   } /* Endif */

    return -1;
}

A_CHAR * Airkiss_Inet_ntoa(A_UINT32 ip)
{
    static char str[20];
    A_MEMSET(str, 0, sizeof (str));
    qcom_sprintf(str, "%u.%u.%u.%u",
            (unsigned) (ip >> 24),
            (unsigned) ((ip >> 16) & 0xff), (unsigned) ((ip >> 8) & 0xff), (unsigned) (ip & 0xff));
    return str;
}

A_UINT32 Airkiss_InetAddr(A_CHAR *str)
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
