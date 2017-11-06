
#include <stdio.h>
#include <string.h>
#include <qcom/qcom_utils.h>
#include <qcom/base.h>
#include <qcom/basetypes.h>
#include <qcom/stdint.h>
#include "qcom/socket_api.h"
#include "qcom/select_api.h"
#include "qcom/qcom_network.h"
#include "qcom/timetype.h"
#include "hilink_socket.h"
#include "hilink_osadapter.h"
#include "ww_hlink.h"


#define MULTICAST_GROUP_ADDR4 "238.238.238.238"
#define IP_LEN (15+1)

unsigned int wmf_inet_addr(char *str)
{
	DEBUG;
	unsigned int ipaddr;
	unsigned int data[4];
	unsigned int ret;

	ret = A_SSCANF(str, "%d.%d.%d.%d", data, data + 1, data + 2, data + 3);
	if( data[0] == 255 && data[1] == 255 )
	{
		data[2] = 255;
		data[3] = 255;
	}
	printf("ipaddr: %d:%d:%d:%d\n", data[0],data[1],data[2],data[3]);
	if (ret < 0) {
		printf("####invalid str ip\n");
		return 0;
	} else {
		printf("#str=%s,data=0x%x%x%x%x\n",str,data[0],data[1],data[2],data[3]);
		ipaddr = data[0] << 24 | data[1] << 16 | data[2] << 8 | data[3];
	}
	return htonl(ipaddr);		//�����ֽ���ת��Ϊ�����ֽ���
}

int wmf_inet_ntoa(const int ip, char *str)
{
	DEBUG;
	memset(str, 0, sizeof (str));
	qcom_sprintf(str, "%u.%u.%u.%u",
	      (unsigned) (ip >> 24),
	      (unsigned) ((ip >> 16) & 0xff), (unsigned) ((ip >> 8) & 0xff), (unsigned) (ip & 0xff));
	return 0;
}


int hilink_sec_get_Ac(unsigned char* pAc, unsigned int ulLen)
{
	DEBUG;
	unsigned char AC[48] = {0x29, 0x21, 0x2d, 0x20, 0x74, 0x51, 0x34, 0x29, 0x46, 0x74, 0x49, 0x66, 0x6a, 0x61, 0x4a, 0x66, 0xe2, 0x95, 0x3c, 0x6c, 0xcb, 0xdb, 0x19, 0x01, 0xfe, 0x8f, 0xa5, 0xfc, 0xe2, 0x2b, 0x1b, 0x61, 0x38, 0x92, 0x1c, 0x6b, 0x93, 0x0b, 0x64, 0x1f, 0x2b, 0x00, 0x46, 0x16, 0x55, 0x1e, 0x79, 0x1a};
	if (NULL == pAc)
	{
		printf("\n\r invalid PARAM\n\r");
		return -1;
	}
	memcpy(pAc, AC, 48);
	return 0;
}
int hilink_network_state(int* state) 
{
   	DEBUG;
	A_STATUS ret = -1;
	uint32_t dns[8];
	uint32_t num;
	ret = qcom_dns_server_address_get(dns, &num);
	if(ret == A_OK)
	{
		ret = qcom_ping(dns[0], sizeof(dns[0]));
		if(ret == A_OK)
		{
			return 0;
		}else{
			return -1;
		}
	}else{
		return -1;
	}
	return 0;
}
int hilink_get_local_ip(char* local_ip, unsigned char ip_len)
{
	DEBUG;
	char ip_buf[IP_LEN];
  	 A_UINT32 ip=0, mask, gate;
    	if(A_OK == qcom_ip_address_get(0, &ip, &mask, &gate))
    	{

		memset(ip_buf, 0, IP_LEN);
		qcom_sprintf(ip_buf, "%u.%u.%u.%u",
            	(unsigned) (ip >> 24),
            	(unsigned) ((ip >> 16) & 0xff), (unsigned) ((ip >> 8) & 0xff), (unsigned) (ip & 0xff));
		strncpy(local_ip, ip_buf, strlen(ip_buf));
		ip_len = strlen(ip_buf);
		return 0;		
	}
	return -1;
}
//add wangyu
int hilink_udp_new(unsigned short lport)
{
	DEBUG;
	int fd = HILINK_SOCKET_NO_ERROR;
	
#if 1
	struct sockaddr_in servaddr;

	fd = qcom_socket(AF_INET,SOCK_DGRAM,0);
	if(fd < 0)
	{
		printf("creat socket fd failed\n");
		return HILINK_SOCKET_CREAT_UDP_FD_FAILED;
	}

	if(lport != 0)
	{
		memset(&servaddr, 0, sizeof(struct sockaddr_in));
		servaddr.sin_family = AF_INET;
		servaddr.sin_addr.s_addr = htonl(INADDR_ANY);
		servaddr.sin_port = htons(lport);
		if(qcom_bind(fd,(struct sockaddr *)&servaddr, sizeof(struct sockaddr_in)) < 0)
		{
			qcom_socket_close(fd);
			printf("bind port %u failed\n",lport);
			return HILINK_SOCKET_CREAT_UDP_FD_FAILED;
		}
	}else
	{
		return HILINK_SOCKET_NULL_PTR;
	}
#endif
    return fd;
}
void hilink_udp_remove(int fd)
{
	DEBUG;
	int ret = -1;
	if(fd>0)
	 {
		ret = qcom_socket_close(fd);
		if(ret < 0)
			//return HILINK_SOCKET_REMOVE_UDP_FD_FAILED;
			return;
	 }
}

