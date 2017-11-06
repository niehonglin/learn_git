#include "hilink_profile.h"
#include "hilink_osadapter.h"
#include <unistd.h>

#include "qcom_common.h"
#include "qcom_gpio.h"
#include "qcom_gpio_interrupts.h"
#include "wmi.h"
#include "qcom/qcom_wlan.h"
#include "threadx/tx_api.h"
#include "ww_hlink.h"
#include "hilink_link.h"
#include "jsmn.h"
#include "ww_hlink.h"



TX_THREAD channel_task_handle;
TX_THREAD beacon_task_handle;

char *channel_change_stack_pointer;
char *beacon_send_stack_pointer;
A_UINT8 cur_dev_id = 0;		//nhl
uint8_t config_net_flag = 0;

#define WMF_PRINT_4    printf
#define CHAN_CHA_STACK_SIZE (2* 1024 )
//#define BEAC_SEN_STACK_SIZE (2 * 1024 )
//#define HILINK_BEACON_PERIOD    50 
#define WIFI_RX_LEN0_OPEN    64
#define WIFI_RX_LEN0_WEP     72
#define WIFI_RX_LEN0_TKIP    84
#define WIFI_RX_LEN0_AES     80
//#define device_id 0
#define startIP 0xC0A801C3
#define endIP   0xC0A801C7
#define IP_LEASE    (60 * 60 * 1000)
static int s_cur_channel;
static hilink_s_context hilink_s_ctx;
static hilink_s_result hilink_s_res;

/* Hilink SmartLink runtime context instantiation */

static hilink_s_context hilink_s_ctx;
void channel_change_task_entry(ULONG which_thread)
{
    DEBUG;
    while(1)
    {
        if (0 == hilink_link_get_lock_ready())		//�ж��Ƿ���Ҫ�л��ŵ�
        {
            s_cur_channel++;
            if (s_cur_channel >= 13)
            {
               s_cur_channel = 1;
            }
            
            //s_cur_channel=6;
            /*切换信道*/
             qcom_set_channel(0,s_cur_channel);
            /*清除HiLink.Link的buffer及状态*/
            hilink_link_reset();		//ʹHiLink�ָ�Ϊ��ʼ״̬
        }

        tx_thread_sleep(100);
    }
}
static int  beacon_send = 0;

/*
struct {
	const char* sn;				//<�豸Ψһ��ʶ������sn�ţ����ȷ�Χ��0,40]
	const char* prodId;        	 //<�豸HiLink��֤�ţ����ȷ�Χ��0,5]
	const char* model;			//<�豸�ͺţ����ȷ�Χ��0,32]
	const char* dev_t;			//<�豸���ͣ����ȷ�Χ��0,4]
	const char* manu;			//<�豸�����̣����ȷ�Χ��0,4]
	const char* mac;			//<�豸MAC��ַ���̶�32�ֽ�
	const char* hiv;			//<�豸HilinkЭ��汾�����ȷ�Χ��0,32]
	const char* fwv;			//<�豸�̼��汾�����ȷ�Χ[0,64]
	const char* hwv;			//<�豸Ӳ���汾�����ȷ�Χ[0,64]
	const char* swv;			//<�豸����汾�����ȷ�Χ[0,64]
	const int prot_t;			//<�豸Э�����ͣ�ȡֵ��Χ[1,3]
} 
*/
dev_info_t air_condition = {
	"3",		//sn
	"9002",	//proid
	"",		//
	"",
	"",
	"345bbb0aefe0",		//mac add
	"v1.0.5",		//hilink pro
	"",		//firmware
	"",		//hardware
	"",		//software
	1,
};

 svc_info_t air_service[5] ={
	{"binarySwitch", "switch"},
	{"airConditioner", "airConditioner"},
	{"temperature", "temperature"},
	{"wind", "wind"},
	{"faultDetection", "faultDetection"},
 };

extern  jsmntok_t  *tokens;

//��ȡ�����ֶ�ֵ
int hilink_get_char_state(const char* svc_id, const char* in, unsigned int in_len, char** out, unsigned int* out_len)
{
	DEBUG;
	return 0;
}

int hilink_notify_wifi_param(char* ssid, unsigned int ssid_len,char* pwd, unsigned int pwd_len, int mode )
{
	DEBUG;
	return 0;
}

int hilink_ota_get_ver(char** version, int* ver_len)
{
	DEBUG;
	return 0;
}

int hilink_ota_get_intro(char** introduction, int* intro_len)
{
	DEBUG;
	return 0;
}

