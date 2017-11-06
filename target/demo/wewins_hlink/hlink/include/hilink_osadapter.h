/**---------------------------------------------------------------------------
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

/** @defgroup hilink_osadapter ����ϵͳ����
 *@ingroup osadapter
 */
#ifndef HILINK_OSADAPTER_H_
#define HILINK_OSADAPTER_H_

#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif /* __cpluscplus */
#endif /* __cpluscplus */

typedef enum {
	NW_DISCONNECTED		= 0,		/*�������ӶϿ�*/
	NW_CONNECTED		= 1		/*����������*/
} network_state_t;

/**
 *@ingroup hilink_osadapter
 *@brief ��ȡ������ǰ���ӵ�wifi ssid��
 *
 *@par ����:
 *<ul>
 *<li>��ȡ������ǰ���ӵ�wifi ssid��</li>
 *</ul>
 *@attention
 *<ul>
 *<li>��</li>
 *</ul>
 *
 *@param pssid	   [IN/OUT]	����ssid
 *@param pssidlen  [IN/OUT]	ssid����
 *
 *@retval 0����ȡ�ɹ�
 *@retval -1����ȡʧ��
 *@par ����:
 *<ul><li>hilink_osadapter.h���ýӿ��������ڵ�ͷ�ļ���</li></ul>
 *@see ��
 *@since Huawei Liteos V100R002C00
 */

extern int hilink_getssid(unsigned char *pssid, unsigned char *pssidlen);

/**
*@ingroup hilink_osadapter
*@brief ��ȡԶ������ip��ַ��
*
*@par ����:
*ͨ��DNS������������ȡԶ������ip��ַ���˽ӿ�ʵ��Ϊ��������
*@attention
*<ul>
*<li>��</li>
*</ul>
*
*@param hostname    [IN] Զ���������ơ�
*@param ip_list     [OUT] ���Զ������ip��ַ�б�����顣
*@param num         [IN] ���Զ������ip��ַ�б������Ĵ�С����ΧΪ[1��4]��
*
*@retval 0   ��ȡ�ɹ�
*@retval -1  ��ȡʧ��
*@par ����:
*<ul><li>hilink_osadapter.h���ýӿ��������ڵ�ͷ�ļ���</li></ul>
*@see hilink_resolve_host
*@since Huawei Liteos V100R002C00
*/

extern int hilink_gethostbyname(char* hostname, char ip_list[][40], int num);
 
 /**
  *@ingroup hilink_osadapter
  *@brief ��ȡ����״̬��
  * 
  *@par ����:
  *<ul> 
  *<li>��ȡ����״̬��</li> 
  *</ul>
 
  *@attention 
  *<ul> 
  *<li>state��������״̬��</li> 
  *</ul>
  *
  *@param state  [IN/OUT]	 ����״̬,ȡֵ��ΧΪ[0,1]
  *
  *@retval 0	��ȡ�ɹ�
  *@retval ��0	��ȡʧ�� 
  *@par ����:
  *<ul><li>hilink_osadapter.h���ýӿ��������ڵ�ͷ�ļ���</li></ul>
  *@see ��
  *@since Huawei Liteos V100R002C00
  */

extern int hilink_network_state(int* state);

/*
 *@ingroup hilink_osadapter
 *@brief ��ȡ��ǰʱ�䡣
 *
 *@par ����:
 *<ul>
 *<li>��ȡ��ǰʱ�䣬�Ժ���Ϊ��λ��</li>
 *</ul>
 
 *@attention
 *<ul>
 *<li>��</li>
 *</ul>
 *
 *@param ms		[OUT]	��ǰʱ��
 *
 *@retval 0    ��ȡ�ɹ�
 *@retval -1   ��ȡʧ��
 *@par ����:
 *<ul><li>hilink_osadapter.h���ýӿ��������ڵ�ͷ�ļ���</li></ul>
 *@see ��
 *@since Huawei Liteos V100R002C00
 */

