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
** Hilink Device SDK ¶ÔÍâ¿ª·Å½Ó¿Ú
******************************************************************************/

/**
 * @ingroup hilink_profile
 * Éè±¸ĞÅÏ¢µÄ½á¹¹Ìå£¬ÓÉÉè±¸¿ª·¢ÕßÌá¹©
 */

typedef struct {
	const char* sn;				/**<Éè±¸Î¨Ò»±êÊ¶£¬±ÈÈçsnºÅ£¬³¤¶È·¶Î§£¨0,40]*/
	const char* prodId;        	 /**<Éè±¸HiLinkÈÏÖ¤ºÅ£¬³¤¶È·¶Î§£¨0,5]*/
	const char* model;			/**<Éè±¸ĞÍºÅ£¬³¤¶È·¶Î§£¨0,32]*/
	const char* dev_t;			/**<Éè±¸ÀàĞÍ£¬³¤¶È·¶Î§£¨0,4]*/
	const char* manu;			/**<Éè±¸ÖÆÔìÉÌ£¬³¤¶È·¶Î§£¨0,4]*/
	const char* mac;			/**<Éè±¸MACµØÖ·£¬¹Ì¶¨32×Ö½Ú*/
	const char* hiv;			/**<Éè±¸HilinkĞ­Òé°æ±¾£¬³¤¶È·¶Î§£¨0,32]*/
	const char* fwv;			/**<Éè±¸¹Ì¼ş°æ±¾£¬³¤¶È·¶Î§[0,64]*/
	const char* hwv;			/**<Éè±¸Ó²¼ş°æ±¾£¬³¤¶È·¶Î§[0,64]*/
	const char* swv;			/**<Éè±¸Èí¼ş°æ±¾£¬³¤¶È·¶Î§[0,64]*/
	const int prot_t;			/**<Éè±¸Ğ­ÒéÀàĞÍ£¬È¡Öµ·¶Î§[1,3]*/
} dev_info_t;

/**
 * @ingroup hilink_profile
 * ·şÎñĞÅÏ¢µÄ½á¹¹Ìå£¬ÓÉÉè±¸¿ª·¢ÕßÌá¹©
 */

typedef struct  {
    const char* st;            		/**<·şÎñÀàĞÍ£¬³¤¶È·¶Î§£¨0,32]*/
    const char* svc_id;        /**<·şÎñID£¬³¤¶È·¶Î§£¨0,64]*/
} svc_info_t;

/**
 *@ingroup hilink_profile
 *@brief ³õÊ¼»¯Hilink Device SDK¡£
 *
 *@par ÃèÊö:
 *³õÊ¼»¯Hilink DeviceÉè±¸ĞÅÏ¢ºÍ·şÎñĞÅÏ¢ÒÔ¼°·şÎñ¸öÊı
 *@attention
 *<ul>
 *<li>Ã¿´ÎwifiÁ¬½Ó³É¹¦Ö®ºó£¬µ÷ÓÃÒ»´Î¼´¿É¡£</li>
 *<li>Hilink Device SDK²»»á¿ª±ÙÄÚ´æÓÃÓÚ´æ´¢dev_infoºÍsvc£¬Ö»»á±£´æËüÃÇµÄÖ¸Õë¡£Òò´ËSDKÔËĞĞÊ±²»ÄÜÊÍ·ÅÕâÁ½¸öÖ¸Õë¡£</li>
 *<li>dev_infoÖ¸Õë²»ÄÜÎª¿Õ£¡</li>
 *<li>svcÖ¸Õë²»ÄÜÎª¿Õ£¡</li>
 *<li>svcÖ¸Õë¶ÔÓ¦µÄ½á¹¹ÌåÊı×é¸öÊı±ØĞëÓësvc_numÏàµÈ£¡</li>
 *<li>º¯ÊıµÄÈë²ÎÕıÈ·ĞÔÓÉÉè±¸¿ª·¢Õß»ò³§ÉÌ±£Ö¤¡£</li>
 *<li>¸Ãº¯ÊıÓÉÉè±¸¿ª·¢Õß»ò³§ÉÌµ÷ÓÃ¡£</li>
 *</ul>
 *
 *@param dev_info       [IN] Éè±¸ĞÅÏ¢½á¹¹ÌåÖ¸Õë¡£dev_infoÖ¸Õë²»ÄÜÎª¿Õ£¡±ØĞë±£Ö¤Èë²ÎµÄÕıÈ·ĞÔ£¡
 *@param svc            [IN] ·şÎñĞÅÏ¢½á¹¹ÌåÊı×éÖ¸Õë¡£svcÖ¸Õë²»ÄÜÎª¿Õ£¡±ØĞë±£Ö¤Èë²ÎµÄÕıÈ·ĞÔ£¡
 *@param svc_num        [IN] ·şÎñ¸öÊı£¬¼´²ÎÊısvcµÄ´óĞ¡£¬½¨ÒéÖ»Ö§³ÖÒ»¸ö,·¶Î§Îª[1,8]¡£svcÖ¸Õë¶ÔÓ¦µÄ½á¹¹ÌåÊı×é¸öÊı±ØĞëÓësvc_numÏàµÈ£¡±ØĞë±£Ö¤Èë²ÎµÄÕıÈ·ĞÔ£¡
 *
 *@retval 0       Hilink Device SDK ³õÊ¼»¯³É¹¦¡£
 *@retval -12     Hilink Device SDK Èë²Î·Ç·¨¡£
 *@retval -17     Hilink Device SDK¶ÁÈ¡flash´íÎó£¬ĞèÒªÆäµ÷ÓÃ½Ó¿Ú×öÒì³£´¦Àí¡£
 *@par ÒÀÀµ:
 *<ul><li>hilink_profile.h£º¸Ã½Ó¿ÚÉùÃ÷ËùÔÚµÄÍ·ÎÄ¼ş¡£</li></ul>
 *@see ÎŞ¡£
 *@since Huawei Liteos V100R002C00
 */