int hilink_ota_trig(int mode)
{
	DEBUG;
	return 0;
}

//�޸��豸��ǰ�����ֶ�ֵ
int hilink_put_char_state(const char* svc_id,
		const char* payload, unsigned int len)
{
	DEBUG;
	return 0;
}

int hilink_getssid(unsigned char *pssid, unsigned char *pssidlen)
{
	DEBUG;
	A_STATUS ret = -1;
	ret = qcom_get_ssid(0, pssid);
	if(ret == A_OK)
	{
		return 0;
	}
	else{
		return -1;
	}
	*pssidlen = strlen(pssid);
}

static void softap_callback(A_UINT8 device_id, int val)
{
	printf("softap_callback test dev %d, val %d\n", device_id, val);
}

int wmf_station_start(char *ssid,char *pass)
{
  A_UINT32 bss_number = 0;
  A_STATUS status     = 0;
  //A_UINT32 listen_time;
  //A_UINT8  rssi;
  //A_UINT16 channel;
  A_UINT32 ip      = 0;
  A_UINT32 mask    = 0;
  A_UINT32 gateway = 0;
  WMF_PRINT_4("station start : %s \t %s\n",ssid,pass);
  status = qcom_op_set_mode(cur_dev_id,QCOM_WLAN_DEV_MODE_STATION);
  
  //qcom_sta_get_rssi(cur_dev_id,&rssi);
  //WMF_PRINT("------sta_get_rssi:%d ------\n",rssi);
  //qcom_sta_get_listen_time(cur_dev_id,&listen_time);
  //WMF_PRINT("------sta_get_listen_time:%d ------\n",listen_time);
  //WMF_PRINT("------ap ssid=%s key=%s\n",station_cfg->ssid[index],station_cfg->pwd[index]);
  qcom_sec_set_passphrase(cur_dev_id,pass);	
  status = qcom_sta_connect_with_scan(cur_dev_id,ssid);
  //status = qcom_sta_connect_without_scan(cur_dev_id,ssid);
  if(status!=0){
    return 0;
  }
  //qcom_get_channel(cur_dev_id,&channel);
  //WMF_PRINT("scan channel = %d \n",channel);
  qcom_thread_msleep(5000);
  status = qcom_scan_get_bss_number(cur_dev_id,&bss_number);
  if(status!=0){
    return 0;
  }
  //WMF_PRINT_1("scan bss num = %d \n",bss_number);
  if(bss_number==0){
    return 0;
  }
  qcom_dhcpc_enable(cur_dev_id,TRUE);
  qcom_thread_msleep(5000);
  status = qcom_ip_address_get(cur_dev_id,&ip, &mask, &gateway);
#if 1
  if(ip == 0){
  	printf("get ip failed\n");
    //WMF_PRINT_1("ip=0\n");
    //qcom_ip_address_get(cur_dev_id,&ip, &mask, &gateway);
    //qcom_sys_reset();//or reset
    return 0;
  }
#endif
  //WMF_PRINT_4("ip addr=0x%02x,mask=0x%02x,gate:0x%02x\n",ip,mask,gateway);
  //qcom_set_connect_callback(cur_dev_id,wmf_connectin_callback);
  return 1;
}