extern int hilink_gettime(unsigned long* ms);



/**
 *@ingroup hilink_osadapter
 *@brief ������������ĳ�ʼ��������
 *
 *@par ����:
 *<ul>
 *<li>����ϵͳ�ṩ������ֵ�������������</li>
 *</ul>
 *@attention
 *<ul>
 *<li>srand��rand()���ʹ�ò�����������С�</li>
 *</ul>
 *
 *@param ��
 *
 *@retval �����
 *@par ����:
 *<ul><li>hilink_osadapter.h���ýӿ��������ڵ�ͷ�ļ���</li></ul>
 *@see hilink_srand
 *@since Huawei Liteos V100R002C00
 */

extern int hilink_rand(void);

/**
 *@ingroup hilink_osadapter
 *@brief ������������ĳ�ʼ��������
 *
 *@par ����:
 *<ul>
 *<li>�����û��ṩ��seedֵ�������������</li>
 *</ul>
 *@attention
 *<ul>
 *<li>srand��rand()���ʹ�ò�����������С�</li>
 *</ul>
 *
 *@param seed   [IN]     ���������������ֵ������ֵȡ���������
 *
 *@retval �����
 *@par ����:
 *<ul><li>hilink_osadapter.h���ýӿ��������ڵ�ͷ�ļ���</li></ul>
 *@see hilink_rand
 *@since Huawei Liteos V100R002C00
 */

extern void hilink_srand(unsigned int seed);


/**
 *@ingroup hilink_osadapter
 *@brief ��flash��д���ݡ�
 *
 *@par ����:
 *<ul>
 *<li>��len�ֽڳ�������д��flash��,len�Ϊ1024��</li>
 *</ul>
 *@attention
 *<ul>
 *<li>ָ�����ָ��Ϸ��ڴ�</li>
 *</ul>
 *
 *@param buf   [IN/OUT]     ���������׵�ַ
 *@param len   [IN]     	�������ĳ���
 *
 *@retval 0    д��ɹ�
 *@retval -1   д��ʧ��
 *@par ����:
 *<ul><li>hilink_osadapter.h���ýӿ��������ڵ�ͷ�ļ���</li></ul>
 *@see hilink_read_flash
 *@since Huawei Liteos V100R002C00
 */

extern int hilink_save_flash(char* buf, unsigned int len);

/**
 *@ingroup hilink_osadapter
 *@brief ��flash�ж�ȡ���ݡ�
 *
 *@par ����:
 *<ul>
 *<li>��flash�ж�ȡlen�ֽڳ������ݡ�</li>
 *</ul>
 *@attention
 *<ul>
 *<li>ָ�����ָ��Ϸ��ڴ档</li>
 *</ul>
 *
 *@param buf   [IN/OUT]     ���������׵�ַ
 *@param len   [IN]     	�������ĳ���
 *
 *@retval 0    ��ȡ�ɹ�
 *@retval -1   ��ȡʧ��
 *@par ����:
 *<ul><li>hilink_osadapter.h���ýӿ��������ڵ�ͷ�ļ���</li></ul>
 *@see hilink_save_flash
 *@since Huawei Liteos V100R002C00
 */

extern int hilink_read_flash(char* buf, unsigned int len);


/**
 *@ingroup hilink_osadapter
 *@brief ��������ַ����ĳ��ȡ�
 *
 *@par ����:
 *<ul>
 *<li>��������ַ����ģ�unsigned int�ͣ����ȣ�������'\0'���ڡ�</li>
 *</ul>
 *@attention
 *<ul>
 *<li>ָ�����ָ��Ϸ��ڴ档</li>
 *</ul>
 *
 *@param src   [IN]     	�ַ������׵�ַ
 *
 *@retval �ַ����ĳ���
 *@par ����:
 *<ul><li>hilink_osadapter.h���ýӿ��������ڵ�ͷ�ļ���</li></ul>
 *@see ��
 *@since Huawei Liteos V100R002C00
 */

