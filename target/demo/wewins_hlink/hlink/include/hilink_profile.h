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
** Hilink Device SDK 对外开放接口
******************************************************************************/

/**
 * @ingroup hilink_profile
 * 设备信息的结构体，由设备开发者提供
 */

typedef struct {
	const char* sn;				/**<设备唯一标识，比如sn号，长度范围（0,40]*/
	const char* prodId;        	 /**<设备HiLink认证号，长度范围（0,5]*/
	const char* model;			/**<设备型号，长度范围（0,32]*/
	const char* dev_t;			/**<设备类型，长度范围（0,4]*/
	const char* manu;			/**<设备制造商，长度范围（0,4]*/
	const char* mac;			/**<设备MAC地址，固定32字节*/
	const char* hiv;			/**<设备Hilink协议版本，长度范围（0,32]*/
	const char* fwv;			/**<设备固件版本，长度范围[0,64]*/
	const char* hwv;			/**<设备硬件版本，长度范围[0,64]*/
	const char* swv;			/**<设备软件版本，长度范围[0,64]*/
	const int prot_t;			/**<设备协议类型，取值范围[1,3]*/
} dev_info_t;

/**
 * @ingroup hilink_profile
 * 服务信息的结构体，由设备开发者提供
 */

typedef struct  {
    const char* st;            		/**<服务类型，长度范围（0,32]*/
    const char* svc_id;        /**<服务ID，长度范围（0,64]*/
} svc_info_t;

/**
 *@ingroup hilink_profile
 *@brief 初始化Hilink Device SDK。
 *
 *@par 描述:
 *初始化Hilink Device设备信息和服务信息以及服务个数
 *@attention
 *<ul>
 *<li>每次wifi连接成功之后，调用一次即可。</li>
 *<li>Hilink Device SDK不会开辟内存用于存储dev_info和svc，只会保存它们的指针。因此SDK运行时不能释放这两个指针。</li>
 *<li>dev_info指针不能为空！</li>
 *<li>svc指针不能为空！</li>
 *<li>svc指针对应的结构体数组个数必须与svc_num相等！</li>
 *<li>函数的入参正确性由设备开发者或厂商保证。</li>
 *<li>该函数由设备开发者或厂商调用。</li>
 *</ul>
 *
 *@param dev_info       [IN] 设备信息结构体指针。dev_info指针不能为空！必须保证入参的正确性！
 *@param svc            [IN] 服务信息结构体数组指针。svc指针不能为空！必须保证入参的正确性！
 *@param svc_num        [IN] 服务个数，即参数svc的大小，建议只支持一个,范围为[1,8]。svc指针对应的结构体数组个数必须与svc_num相等！必须保证入参的正确性！
 *
 *@retval 0       Hilink Device SDK 初始化成功。
 *@retval -12     Hilink Device SDK 入参非法。
 *@retval -17     Hilink Device SDK读取flash错误，需要其调用接口做异常处理。
 *@par 依赖:
 *<ul><li>hilink_profile.h：该接口声明所在的头文件。</li></ul>
 *@see 无。
 *@since Huawei Liteos V100R002C00
 */

extern int hilink_m2m_init(dev_info_t* dev_info, svc_info_t* svc, unsigned short svc_num);

/**
 *@ingroup hilink_profile
 *@brief 启动Hilink Device SDK。
 *
 *@par 描述:
 *启动Hilink Device SDK开始状态机管理，必须在hilink_m2m_init初始化成功后调用！
 *@attention
 *<ul>
 *<li>需要循环调用，建议调用间隔最大不超过50毫秒。若该函数长时间未被调用，则可能返回异常信息。</li>
 *<li>该函数由设备开发者或厂商调用。</li>
 *</ul>
 *
 *@param 无。
 *
 *@retval 0     Hilink Device SDK 运行正常。
 *@retval -3    Hilink Device SDK获取时间错误，需要其调用接口做异常处理。
 *@retval -16   Hilink Device SDK写flash错误，需要其调用接口做异常处理。
 *@retval -17   Hilink Device SDK读取flash错误，需要其调用接口做异常处理。
 *@retval 非0   其他详见hilink_error.h。
 *@par 依赖:
 *<ul><li>hilink_profile.h：该接口声明所在的头文件。</li></ul>
 *@see 无。
 *@since Huawei Liteos V100R002C00
 */
extern int hilink_m2m_process(void);