extern int hilink_m2m_init(dev_info_t* dev_info, svc_info_t* svc, unsigned short svc_num);

/**
 *@ingroup hilink_profile
 *@brief Æô¶¯Hilink Device SDK¡£
 *
 *@par ÃèÊö:
 *Æô¶¯Hilink Device SDK¿ªÊ¼×´Ì¬»ú¹ÜÀí£¬±ØĞëÔÚhilink_m2m_init³õÊ¼»¯³É¹¦ºóµ÷ÓÃ£¡
 *@attention
 *<ul>
 *<li>ĞèÒªÑ­»·µ÷ÓÃ£¬½¨Òéµ÷ÓÃ¼ä¸ô×î´ó²»³¬¹ı50ºÁÃë¡£Èô¸Ãº¯Êı³¤Ê±¼äÎ´±»µ÷ÓÃ£¬Ôò¿ÉÄÜ·µ»ØÒì³£ĞÅÏ¢¡£</li>
 *<li>¸Ãº¯ÊıÓÉÉè±¸¿ª·¢Õß»ò³§ÉÌµ÷ÓÃ¡£</li>
 *</ul>
 *
 *@param ÎŞ¡£
 *
 *@retval 0     Hilink Device SDK ÔËĞĞÕı³£¡£
 *@retval -3    Hilink Device SDK»ñÈ¡Ê±¼ä´íÎó£¬ĞèÒªÆäµ÷ÓÃ½Ó¿Ú×öÒì³£´¦Àí¡£
 *@retval -16   Hilink Device SDKĞ´flash´íÎó£¬ĞèÒªÆäµ÷ÓÃ½Ó¿Ú×öÒì³£´¦Àí¡£
 *@retval -17   Hilink Device SDK¶ÁÈ¡flash´íÎó£¬ĞèÒªÆäµ÷ÓÃ½Ó¿Ú×öÒì³£´¦Àí¡£
 *@retval ·Ç0   ÆäËûÏê¼ûhilink_error.h¡£
 *@par ÒÀÀµ:
 *<ul><li>hilink_profile.h£º¸Ã½Ó¿ÚÉùÃ÷ËùÔÚµÄÍ·ÎÄ¼ş¡£</li></ul>
 *@see ÎŞ¡£
 *@since Huawei Liteos V100R002C00
 */
extern int hilink_m2m_process(void);

/**
 *@ingroup hilink_profile
 *@brief ÉèÖÃm2m main task id
*
 *@par ÃèÊö:
 *ÔÚ hilink_m2m_process Ñ­»·Ö®Ç°µ÷ÓÃ
 *@attention
 *<ul>
  *<li>¸Ãº¯ÊıÓÉÉè±¸¿ª·¢Õß»ò³§ÉÌµ÷ÓÃ¡£</li>
 *</ul>
 *
 *@param tid    [IN] hilink_m2m_process ËùÔÚtaskid
 *
 *@retval 0     ÉèÖÃ³É¹¦
 *@retval ·Ç0   ÉèÖÃÊ§°Ü
 *@par ÒÀÀµ:
 *<ul><li>hilink_profile.h£º¸Ã½Ó¿ÚÉùÃ÷ËùÔÚµÄÍ·ÎÄ¼ş¡£</li></ul>
 *@see ÎŞ¡£
 *@since hilink sdk v1.0.3
 */

extern void hilink_m2m_set_taskid(int tid);

