/*
  * Copyright (c) 2014 Qualcomm Atheros, Inc..
  * All Rights Reserved.
  * Qualcomm Atheros Confidential and Proprietary.
  */

#include "qcom_system.h"
/*  this head file should not be exported to customers directly. */
//#include <osapi.h>
#include "base.h"
#include "targaddrs.h"
#include "install_patches.h"

void user_main(void);

void
app_start(void)
{
	QCOM_SYS_START_PARA_t sysStartPara;
    qcom_sys_targinit();
	sysStartPara.isMccEnabled = 0;
	sysStartPara.numOfVdev = 1;
	
#if defined(FPGA)
    #if defined(ENABLED_MCC_MODE)
    sysStartPara.isMccEnabled = ENABLED_MCC_MODE;
    #endif

    #if defined(NUM_OF_VDEVS)
    sysStartPara.numOfVdev = NUM_OF_VDEVS;
    #endif
#else
    /*
    * For asic version, the MCC and device number are set in sdk_proxy.
    */
    if (HOST_INTEREST->hi_option_flag2 & HI_OPTION_MCC_ENABLE) {
        sysStartPara.isMccEnabled = 1;
    }

    sysStartPara.numOfVdev = ((HOST_INTEREST->hi_option_flag >> HI_OPTION_NUM_DEV_SHIFT) & HI_OPTION_NUM_DEV_MASK);
#endif

#ifdef AR6002_REV74
    /* Install feature patches. These function pointers should be installed 
     * before ROM code calls install_patches() */
    register_feature_install_patches(QCOM_SCAN_PATCH_ID, qcom_install_scan_patches);
    register_feature_install_patches(QCOM_DNS_PATCH_ID, qcom_install_dns_patches); 
    register_feature_install_patches(QCOM_SNTP_PATCH_ID, qcom_install_sntp_patches); 
    register_feature_install_patches(QCOM_IPV6_PATCH_ID, qcom_install_ipv6_patches); 
    register_feature_install_patches(QCOM_HTTP_PATCH_ID, qcom_install_http_patches); 
    register_feature_install_patches(QCOM_STRRCL_PATCH_ID, qcom_install_strrcl_patches); 
    register_feature_install_patches(QCOM_SSL_PATCH_ID, qcom_install_ssl_patches); 
    register_feature_install_patches(QCOM_OTA_PATCH_ID, qcom_install_ota_patches); 

    /* Install more patch install functions here. If a patch is not needed, install 
     * it will NULL. Otherwise it can cause crash */
#endif /* AR6002_REV74 */

    qcom_sys_start(user_main, &sysStartPara);


}