extern unsigned int hilink_strlen(const char* src);

/**
 *@ingroup hilink_osadapter
 *@brief �����ַ����е����ݡ�
 *
 *@par ����:
 *<ul>
 *<li>��src��ָ����ַ�������src��ַ��ʼ��ǰlen���ֽڸ��Ƶ�dst��ָ�������У�������dst��</li>
 *</ul>
 *@attention
 *<ul>
 *<li>ָ�����ָ��Ϸ��ڴ档</li>
 *</ul>
 *
 *@param dst   [IN]     Ŀ���ַ�����
 *@param src   [IN]     Դ�ַ�������ʼλ��
 *@param len   [IN]     Ҫ���Ƶ��ֽ���
 *
 *@retval dst Ŀ���ַ�������׵�ַ
 *@par ����:
 *<ul><li>hilink_osadapter.h���ýӿ��������ڵ�ͷ�ļ���</li></ul>
 *@see hilink_memcpy | hilink_strncat
 *@since Huawei Liteos V100R002C00
 */

extern char* hilink_strncpy(char* dst, const char* src, unsigned int len);


/**
 *@ingroup hilink_osadapter
 *@brief �����ַ�����
 *
 *@par ����:
 *<ul>
 *<li>��src��ָ�ַ�����ӵ�dst��β��ʵ���ַ��������ӣ����ӹ��̸���dst��β����'/0'��</li>
 *</ul>
 *@attention
 *<ul>
 *<li>ָ�����ָ��Ϸ��ڴ�</li>
 *</ul>
 *
 *@param dst   [IN]     Ŀ���ַ�����ָ��
 *@param src   [IN]     Դ�ַ�����ָ��
 *
 *@retval dst  Ŀ���ַ�����ָ��
 *@par ����:
 *<ul><li>hilink_osadapter.h���ýӿ��������ڵ�ͷ�ļ���</li></ul>
 *@see hilink_strncpy
 *@since Huawei Liteos V100R002C00
 */

extern char* hilink_strncat(char *dst, const char *src, unsigned int len);

/**
 *@ingroup hilink_osadapter
 *@brief �Ƚ������ַ����Ĵ�С��
 *
 *@par ����:
 *<ul>
 *<li>�����û��ṩ���ַ����׵�ַ�����ȣ��Ƚ������ַ����Ĵ�С��ָ�����ָ��Ϸ��ڴ档</li>
 *</ul>
 *@attention
 *<ul>
 *<li>��</li>
 *</ul>
 *
 *@param str1   [IN]     �ַ���1���׵�ַ
 *@param str2   [IN]     �ַ���2���׵�ַ
 *@param len	[IN]     �ַ����ĳ���
 *
 *@retval ����0   str1����str2
 *@retval С��0   str1С��str2
 *@retval ����0   str1����str2
 *@par ����:
 *<ul><li>hilink_osadapter.h���ýӿ��������ڵ�ͷ�ļ���</li></ul>
 *@see hilink_memcmp
 *@since Huawei Liteos V100R002C00
 */

extern int hilink_strncmp(const char* str1, const char* str2, unsigned int len);

/**
 *@ingroup hilink_osadapter
 *@brief ����һ���ַ���һ���ַ������״γ��ֵ�λ�á�
 *
 *@par ����:
 *<ul>
 *<li>����һ���ַ�ch����һ���ַ���str���״γ��ֵ�λ�ã����������λ�õĵ�ַ�����δ���ҵ�ָ���ַ�����ô����������NULL��</li>
 *</ul>
 *@attention
 *<ul>
 *<li>��κ�����</li>
 *</ul>
 *
 *@param str   	[IN]    �ַ������׵�ַ
 *@param ch   	[IN]    Ҫ���ҵ��ַ�
 *
 *@retval Null,δ���ҵ�ָ���ַ�
 *@retval ��Null,�ַ��״γ���λ�õĵ�ַ
 *@par ����:
 *<ul><li>hilink_osadapter.h���ýӿ��������ڵ�ͷ�ļ���</li></ul>
 *@see hilink_strrchr
 *@since Huawei Liteos V100R002C00
 */