/**
 *@ingroup hilink_profile
 *@brief ÉÏ±¨reportÄÜÁ¦ÊôĞÔ×´Ì¬¡£
 *
 *@par ÃèÊö:
 *·şÎñ×Ö¶Î×´Ì¬·¢Éú¸Ä±äÖ÷¶¯ÉÏ±¨µ½ÔÆÆ½Ì¨(Á¬½ÓÔÆÆ½Ì¨Ê±)»òÕßhilinkÍø¹Ø(Á¬½ÓhilinkÍø¹ØÊ±)¡£
 *@attention

 *<ul>
 *<li>Ö»¾ßÓĞreportÄÜÁ¦µÄÊôĞÔ×Ö¶ÎĞèÊ¹ÓÃ´Ë½Ó¿ÚÉÏ±¨ÊÂ¼ş£¬Ö»ÔÊĞíÓëhilink_m2m_processÔÚÒ»¸öÈÎÎñÖĞµ÷ÓÃ</li>
 *<li>¸Ãº¯ÊıÓÉÉè±¸¿ª·¢Õß»ò³§ÉÌµ÷ÓÃ¡£</li>
 *</ul>
 *
 *@param svc_id         [IN] ·şÎñID¡£
 *@param payload        [IN] Json¸ñÊ½µÄ×Ö¶ÎÓëÆäÖµ
 *@param len            [IN] payloadµÄ³¤¶È
 *@param tid            [IN] ½Ó¿Úµ÷ÓÃËùÔÚtaskid ĞèÍ¬m2m taskid, ÓÃÓÚ±£Ö¤Í¬Ò»ÈÎÎñÖĞµ÷ÓÃ

 *@retval 0     ·şÎñ×´Ì¬ÉÏ±¨³É¹¦¡£
 *@retval ·Ç0   ·şÎñ×´Ì¬ÉÏ±¨²»³É¹¦¡£
 *@par ÒÀÀµ:
 *<ul><li>hilink_profile.h£º¸Ã½Ó¿ÚÉùÃ÷ËùÔÚµÄÍ·ÎÄ¼ş¡£</li></ul>
 *@see hilink_get_char_state | hilink_put_char_state
 *@since hilink sdk v1.0.3
 */

extern int hilink_report_char_state(char* svc_id,
		char* payload, unsigned int len, int tid);
/**
 *@ingroup hilink_profile
 *@brief ÉÏ±¨·şÎñ×´Ì¬¡£
 *
 *@par ÃèÊö:
 *·şÎñ×Ö¶Î×´Ì¬·¢Éú¸Ä±äÖ÷¶¯ÉÏ±¨µ½ÔÆÆ½Ì¨(Á¬½ÓÔÆÆ½Ì¨Ê±)»òÕßhilinkÍø¹Ø(Á¬½ÓhilinkÍø¹ØÊ±)¡£
 *@attention
 *<ul>
 *<li>Éè±¸ÔÚÁ¬½Óµ½hilinkÍø¹Ø»òÔÆÆ½Ì¨Ê±»áÖ÷¶¯ÉÏ±¨Ò»´Î·şÎñËùÓĞ×Ö¶ÎµÄ×´Ì¬¡£Ä¿Ç°´Ë½Ó¿Ú¼ÇÂ¼ÉÏ±¨ÊÂ¼ş£¬·şÎñ×Ö¶Î×´Ì¬ÓÉHilink Device SDKÍ³Ò»ÉÏ±¨£¬´ó¸ÅÑÓÊ±200ms¡£</li>
 *<li>¸Ãº¯ÊıÓÉÉè±¸¿ª·¢Õß»ò³§ÉÌµ÷ÓÃ¡£</li>
 *</ul>
 *
 *@param svc_id         [IN] ·şÎñID¡£
 *@param payload        [IN] Json¸ñÊ½µÄ×Ö¶ÎÓëÆäÖµ£¬´Ë²ÎÊıÔİÎ´Ê¹ÓÃ£¬½¨Òé±£Áô£¬ÉèÖÃÎªNULL¡£
 *@param len            [IN] payloadµÄ³¤¶È£¬·¶Î§Îª[0£¬512)£¬²ÎÊıÔİÎ´Ê¹ÓÃ£¬½¨Òé±£Áô£¬ÉèÖÃÎª0¡£

 *@retval 0     ·şÎñ×´Ì¬ÉÏ±¨³É¹¦¡£
 *@retval ·Ç0   ·şÎñ×´Ì¬ÉÏ±¨²»³É¹¦¡£
 *@par ÒÀÀµ:
 *<ul><li>hilink_profile.h£º¸Ã½Ó¿ÚÉùÃ÷ËùÔÚµÄÍ·ÎÄ¼ş¡£</li></ul>
 *@see hilink_get_char_state | hilink_put_char_state
 *@since Huawei Liteos V100R002C00
 */

extern int hilink_upload_char_state(char* svc_id,
		char* payload, unsigned int len);