/**
 *@ingroup hilink_profile
 *@brief 设置m2m main task id
*
 *@par 描述:
 *在 hilink_m2m_process 循环之前调用
 *@attention
 *<ul>
  *<li>该函数由设备开发者或厂商调用。</li>
 *</ul>
 *
 *@param tid    [IN] hilink_m2m_process 所在taskid
 *
 *@retval 0     设置成功
 *@retval 非0   设置失败
 *@par 依赖:
 *<ul><li>hilink_profile.h：该接口声明所在的头文件。</li></ul>
 *@see 无。
 *@since hilink sdk v1.0.3
 */

extern void hilink_m2m_set_taskid(int tid);

/**
 *@ingroup hilink_profile
 *@brief 上报report能力属性状态。
 *
 *@par 描述:
 *服务字段状态发生改变主动上报到云平台(连接云平台时)或者hilink网关(连接hilink网关时)。
 *@attention

 *<ul>
 *<li>只具有report能力的属性字段需使用此接口上报事件，只允许与hilink_m2m_process在一个任务中调用</li>
 *<li>该函数由设备开发者或厂商调用。</li>
 *</ul>
 *
 *@param svc_id         [IN] 服务ID。
 *@param payload        [IN] Json格式的字段与其值
 *@param len            [IN] payload的长度
 *@param tid            [IN] 接口调用所在taskid 需同m2m taskid, 用于保证同一任务中调用

 *@retval 0     服务状态上报成功。
 *@retval 非0   服务状态上报不成功。
 *@par 依赖:
 *<ul><li>hilink_profile.h：该接口声明所在的头文件。</li></ul>
 *@see hilink_get_char_state | hilink_put_char_state
 *@since hilink sdk v1.0.3
 */

extern int hilink_report_char_state(char* svc_id,
		char* payload, unsigned int len, int tid);
/**
 *@ingroup hilink_profile
 *@brief 上报服务状态。
 *
 *@par 描述:
 *服务字段状态发生改变主动上报到云平台(连接云平台时)或者hilink网关(连接hilink网关时)。
 *@attention
 *<ul>
 *<li>设备在连接到hilink网关或云平台时会主动上报一次服务所有字段的状态。目前此接口记录上报事件，服务字段状态由Hilink Device SDK统一上报，大概延时200ms。</li>
 *<li>该函数由设备开发者或厂商调用。</li>
 *</ul>
 *
 *@param svc_id         [IN] 服务ID。
 *@param payload        [IN] Json格式的字段与其值，此参数暂未使用，建议保留，设置为NULL。
 *@param len            [IN] payload的长度，范围为[0，512)，参数暂未使用，建议保留，设置为0。

 *@retval 0     服务状态上报成功。
 *@retval 非0   服务状态上报不成功。
 *@par 依赖:
 *<ul><li>hilink_profile.h：该接口声明所在的头文件。</li></ul>
 *@see hilink_get_char_state | hilink_put_char_state
 *@since Huawei Liteos V100R002C00
 */

extern int hilink_upload_char_state(char* svc_id,
		char* payload, unsigned int len);

/**
 *@ingroup hilink_profile
 *@brief 获取服务字段值。
 *
 *@par 描述:
 *获取设备服务字段值，只允许获取具有GET能力的属性字段
 *@attention
 *<ul>
 *<li>获取服务字段值。</li>
 *<li>此函数由设备开发者或厂商实现。</li>
 *</ul>
 *
 *@param svc_id         [IN] 服务ID。厂商实现该函数时，需要对sid判断。
 *@param in             [IN] 接收到的Json格式的字段与其值。
 *@param in_len         [IN] 接收到的in的长度，范围为[0，512)。
 *@param out            [OUT] 返回保存服务字段值内容的指针,内存由厂商开辟，使用完成后，由Hilink Device SDK释放。
 *@param out_len        [OUT] 读取到的payload的长度，范围为[0，512)。
 *
 *@retval 0     服务状态字段值获取成功。
 *@retval 非0   服务状态字段值不成功。
 *@par 依赖:
 *<ul><li>hilink_profile.h：该接口声明所在的头文件。</li></ul>
 *@see hilink_upload_char_state | hilink_put_char_state
 *@since Huawei Liteos V100R002C00
 */

extern int hilink_get_char_state(const char* svc_id, const char* in, unsigned int in_len, char** out, \
	unsigned int* out_len);