extern char* hilink_strchr(char* str, char ch);

/**
 *@ingroup hilink_osadapter
 *@brief ����һ���ַ���һ���ַ�����ĩ�γ��ֵ�λ�á�
 *
 *@par ����:
 *<ul>
 *<li>����һ���ַ�c����һ���ַ���str��ĩ�γ��ֵ�λ�ã����������λ�õĵ�ַ�����δ���ҵ�ָ���ַ�����ô����������NULL��</li>
 *</ul>
 *@attention
 *<ul>
 *<li>��κ�����</li>
 *</ul>
 *
 *@param str   	[IN]    �ַ������׵�ַ
 *@param c   	[IN]    Ҫ���ҵ��ַ�
 *
 *@retval Null     δ���ҵ�ָ���ַ�
 *@retval ��Null   �ַ�ĩ�γ���λ�õĵ�ַ
 *@par ����:
 *<ul><li>hilink_osadapter.h���ýӿ��������ڵ�ͷ�ļ���</li></ul>
 *@see hilink_strchr
 *@since Huawei Liteos V100R002C00
 */

extern char* hilink_strrchr(const char* str, char c);

/**
 *@ingroup hilink_osadapter
 *@brief ���ַ���ת������������
 *
 *@par ����:
 *<ul>
 *<li>�����һ���ǿո��ַ����ڣ������ֻ�����������ʼ������ת����֮���⵽������(���������� \0) �ַ�ʱֹͣת����</li>
 *</ul>
 *@attention
 *<ul>
 *<li>��κ�����</li>
 *</ul>
 *
 *@param str   		[IN]    �ַ������׵�ַ
 *
 *@retval ������
 *@par ����:
 *<ul><li>hilink_osadapter.h���ýӿ��������ڵ�ͷ�ļ���</li></ul>
 *@see ��
 *@since Huawei Liteos V100R002C00
 */

extern int hilink_atoi(const char* str);

/**
 *@ingroup hilink_osadapter
 *@brief �ַ�����ʽ��������
 *
 *@par ����:
 *<ul>
 *<li>�Ѹ�ʽ��������д��ĳ���ַ�����������</li>
 *</ul>
 *@attention
 *<ul>
 *<li>��κ���</li>
 *</ul>
 *
 *@param buf   		[IN/OUT]    ָ��Ҫд����ַ����Ļ�����
 *@param format   	[IN]     	��ʽ������Ϣ
 *@param ...   		[IN]     	��ѡ�������������κ����͵�����
 *
 *@retval С��0       д��ʧ��
 *@retval ���ڵ���0   д����ַ�������
 *@par ����:
 *<ul><li>hilink_osadapter.h���ýӿ��������ڵ�ͷ�ļ���</li></ul>
 *@see hilink_sprintf
 *@since Huawei Liteos V100R002C00
 */

extern int hilink_sprintf(char* buf, const char* format, ...);

/**
 *@ingroup hilink_osadapter
 *@brief �ַ�����ʽ��������
 *
 *@par ����:
 *<ul>
 *<li>�Ѹ�ʽ��������д��ĳ���ַ�����������</li>
 *</ul>
 *@attention
 *<ul>
 *<li>��κ�����</li>
 *</ul>
 *
 *@param buf   		[IN/OUT]    ָ��Ҫд����ַ����Ļ�����
 *@param size   	[IN]    	Ҫд����ַ��������Ŀ(��\0)������size�ᱻ�ض�,ĩβ�Զ���\0 
 *@param format   	[IN]     	��ʽ������Ϣ
 *@param ...   		[IN]     	��ѡ�������������κ����͵�����
 *
 *@retval С��0       д��ʧ��
 *@retval ���ڵ���0   �ɹ��򷵻���д����ַ�������
 *@par ����:
 *<ul><li>hilink_osadapter.h���ýӿ��������ڵ�ͷ�ļ���</li></ul>
 *@see hilink_sprintf
 *@since Huawei Liteos V100R002C00
 */