/**
 *@ingroup hilink_profile
 *@brief »ñÈ¡·şÎñ×Ö¶ÎÖµ¡£
 *
 *@par ÃèÊö:
 *»ñÈ¡Éè±¸·şÎñ×Ö¶ÎÖµ£¬Ö»ÔÊĞí»ñÈ¡¾ßÓĞGETÄÜÁ¦µÄÊôĞÔ×Ö¶Î
 *@attention
 *<ul>
 *<li>»ñÈ¡·şÎñ×Ö¶ÎÖµ¡£</li>
 *<li>´Ëº¯ÊıÓÉÉè±¸¿ª·¢Õß»ò³§ÉÌÊµÏÖ¡£</li>
 *</ul>
 *
 *@param svc_id         [IN] ·şÎñID¡£³§ÉÌÊµÏÖ¸Ãº¯ÊıÊ±£¬ĞèÒª¶ÔsidÅĞ¶Ï¡£
 *@param in             [IN] ½ÓÊÕµ½µÄJson¸ñÊ½µÄ×Ö¶ÎÓëÆäÖµ¡£
 *@param in_len         [IN] ½ÓÊÕµ½µÄinµÄ³¤¶È£¬·¶Î§Îª[0£¬512)¡£
 *@param out            [OUT] ·µ»Ø±£´æ·şÎñ×Ö¶ÎÖµÄÚÈİµÄÖ¸Õë,ÄÚ´æÓÉ³§ÉÌ¿ª±Ù£¬Ê¹ÓÃÍê³Éºó£¬ÓÉHilink Device SDKÊÍ·Å¡£
 *@param out_len        [OUT] ¶ÁÈ¡µ½µÄpayloadµÄ³¤¶È£¬·¶Î§Îª[0£¬512)¡£
 *
 *@retval 0     ·şÎñ×´Ì¬×Ö¶ÎÖµ»ñÈ¡³É¹¦¡£
 *@retval ·Ç0   ·şÎñ×´Ì¬×Ö¶ÎÖµ²»³É¹¦¡£
 *@par ÒÀÀµ:
 *<ul><li>hilink_profile.h£º¸Ã½Ó¿ÚÉùÃ÷ËùÔÚµÄÍ·ÎÄ¼ş¡£</li></ul>
 *@see hilink_upload_char_state | hilink_put_char_state
 *@since Huawei Liteos V100R002C00
 */

extern int hilink_get_char_state(const char* svc_id, const char* in, unsigned int in_len, char** out, \
	unsigned int* out_len);

/**
 *@ingroup hilink_profile
 *@brief ĞŞ¸Ä·şÎñµ±Ç°×Ö¶ÎÖµ¡£
 *

 *@par ÃèÊö:
 *ĞŞ¸ÄÉè±¸µ±Ç°·şÎñ×Ö¶ÎÖµ
 *@attention
 *<ul>
 *<li>Ö§³ÖĞŞ¸Ä·şÎñµÄÖ¸¶¨×Ö¶ÎµÄÖµ¡£</li>
 *<li>´Ëº¯ÊıÓÉÉè±¸¿ª·¢Õß»ò³§ÉÌÊµÏÖ¡£</li>
 *</ul>
 *

 *@param svc_id         [IN] ·şÎñID¡£
 *@param payload        [IN] ½ÓÊÕµ½ĞèÒªĞŞ¸ÄµÄJson¸ñÊ½µÄ×Ö¶ÎÓëÆäÖµ¡£
 *@param len            [IN] ½ÓÊÕµ½µÄpayloadµÄ³¤¶È£¬·¶Î§Îª[0£¬512)¡£
 *

 *@retval #M2M_SEARCH_GW_INVALID_PACKET    -101£¬»ñµÃ±¨ÎÄ²»·ûºÏÒªÇó¡£
 *@retval #M2M_SVC_STUTAS_VALUE_MODIFYING  -111£¬·şÎñ×´Ì¬ÖµÕıÔÚĞŞ¸ÄÖĞ£¬ĞŞ¸Ä³É¹¦ºóµ×²ãÉè±¸±ØĞëÖ÷¶¯ÉÏ±¨¡£
 *@retval #M2M_NO_ERROR                    0£¬   ÎŞ´íÎó£¬±íÊ¾·şÎñ×´Ì¬ÖµĞŞ¸Ä³É¹¦£¬²»ĞèÒªµ×²ãÉè±¸Ö÷¶¯ÉÏ±¨£¬ÓÉHilink Device SDKÉÏ±¨¡£
 *@par ÒÀÀµ:
 *<ul><li>hilink_profile.h£º¸Ã½Ó¿ÚÉùÃ÷ËùÔÚµÄÍ·ÎÄ¼ş¡£</li></ul>
 *@see hilink_upload_char_state | hilink_get_char_state
 *@since Huawei Liteos V100R002C00
 */

extern int hilink_put_char_state(const char* svc_id,
		const char* payload, unsigned int len);

