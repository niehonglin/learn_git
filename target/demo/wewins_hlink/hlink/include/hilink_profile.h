/*---------------------------------------------------------------------------
 * Copyright (c) <2013-2015>, <Huawei Technologies Co., Ltd>
 * All rights reserved.
 * Redistribution and use in source and binary forms, with or without modification,
 * are permitted provided that the following conditions are met:
 * 1. Redistributions of source code must retain the above copyright notice, this list of
 * conditions and the following disclaimer.
 * 2. Redistributions in binary form must reproduce the above copyright notice, this list
 * of conditions and the following disclaimer in the documentation and/or other materials
 * provided with the distribution.
 * 3. Neither the name of the copyright holder nor the names of its contributors may be used
 * to endorse or promote products derived from this software without specific prior written
 * permission.
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
 * "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO,
 * THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR
 * PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR
 * CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL,
 * EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO,
 * PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS;
 * OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY,
 * WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR
 * OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF
 * ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 *---------------------------------------------------------------------------*/
/*----------------------------------------------------------------------------
 * Notice of Export Control Law
 * ===============================================
 * Huawei Liteos may be subject to applicable export control laws and regulations, which might
 * include those applicable to Huawei Liteos of U.S. and the country in which you are located.
 * Import, export and usage of Huawei Liteos in any manner by you shall be in compliance with such
 * applicable export control laws and regulations.
 *---------------------------------------------------------------------------*/

/** @defgroup hilink_profile Hilink Device SDK
 *@ingroup kernel
 */
#ifndef PROFILE_H_
#define PROFILE_H_