/* *@retval #HILINK_SOCKET_NULL_PTR      			-1��buf����rip����Ϊ��
 *@retval #HILINK_SOCKET_NO_ERROR      			0�� ��������ʱ������
 *@retval #HILINK_SOCKET_SEND_UDP_PACKET_FAILED -3�����ͳ��ִ���
 *@retval ����0     ���ͳɹ������ط������ݵ��ֽ�����
 */
int hilink_udp_send(int fd, const unsigned char* buf, unsigned short len,
        const char* rip, unsigned short rport)
{
	DEBUG;
	int ret = -1;
	if(!buf || !rip)
	{
		return HILINK_SOCKET_NULL_PTR;
	}
#if 1
	struct sockaddr_in dstaddr;
	memset(&dstaddr, 0, sizeof(struct sockaddr_in));
	dstaddr.sin_addr.s_addr = wmf_inet_addr((char*)rip);
	dstaddr.sin_port = htons(rport);

	ret = qcom_sendto(fd, (char*)buf, len, 0, (struct sockaddr*)&dstaddr,
	        sizeof(struct sockaddr_in));
	if(ret == len)
	{
		return ret;
	}
	else{
		return HILINK_SOCKET_SEND_UDP_PACKET_FAILED;
	}
#endif
}

/*
*@retval #HILINK_SOCKET_NULL_PTR      			-1��buf��rip��rport����Ϊ��
 *@retval #HILINK_SOCKET_NO_ERROR      			0�� ��ȡ����ʱ������
 *@retval #HILINK_SOCKET_REND_UDP_PACKET_FAILED -4����ȡ���ִ���
 *@retval ����0     ��ȡ�ɹ������ؽ��յ����ݵ��ֽ�����
*/
int hilink_udp_read(int fd, unsigned char* buf, unsigned short len,
         char* rip, unsigned short riplen, unsigned short* rport)
{
	DEBUG;
	int length = 0;
	if(buf == NULL || rip == NULL || rport == NULL)
	{
		return HILINK_SOCKET_NULL_PTR;
	}
#if 1
	struct sockaddr_in fromaddr;

	int addrlen = sizeof(fromaddr);

	length = (int)(qcom_recvfrom(fd, (char*)buf, len, 0,
	            (struct sockaddr*)&fromaddr, &addrlen));
	if(length < 0)
	{
	     return HILINK_SOCKET_READ_UDP_PACKET_FAILED;
	}

    	buf[length] = '\0';
    	printf("udp recv data:%s,length = %d\n", buf, length);
		
	wmf_inet_ntoa(fromaddr.sin_addr.s_addr, rip);
		
	*rport = ntohs(fromaddr.sin_port);

	printf("remote ip:%s,port:%u\n", rip, *rport);
#endif
    return length;

}