void ap_start(void)
{
    char devicessid[33]={0};
    unsigned int devicessid_len = 0;
    char *hilink_sec_key="00000000000000000AF273"; /* 当前可以赋值为:后5用MAC地址后5个字节，前面补字符0*/
    char *hilink_sn="3";
    char *hilink_prod_id="9002";
    beacon_send = 1;
    char mac_str[18]={0};
    unsigned char mac[6];

    qcom_mac_get(0, mac);

    sprintf(mac_str,"%02x%02x%02x%02x%02x%02x",mac[0],mac[1],mac[2],mac[3],mac[4],mac[5]);

    printf("device mac: %s \n",mac_str);

	/*hilink_link_get_devicessid(const char* ssid_type,
                               const char* device_id,
                               const char* device_sn,
                               const char* sec_type,
                               const char* sec_key,
                               char* device_ssid_out, unsigned int* ssid_len_out) XIP_ATTRIBUTE_FUN;
                               */
    /**相关参数可以从 智能家庭产品设备名称显示规范 附表-HiLink认证设备清单 获得*/
    hilink_link_get_devicessid("11", hilink_prod_id, hilink_sn,
                              "B",  hilink_sec_key, devicessid, &devicessid_len);
	printf("device ssid = %s \n ",devicessid);
    /* output e.g  device ssid:Hi1190023B00000000000000000AF273,ssidlen:32 */

   // if(beacon_send==1)
    //qcom_ap_hidden_mode_enable(device_id,1);
    //else

    //qcom_ap_hidden_mode_enable(device_id,0);

    //state = qcom_op_set_mode(device_id,1);
    //state = qcom_set_ssid(device_id,devicessid);
    //state = qcom_set_channel(device_id,3);
    //state = qcom_set_phy_mode(device_id,3);
    //state = qcom_set_tx_power(device_id, 17);
   
    qcom_set_connect_callback(0, softap_callback);
    qcom_op_set_mode(0,1);
    qcom_ap_start(0,devicessid);
   
}
int smtlink_complete(void)
{
    int status = 0;
    memset(&hilink_s_res, 0, sizeof(hilink_s_result));
    status = hilink_link_get_result(&hilink_s_res);		//��������Ӧ��SSID������
   // printf("smtlink_complete status :%d \n",status);
    if(status == 0)
    {
         printf("hilink_s_res.ssid is :%s\n",hilink_s_res.ssid);
         printf("hilink_s_res.pwd is :%s\n",hilink_s_res.pwd);
        /**Notice 连接wifi即可停发 beacon,平台相关*/
        //beacon_send = 0;

        /**hilink_s_res.enc_type为 HI_WIFI_ENC_UNKNOWN时，需自适应获取加密方式，平台相关
           如果需要重启连接wifi则需要保存result和notifypacket*/
        //wifi_sta_connect(hilink_s_res.ssid, hilink_s_res.ssid_len, hilink_s_res.pwd, hilink_s_res.pwd_len, hilink_s_res.enc_type);
    }

    return status;
}
int smtlink_parse_cb(const void * frame, unsigned short len)
{
    int ret;
    int lock_channel = s_cur_channel;
   // int i;
#if 0
    if(len>303)
   {
    unsigned char mac[6]={0x44,0x6E,0xE5,0x69,0x68,0xF6};  //手机mac
    //IEEE80211_FRAME stFrame80211;
    //memcpy((void*)&stFrame80211, (const void*)frame, sizeof(stFrame80211));
      {
          printf("----len=[%d]\n\r",len);
         if((((char*)frame)[0]=0x88)&&(((char*)frame)[1]=0x41))
	 {
          for(i=0;i<24;i++)
          {
                printf("0x%02X,",((char*)frame)[i]);
          }
          printf("\n\r\n\r",len);
	}
      }
  }
#endif

#if 0
	if(frame!= NULL)
	{
		printf("frame length = %d \t\t  frame data: ",len);
		 for(i=0;i<24;i++)
	          {
	                printf("0x%02X,",((char*)frame)[i]);
	          }
	          printf("\n\r\n\r");
	}
#endif
   // printf("wangyu test ..................\n");
    ret = hilink_link_parse(frame, len);		//����������
    
    if(ret>0)
    {
      printf(" hilink_link_parse ret is :%d \n",ret );
    }
    if (ret == HI_WIFI_STATUS_FINISH)
    {
        smtlink_complete();
        /**to call sample_smtlink_complete get ssid and passpword*/
    }
    else if (ret == HI_WIFI_STATUS_CHANNEL_LOCKED)
    {
        printf("hilink_smtlink_lock at channel[%d]\n\r", lock_channel);
        /**to lock channel.考虑到多线程，hilink_link_parse 调用完后可能由于时间差，已经切频，这里可以重设信道.
        * 另外考虑到信道频谱物理特性，会有锁错信道的情况出现，如果芯片支持解析802.11 radio frame获得信道号，则可有效避免这样的问题
        */
        s_cur_channel = lock_channel;
        qcom_set_channel(0, lock_channel);
    }
    else if (ret == HI_WIFI_STATUS_CHANNEL_UNLOCKED)
    {
        /**to release channel*/
    }

    return ret;
}