/**
 *@ingroup hilink_profile
 *@brief 修改服务当前字段值。
 *

 *@par 描述:
 *修改设备当前服务字段值
 *@attention
 *<ul>
 *<li>支持修改服务的指定字段的值。</li>
 *<li>此函数由设备开发者或厂商实现。</li>
 *</ul>
 *

 *@param svc_id         [IN] 服务ID。
 *@param payload        [IN] 接收到需要修改的Json格式的字段与其值。
 *@param len            [IN] 接收到的payload的长度，范围为[0，512)。
 *

 *@retval #M2M_SEARCH_GW_INVALID_PACKET    -101，获得报文不符合要求。
 *@retval #M2M_SVC_STUTAS_VALUE_MODIFYING  -111，服务状态值正在修改中，修改成功后底层设备必须主动上报。
 *@retval #M2M_NO_ERROR                    0，   无错误，表示服务状态值修改成功，不需要底层设备主动上报，由Hilink Device SDK上报。
 *@par 依赖:
 *<ul><li>hilink_profile.h：该接口声明所在的头文件。</li></ul>
 *@see hilink_upload_char_state | hilink_get_char_state
 *@since Huawei Liteos V100R002C00
 */

extern int hilink_put_char_state(const char* svc_id,
		const char* payload, unsigned int len);

/**
 *@ingroup hilink_profile
 *@brief 获取设备在线状态
 *

 *@par 描述:
 *设备离线上线状态，可根据状态做相应的处理
 *@attention
 *<ul>

 *<li>该函数由设备开发者或厂商调用</li>
 *</ul>

 *@retval 0     0 offline 1 online
 *<ul><li>hilink_profile.h：该接口声明所在的头文件。</li></ul>
 *@see 无
 *@since hilink sdk v1.0.3
 */

extern int hilink_get_devstatus(void);
/**
 *@ingroup hilink_profile
 *@brief 设备离线上线通知
 *

 *@par 描述:
 *设备离线上线通知，可根据通知消息做相应的处理
 *@attention
 *<ul>
 *<li>此函数由设备开发者或厂商实现,禁止做耗时操作</li>
 *</ul>

 *
 *@param status            [IN] 0 offline 1 online
 *
 *@retval 0     成功。
 *

 *<ul><li>hilink_profile.h：该接口声明所在的头文件。</li></ul>
 *@see 无
 *@since hilink sdk v1.0.3
 */

extern void hilink_notify_devstatus(int status);

/**

 *@ingroup hilink_profile

 *@brief 修改wifi参数。

 *

 *@par 描述:

 *通知device的应用层修改WiFi参数，使用新的WiFi参数连接到指定的路由器。

 *@attention

 *<ul>

 *<li>当ssid、pwd均为空，且mode=-1时，表示清除WiFi参数信息。</li>

 *<li>当ssid不为空时，表示修改WiFi参数信息。</li>

 *<li>此函数由设备开发者或厂商实现。</li>

 *</ul>

 *

 *@param ssid           [IN] 路由器的SSID。

 *@param ssid_len       [IN] 路由器的SSID的长度，范围为[0，33)。

 *@param pwd            [IN] 路由器的密码。

 *@param pwd_len        [IN] 路由器的密码的长度，范围为[0，65)。

 *@param mode                 [IN] 路由器加密模式，范围[-4,6]。

 *

 *@retval 0     WiFi参数修改成功。

 *@retval 非0   WiFi参数修改不成功。

 *@par 依赖:

 *<ul><li>hilink_profile.h：该接口声明所在的头文件。</li></ul>

 *@see 无。

 *@since Huawei Liteos V100R002C00

 */

extern int hilink_notify_wifi_param(char* ssid, unsigned int ssid_len,
		char* pwd, unsigned int pwd_len, int mode );


 /**

 *@ingroup hilink_profile

 *@brief 触发固件检测升级或启动升级。

 *

 *@par 描述:

 *网关或云端向Device发送触发固件检测升级或启动升级命令。

 *@attention

 *<ul>

 *<li>此函数由设备开发者或厂商实现。需要做入口参数检测，传入参数0和1为合法。</li>

 *</ul>

 *

 *@param mode           [IN] mode=0为设备固件检测升级，mode=1表示设备启动固件升级，范围为[0,1]。

 *

 *@retval #M2M_NO_ERROR         0，   无错误。

 *@retval #M2M_OTA__DETECT_FAILURE     -900，触发固件检测升级失败。

 *@retval #M2M_OTA__START_FAILURE      -901，固件启动升级失败。

 *@retval #M2M_ARG_INVALID			   -12， 入参无效。

 *@par 依赖:

 *<ul><li>hilink_profile.h：该接口声明所在的头文件。</li></ul>

 *@see 无。

 *@since Huawei Liteos V100R002C00

 */