/*
 *@retval #HILINK_SOCKET_NULL_PTR      			-1��desΪ�ա�
 *@retval #HILINK_SOCKET_TCP_CONNECT_FAILED     -6����������ʧ�ܡ�
 *@retval ����0     ���ӳɹ�,����tcp�׽��֣�Ȼ��ʹ��hilink_tcp_state�ж������Ƿ���ȫ������
 */
int hilink_tcp_connect(const char* dst, unsigned short port)
{
	DEBUG;
	if(!dst)
	{
		return HILINK_SOCKET_NULL_PTR;
	}

#if 1
	struct sockaddr_in servaddr;
	int fd;

	fd = qcom_socket(AF_INET, SOCK_STREAM, 0);
	if(fd < 0)
	{
	    printf("creat socket fd failed\n");
	    return HILINK_SOCKET_TCP_CONNECT_FAILED;
	}

	memset(&servaddr, 0, sizeof(struct sockaddr_in));
	servaddr.sin_family = AF_INET;
	servaddr.sin_addr.s_addr = wmf_inet_addr((char*)dst);
	servaddr.sin_port = htons(port);
	if(qcom_connect(fd, (struct sockaddr *)&servaddr, sizeof(struct sockaddr_in)) == 0)
	{
	    return fd;
	}
	else
	{
	    qcom_socket_close(fd);
	    return HILINK_SOCKET_TCP_CONNECT_FAILED;
	}
#endif
}

void hilink_tcp_disconnect(int fd)
{
	DEBUG;
	if(fd)
		qcom_socket_close(fd);
}

/*
 *@retval #HILINK_SOCKET_TCP_CONNECTING      	-5�����������С�
 *@retval #HILINK_SOCKET_TCP_CONNECT_FAILED     -6������ʧ�ܡ�
 *@retval #HILINK_SOCKET_NO_ERROR     			0�� ���ӳɹ���
 */
int hilink_tcp_state(int fd)
{
	DEBUG;
	int errcode = HILINK_SOCKET_NO_ERROR;
	if(fd < 0) {
		return HILINK_SOCKET_TCP_CONNECT_FAILED;
	}
	q_fd_set rset, wset;
	int ready_n;

	FD_ZERO(&rset);
	FD_SET(fd, &rset);
	FD_SET(fd, &wset);

	struct timeval timeout;
	timeout.tv_sec = 3;
	timeout.tv_usec = 0;

	ready_n = qcom_select(fd + 1, &rset, &wset, NULL, &timeout);
	if(0 == ready_n)
	{
		printf("select timeout\n");  
		errcode = HILINK_SOCKET_TCP_CONNECTING;         
	}
	else if(ready_n < 0)
	{
		printf("select error\n");  
		errcode = HILINK_SOCKET_TCP_CONNECT_FAILED; 
	}
	else
	{
		int ret;
		int len;
		if(0 != qcom_getsockopt (fd, SOL_SOCKET, SO_ERROR, &ret, len))
		{
			printf("getsocketopt failed\r\n");
			errcode = HILINK_SOCKET_TCP_CONNECT_FAILED;
		}
		printf("getsocketopt ret=%d\r\n",ret);
		if(0 != ret)
		{
			errcode = HILINK_SOCKET_TCP_CONNECT_FAILED;
		}
	}
             
    return errcode;
}

/* *@retval #HILINK_SOCKET_NULL_PTR      			-1��buf����Ϊ�ա�
 *@retval #HILINK_SOCKET_NO_ERROR      			0�� ��ȡ����ʱ������
 *@retval #HILINK_SOCKET_READ_TCP_PACKET_FAILED -8����ȡ���ִ���
 *@retval ����0     ��ȡ�ɹ������ض�ȡ���ݵ��ֽ�����
 */