/**
 *@ingroup hilink_profile
 *@brief »ñÈ¡Éè±¸ÔÚÏß×´Ì¬
 *

 *@par ÃèÊö:
 *Éè±¸ÀëÏßÉÏÏß×´Ì¬£¬¿É¸ù¾İ×´Ì¬×öÏàÓ¦µÄ´¦Àí
 *@attention
 *<ul>

 *<li>¸Ãº¯ÊıÓÉÉè±¸¿ª·¢Õß»ò³§ÉÌµ÷ÓÃ</li>
 *</ul>

 *@retval 0     0 offline 1 online
 *<ul><li>hilink_profile.h£º¸Ã½Ó¿ÚÉùÃ÷ËùÔÚµÄÍ·ÎÄ¼ş¡£</li></ul>
 *@see ÎŞ
 *@since hilink sdk v1.0.3
 */

extern int hilink_get_devstatus(void);
/**
 *@ingroup hilink_profile
 *@brief Éè±¸ÀëÏßÉÏÏßÍ¨Öª
 *

 *@par ÃèÊö:
 *Éè±¸ÀëÏßÉÏÏßÍ¨Öª£¬¿É¸ù¾İÍ¨ÖªÏûÏ¢×öÏàÓ¦µÄ´¦Àí
 *@attention
 *<ul>
 *<li>´Ëº¯ÊıÓÉÉè±¸¿ª·¢Õß»ò³§ÉÌÊµÏÖ,½ûÖ¹×öºÄÊ±²Ù×÷</li>
 *</ul>

 *
 *@param status            [IN] 0 offline 1 online
 *
 *@retval 0     ³É¹¦¡£
 *

 *<ul><li>hilink_profile.h£º¸Ã½Ó¿ÚÉùÃ÷ËùÔÚµÄÍ·ÎÄ¼ş¡£</li></ul>
 *@see ÎŞ
 *@since hilink sdk v1.0.3
 */

extern void hilink_notify_devstatus(int status);

/**

 *@ingroup hilink_profile

 *@brief ĞŞ¸Äwifi²ÎÊı¡£

 *

 *@par ÃèÊö:

 *Í¨ÖªdeviceµÄÓ¦ÓÃ²ãĞŞ¸ÄWiFi²ÎÊı£¬Ê¹ÓÃĞÂµÄWiFi²ÎÊıÁ¬½Óµ½Ö¸¶¨µÄÂ·ÓÉÆ÷¡£

 *@attention

 *<ul>

 *<li>µ±ssid¡¢pwd¾ùÎª¿Õ£¬ÇÒmode=-1Ê±£¬±íÊ¾Çå³ıWiFi²ÎÊıĞÅÏ¢¡£</li>

 *<li>µ±ssid²»Îª¿ÕÊ±£¬±íÊ¾ĞŞ¸ÄWiFi²ÎÊıĞÅÏ¢¡£</li>

 *<li>´Ëº¯ÊıÓÉÉè±¸¿ª·¢Õß»ò³§ÉÌÊµÏÖ¡£</li>

 *</ul>

 *

 *@param ssid           [IN] Â·ÓÉÆ÷µÄSSID¡£

 *@param ssid_len       [IN] Â·ÓÉÆ÷µÄSSIDµÄ³¤¶È£¬·¶Î§Îª[0£¬33)¡£

 *@param pwd            [IN] Â·ÓÉÆ÷µÄÃÜÂë¡£

 *@param pwd_len        [IN] Â·ÓÉÆ÷µÄÃÜÂëµÄ³¤¶È£¬·¶Î§Îª[0£¬65)¡£

 *@param mode                 [IN] Â·ÓÉÆ÷¼ÓÃÜÄ£Ê½£¬·¶Î§[-4,6]¡£

 *

 *@retval 0     WiFi²ÎÊıĞŞ¸Ä³É¹¦¡£

 *@retval ·Ç0   WiFi²ÎÊıĞŞ¸Ä²»³É¹¦¡£

 *@par ÒÀÀµ:

 *<ul><li>hilink_profile.h£º¸Ã½Ó¿ÚÉùÃ÷ËùÔÚµÄÍ·ÎÄ¼ş¡£</li></ul>

 *@see ÎŞ¡£

 *@since Huawei Liteos V100R002C00

 */