#ifdef __cplusplus
extern "C" {
#endif

#ifndef HILINK_M2M_MAX_SVC_NUM
#define HILINK_M2M_MAX_SVC_NUM     16
#endif

/******************************************************************************
** Hilink Device SDK ���⿪�Žӿ�
******************************************************************************/

/**
 * @ingroup hilink_profile
 * �豸��Ϣ�Ľṹ�壬���豸�������ṩ
 */

typedef struct {
	const char* sn;				/**<�豸Ψһ��ʶ������sn�ţ����ȷ�Χ��0,40]*/
	const char* prodId;        	 /**<�豸HiLink��֤�ţ����ȷ�Χ��0,5]*/
	const char* model;			/**<�豸�ͺţ����ȷ�Χ��0,32]*/
	const char* dev_t;			/**<�豸���ͣ����ȷ�Χ��0,4]*/
	const char* manu;			/**<�豸�����̣����ȷ�Χ��0,4]*/
	const char* mac;			/**<�豸MAC��ַ���̶�32�ֽ�*/
	const char* hiv;			/**<�豸HilinkЭ��汾�����ȷ�Χ��0,32]*/
	const char* fwv;			/**<�豸�̼��汾�����ȷ�Χ[0,64]*/
	const char* hwv;			/**<�豸Ӳ���汾�����ȷ�Χ[0,64]*/
	const char* swv;			/**<�豸����汾�����ȷ�Χ[0,64]*/
	const int prot_t;			/**<�豸Э�����ͣ�ȡֵ��Χ[1,3]*/
} dev_info_t;

/**
 * @ingroup hilink_profile
 * ������Ϣ�Ľṹ�壬���豸�������ṩ
 */

typedef struct  {
    const char* st;            		/**<�������ͣ����ȷ�Χ��0,32]*/
    const char* svc_id;        /**<����ID�����ȷ�Χ��0,64]*/
} svc_info_t;

/**
 *@ingroup hilink_profile
 *@brief ��ʼ��Hilink Device SDK��
 *
 *@par ����:
 *��ʼ��Hilink Device�豸��Ϣ�ͷ�����Ϣ�Լ��������
 *@attention
 *<ul>
 *<li>ÿ��wifi���ӳɹ�֮�󣬵���һ�μ��ɡ�</li>
 *<li>Hilink Device SDK���Ὺ���ڴ����ڴ洢dev_info��svc��ֻ�ᱣ�����ǵ�ָ�롣���SDK����ʱ�����ͷ�������ָ�롣</li>
 *<li>dev_infoָ�벻��Ϊ�գ�</li>
 *<li>svcָ�벻��Ϊ�գ�</li>
 *<li>svcָ���Ӧ�Ľṹ���������������svc_num��ȣ�</li>
 *<li>�����������ȷ�����豸�����߻��̱�֤��</li>
 *<li>�ú������豸�����߻��̵��á�</li>
 *</ul>
 *
 *@param dev_info       [IN] �豸��Ϣ�ṹ��ָ�롣dev_infoָ�벻��Ϊ�գ����뱣֤��ε���ȷ�ԣ�
 *@param svc            [IN] ������Ϣ�ṹ������ָ�롣svcָ�벻��Ϊ�գ����뱣֤��ε���ȷ�ԣ�
 *@param svc_num        [IN] ���������������svc�Ĵ�С������ֻ֧��һ��,��ΧΪ[1,8]��svcָ���Ӧ�Ľṹ���������������svc_num��ȣ����뱣֤��ε���ȷ�ԣ�
 *
 *@retval 0       Hilink Device SDK ��ʼ���ɹ���
 *@retval -12     Hilink Device SDK ��ηǷ���
 *@retval -17     Hilink Device SDK��ȡflash������Ҫ����ýӿ����쳣����
 *@par ����:
 *<ul><li>hilink_profile.h���ýӿ��������ڵ�ͷ�ļ���</li></ul>
 *@see �ޡ�
 *@since Huawei Liteos V100R002C00
 */

extern int hilink_m2m_init(dev_info_t* dev_info, svc_info_t* svc, unsigned short svc_num);

/**
 *@ingroup hilink_profile
 *@brief ����Hilink Device SDK��
 *
 *@par ����:
 *����Hilink Device SDK��ʼ״̬������������hilink_m2m_init��ʼ���ɹ�����ã�
 *@attention
 *<ul>
 *<li>��Ҫѭ�����ã�������ü����󲻳���50���롣���ú�����ʱ��δ�����ã�����ܷ����쳣��Ϣ��</li>
 *<li>�ú������豸�����߻��̵��á�</li>
 *</ul>
 *
 *@param �ޡ�
 *
 *@retval 0     Hilink Device SDK ����������
 *@retval -3    Hilink Device SDK��ȡʱ�������Ҫ����ýӿ����쳣����
 *@retval -16   Hilink Device SDKдflash������Ҫ����ýӿ����쳣����
 *@retval -17   Hilink Device SDK��ȡflash������Ҫ����ýӿ����쳣����
 *@retval ��0   �������hilink_error.h��
 *@par ����:
 *<ul><li>hilink_profile.h���ýӿ��������ڵ�ͷ�ļ���</li></ul>
 *@see �ޡ�
 *@since Huawei Liteos V100R002C00
 */
extern int hilink_m2m_process(void);

/**
 *@ingroup hilink_profile
 *@brief ����m2m main task id
*
 *@par ����:
 *�� hilink_m2m_process ѭ��֮ǰ����
 *@attention
 *<ul>
  *<li>�ú������豸�����߻��̵��á�</li>
 *</ul>
 *
 *@param tid    [IN] hilink_m2m_process ����taskid
 *
 *@retval 0     ���óɹ�
 *@retval ��0   ����ʧ��
 *@par ����:
 *<ul><li>hilink_profile.h���ýӿ��������ڵ�ͷ�ļ���</li></ul>
 *@see �ޡ�
 *@since hilink sdk v1.0.3
 */

extern void hilink_m2m_set_taskid(int tid);

/**
 *@ingroup hilink_profile
 *@brief �ϱ�report��������״̬��
 *
 *@par ����:
 *�����ֶ�״̬�����ı������ϱ�����ƽ̨(������ƽ̨ʱ)����hilink����(����hilink����ʱ)��
 *@attention

 *<ul>
 *<li>ֻ����report�����������ֶ���ʹ�ô˽ӿ��ϱ��¼���ֻ������hilink_m2m_process��һ�������е���</li>
 *<li>�ú������豸�����߻��̵��á�</li>
 *</ul>
 *
 *@param svc_id         [IN] ����ID��
 *@param payload        [IN] Json��ʽ���ֶ�����ֵ
 *@param len            [IN] payload�ĳ���
 *@param tid            [IN] �ӿڵ�������taskid ��ͬm2m taskid, ���ڱ�֤ͬһ�����е���

 *@retval 0     ����״̬�ϱ��ɹ���
 *@retval ��0   ����״̬�ϱ����ɹ���
 *@par ����:
 *<ul><li>hilink_profile.h���ýӿ��������ڵ�ͷ�ļ���</li></ul>
 *@see hilink_get_char_state | hilink_put_char_state
 *@since hilink sdk v1.0.3
 */

extern int hilink_report_char_state(char* svc_id,
		char* payload, unsigned int len, int tid);
/**
 *@ingroup hilink_profile
 *@brief �ϱ�����״̬��
 *
 *@par ����:
 *�����ֶ�״̬�����ı������ϱ�����ƽ̨(������ƽ̨ʱ)����hilink����(����hilink����ʱ)��
 *@attention
 *<ul>
 *<li>�豸�����ӵ�hilink���ػ���ƽ̨ʱ�������ϱ�һ�η��������ֶε�״̬��Ŀǰ�˽ӿڼ�¼�ϱ��¼��������ֶ�״̬��Hilink Device SDKͳһ�ϱ��������ʱ200ms��</li>
 *<li>�ú������豸�����߻��̵��á�</li>
 *</ul>
 *
 *@param svc_id         [IN] ����ID��
 *@param payload        [IN] Json��ʽ���ֶ�����ֵ���˲�����δʹ�ã����鱣��������ΪNULL��
 *@param len            [IN] payload�ĳ��ȣ���ΧΪ[0��512)��������δʹ�ã����鱣��������Ϊ0��

 *@retval 0     ����״̬�ϱ��ɹ���
 *@retval ��0   ����״̬�ϱ����ɹ���
 *@par ����:
 *<ul><li>hilink_profile.h���ýӿ��������ڵ�ͷ�ļ���</li></ul>
 *@see hilink_get_char_state | hilink_put_char_state
 *@since Huawei Liteos V100R002C00
 */

extern int hilink_upload_char_state(char* svc_id,
		char* payload, unsigned int len);

/**
 *@ingroup hilink_profile
 *@brief ��ȡ�����ֶ�ֵ��
 *
 *@par ����:
 *��ȡ�豸�����ֶ�ֵ��ֻ�����ȡ����GET�����������ֶ�
 *@attention
 *<ul>
 *<li>��ȡ�����ֶ�ֵ��</li>
 *<li>�˺������豸�����߻���ʵ�֡�</li>
 *</ul>
 *
 *@param svc_id         [IN] ����ID������ʵ�ָú���ʱ����Ҫ��sid�жϡ�
 *@param in             [IN] ���յ���Json��ʽ���ֶ�����ֵ��
 *@param in_len         [IN] ���յ���in�ĳ��ȣ���ΧΪ[0��512)��
 *@param out            [OUT] ���ر�������ֶ�ֵ���ݵ�ָ��,�ڴ��ɳ��̿��٣�ʹ����ɺ���Hilink Device SDK�ͷš�
 *@param out_len        [OUT] ��ȡ����payload�ĳ��ȣ���ΧΪ[0��512)��
 *
 *@retval 0     ����״̬�ֶ�ֵ��ȡ�ɹ���
 *@retval ��0   ����״̬�ֶ�ֵ���ɹ���
 *@par ����:
 *<ul><li>hilink_profile.h���ýӿ��������ڵ�ͷ�ļ���</li></ul>
 *@see hilink_upload_char_state | hilink_put_char_state
 *@since Huawei Liteos V100R002C00
 */

extern int hilink_get_char_state(const char* svc_id, const char* in, unsigned int in_len, char** out, \
	unsigned int* out_len);

/**
 *@ingroup hilink_profile
 *@brief �޸ķ���ǰ�ֶ�ֵ��
 *

 *@par ����:
 *�޸��豸��ǰ�����ֶ�ֵ
 *@attention
 *<ul>
 *<li>֧���޸ķ����ָ���ֶε�ֵ��</li>
 *<li>�˺������豸�����߻���ʵ�֡�</li>
 *</ul>
 *

 *@param svc_id         [IN] ����ID��
 *@param payload        [IN] ���յ���Ҫ�޸ĵ�Json��ʽ���ֶ�����ֵ��
 *@param len            [IN] ���յ���payload�ĳ��ȣ���ΧΪ[0��512)��
 *

 *@retval #M2M_SEARCH_GW_INVALID_PACKET    -101����ñ��Ĳ�����Ҫ��
 *@retval #M2M_SVC_STUTAS_VALUE_MODIFYING  -111������״ֵ̬�����޸��У��޸ĳɹ���ײ��豸���������ϱ���
 *@retval #M2M_NO_ERROR                    0��   �޴��󣬱�ʾ����״ֵ̬�޸ĳɹ�������Ҫ�ײ��豸�����ϱ�����Hilink Device SDK�ϱ���
 *@par ����:
 *<ul><li>hilink_profile.h���ýӿ��������ڵ�ͷ�ļ���</li></ul>
 *@see hilink_upload_char_state | hilink_get_char_state
 *@since Huawei Liteos V100R002C00
 */

extern int hilink_put_char_state(const char* svc_id,
		const char* payload, unsigned int len);

/**
 *@ingroup hilink_profile
 *@brief ��ȡ�豸����״̬
 *

 *@par ����:
 *�豸��������״̬���ɸ���״̬����Ӧ�Ĵ���
 *@attention
 *<ul>

 *<li>�ú������豸�����߻��̵���</li>
 *</ul>

 *@retval 0     0 offline 1 online
 *<ul><li>hilink_profile.h���ýӿ��������ڵ�ͷ�ļ���</li></ul>
 *@see ��
 *@since hilink sdk v1.0.3
 */

extern int hilink_get_devstatus(void);
/**
 *@ingroup hilink_profile
 *@brief �豸��������֪ͨ
 *

 *@par ����:
 *�豸��������֪ͨ���ɸ���֪ͨ��Ϣ����Ӧ�Ĵ���
 *@attention
 *<ul>
 *<li>�˺������豸�����߻���ʵ��,��ֹ����ʱ����</li>
 *</ul>

 *
 *@param status            [IN] 0 offline 1 online
 *
 *@retval 0     �ɹ���
 *

 *<ul><li>hilink_profile.h���ýӿ��������ڵ�ͷ�ļ���</li></ul>
 *@see ��
 *@since hilink sdk v1.0.3
 */

extern void hilink_notify_devstatus(int status);

/**

 *@ingroup hilink_profile

 *@brief �޸�wifi������

 *

 *@par ����:

 *֪ͨdevice��Ӧ�ò��޸�WiFi������ʹ���µ�WiFi�������ӵ�ָ����·������

 *@attention

 *<ul>

 *<li>��ssid��pwd��Ϊ�գ���mode=-1ʱ����ʾ���WiFi������Ϣ��</li>

 *<li>��ssid��Ϊ��ʱ����ʾ�޸�WiFi������Ϣ��</li>

 *<li>�˺������豸�����߻���ʵ�֡�</li>

 *</ul>

 *

 *@param ssid           [IN] ·������SSID��

 *@param ssid_len       [IN] ·������SSID�ĳ��ȣ���ΧΪ[0��33)��

 *@param pwd            [IN] ·���������롣

 *@param pwd_len        [IN] ·����������ĳ��ȣ���ΧΪ[0��65)��

 *@param mode                 [IN] ·��������ģʽ����Χ[-4,6]��

 *

 *@retval 0     WiFi�����޸ĳɹ���

 *@retval ��0   WiFi�����޸Ĳ��ɹ���

 *@par ����:

 *<ul><li>hilink_profile.h���ýӿ��������ڵ�ͷ�ļ���</li></ul>

 *@see �ޡ�

 *@since Huawei Liteos V100R002C00

 */

extern int hilink_notify_wifi_param(char* ssid, unsigned int ssid_len,
		char* pwd, unsigned int pwd_len, int mode );


 /**

 *@ingroup hilink_profile

 *@brief �����̼��������������������

 *

 *@par ����:

 *���ػ��ƶ���Device���ʹ����̼���������������������

 *@attention

 *<ul>

 *<li>�˺������豸�����߻���ʵ�֡���Ҫ����ڲ�����⣬�������0��1Ϊ�Ϸ���</li>

 *</ul>

 *

 *@param mode           [IN] mode=0Ϊ�豸�̼����������mode=1��ʾ�豸�����̼���������ΧΪ[0,1]��

 *

 *@retval #M2M_NO_ERROR         0��   �޴���

 *@retval #M2M_OTA__DETECT_FAILURE     -900�������̼��������ʧ�ܡ�

 *@retval #M2M_OTA__START_FAILURE      -901���̼���������ʧ�ܡ�

 *@retval #M2M_ARG_INVALID			   -12�� �����Ч��

 *@par ����:

 *<ul><li>hilink_profile.h���ýӿ��������ڵ�ͷ�ļ���</li></ul>

 *@see �ޡ�

 *@since Huawei Liteos V100R002C00

 */

extern int hilink_ota_trig(int mode);


 /**

 *@ingroup hilink_profile

 *@brief ��ȡ�̼��汾������Ϣ��

 *

 *

 *@par ����:

 *@attention

 *<ul>

 *<li>��ǰ�̼��汾Ϊ����ʱ��version����ΪNULL��</li>

 *<li>�˺������豸�����߻���ʵ�֡�</li>

 *</ul>

 *

 *@param version                [OUT] �豸�̼��汾�š��ɳ��̿����ڴ棬���ر�������ֶ�ֵ���ݵ�ָ��,ʹ����ɺ���Hilink Device SDK�ͷš�

 *@param ver_len                [OUT] �豸�̼��汾���ȣ���ΧΪ[0��64)��

 *

 *@retval #M2M_NO_ERROR                           0��   �޴���

 *@retval #M2M_OTA_GET_VERSION_FAILURE            -902����ȡ�̼��汾��ʧ�ܡ�

 *@par ����:

 *<ul><li>hilink_profile.h���ýӿ��������ڵ�ͷ�ļ���</li></ul>

 *@see hilink_ota_rpt_ver

 *@since Huawei Liteos V100R002C00

 */

extern int hilink_ota_get_ver(char** version, int* ver_len);


 /**

 *@ingroup hilink_profile

 *@brief ��ȡ�̼��汾������Ϣ��

 *

 *@par ����:

 *@attention

 *<ul>

 *<li>��ǰ�̼��汾Ϊ����ʱ��introductionΪNULL��</li>

 *<li>�˺������豸�����߻���ʵ�֡�</li>

 *</ul>

 *

 *@param introduction           [OUT] �豸�̼��汾������Ϣ���ɳ��̿����ڴ棬���ر�������ֶ�ֵ���ݵ�ָ��,ʹ����ɺ���Hilink Device SDK�ͷš�

 *@param intro_len              [OUT] �豸�̼��汾������Ϣ���ȣ����鷶ΧΪ[0��512)��������ֳ���̫�����ᱻ�ضϴ���

 *

 *@retval #M2M_NO_ERROR                           0��   �޴���

 *@retval #M2M_OTA_GET_VERSION_INFO_FAILURE       -903����ȡ�̼��汾������Ϣʧ�ܡ�

 *@par ����:

 *<ul><li>hilink_profile.h���ýӿ��������ڵ�ͷ�ļ���</li></ul>

 *@see hilink_ota_rpt_ver

 *@since Huawei Liteos V100R002C00

 */

extern int hilink_ota_get_intro(char** introduction, int* intro_len);


 /**
 *@ingroup hilink_profile
 *@brief 上报固件设备版本信息。
 *
 *@par 描述:
 *向网关或云端上报Device固件版本信息。
 *@attention
 *<ul>
 *<li>目前此接口只记录上报事件，服务字段状态由Hilink Device SDK统一上报，大概延时200ms</li>
 *<li>此函数由设备开发者或厂商调用。</li>
 *</ul>
 *
 *@param version                [OUT] 设备固件版本号。此参数暂未使用，建议保留，设置为NULL。
 *@param ver_len                [OUT] 设备固件版本长度，范围为[0，64)。此参数暂未使用，建议保留，设置为0。
 *@param introduction           [OUT] 设备固件版本描述信息。此参数暂未使用，建议保留，设置为NULL。
 *@param intro_len              [OUT] 设备固件版本描述信息长度，建议范围为[0，512),如果出现长度太长，会被截断处理。此参数暂未使用，建议保留，设置为0。
 *
 *@retval 0     无错误。
 *@retval 非0   上报固件升级版本信息失败。
 *@par 依赖:
 *<ul><li>hilink_profile.h：该接口声明所在的头文件。</li></ul>
 *@see hilink_ota_rpt_prg
 *@since Huawei Liteos V100R002C00
 */
extern int hilink_ota_rpt_ver(char* version, int ver_len,
    char* introduction, int intro_len);


 /**
 *@ingroup hilink_profile
 *@brief 上报固件升级进度。
 *
 *@par 描述:
 *向网关或云端上报Device固件升级进度。目前此接口只记录上报事件，服务字段状态由Hilink Device SDK统一上报，大概延时200ms。
 *@attention
 *<ul>
 *<li>progress为[0,100]表示升级进度正常；progress=101表示升级失败。</li>
 *<li>progress=1000，无法网络通信</li>
 *<li>progress=1001，镜像网络下载异常</li>
 *<li>progress=1002，校验失败</li>
 *<li>progress=1003，写入失败</li>
 *<li>bootTime是对于无法计量升级进度的，如版本写入、重启等，通过该时间上报升级完成的大概时间。</li>
 *<li>此函数由设备开发者或厂商调用。</li>
 *</ul>
 *
 *@param progress                [IN] progress为[0,100]表示升级进度正常；progress=101表示升级失败。progress=1000，无法网络通信。progress=1001，镜像网络下载异常。progress=1002，校验失败。progress=1003，写入失败。
 *@param bootTime                [IN] bootTime大于0，表示设备重启等需要的时间。
 *
 *@retval #M2M_NO_ERROR                   0，    无错误。
 *@retval #M2M_OTA_RPT_PRG_FAILURE        -905， 上报固件升级进度失败。
 *@retval #M2M_ARG_INVALID				  -12，  入参无效。
 *@retval #M2M_UPLOAD_DISALBE             -803， 只能在模式为网关和云时，上传成功，否则返回失败。
 *@par 依赖:
 *<ul><li>hilink_profile.h：该接口声明所在的头文件。</li></ul>
 *@see hilink_ota_rpt_ver
 *@since Huawei Liteos V100R002C00
 */
extern int hilink_ota_rpt_prg(int progress, int bootTime);

/**
*@ingroup hilink_profile
*@brief 设备恢复出厂时设置设备注销标志。
*
*@par 描述:
*HiLink设备恢复出厂时，通过调用此接口设置注销标识。
*@attention
*<ul>
*<li>此函数由设备开发者或厂商调用。</li>
*</ul>
*@retval 0     设置注销标识成功。
*@retval 非0   设置注销标识失败。
*@par 依赖:
*<ul><li>hilink_profile.h：该接口声明所在的头文件。</li></ul>
*@see 无。
*@since Huawei Liteos V100R002C00
*/
extern int hilink_save_revoke_flag();



#ifdef __cplusplus
}
#endif


#endif  /* PROFILE_H_ */