extern int hilink_ota_trig(int mode);


 /**

 *@ingroup hilink_profile

 *@brief 获取固件版本描述信息。

 *

 *

 *@par 描述:

 *@attention

 *<ul>

 *<li>当前固件版本为最新时，version返回为NULL。</li>

 *<li>此函数由设备开发者或厂商实现。</li>

 *</ul>

 *

 *@param version                [OUT] 设备固件版本号。由厂商开辟内存，返回保存服务字段值内容的指针,使用完成后，由Hilink Device SDK释放。

 *@param ver_len                [OUT] 设备固件版本长度，范围为[0，64)。

 *

 *@retval #M2M_NO_ERROR                           0，   无错误。

 *@retval #M2M_OTA_GET_VERSION_FAILURE            -902，获取固件版本号失败。

 *@par 依赖:

 *<ul><li>hilink_profile.h：该接口声明所在的头文件。</li></ul>

 *@see hilink_ota_rpt_ver

 *@since Huawei Liteos V100R002C00

 */

extern int hilink_ota_get_ver(char** version, int* ver_len);


 /**

 *@ingroup hilink_profile

 *@brief 获取固件版本描述信息。

 *

 *@par 描述:

 *@attention

 *<ul>

 *<li>当前固件版本为最新时，introduction为NULL。</li>

 *<li>此函数由设备开发者或厂商实现。</li>

 *</ul>

 *

 *@param introduction           [OUT] 设备固件版本描述信息。由厂商开辟内存，返回保存服务字段值内容的指针,使用完成后，由Hilink Device SDK释放。

 *@param intro_len              [OUT] 设备固件版本描述信息长度，建议范围为[0，512)，如果出现长度太长，会被截断处理。

 *

 *@retval #M2M_NO_ERROR                           0，   无错误。

 *@retval #M2M_OTA_GET_VERSION_INFO_FAILURE       -903，获取固件版本描述信息失败。

 *@par 依赖:

 *<ul><li>hilink_profile.h：该接口声明所在的头文件。</li></ul>

 *@see hilink_ota_rpt_ver

 *@since Huawei Liteos V100R002C00

 */

extern int hilink_ota_get_intro(char** introduction, int* intro_len);


 /**
 *@ingroup hilink_profile
 *@brief 涓婃姤鍥轰欢璁惧鐗堟湰淇℃伅銆�
 *
 *@par 鎻忚堪:
 *鍚戠綉鍏虫垨浜戠涓婃姤Device鍥轰欢鐗堟湰淇℃伅銆�
 *@attention
 *<ul>
 *<li>鐩墠姝ゆ帴鍙ｅ彧璁板綍涓婃姤浜嬩欢锛屾湇鍔″瓧娈电姸鎬佺敱Hilink Device SDK缁熶竴涓婃姤锛屽ぇ姒傚欢鏃�200ms</li>
 *<li>姝ゅ嚱鏁扮敱璁惧寮�鍙戣�呮垨鍘傚晢璋冪敤銆�</li>
 *</ul>
 *
 *@param version                [OUT] 璁惧鍥轰欢鐗堟湰鍙枫�傛鍙傛暟鏆傛湭浣跨敤锛屽缓璁繚鐣欙紝璁剧疆涓篘ULL銆�
 *@param ver_len                [OUT] 璁惧鍥轰欢鐗堟湰闀垮害锛岃寖鍥翠负[0锛�64)銆傛鍙傛暟鏆傛湭浣跨敤锛屽缓璁繚鐣欙紝璁剧疆涓�0銆�
 *@param introduction           [OUT] 璁惧鍥轰欢鐗堟湰鎻忚堪淇℃伅銆傛鍙傛暟鏆傛湭浣跨敤锛屽缓璁繚鐣欙紝璁剧疆涓篘ULL銆�
 *@param intro_len              [OUT] 璁惧鍥轰欢鐗堟湰鎻忚堪淇℃伅闀垮害锛屽缓璁寖鍥翠负[0锛�512),濡傛灉鍑虹幇闀垮害澶暱锛屼細琚埅鏂鐞嗐�傛鍙傛暟鏆傛湭浣跨敤锛屽缓璁繚鐣欙紝璁剧疆涓�0銆�
 *
 *@retval 0     鏃犻敊璇��
 *@retval 闈�0   涓婃姤鍥轰欢鍗囩骇鐗堟湰淇℃伅澶辫触銆�
 *@par 渚濊禆:
 *<ul><li>hilink_profile.h锛氳鎺ュ彛澹版槑鎵�鍦ㄧ殑澶存枃浠躲��</li></ul>
 *@see hilink_ota_rpt_prg
 *@since Huawei Liteos V100R002C00
 */