extern int hilink_notify_wifi_param(char* ssid, unsigned int ssid_len,
		char* pwd, unsigned int pwd_len, int mode );


 /**

 *@ingroup hilink_profile

 *@brief ´¥·¢¹Ì¼ş¼ì²âÉı¼¶»òÆô¶¯Éı¼¶¡£

 *

 *@par ÃèÊö:

 *Íø¹Ø»òÔÆ¶ËÏòDevice·¢ËÍ´¥·¢¹Ì¼ş¼ì²âÉı¼¶»òÆô¶¯Éı¼¶ÃüÁî¡£

 *@attention

 *<ul>

 *<li>´Ëº¯ÊıÓÉÉè±¸¿ª·¢Õß»ò³§ÉÌÊµÏÖ¡£ĞèÒª×öÈë¿Ú²ÎÊı¼ì²â£¬´«Èë²ÎÊı0ºÍ1ÎªºÏ·¨¡£</li>

 *</ul>

 *

 *@param mode           [IN] mode=0ÎªÉè±¸¹Ì¼ş¼ì²âÉı¼¶£¬mode=1±íÊ¾Éè±¸Æô¶¯¹Ì¼şÉı¼¶£¬·¶Î§Îª[0,1]¡£

 *

 *@retval #M2M_NO_ERROR         0£¬   ÎŞ´íÎó¡£

 *@retval #M2M_OTA__DETECT_FAILURE     -900£¬´¥·¢¹Ì¼ş¼ì²âÉı¼¶Ê§°Ü¡£

 *@retval #M2M_OTA__START_FAILURE      -901£¬¹Ì¼şÆô¶¯Éı¼¶Ê§°Ü¡£

 *@retval #M2M_ARG_INVALID			   -12£¬ Èë²ÎÎŞĞ§¡£

 *@par ÒÀÀµ:

 *<ul><li>hilink_profile.h£º¸Ã½Ó¿ÚÉùÃ÷ËùÔÚµÄÍ·ÎÄ¼ş¡£</li></ul>

 *@see ÎŞ¡£

 *@since Huawei Liteos V100R002C00

 */

extern int hilink_ota_trig(int mode);


 /**

 *@ingroup hilink_profile

 *@brief »ñÈ¡¹Ì¼ş°æ±¾ÃèÊöĞÅÏ¢¡£

 *

 *

 *@par ÃèÊö:

 *@attention

 *<ul>

 *<li>µ±Ç°¹Ì¼ş°æ±¾Îª×îĞÂÊ±£¬version·µ»ØÎªNULL¡£</li>

 *<li>´Ëº¯ÊıÓÉÉè±¸¿ª·¢Õß»ò³§ÉÌÊµÏÖ¡£</li>

 *</ul>

 *

 *@param version                [OUT] Éè±¸¹Ì¼ş°æ±¾ºÅ¡£ÓÉ³§ÉÌ¿ª±ÙÄÚ´æ£¬·µ»Ø±£´æ·şÎñ×Ö¶ÎÖµÄÚÈİµÄÖ¸Õë,Ê¹ÓÃÍê³Éºó£¬ÓÉHilink Device SDKÊÍ·Å¡£

 *@param ver_len                [OUT] Éè±¸¹Ì¼ş°æ±¾³¤¶È£¬·¶Î§Îª[0£¬64)¡£

 *

 *@retval #M2M_NO_ERROR                           0£¬   ÎŞ´íÎó¡£

 *@retval #M2M_OTA_GET_VERSION_FAILURE            -902£¬»ñÈ¡¹Ì¼ş°æ±¾ºÅÊ§°Ü¡£

 *@par ÒÀÀµ:

 *<ul><li>hilink_profile.h£º¸Ã½Ó¿ÚÉùÃ÷ËùÔÚµÄÍ·ÎÄ¼ş¡£</li></ul>

 *@see hilink_ota_rpt_ver

 *@since Huawei Liteos V100R002C00

 */

extern int hilink_ota_get_ver(char** version, int* ver_len);


 /**

 *@ingroup hilink_profile

 *@brief »ñÈ¡¹Ì¼ş°æ±¾ÃèÊöĞÅÏ¢¡£

 *

 *@par ÃèÊö:

 *@attention

 *<ul>

 *<li>µ±Ç°¹Ì¼ş°æ±¾Îª×îĞÂÊ±£¬introductionÎªNULL¡£</li>

 *<li>´Ëº¯ÊıÓÉÉè±¸¿ª·¢Õß»ò³§ÉÌÊµÏÖ¡£</li>

 *</ul>

 *

 *@param introduction           [OUT] Éè±¸¹Ì¼ş°æ±¾ÃèÊöĞÅÏ¢¡£ÓÉ³§ÉÌ¿ª±ÙÄÚ´æ£¬·µ»Ø±£´æ·şÎñ×Ö¶ÎÖµÄÚÈİµÄÖ¸Õë,Ê¹ÓÃÍê³Éºó£¬ÓÉHilink Device SDKÊÍ·Å¡£

 *@param intro_len              [OUT] Éè±¸¹Ì¼ş°æ±¾ÃèÊöĞÅÏ¢³¤¶È£¬½¨Òé·¶Î§Îª[0£¬512)£¬Èç¹û³öÏÖ³¤¶ÈÌ«³¤£¬»á±»½Ø¶Ï´¦Àí¡£

 *

 *@retval #M2M_NO_ERROR                           0£¬   ÎŞ´íÎó¡£

 *@retval #M2M_OTA_GET_VERSION_INFO_FAILURE       -903£¬»ñÈ¡¹Ì¼ş°æ±¾ÃèÊöĞÅÏ¢Ê§°Ü¡£

 *@par ÒÀÀµ:

 *<ul><li>hilink_profile.h£º¸Ã½Ó¿ÚÉùÃ÷ËùÔÚµÄÍ·ÎÄ¼ş¡£</li></ul>

 *@see hilink_ota_rpt_ver

 *@since Huawei Liteos V100R002C00

 */

