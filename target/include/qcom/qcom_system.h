/**
 * @file
 */
/******************************************************************************
 * Copyright (c) 2013 Qualcomm Atheros, Inc.
 * All Rights Reserved.
 * Qualcomm Atheros Confidential and Proprietary.
 * $ATH_LICENSE_HOSTSDK0_C$

 *    Unless required by applicable law or agreed to in writing, software
 *    distributed under the license is distributed on an "AS IS" BASIS,
 *    WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 *    See the license for the specific language governing permissions and
 *    limitations under the license.
 ******************************************************************************/

#ifndef __QCOM_SYSTEM_H__
#define __QCOM_SYSTEM_H__

#ifdef __cplusplus
extern "C" {
#endif

#define WLAN_NUM_OF_DEVICES 2

enum WDT_CMDS{
    APP_WDT_DISABLE,
    APP_WDT_ENABLE,
    APP_WDT_USER_DEF,
    APP_WDT_USER_DBG,
};

/**This functions should be called from the application app_start.c before intializing any of the uninitialized variables.
*The function is a wrapper for A_INIT() which on invocation clears the bss sections.
*/
void qcom_sys_targinit(void);

/**
 *This function is used to do the initial configuration before ROM continues to load board data and other content from NVRAM.
 *It must be called by the first executable image in NVRAM, typically sdk_proxy.out. It takes a 32-bit parameter. Bit[31-8]
 *is the user application start address passed to ROM (typically 0xa02800). You can take a look at
 *demos/sdk_shell/sdk_shell.target.ld and sdk_flash/1_0_sdk_flash.sh and see how the parameter is calculated and passed.
 *
 * @param param 32-bit parameters. Bit[31-8] is start address of user application (<<8). Bit[7-0] is reserved for future use.
 */
void qcom_sys_init(unsigned int param);

/**
 *This function is to start WLAN operation. It installs a lot of patches then calls wlan_main() to start the WLAN operation.
 *After wlan_main(), it also does some post-processing to setup WMI and HTC correctly. This function must be called firstly
 *before other user functions.
 *
 * @param fn		user main function. RTOS will schedule it after init
 */

typedef struct{
	unsigned char isMccEnabled;
	unsigned char numOfVdev;
}QCOM_SYS_START_PARA_t;
void qcom_sys_start(void(*fn)(void), QCOM_SYS_START_PARA_t *pSysStartPara);

/**
 *This function is to reset to chip. It is only a warm reset by pointing PC to reset vector. We may change it later to support
 watch dog reset or cold reset.
 */
void qcom_sys_reset(void);
void qcom_watchdog_feed(void);
int qcom_watchdog(int enable, int timeout);
void qcom_enable_print(int enable);
void qcom_enable_sleep (int enable);

/*
  Function : qcom_otp_write_mac
       This function writes a mac stream to OTP.
       Only 2 mac streams are allowed to be programed into OTP.
       The new mac address is valide after reboot.

  Parameters :
       unsigned char mac[] : INPUT: mac address.

   Return value
       int : error code
                      0   : OTP ok
                      -1 : OTP error
                      -2 : There is no enough free space for new mac stream in OTP
                      -3 : The number of mac streams exceeds the limited number(2 streams currently)
  */
int qcom_otp_write_mac(unsigned char mac[]);



int qcom_set_bmisstime(unsigned char device_id,unsigned int numBeacons, unsigned int  bmissTime);
int qcom_enable_ani(unsigned int enable);

#ifdef __cplusplus
}
#endif

#endif

