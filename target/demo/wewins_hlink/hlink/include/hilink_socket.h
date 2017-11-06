/*----------------------------------------------------------------------------
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
/** @defgroup hilink_socket 通信
 *@ingroup kernel
 */
#ifndef _HILINK_SOCKET_H_
#define _HILINK_SOCKET_H_

#ifdef __cplusplus
extern "C" {
#endif

/**
 * @ingroup hilink_socket
 * Hilink socket错误码
 */
typedef enum {
    HILINK_SOCKET_NO_ERROR                  = 0,
    HILINK_SOCKET_NULL_PTR                  = -1,
    HILINK_SOCKET_CREAT_UDP_FD_FAILED       = -2,
    HILINK_SOCKET_SEND_UDP_PACKET_FAILED    = -3,
    HILINK_SOCKET_READ_UDP_PACKET_FAILED    = -4,
    HILINK_SOCKET_TCP_CONNECTING            = -5,
    HILINK_SOCKET_TCP_CONNECT_FAILED        = -6,
    HILINK_SOCKET_SEND_TCP_PACKET_FAILED    = -7,
    HILINK_SOCKET_READ_TCP_PACKET_FAILED    = -8,
    HILINK_SOCKET_REMOVE_UDP_FD_FAILED      = -9,
 }hilink_socket_error_t;

/**
 *@ingroup hilink_socket
 *@brief �����µ�udp���ӡ�
 *
 *@par ����:
 *������������udp���ӡ�
 *@attention
 *<ul>
 *<li>��</li>
 *</ul>
 *
 *@param lport [IN] udp�׽��ְ󶨵ı��ض˿ںš�
 *
 *@retval #HILINK_SOCKET_CREAT_UDP_FD_FAILED   -2������udp����ʧ��
 *@retval ����0    ����udp�׽��ֵ�ֵ�����׽����ѳɹ����˱���IP��ָ���˿ںš�
 *@par ����:
 *<ul><li>hilink_socket.h���ýӿ��������ڵ�ͷ�ļ���</li></ul>
 *@see hilink_udp_new
 *@since Huawei Liteos V100R002C00
 */

int hilink_udp_new(unsigned short lport);

/**
 *@ingroup hilink_socket
 *@brief ����udp�׽��֡�
 *
 *@par ����:
 *����udp�׽��֡�
 *@attention
 *<ul>
 *<li>��</li>
 *</ul>
 *
 *@param fd   [IN] hilink_udp_new�������׽��֡�
 *
 *@retval �ޡ�
 *@par ����:
 *<ul><li>hilink_socket.h���ýӿ��������ڵ�ͷ�ļ���</li></ul>
 *@see ��
 *@since Huawei Liteos V100R002C00
 */

void hilink_udp_remove(int fd);

/**
 *@ingroup hilink_socket
 *@brief ����udp����
 *
 *@par ����:
 *��������ʽ����udp����
 *@attention
 *<ul>
 *<li>��</li>
 *</ul>
 *
 *@param fd      [IN] udp�׽��֡�
 *@param buf     [IN] ָ����������ݻ�������ָ�롣
 *@param len     [IN] ���������ݵĳ��ȣ���ΧΪ[0��512)��
 *@param rip     [IN] ���շ�ip��
 *@param rport   [IN] ���շ��˿ںš�
 *
 *@retval #HILINK_SOCKET_NULL_PTR      			-1��buf����rip����Ϊ��
 *@retval #HILINK_SOCKET_NO_ERROR      			0�� ��������ʱ������
 *@retval #HILINK_SOCKET_SEND_UDP_PACKET_FAILED -3�����ͳ��ִ���
 *@retval ����0     ���ͳɹ������ط������ݵ��ֽ�����
 *@par ����:
 *<ul><li>hilink_socket.h���ýӿ��������ڵ�ͷ�ļ���</li></ul>
 *@see hilink_udp_read
 *@since Huawei Liteos V100R002C00
 */
int hilink_udp_send(int fd, const unsigned char* buf, unsigned short len,
        const char* rip, unsigned short rport);

/**
 *@ingroup hilink_socket
 *@brief ��ȡudp����
 *
 *@par ����:
 *��������ʽ��ȡudp����
 *@attention
 *<ul>
 *<li>��</li>
 *</ul>
 *
 *@param fd      [IN] udp�׽��֡�
 *@param buf     [IN] ָ���Ž������ݻ�������ָ�롣
 *@param len     [IN] ��Ž������ݻ���������󳤶ȣ���ΧΪ[0��512)��
 *@param rip     [IN] ָ�������ݷ��ͷ�ip��������ָ�롣
 *@param riplen  [IN] ������ݷ��ͷ�ip�Ļ���������󳤶�Ϊ512��
 *@param rport   [IN] ���ݷ��ͷ��˿ںš�
 *
 *@retval #HILINK_SOCKET_NULL_PTR      			-1��buf��rip��rport����Ϊ��
 *@retval #HILINK_SOCKET_NO_ERROR      			0�� ��ȡ����ʱ������
 *@retval #HILINK_SOCKET_REND_UDP_PACKET_FAILED -4����ȡ���ִ���
 *@retval ����0     ��ȡ�ɹ������ؽ��յ����ݵ��ֽ�����
 *@par ����:
 *<ul><li>hilink_socket.h���ýӿ��������ڵ�ͷ�ļ���</li></ul>
 *@see hilink_udp_send
 *@since Huawei Liteos V100R002C00
 */

int hilink_udp_read(int fd, unsigned char* buf, unsigned short len,
        char* rip, unsigned short riplen, unsigned short* rport);


/**
 *@ingroup hilink_socket
 *@brief ����tcp����
 *
 *@par ����:
 *����������ģʽtcp����
 *@attention
 *<ul>
 *<li>��</li>
 *</ul>
 *
 *@param dst      [IN] ���շ�ip��ַ��
 *@param port     [IN] ���շ��˿ںš�
 *
 *@retval #HILINK_SOCKET_NULL_PTR      			-1��desΪ�ա�
 *@retval #HILINK_SOCKET_TCP_CONNECT_FAILED     -6����������ʧ�ܡ�
 *@retval ����0     ���ӳɹ�,����tcp�׽��֣�Ȼ��ʹ��hilink_tcp_state�ж������Ƿ���ȫ������
 *@par ����:
 *<ul><li>hilink_socket.h���ýӿ��������ڵ�ͷ�ļ���</li></ul>
 *@see hilink_udp_new
 *@since Huawei Liteos V100R002C00
 */

int hilink_tcp_connect(const char* dst, unsigned short port);

/**
 *@ingroup hilink_socket
 *@brief tcp����״̬
 *
 *@par ����:
 *��ѯtcp����״̬
 *@attention
 *<ul>
 *<li>��fdС��0ʱ����Ҫ����������������HILINK_SOCKET_TCP_CONNECT_FAILED</li>
 *</ul>
 *
 *@param fd      [IN] tcp�׽��֡�
 *
 *@retval #HILINK_SOCKET_TCP_CONNECTING      	-5�����������С�
 *@retval #HILINK_SOCKET_TCP_CONNECT_FAILED     -6������ʧ�ܡ�
 *@retval #HILINK_SOCKET_NO_ERROR     			0�� ���ӳɹ���
 *@par ����:
 *<ul><li>hilink_socket.h���ýӿ��������ڵ�ͷ�ļ���</li></ul>
 *@see �ޡ�
 *@since Huawei Liteos V100R002C00
 */

int hilink_tcp_state(int fd);

/**
 *@ingroup hilink_socket
 *@brief �Ͽ�tcp���ӡ�
 *
 *@par ����:
 *�Ͽ�tcp���ӡ�
 *@attention
 *<ul>
 *<li>��</li>
 *</ul>
 *
 *@param fd   [IN] hilink_tcp_connect�������׽��֡�
 *
 *@retval �ޡ�
 *@par ����:
 *<ul><li>hilink_socket.h���ýӿ��������ڵ�ͷ�ļ���</li></ul>
 *@see hilink_udp_remove
 *@since Huawei Liteos V100R002C00
 */

void hilink_tcp_disconnect(int fd);

/**
 *@ingroup hilink_socket
 *@brief ����tcp����
 *
 *@par ����:
 *����������tcp����
 *@attention
 *<ul>
 *<li>��</li>
 *</ul>
 *
 *@param fd      [IN] tcp�׽��֡�
 *@param buf     [IN] ָ����������ݻ�������ָ�롣
 *@param len     [IN] ���������ݵĳ��ȣ���ΧΪ[0��512)��
 *
 *@retval #HILINK_SOCKET_NULL_PTR      			-1��buf����Ϊ�ա�
 *@retval #HILINK_SOCKET_NO_ERROR      			0�� ��������ʱ������
 *@retval #HILINK_SOCKET_SEND_TCP_PACKET_FAILED -7�����ͳ��ִ���
 *@retval ����0     ���ͳɹ������ط������ݵ��ֽ�����
 *@par ����:
 *<ul><li>hilink_socket.h���ýӿ��������ڵ�ͷ�ļ���</li></ul>
 *@see hilink_tcp_read
 *@since Huawei Liteos V100R002C00
 */

int hilink_tcp_send(int fd, const unsigned char* buf, unsigned short len);

/**
 *@ingroup hilink_socket
 *@brief ��ȡtcp����
 *
 *@par ����:
 *��������ȡtcp����
 *@attention
 *<ul>
 *<li>��</li>
 *</ul>
 *
 *@param fd      [IN] tcp�׽��֡�
 *@param buf     [IN] ָ���Ž������ݻ�������ָ�롣
 *@param len     [IN] ��Ž������ݻ���������󳤶ȣ���ΧΪ[0��512)��
 *
 *@retval #HILINK_SOCKET_NULL_PTR      			-1��buf����Ϊ�ա�
 *@retval #HILINK_SOCKET_NO_ERROR      			0�� ��ȡ����ʱ������
 *@retval #HILINK_SOCKET_READ_TCP_PACKET_FAILED -8����ȡ���ִ���
 *@retval ����0     ��ȡ�ɹ������ض�ȡ���ݵ��ֽ�����
 *@par ����:
 *<ul><li>hilink_socket.h���ýӿ��������ڵ�ͷ�ļ���</li></ul>
 *@see hilink_tcp_send
 *@since Huawei Liteos V100R002C00
 */

int hilink_tcp_read(int fd, unsigned char* buf, unsigned short len);

#ifdef __cplusplus
}
#endif

#endif