extern int hilink_snprintf(char* buf, unsigned int size, const char* format, ...);

/**
 *@ingroup hilink_osadapter
 *@brief ��ʽ�����������
 *
 *@par ����:
 *<ul>
 *<li>��ʽ�������һ���������׼����豸���涨��ʽ�����Ϣ��</li>
 *</ul>
 *@attention
 *<ul>
 *<li>��κ���</li>
 *</ul>
 *
 *@param 	format  [IN]     ��ʽ������Ϣ
*@param 	...   	[IN]     ��ѡ�������������κ����͵�����
 *
 *@retval С��0       ���ʧ��
 *@retval ���ڵ���0   ����ĳ���
 *@par ����:
 *<ul><li>hilink_osadapter.h���ýӿ��������ڵ�ͷ�ļ���</li></ul>
 *@see hilink_sprintf
 *@since Huawei Liteos V100R002C00
 */

extern int hilink_printf(const char* format, ...);


/**
 *@ingroup hilink_osadapter
 *@brief ��һ���ڴ�������ĳ��������ֵ��
 *
 *@par ����:
 *<ul>
 *<li>��dst��ǰlen���ֽ���c�滻������dst ��</li>
 *</ul>
 *@attention
 *<ul>
 *<li>�Խϴ�Ľṹ�������������������һ����췽��</li>
 *</ul>
 *
 *@param dst   [IN/OUT]     Ŀ�����ʼλ��
 *@param c     [IN]     	Ҫ����ֵ
 *@param len   [IN]     	Ҫ����ֵ�ֽ���
 *
 *@retval ��
 *@par ����:
 *<ul><li>hilink_osadapter.h���ýӿ��������ڵ�ͷ�ļ���</li></ul>
 *@see hilink_free
 *@since Huawei Liteos V100R002C00
 */

extern void* hilink_memset(void* dst, int c, unsigned int len);

/**
 *@ingroup hilink_osadapter
 *@brief �����ڴ��е����ݡ�
 *
 *@par ����:
 *<ul>
 *<li>��Դsrc��ָ���ڴ��ַ����ʼλ�ÿ�ʼ����len���ֽڵ�Ŀ��dst��ָ���ڴ��ַ����ʼλ���С�</li>
 *</ul>
 *@attention
 *<ul>
 *<li>ָ�����ָ��Ϸ��ڴ档</li>
 *</ul>
 *
 *@param dst   [IN/OUT]     Ŀ���ڴ����ʼλ��
 *@param src   [IN]     	Դ�ڴ����ʼλ��
 *@param len   [IN]     	Ҫ���Ƶ��ֽ���
 *
 *@retval ��
 *@par ����:
 *<ul><li>hilink_osadapter.h���ýӿ��������ڵ�ͷ�ļ���</li></ul>
 *@see hilink_strncpy
 *@since Huawei Liteos V100R002C00
 */

extern void* hilink_memcpy(void* dst, const void* src, unsigned int len);

/**
 *@ingroup hilink_osadapter
 *@brief �Ƚ������ڴ�����
 *
 *@par ����:
 *<ul>
 *<li>�����û��ṩ���ڴ��׵�ַ�����ȣ��Ƚ������ڴ��ǰlen���ֽڡ�ָ�����ָ��Ϸ��ڴ档</li>
 *</ul>
 *@attention
 *<ul>
 *<li>�ú����ǰ��ֽڱȽϵġ�</li>
 *</ul>
 *
 *@param buf1   [IN]     �ڴ�1���׵�ַ
 *@param buf2   [IN]     �ڴ�2���׵�ַ
 *@param len	[IN]     Ҫ�Ƚϵ��ֽ���
 *
 *@retval ����0   buf1����buf2
 *@retval С��0   buf1С��buf2
 *@retval ����0   buf1����buf2
 *@par ����:
 *<ul><li>hilink_osadapter.h���ýӿ��������ڵ�ͷ�ļ���</li></ul>
 *@see hilink_strncmp
 *@since Huawei Liteos V100R002C00
 */