extern PROMISC_MODE  promiscMode;
int start_hlink(void)
{
    int ret;
    int state = 0;
    hilink_s_pkt0len st_pkt0len;

    /* Print hilink version info */
    //printf("[HiLink] %s\r\n", hilink_link_get_version());
     /*����ΪAPģʽ*/
    ap_start();
#if 1
    /* Init runtime context */
    memset(&hilink_s_ctx, 0, sizeof(hilink_s_context));
    memset(&st_pkt0len,   0, sizeof(hilink_s_pkt0len));
    ret = hilink_link_init(&hilink_s_ctx);
    if(ret == 0)
    {
    	printf("Hilink init successful\n");
        //hilink_link_reset();		//nhl
       /*����wifi��ͬ�������͵�QOS Data��0��׼����,ƽ̨���,�����Բ��Ի���оƬ�̻��.
        APP ���͵����ݳ�����len = 0, �豸���յ��Ķ�Ӧwifi frame len����ֵ����ͬ�������ͳ��Ȳ�һ��*/
        st_pkt0len.len_open = WIFI_RX_LEN0_OPEN;
        st_pkt0len.len_wep  = WIFI_RX_LEN0_WEP;
        st_pkt0len.len_tkip = WIFI_RX_LEN0_TKIP;
        st_pkt0len.len_aes  = WIFI_RX_LEN0_AES;
        hilink_link_set_pkt0len(&st_pkt0len);
		config_net_flag = 1;

	//qcom_promiscuous_enable(1);		//ʧ�ܻ���ģʽ
#if 0
    	 /*�����л��ŵ�����,ƽ̨���*/
        channel_change_stack_pointer=qcom_mem_alloc(CHAN_CHA_STACK_SIZE);
        state = tx_thread_create(&channel_task_handle, 
				"channel_change", 
				 channel_change_task_entry, 
				 0, 
				 channel_change_stack_pointer, 
                                 CHAN_CHA_STACK_SIZE, 16, 16, 4, TX_AUTO_START);
        if (state < 0)
        {
            printf("create channel_change_task fail!\r\n");
        }
        //printf("create channel_change_task suucess!\r\n");
       
	
        //ap_start();
	promiscMode = PROMISC_MODE_NORMAL;
	qcom_promiscuous_enable(1);
      	/*ע��802.11 ���ݽ��ջص�,ƽ̨���*/
        //sample_wifi_register_cb(sample_smtlink_parse_cb, NULL);
	//qcom_set_promiscuous_rx_cb((QCOM_PROMISCUOUS_CB)smartconfig_promisc_rx_cb);
	state=qcom_set_promiscuous_rx_cb((QCOM_PROMISCUOUS_CB)smtlink_parse_cb);
#endif
        printf("wangyu test 2..............\n");
    }
#endif

    return ret;
}
void hlink_entry(ULONG which_thread)
{
	int ret = 0;
	uint8_t router_flag= 0;
	uint8_t init_SDK_flag= 0;
#define PRINTF_ENBALE 1
	extern void user_pre_init(void);
	user_pre_init();
	qcom_enable_print(PRINTF_ENBALE);
	//Add all the alternate configurations for each GPIO pin
	//add_gpio_configurations();
	config_net_flag = 0;
	//test
   	 start_hlink();
	//qcom_op_set_mode(cur_dev_id,QCOM_WLAN_DEV_MODE_STATION);

	if(config_net_flag )
	{
		printf("hlink entry \t setup station mode \n");	
	RETRY:
		ret = wmf_station_start("tenda_cdwewin","cdww-9527");
		if(ret > 0)
		{
			printf("connect router successful\n");
			router_flag = 1;
		}else
		{
			printf("connect router fail\n");
			goto RETRY;
		}
	}

	if(router_flag)
	{
		//��ʼ��Hilink Device SDK
		ret = hilink_m2m_init(&air_condition,(svc_info_t *)&air_service,5);
		hilink_m2m_set_taskid(0);
		if(ret == 0)
		{
			printf("init hilink device SDK successful\n ");
			init_SDK_flag = 1;
		}else
		{
			printf("init hilink sdk failed,ret = %d \n",ret);
		}
	}
	char *json_str = "{\
		\"name\":\"zhangsan\",\
		\"age\":\"18\" ,\
		\"sex\":\"male\",\
		\"weight\":\"60\"\
		}";
	void * json_ret = NULL;
	json_ret =(void*) hilink_json_parse(json_str);
	if(!json_ret)
		printf("json parse failure\n");
	else
		print_json();
#if 0
	int length= 0;
	int value = 0;
	char * return_ptr = NULL;
	return_ptr = hilink_json_get_string_value(tokens+1,"name",&length);
	if(return_ptr)
		printf("%s \n",return_ptr);

	ret = hilink_json_get_number_value(tokens+3,"age",&value);
	if(ret == 0)
		printf("the json value is %d \n",value);
#endif

   	while(1)
	{
		if(init_SDK_flag)
		{
			//����Hilink Device SDK
			ret = hilink_m2m_process();		
			if(ret == 0)
			{
				printf("hilink sdk run ok\n");
			}else{
				printf("hilink sdk run error , ret = %d \n",ret);
			}
		}
	
		tx_thread_sleep(30);
	}
}