extern int hilink_ota_rpt_ver(char* version, int ver_len,
    char* introduction, int intro_len);


 /**
 *@ingroup hilink_profile
 *@brief 涓婃姤鍥轰欢鍗囩骇杩涘害銆�
 *
 *@par 鎻忚堪:
 *鍚戠綉鍏虫垨浜戠涓婃姤Device鍥轰欢鍗囩骇杩涘害銆傜洰鍓嶆鎺ュ彛鍙褰曚笂鎶ヤ簨浠讹紝鏈嶅姟瀛楁鐘舵�佺敱Hilink Device SDK缁熶竴涓婃姤锛屽ぇ姒傚欢鏃�200ms銆�
 *@attention
 *<ul>
 *<li>progress涓篬0,100]琛ㄧず鍗囩骇杩涘害姝ｅ父锛沺rogress=101琛ㄧず鍗囩骇澶辫触銆�</li>
 *<li>progress=1000锛屾棤娉曠綉缁滈�氫俊</li>
 *<li>progress=1001锛岄暅鍍忕綉缁滀笅杞藉紓甯�</li>
 *<li>progress=1002锛屾牎楠屽け璐�</li>
 *<li>progress=1003锛屽啓鍏ュけ璐�</li>
 *<li>bootTime鏄浜庢棤娉曡閲忓崌绾ц繘搴︾殑锛屽鐗堟湰鍐欏叆銆侀噸鍚瓑锛岄�氳繃璇ユ椂闂翠笂鎶ュ崌绾у畬鎴愮殑澶ф鏃堕棿銆�</li>
 *<li>姝ゅ嚱鏁扮敱璁惧寮�鍙戣�呮垨鍘傚晢璋冪敤銆�</li>
 *</ul>
 *
 *@param progress                [IN] progress涓篬0,100]琛ㄧず鍗囩骇杩涘害姝ｅ父锛沺rogress=101琛ㄧず鍗囩骇澶辫触銆俻rogress=1000锛屾棤娉曠綉缁滈�氫俊銆俻rogress=1001锛岄暅鍍忕綉缁滀笅杞藉紓甯搞�俻rogress=1002锛屾牎楠屽け璐ャ�俻rogress=1003锛屽啓鍏ュけ璐ャ��
 *@param bootTime                [IN] bootTime澶т簬0锛岃〃绀鸿澶囬噸鍚瓑闇�瑕佺殑鏃堕棿銆�
 *
 *@retval #M2M_NO_ERROR                   0锛�    鏃犻敊璇��
 *@retval #M2M_OTA_RPT_PRG_FAILURE        -905锛� 涓婃姤鍥轰欢鍗囩骇杩涘害澶辫触銆�
 *@retval #M2M_ARG_INVALID				  -12锛�  鍏ュ弬鏃犳晥銆�
 *@retval #M2M_UPLOAD_DISALBE             -803锛� 鍙兘鍦ㄦā寮忎负缃戝叧鍜屼簯鏃讹紝涓婁紶鎴愬姛锛屽惁鍒欒繑鍥炲け璐ャ��
 *@par 渚濊禆:
 *<ul><li>hilink_profile.h锛氳鎺ュ彛澹版槑鎵�鍦ㄧ殑澶存枃浠躲��</li></ul>
 *@see hilink_ota_rpt_ver
 *@since Huawei Liteos V100R002C00
 */
extern int hilink_ota_rpt_prg(int progress, int bootTime);

/**
*@ingroup hilink_profile
*@brief 璁惧鎭㈠鍑哄巶鏃惰缃澶囨敞閿�鏍囧織銆�
*
*@par 鎻忚堪:
*HiLink璁惧鎭㈠鍑哄巶鏃讹紝閫氳繃璋冪敤姝ゆ帴鍙ｈ缃敞閿�鏍囪瘑銆�
*@attention
*<ul>
*<li>姝ゅ嚱鏁扮敱璁惧寮�鍙戣�呮垨鍘傚晢璋冪敤銆�</li>
*</ul>
*@retval 0     璁剧疆娉ㄩ攢鏍囪瘑鎴愬姛銆�
*@retval 闈�0   璁剧疆娉ㄩ攢鏍囪瘑澶辫触銆�
*@par 渚濊禆:
*<ul><li>hilink_profile.h锛氳鎺ュ彛澹版槑鎵�鍦ㄧ殑澶存枃浠躲��</li></ul>
*@see 鏃犮��
*@since Huawei Liteos V100R002C00
*/
extern int hilink_save_revoke_flag();



#ifdef __cplusplus
}
#endif


#endif  /* PROFILE_H_ */