extern int hilink_memcmp(const void* buf1, const void* buf2, unsigned int len);

/**
 *@ingroup hilink_osadapter
 *@brief �ͷ�ָ��pt��ռ�õ��ڴ�ռ䡣
 *
 *@par ����:
 *<ul>
 *<li>�����û��ṩ��ָ��pt���ͷ�����ռ�õĿռ䡣ָ�����ָ��Ϸ��ڴ档</li>
 *</ul>
 *@attention
 *<ul>
 *<li>��</li>
 *</ul>
 *
 *@param pt   [IN]     ָ��
 *
 *@retval ��
 *@par ����:
 *<ul><li>hilink_osadapter.h���ýӿ��������ڵ�ͷ�ļ���</li></ul>
 *@see hilink_memset
 *@since Huawei Liteos V100R002C00
 */

extern void hilink_free(void* pt);


/**
 *@ingroup hilink_osadapter
 *@brief �������ֽ�˳��ת��Ϊ�����ֽ�˳��
 *
 *@par ����:
 *<ul>
 *<li>���û��ṩ�������ֽ�˳���16λ��ת��Ϊ�����ֽ�˳���ʾ��16λ����</li>
 *</ul>
 *@attention
 *<ul>
 *<li>��</li>
 *</ul>
 *
 *@param ns   [IN]     �����ֽ�˳���ʾ��16λ��
 *
 *@retval �����ֽ�˳���ʾ��16λ��
 *@par ����:
 *<ul><li>hilink_osadapter.h���ýӿ��������ڵ�ͷ�ļ���</li></ul>
 *@see hilink_ntohs
 *@since Huawei Liteos V100R002C00
 */

extern unsigned short hilink_htons(unsigned short hs);

/**
 *@ingroup hilink_osadapter
 *@brief �������ֽ�˳��ת��Ϊ�����ֽ�˳��
 *
 *@par ����:
 *<ul>
 *<li>���û��ṩ�������ֽ�˳���16λ��ת��Ϊ�����ֽ�˳���ʾ��16λ����</li>
 *</ul>
 *@attention
 *<ul>
 *<li>��</li>
 *</ul>
 *
 *@param ns   [IN]     �����ֽ�˳���ʾ��16λ��
 *
 *@retval �����ֽ�˳���ʾ��16λ��
 *@par ����:
 *<ul><li>hilink_osadapter.h���ýӿ��������ڵ�ͷ�ļ���</li></ul>
 *@see hilink_htons
 *@since Huawei Liteos V100R002C00
 */

extern unsigned short hilink_ntohs(unsigned short ns);


/**
 *@ingroup hilink_osadapter
 *@brief ���ַ�����ʽ��json���ݽ���Ϊjson�ṹ�����͵����ݡ�
 *
 *@par ����:
 *<ul>
 *<li>�����û��ṩ���ַ�����ʽ��json���ݣ����н�����json�ṹ�����͵����ݡ�</li>
 *</ul>
 *@attention
 *<ul>
 *<li>��</li>
 *</ul>
 *
 *@param value   [IN]     �ַ�����ʽ��json����
 *
 *@retval NULL ����ʧ��
 *@retval ��NULL �����ɹ�������ֵΪjson�ṹ��ָ��
 *@par ����:
 *<ul><li>hilink_osadapter.h���ýӿ��������ڵ�ͷ�ļ���</li></ul>
 *@see hilink_json_delete
 *@since Huawei Liteos V100R002C00
 */

extern void* hilink_json_parse(const char *value);