int hilink_tcp_read(int fd, unsigned char* buf, unsigned short len)
{
	DEBUG;
	int ret = -1;

	if(buf == NULL)
	{
		return HILINK_SOCKET_NULL_PTR;
	}

	ret = (int)(qcom_recv(fd, (char*)buf, len, MSG_DONTWAIT));
	if(ret < 0)
	{
	 	return HILINK_SOCKET_READ_TCP_PACKET_FAILED;
	}

	return ret;
}

/* *@retval #HILINK_SOCKET_NULL_PTR      			-1��buf����Ϊ�ա�
 *@retval #HILINK_SOCKET_NO_ERROR      			0�� ��������ʱ������
 *@retval #HILINK_SOCKET_SEND_TCP_PACKET_FAILED -7�����ͳ��ִ���
 *@retval ����0     ���ͳɹ������ط������ݵ��ֽ�����
 */
int hilink_tcp_send(int fd, const unsigned char* buf, unsigned short len)
{
	DEBUG;
	int ret = -1;

	if(buf == NULL)
	{
		return HILINK_SOCKET_NULL_PTR;
	}

	ret = (int)(qcom_send(fd, (char*)buf, len, MSG_DONTWAIT));
	if(ret != len)
	{
		return HILINK_SOCKET_SEND_TCP_PACKET_FAILED;
	}
	return ret;
}
unsigned short hilink_htons(unsigned short hs)
{
	DEBUG;
	return htons(hs);
}
unsigned short hilink_ntohs(unsigned short ns)
{
	DEBUG;
	return ntohs(ns);
}
extern A_INT32 isdigit(A_INT32 c);
A_INT32
inet_aton(A_CHAR *name,
          /* [IN] dotted decimal IP address */
          A_UINT32 * ipaddr_ptr
          /* [OUT] binary IP address */
    )
{                               /* Body */
	DEBUG;
    A_INT8 ipok = FALSE;
    A_UINT32 dots;
    A_UINT32 byte;
    A_UINT32 addr;

    addr = 0;
    dots = 0;
    for (;;) {
        if (!isdigit(*name))
            break;

        byte = 0;
        while (isdigit(*name)) {
            byte *= 10;
            byte += *name - '0';
            if (byte > 255)
                break;
            name++;
        }                       /* Endwhile */

        if (byte > 255)
            break;

        addr <<= 8;
        addr += byte;
        if (*name == '.') {
            dots++;
            if (dots > 3)
                break;
            name++;
            continue;
        }
      if ((*name == '\0') && (dots == 3)) {
         ipok = TRUE;
      } /* Endif */
      break;
   } /* Endfor */
   if (!ipok) {
      return 0;
   } /* Endif */
   if (ipaddr_ptr) {
      *ipaddr_ptr = addr;
   } /* Endif */
   return -1;
}                               /* Endbody */

int ww_inet_ntoa(const int ip, char *str)
{
	DEBUG;
	memset(str, 0, sizeof (str));
	qcom_sprintf(str, "%u.%u.%u.%u",
	      (unsigned) (ip >> 24),
	      (unsigned) ((ip >> 16) & 0xff), (unsigned) ((ip >> 8) & 0xff), (unsigned) (ip & 0xff));
	return 0;
}


int hilink_gethostbyname(char* hostname, char ip_list[][40], int num)
{
	DEBUG;
	if(strlen(hostname) == 0)
		return -1;
#if 1
	 A_UINT32 ip;
	 int res;
	 res = qcom_dnsc_get_host_by_name(hostname,&ip);
	 if (res!=0)
	 {
	        return -1;
	 }
	 if(num>0)
	{
		ww_inet_ntoa(ip, ip_list[0]);
		printf("tmp ip = %s\n", ip_list[0]);
	}
#endif

       return 0;
}






