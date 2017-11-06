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
/** @defgroup hilink_socket é€šä¿¡
 *@ingroup kernel
 */
#ifndef _HILINK_SOCKET_H_
#define _HILINK_SOCKET_H_

#ifdef __cplusplus
extern "C" {
#endif

/**
 * @ingroup hilink_socket
 * Hilink socketé”™è¯¯ç 
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
 *@brief ´´½¨ĞÂµÄudpÁ¬½Ó¡£
 *
 *@par ÃèÊö:
 *´´½¨·Ç×èÈûµÄudpÁ¬½Ó¡£
 *@attention
 *<ul>
 *<li>ÎŞ</li>
 *</ul>
 *
 *@param lport [IN] udpÌ×½Ó×Ö°ó¶¨µÄ±¾µØ¶Ë¿ÚºÅ¡£
 *
 *@retval #HILINK_SOCKET_CREAT_UDP_FD_FAILED   -2£¬´´½¨udpÁ¬½ÓÊ§°Ü
 *@retval ´óÓÚ0    ·µ»ØudpÌ×½Ó×ÖµÄÖµ£¬ÇÒÌ×½Ó×ÖÒÑ³É¹¦°ó¶¨ÁË±¾µØIPºÍÖ¸¶¨¶Ë¿ÚºÅ¡£
 *@par ÒÀÀµ:
 *<ul><li>hilink_socket.h£º¸Ã½Ó¿ÚÉùÃ÷ËùÔÚµÄÍ·ÎÄ¼ş¡£</li></ul>
 *@see hilink_udp_new
 *@since Huawei Liteos V100R002C00
 */

int hilink_udp_new(unsigned short lport);

/**
 *@ingroup hilink_socket
 *@brief Ïú»ÙudpÌ×½Ó×Ö¡£
 *
 *@par ÃèÊö:
 *Ïú»ÙudpÌ×½Ó×Ö¡£
 *@attention
 *<ul>
 *<li>ÎŞ</li>
 *</ul>
 *
 *@param fd   [IN] hilink_udp_new´´½¨µÄÌ×½Ó×Ö¡£
 *
 *@retval ÎŞ¡£
 *@par ÒÀÀµ:
 *<ul><li>hilink_socket.h£º¸Ã½Ó¿ÚÉùÃ÷ËùÔÚµÄÍ·ÎÄ¼ş¡£</li></ul>
 *@see ÎŞ
 *@since Huawei Liteos V100R002C00
 */

void hilink_udp_remove(int fd);

/**
 *@ingroup hilink_socket
 *@brief ·¢ËÍudpÊı¾İ
 *
 *@par ÃèÊö:
 *·Ç×èÈû·½Ê½·¢ËÍudpÊı¾İ
 *@attention
 *<ul>
 *<li>ÎŞ</li>
 *</ul>
 *
 *@param fd      [IN] udpÌ×½Ó×Ö¡£
 *@param buf     [IN] Ö¸Ïò´ı·¢ËÍÊı¾İ»º³åÇøµÄÖ¸Õë¡£
 *@param len     [IN] ´ı·¢ËÍÊı¾İµÄ³¤¶È£¬·¶Î§Îª[0£¬512)¡£
 *@param rip     [IN] ½ÓÊÕ·½ip¡£
 *@param rport   [IN] ½ÓÊÕ·½¶Ë¿ÚºÅ¡£
 *
 *@retval #HILINK_SOCKET_NULL_PTR      			-1£¬buf»òÕßrip²ÎÊıÎª¿Õ
 *@retval #HILINK_SOCKET_NO_ERROR      			0£¬ ·¢ËÍÊı¾İÊ±×èÈû¡£
 *@retval #HILINK_SOCKET_SEND_UDP_PACKET_FAILED -3£¬·¢ËÍ³öÏÖ´íÎó
 *@retval ´óÓÚ0     ·¢ËÍ³É¹¦£¬·µ»Ø·¢ËÍÊı¾İµÄ×Ö½ÚÊı¡£
 *@par ÒÀÀµ:
 *<ul><li>hilink_socket.h£º¸Ã½Ó¿ÚÉùÃ÷ËùÔÚµÄÍ·ÎÄ¼ş¡£</li></ul>
 *@see hilink_udp_read
 *@since Huawei Liteos V100R002C00
 */
int hilink_udp_send(int fd, const unsigned char* buf, unsigned short len,
        const char* rip, unsigned short rport);

/**
 *@ingroup hilink_socket
 *@brief ¶ÁÈ¡udpÊı¾İ
 *
 *@par ÃèÊö:
 *·Ç×èÈû·½Ê½¶ÁÈ¡udpÊı¾İ
 *@attention
 *<ul>
 *<li>ÎŞ</li>
 *</ul>
 *
 *@param fd      [IN] udpÌ×½Ó×Ö¡£
 *@param buf     [IN] Ö¸Ïò´æ·Å½ÓÊÕÊı¾İ»º³åÇøµÄÖ¸Õë¡£
 *@param len     [IN] ´æ·Å½ÓÊÕÊı¾İ»º³åÇøµÄ×î´ó³¤¶È£¬·¶Î§Îª[0£¬512)¡£
 *@param rip     [IN] Ö¸Ïò´æ·ÅÊı¾İ·¢ËÍ·½ip»º³åÇøµÄÖ¸Õë¡£
 *@param riplen  [IN] ´æ·ÅÊı¾İ·¢ËÍ·½ipµÄ»º³åÇøµÄ×î´ó³¤¶ÈÎª512¡£
 *@param rport   [IN] Êı¾İ·¢ËÍ·½¶Ë¿ÚºÅ¡£
 *
 *@retval #HILINK_SOCKET_NULL_PTR      			-1£¬buf»òrip»òrport²ÎÊıÎª¿Õ
 *@retval #HILINK_SOCKET_NO_ERROR      			0£¬ ¶ÁÈ¡Êı¾İÊ±×èÈû¡£
 *@retval #HILINK_SOCKET_REND_UDP_PACKET_FAILED -4£¬¶ÁÈ¡³öÏÖ´íÎó
 *@retval ´óÓÚ0     ¶ÁÈ¡³É¹¦£¬·µ»Ø½ÓÊÕµ½Êı¾İµÄ×Ö½ÚÊı¡£
 *@par ÒÀÀµ:
 *<ul><li>hilink_socket.h£º¸Ã½Ó¿ÚÉùÃ÷ËùÔÚµÄÍ·ÎÄ¼ş¡£</li></ul>
 *@see hilink_udp_send
 *@since Huawei Liteos V100R002C00
 */

int hilink_udp_read(int fd, unsigned char* buf, unsigned short len,
        char* rip, unsigned short riplen, unsigned short* rport);


/**
 *@ingroup hilink_socket
 *@brief ´´½¨tcpÁ¬½Ó
 *
 *@par ÃèÊö:
 *´´½¨·Ç×èÈûÄ£Ê½tcpÁ¬½Ó
 *@attention
 *<ul>
 *<li>ÎŞ</li>
 *</ul>
 *
 *@param dst      [IN] ½ÓÊÕ·½ipµØÖ·¡£
 *@param port     [IN] ½ÓÊÕ·½¶Ë¿ÚºÅ¡£
 *
 *@retval #HILINK_SOCKET_NULL_PTR      			-1£¬desÎª¿Õ¡£
 *@retval #HILINK_SOCKET_TCP_CONNECT_FAILED     -6£¬´´½¨Á¬½ÓÊ§°Ü¡£
 *@retval ´óÓÚ0     Á¬½Ó³É¹¦,·µ»ØtcpÌ×½Ó×Ö£¬È»ºóÊ¹ÓÃhilink_tcp_stateÅĞ¶ÏÁ¬½ÓÊÇ·ñÍêÈ«½¨Á¢¡£
 *@par ÒÀÀµ:
 *<ul><li>hilink_socket.h£º¸Ã½Ó¿ÚÉùÃ÷ËùÔÚµÄÍ·ÎÄ¼ş¡£</li></ul>
 *@see hilink_udp_new
 *@since Huawei Liteos V100R002C00
 */

int hilink_tcp_connect(const char* dst, unsigned short port);

/**
 *@ingroup hilink_socket
 *@brief tcpÁ¬½Ó×´Ì¬
 *
 *@par ÃèÊö:
 *²éÑ¯tcpÁ¬½Ó×´Ì¬
 *@attention
 *<ul>
 *<li>µ±fdĞ¡ÓÚ0Ê±£¬ĞèÒª¿ÉÒÔÕı³£´¦Àí£¬·µ»ØHILINK_SOCKET_TCP_CONNECT_FAILED</li>
 *</ul>
 *
 *@param fd      [IN] tcpÌ×½Ó×Ö¡£
 *
 *@retval #HILINK_SOCKET_TCP_CONNECTING      	-5£¬ÕıÔÚÁ¬½ÓÖĞ¡£
 *@retval #HILINK_SOCKET_TCP_CONNECT_FAILED     -6£¬Á¬½ÓÊ§°Ü¡£
 *@retval #HILINK_SOCKET_NO_ERROR     			0£¬ Á¬½Ó³É¹¦¡£
 *@par ÒÀÀµ:
 *<ul><li>hilink_socket.h£º¸Ã½Ó¿ÚÉùÃ÷ËùÔÚµÄÍ·ÎÄ¼ş¡£</li></ul>
 *@see ÎŞ¡£
 *@since Huawei Liteos V100R002C00
 */

int hilink_tcp_state(int fd);

/**
 *@ingroup hilink_socket
 *@brief ¶Ï¿ªtcpÁ¬½Ó¡£
 *
 *@par ÃèÊö:
 *¶Ï¿ªtcpÁ¬½Ó¡£
 *@attention
 *<ul>
 *<li>ÎŞ</li>
 *</ul>
 *
 *@param fd   [IN] hilink_tcp_connect´´½¨µÄÌ×½Ó×Ö¡£
 *
 *@retval ÎŞ¡£
 *@par ÒÀÀµ:
 *<ul><li>hilink_socket.h£º¸Ã½Ó¿ÚÉùÃ÷ËùÔÚµÄÍ·ÎÄ¼ş¡£</li></ul>
 *@see hilink_udp_remove
 *@since Huawei Liteos V100R002C00
 */

void hilink_tcp_disconnect(int fd);

/**
 *@ingroup hilink_socket
 *@brief ·¢ËÍtcpÊı¾İ
 *
 *@par ÃèÊö:
 *·Ç×èÈû·¢ËÍtcpÊı¾İ
 *@attention
 *<ul>
 *<li>ÎŞ</li>
 *</ul>
 *
 *@param fd      [IN] tcpÌ×½Ó×Ö¡£
 *@param buf     [IN] Ö¸Ïò´ı·¢ËÍÊı¾İ»º³åÇøµÄÖ¸Õë¡£
 *@param len     [IN] ´ı·¢ËÍÊı¾İµÄ³¤¶È£¬·¶Î§Îª[0£¬512)¡£
 *
 *@retval #HILINK_SOCKET_NULL_PTR      			-1£¬buf²ÎÊıÎª¿Õ¡£
 *@retval #HILINK_SOCKET_NO_ERROR      			0£¬ ·¢ËÍÊı¾İÊ±×èÈû¡£
 *@retval #HILINK_SOCKET_SEND_TCP_PACKET_FAILED -7£¬·¢ËÍ³öÏÖ´íÎó
 *@retval ´óÓÚ0     ·¢ËÍ³É¹¦£¬·µ»Ø·¢ËÍÊı¾İµÄ×Ö½ÚÊı¡£
 *@par ÒÀÀµ:
 *<ul><li>hilink_socket.h£º¸Ã½Ó¿ÚÉùÃ÷ËùÔÚµÄÍ·ÎÄ¼ş¡£</li></ul>
 *@see hilink_tcp_read
 *@since Huawei Liteos V100R002C00
 */

int hilink_tcp_send(int fd, const unsigned char* buf, unsigned short len);

/**
 *@ingroup hilink_socket
 *@brief ¶ÁÈ¡tcpÊı¾İ
 *
 *@par ÃèÊö:
 *·Ç×èÈû¶ÁÈ¡tcpÊı¾İ
 *@attention
 *<ul>
 *<li>ÎŞ</li>
 *</ul>
 *
 *@param fd      [IN] tcpÌ×½Ó×Ö¡£
 *@param buf     [IN] Ö¸Ïò´æ·Å½ÓÊÕÊı¾İ»º³åÇøµÄÖ¸Õë¡£
 *@param len     [IN] ´æ·Å½ÓÊÕÊı¾İ»º³åÇøµÄ×î´ó³¤¶È£¬·¶Î§Îª[0£¬512)¡£
 *
 *@retval #HILINK_SOCKET_NULL_PTR      			-1£¬buf²ÎÊıÎª¿Õ¡£
 *@retval #HILINK_SOCKET_NO_ERROR      			0£¬ ¶ÁÈ¡Êı¾İÊ±×èÈû¡£
 *@retval #HILINK_SOCKET_READ_TCP_PACKET_FAILED -8£¬¶ÁÈ¡³öÏÖ´íÎó
 *@retval ´óÓÚ0     ¶ÁÈ¡³É¹¦£¬·µ»Ø¶ÁÈ¡Êı¾İµÄ×Ö½ÚÊı¡£
 *@par ÒÀÀµ:
 *<ul><li>hilink_socket.h£º¸Ã½Ó¿ÚÉùÃ÷ËùÔÚµÄÍ·ÎÄ¼ş¡£</li></ul>
 *@see hilink_tcp_send
 *@since Huawei Liteos V100R002C00
 */

int hilink_tcp_read(int fd, unsigned char* buf, unsigned short len);

#ifdef __cplusplus
}
#endif

#endif