extern int hilink_ota_get_intro(char** introduction, int* intro_len);


 /**
 *@ingroup hilink_profile
 *@brief ä¸ŠæŠ¥å›ºä»¶è®¾å¤‡ç‰ˆæœ¬ä¿¡æ¯ã€‚
 *
 *@par æè¿°:
 *å‘ç½‘å…³æˆ–äº‘ç«¯ä¸ŠæŠ¥Deviceå›ºä»¶ç‰ˆæœ¬ä¿¡æ¯ã€‚
 *@attention
 *<ul>
 *<li>ç›®å‰æ­¤æ¥å£åªè®°å½•ä¸ŠæŠ¥äº‹ä»¶ï¼ŒæœåŠ¡å­—æ®µçŠ¶æ€ç”±Hilink Device SDKç»Ÿä¸€ä¸ŠæŠ¥ï¼Œå¤§æ¦‚å»¶æ—¶200ms</li>
 *<li>æ­¤å‡½æ•°ç”±è®¾å¤‡å¼€å‘è€…æˆ–å‚å•†è°ƒç”¨ã€‚</li>
 *</ul>
 *
 *@param version                [OUT] è®¾å¤‡å›ºä»¶ç‰ˆæœ¬å·ã€‚æ­¤å‚æ•°æš‚æœªä½¿ç”¨ï¼Œå»ºè®®ä¿ç•™ï¼Œè®¾ç½®ä¸ºNULLã€‚
 *@param ver_len                [OUT] è®¾å¤‡å›ºä»¶ç‰ˆæœ¬é•¿åº¦ï¼ŒèŒƒå›´ä¸º[0ï¼Œ64)ã€‚æ­¤å‚æ•°æš‚æœªä½¿ç”¨ï¼Œå»ºè®®ä¿ç•™ï¼Œè®¾ç½®ä¸º0ã€‚
 *@param introduction           [OUT] è®¾å¤‡å›ºä»¶ç‰ˆæœ¬æè¿°ä¿¡æ¯ã€‚æ­¤å‚æ•°æš‚æœªä½¿ç”¨ï¼Œå»ºè®®ä¿ç•™ï¼Œè®¾ç½®ä¸ºNULLã€‚
 *@param intro_len              [OUT] è®¾å¤‡å›ºä»¶ç‰ˆæœ¬æè¿°ä¿¡æ¯é•¿åº¦ï¼Œå»ºè®®èŒƒå›´ä¸º[0ï¼Œ512),å¦‚æœå‡ºç°é•¿åº¦å¤ªé•¿ï¼Œä¼šè¢«æˆªæ–­å¤„ç†ã€‚æ­¤å‚æ•°æš‚æœªä½¿ç”¨ï¼Œå»ºè®®ä¿ç•™ï¼Œè®¾ç½®ä¸º0ã€‚
 *
 *@retval 0     æ— é”™è¯¯ã€‚
 *@retval é0   ä¸ŠæŠ¥å›ºä»¶å‡çº§ç‰ˆæœ¬ä¿¡æ¯å¤±è´¥ã€‚
 *@par ä¾èµ–:
 *<ul><li>hilink_profile.hï¼šè¯¥æ¥å£å£°æ˜æ‰€åœ¨çš„å¤´æ–‡ä»¶ã€‚</li></ul>
 *@see hilink_ota_rpt_prg
 *@since Huawei Liteos V100R002C00
 */
