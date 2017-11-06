/**
 * @file
 */
/******************************************************************************
 * Copyright (c) 2014 Qualcomm Atheros, Inc.
 * All Rights Reserved.
 * Qualcomm Atheros Confidential and Proprietary.
 * $ATH_LICENSE_HOSTSDK0_C$

 *    Unless required by applicable law or agreed to in writing, software
 *    distributed under the license is distributed on an "AS IS" BASIS,
 *    WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 *    See the license for the specific language governing permissions and
 *    limitations under the license.
 ******************************************************************************/

#ifndef __INSTALL_PATCHES_H__
#define __INSTALL_PATCHES_H__

/* List of feature IDs for which application can chose to install
 * system patches. This ordering is important to take care of the cases
 * when 2 features patch the same function. 
 * For example, OTA and HTTP patches same functions where the later one 
 * invokes the first one. Always the one that directly invokes ROM call or 
 * patches the function completely by itself must be of lower numbers. 
 * The later  patch might call earlier patched functions(In this case, it 
 * is possible that OTA patch can call HTTP patch) which could inturn go 
 * to multiple levels of patch functions and then the ROM functions.
 * The order of patch installation is critical in such cases and logic 
 * used is always lower number to higher number which is why defining this 
 * macro in right order is important */
#define QCOM_SCAN_PATCH_ID              0
#define QCOM_DNS_PATCH_ID               1
#define QCOM_SNTP_PATCH_ID              2
#define QCOM_IPV6_PATCH_ID              3
#define QCOM_HTTP_PATCH_ID              4
#define QCOM_SSL_PATCH_ID               5
#define QCOM_STRRCL_PATCH_ID            6
#define QCOM_OTA_PATCH_ID               7
#define MAX_NUM_FEATURE_INSTALL_PATCHES 8

/* This function allows application to install patches for its interested 
 * features */
typedef void (*install_patch_t)(void);
void register_feature_install_patches(unsigned int idx, install_patch_t patch_fn);

void qcom_install_strrcl_patches(void);
void qcom_install_dns_patches(void);
void qcom_install_scan_patches(void);
void qcom_install_sntp_patches(void);
void qcom_install_ipv6_patches(void);
void qcom_install_http_patches(void);
void qcom_install_ota_patches(void);
void qcom_install_ssl_patches(void);



#endif /* __INSTALL_PATCHES_H__ */