/**
 *@ingroup hilink_osadapter
 *@brief ��ȡjson�ṹ���е��ַ���value��
 *
 *@par ����:
 *<ul>
 *<li>�����û��ṩ��json�ṹ��ָ��object����Ҫ��ȡ��item�����֣���json�ṹ���л�ȡ��Ӧ���ַ���value��</li>
 *</ul>
 *@attention
 *<ul>
 *<li>��</li>
 *</ul>
 *
 *@param object		[IN]     json�ṹ��ָ��
 *@param name		[IN]     ָ����Ҫ��ȡ��item������
 *@param len		[OUT] ��ȡ���ַ����ĳ���
 *
 *@retval NULL      ��ȡʧ��
 *@retval ��NULL    ��ȡ�ɹ�������ֵΪ�ַ���value�׵�ַ
 *@par ����:
 *<ul><li>hilink_osadapter.h���ýӿ��������ڵ�ͷ�ļ���</li></ul>
 *@see hilink_json_get_number_value
 *@since Huawei Liteos V100R002C00
 */

extern char* hilink_json_get_string_value(void* object, \
	char* name, unsigned int *len);

/**
 *@ingroup hilink_osadapter
 *@brief ��ȡjson�ṹ���е���ֵvalue��
 *
 *@par ����:
 *<ul>
 *<li>�����û��ṩ��json�ṹ��ָ��object����Ҫ��ȡ��item�����֣���json�ṹ���л�ȡ��Ӧ����ֵvalue��</li>
 *</ul>
 *@attention
 *<ul>
 *<li>��</li>
 *</ul>
 *
 *@param object		[IN]     json�ṹ��ָ��
 *@param name		[IN]     ָ����Ҫ��ȡ��item������
 *@param value		[OUT] �����ȡ��item��ֵ
 *
 *@retval 0    ��ȡ�ɹ�
 *@retval -1   ��ȡʧ��
 *@par ����:
 *<ul><li>hilink_osadapter.h���ýӿ��������ڵ�ͷ�ļ���</li></ul>
 *@see hilink_json_get_string_value
 *@since Huawei Liteos V100R002C00
 */

extern int hilink_json_get_number_value(void* object, \
	char* name, int *value);

/**
 *@ingroup hilink_osadapter
 *@brief �ͷ�json�ṹ����ռ�õĿռ䡣
 *
 *@par ����:
 *<ul>
 *<li>�����û��ṩ��json�ṹ��ָ��object���ͷ�json������ռ�õĿռ䡣json�ṹ��ָ�����ָ��Ϸ��ڴ档</li>
 *</ul>
 *@attention
 *<ul>
 *<li>��</li>
 *</ul>
 *
 *@param object   [IN]     json�ṹ��ָ��
 *
 *@retval ��
 *@par ����:
 *<ul><li>hilink_osadapter.h���ýӿ��������ڵ�ͷ�ļ���</li></ul>
 *@see hilink_json_parse
 *@since Huawei Liteos V100R002C00
 */

extern void hilink_json_delete(void* object);

 /**
 *@ingroup hilink_bi_get_cr
 *@brief ��ȡBI��Կ���ġ�
 *
 *@par ����:
 *<ul>
 *<li>��ȡhilink�豸BIģ��Ԥ�Ƶ���Կ���ġ�</li>
 *</ul>
 *@attention
 *<ul>
 *<li>��</li>
 *</ul>
 *
 *@param buf   		[IN/OUT]    ���ջ�����
 *@param size   	[IN]    	���ջ�������С(��λ:byte)
 *
 *@retval 0      ��ȡ��Կ���ĳɹ�
 *@retval С��0  ��ȡ��Կ����ʧ�� 
 *@par ����:
 *<ul><li>hilink_osadapter.h���ýӿ��������ڵ�ͷ�ļ���</li></ul>
 *@see hilink_bi_get_cr
 *@since Huawei Liteos V100R002C00
 */

extern int hilink_bi_get_cr(char *buf, unsigned int size);

#ifdef __cplusplus
#if __cplusplus
}
#endif /* __cpluscplus */
#endif /* __cpluscplus */

#endif  /* HILINK_OSADAPTER_H_ */