extern int hilink_ota_rpt_ver(char* version, int ver_len,
    char* introduction, int intro_len);


 /**
 *@ingroup hilink_profile
 *@brief ä¸ŠæŠ¥å›ºä»¶å‡çº§è¿›åº¦ã€‚
 *
 *@par æè¿°:
 *å‘ç½‘å…³æˆ–äº‘ç«¯ä¸ŠæŠ¥Deviceå›ºä»¶å‡çº§è¿›åº¦ã€‚ç›®å‰æ­¤æ¥å£åªè®°å½•ä¸ŠæŠ¥äº‹ä»¶ï¼ŒæœåŠ¡å­—æ®µçŠ¶æ€ç”±Hilink Device SDKç»Ÿä¸€ä¸ŠæŠ¥ï¼Œå¤§æ¦‚å»¶æ—¶200msã€‚
 *@attention
 *<ul>
 *<li>progressä¸º[0,100]è¡¨ç¤ºå‡çº§è¿›åº¦æ­£å¸¸ï¼›progress=101è¡¨ç¤ºå‡çº§å¤±è´¥ã€‚</li>
 *<li>progress=1000ï¼Œæ— æ³•ç½‘ç»œé€šä¿¡</li>
 *<li>progress=1001ï¼Œé•œåƒç½‘ç»œä¸‹è½½å¼‚å¸¸</li>
 *<li>progress=1002ï¼Œæ ¡éªŒå¤±è´¥</li>
 *<li>progress=1003ï¼Œå†™å…¥å¤±è´¥</li>
 *<li>bootTimeæ˜¯å¯¹äºæ— æ³•è®¡é‡å‡çº§è¿›åº¦çš„ï¼Œå¦‚ç‰ˆæœ¬å†™å…¥ã€é‡å¯ç­‰ï¼Œé€šè¿‡è¯¥æ—¶é—´ä¸ŠæŠ¥å‡çº§å®Œæˆçš„å¤§æ¦‚æ—¶é—´ã€‚</li>
 *<li>æ­¤å‡½æ•°ç”±è®¾å¤‡å¼€å‘è€…æˆ–å‚å•†è°ƒç”¨ã€‚</li>
 *</ul>
 *
 *@param progress                [IN] progressä¸º[0,100]è¡¨ç¤ºå‡çº§è¿›åº¦æ­£å¸¸ï¼›progress=101è¡¨ç¤ºå‡çº§å¤±è´¥ã€‚progress=1000ï¼Œæ— æ³•ç½‘ç»œé€šä¿¡ã€‚progress=1001ï¼Œé•œåƒç½‘ç»œä¸‹è½½å¼‚å¸¸ã€‚progress=1002ï¼Œæ ¡éªŒå¤±è´¥ã€‚progress=1003ï¼Œå†™å…¥å¤±è´¥ã€‚
 *@param bootTime                [IN] bootTimeå¤§äº0ï¼Œè¡¨ç¤ºè®¾å¤‡é‡å¯ç­‰éœ€è¦çš„æ—¶é—´ã€‚
 *
 *@retval #M2M_NO_ERROR                   0ï¼Œ    æ— é”™è¯¯ã€‚
 *@retval #M2M_OTA_RPT_PRG_FAILURE        -905ï¼Œ ä¸ŠæŠ¥å›ºä»¶å‡çº§è¿›åº¦å¤±è´¥ã€‚
 *@retval #M2M_ARG_INVALID				  -12ï¼Œ  å…¥å‚æ— æ•ˆã€‚
 *@retval #M2M_UPLOAD_DISALBE             -803ï¼Œ åªèƒ½åœ¨æ¨¡å¼ä¸ºç½‘å…³å’Œäº‘æ—¶ï¼Œä¸Šä¼ æˆåŠŸï¼Œå¦åˆ™è¿”å›å¤±è´¥ã€‚
 *@par ä¾èµ–:
 *<ul><li>hilink_profile.hï¼šè¯¥æ¥å£å£°æ˜æ‰€åœ¨çš„å¤´æ–‡ä»¶ã€‚</li></ul>
 *@see hilink_ota_rpt_ver
 *@since Huawei Liteos V100R002C00
 */
extern int hilink_ota_rpt_prg(int progress, int bootTime);

/**
*@ingroup hilink_profile
*@brief è®¾å¤‡æ¢å¤å‡ºå‚æ—¶è®¾ç½®è®¾å¤‡æ³¨é”€æ ‡å¿—ã€‚
*
*@par æè¿°:
*HiLinkè®¾å¤‡æ¢å¤å‡ºå‚æ—¶ï¼Œé€šè¿‡è°ƒç”¨æ­¤æ¥å£è®¾ç½®æ³¨é”€æ ‡è¯†ã€‚
*@attention
*<ul>
*<li>æ­¤å‡½æ•°ç”±è®¾å¤‡å¼€å‘è€…æˆ–å‚å•†è°ƒç”¨ã€‚</li>
*</ul>
*@retval 0     è®¾ç½®æ³¨é”€æ ‡è¯†æˆåŠŸã€‚
*@retval é0   è®¾ç½®æ³¨é”€æ ‡è¯†å¤±è´¥ã€‚
*@par ä¾èµ–:
*<ul><li>hilink_profile.hï¼šè¯¥æ¥å£å£°æ˜æ‰€åœ¨çš„å¤´æ–‡ä»¶ã€‚</li></ul>
*@see æ— ã€‚
*@since Huawei Liteos V100R002C00
*/
extern int hilink_save_revoke_flag();



#ifdef __cplusplus
}
#endif


#endif  /* PROFILE_H_ */


