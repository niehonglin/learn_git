/*
  * Copyright (c) 2015 Qualcomm Atheros, Inc..
  * All Rights Reserved.
  * Qualcomm Atheros Confidential and Proprietary.
  */

#include "qcom_common.h"
#include "swat_parse.h"
#include "qcom/tx_alloc_api.h"
#include "swat_bench_core.h"
#include "qcom/socket_api.h"
#include "qcom/select_api.h"
#include "qcom/qcom_ssl.h"
#include <qcom/qcom_timer.h>
#include "qcom_app_strrcl.h"
#include "swat_parse.h"

#define		CALL_TCP_RX	0x1001
#define		CALL_TCP_TX	0x1002
#define		CALL_UDP_RX	0x2001
#define		CALL_UDP_TX	0x2002

/*Flag controls IPv4 - v6 switch*/
int v6_enabled = 0;
volatile A_BOOL flag_strrcl_request = 0;
volatile A_BOOL flag_strrcl_start = 0;
volatile A_BOOL recall_flag = 0;
volatile A_BOOL flag_new_console_task;

volatile A_UINT32 strrcl_time_ms = 0;

A_BOOL   app_resume_done = 0;

SSL_ROLE_T ssl_role;

#if defined(KF_ROM_1_1_2_INICHE)


static qcom_timer_t benchTcpTxTimer[CALC_STREAM_NUMBER];
static qcom_timer_t benchUdpTxTimer[CALC_STREAM_NUMBER];
#ifdef SWAT_BENCH_RAW
static qcom_timer_t benchRawTxTimer[CALC_STREAM_NUMBER];
#endif
static A_INT32 benchTcpTxTimerStop[CALC_STREAM_NUMBER];
static A_INT32 benchUdpTxTimerStop[CALC_STREAM_NUMBER];
#ifdef SWAT_BENCH_RAW
static A_INT32 benchRawTxTimerStop[CALC_STREAM_NUMBER];
#endif

extern STREAM_CXT_t cxtTcpTxPara[CALC_STREAM_NUMBER];

#define  STORE_RECALL_SIZE   (sizeof(struct strrcl_buf))

//A_UINT8  buf_strrcl[sizeof(struct strrcl_buf)];
A_UINT32 buf_strrcl_len = STORE_RECALL_SIZE;

void tcp_data_store (struct strrcl_buf *pStrrcl, STREAM_CXT_t *pTcpPara)
{
   int   i;
//   struct strrcl_buf *pStrrcl = (struct strrcl_buf *)buf_strrcl;

   for (i=0; i< CALC_STREAM_NUMBER; i++, pTcpPara++)
   {
       pStrrcl->socketLocal[i] = pTcpPara->socketLocal;
       pStrrcl->pktSize[i] = pTcpPara->param.pktSize;
       pStrrcl->mode[i] = pTcpPara->param.mode;
       pStrrcl->numOfPkts[i] = pTcpPara->param.numOfPkts;
       pStrrcl->current_packets[i] = pTcpPara->param.current_packets;
       pStrrcl->ipAddr[i] = pTcpPara->param.ipAddress;
       pStrrcl->port[i] = pTcpPara->param.port;
       pStrrcl->clientFd[i] = pTcpPara->clientFd;
       qcom_printf("pktsize=%d\n", pTcpPara->param.pktSize);
   }
}

void tcp_data_restore(STREAM_CXT_t *pTcpPara, A_UINT8 *pbuf)
{
   int    i;
   struct strrcl_buf *pStrrcl = (struct strrcl_buf *)pbuf;

   for (i=0; i< CALC_STREAM_NUMBER; i++, pTcpPara++)
   {
       pTcpPara->socketLocal = pStrrcl->socketLocal[i];
       pTcpPara->clientFd = pStrrcl->clientFd[i];

       pTcpPara->param.pktSize = pStrrcl->pktSize[i];
       pTcpPara->param.mode = pStrrcl->mode[i];
       pTcpPara->param.numOfPkts = pStrrcl->numOfPkts[i];
       pTcpPara->param.current_packets = pStrrcl->current_packets[i];
       pTcpPara->param.ipAddress = pStrrcl->ipAddr[i];
       pTcpPara->param.port = pStrrcl->port[i];
   }
}

void  udp_data_store(struct strrcl_buf *pStrrcl, STREAM_CXT_t *pUdpPara)
{
   int   i;
//   struct strrcl_buf *pStrrcl = (struct strrcl_buf *)buf_strrcl;

   for (i=0; i< CALC_STREAM_NUMBER; i++, pUdpPara++)
   {
       pStrrcl->socketLocal[i] = pUdpPara->socketLocal;
       pStrrcl->pktSize[i] = pUdpPara->param.pktSize;
       pStrrcl->mode[i] = pUdpPara->param.mode;
       pStrrcl->numOfPkts[i] = pUdpPara->param.numOfPkts;
       pStrrcl->current_packets[i] = pUdpPara->param.current_packets;
       pStrrcl->ipAddr[i] = pUdpPara->param.ipAddress;
       pStrrcl->port[i] = pUdpPara->param.port;
   }
}

void udp_data_restore(STREAM_CXT_t *pUdpPara, A_UINT8 *pbuf)
{
   int    i;
   struct strrcl_buf *pStrrcl = (struct strrcl_buf *)pbuf;

   for (i=0; i< CALC_STREAM_NUMBER; i++, pUdpPara++)
   {
       pUdpPara->socketLocal = pStrrcl->socketLocal[i];
       pUdpPara->param.pktSize = pStrrcl->pktSize[i];
       pUdpPara->param.mode = pStrrcl->mode[i];
       pUdpPara->param.numOfPkts = pStrrcl->numOfPkts[i];
       pUdpPara->param.current_packets = pStrrcl->current_packets[i];
       pUdpPara->param.ipAddress = pStrrcl->ipAddr[i];
       pUdpPara->param.port = pStrrcl->port[i];
    }
}

void
tcp_tx_init(A_UINT32 ipAddress, IP6_ADDR_T ip6Address, A_UINT32 port, A_UINT32 protocol, A_UINT32 pktSize,
                                                        A_UINT32 mode, A_UINT32 seconds, A_UINT32 numOfPkts,
                                                        A_UINT32 local_ipAddress, A_UINT32 ip_hdr_inc, A_UINT32 delay)
{
    A_UINT8 index = 0;
    A_UINT8 ret = 0;

   
    for (index = 0; index < CALC_STREAM_NUMBER; index++) {
            ret = swat_cxt_index_find(&tcpTxIndex[index]);

            if (CALC_STREAM_NUMBER_INVALID != ret) {
                swat_cxt_index_configure(&tcpTxIndex[index]);
                break;
            }
    }
    if (CALC_STREAM_NUMBER_INVALID == ret) {
            SWAT_PTF("Warning tcpTxIndex is full\n");
            return;
        }

            /* Database Initial */
    swat_database_initial(&cxtTcpTxPara[index]);
            /* Update Database */
    swat_database_set(&cxtTcpTxPara[index], ipAddress, &ip6Address, INADDR_ANY, NULL, NULL, port, protocol,
                          pktSize, mode, seconds, numOfPkts, TEST_DIR_TX, local_ipAddress,ip_hdr_inc,0,delay);
        /* Client */
    cxtTcpTxPara[index].index = index;
}

void tcp_rx_init(A_UINT32 protocol, A_UINT32 port, A_UINT32 localIpAddress, A_UINT32 mcastIpAddress, 
                 IP6_ADDR_T* lIp6, IP6_ADDR_T* mIp6, A_UINT32 ip_hdr_inc, A_UINT32 rawmode)
{
    A_UINT8 index = 0;
    A_UINT8 ret = 0;

        /*Calc Handle */
    for (index = 0; index < CALC_STREAM_NUMBER; index++) {
            ret = swat_cxt_index_find(&tcpRxIndex[index]);

            if (CALC_STREAM_NUMBER_INVALID != ret) {
                swat_cxt_index_configure(&tcpRxIndex[index]);
                break;
            }
    }
 
    if (CALC_STREAM_NUMBER_INVALID == ret) {
            SWAT_PTF("Warning tcpRxIndex is full\n");
            return;
    }

    /* DataBase Initial */
    swat_database_initial(&cxtTcpRxPara[index]);

    /* Update DataBase */
    swat_database_set(&cxtTcpRxPara[index], INADDR_ANY, NULL, INADDR_ANY, NULL, NULL, port, protocol,
                          1500, 0, 0, 0, TEST_DIR_RX, 0, 0,0,0);

    /* Server */
    cxtTcpRxPara[index].index = index;
}

void
udp_tx_init(A_UINT32 ipAddress, IP6_ADDR_T ip6Address, A_UINT32 port, A_UINT32 protocol, A_UINT32 pktSize,
                                                        A_UINT32 mode, A_UINT32 seconds, A_UINT32 numOfPkts,
                                                        A_UINT32 local_ipAddress, A_UINT32 ip_hdr_inc, A_UINT32 delay)
{
    A_UINT8 index = 0;
    A_UINT8 ret = 0;

        /*Calc Handle */
        for (index = 0; index < CALC_STREAM_NUMBER; index++) {
            ret = swat_cxt_index_find(&udpTxIndex[index]);

            if (CALC_STREAM_NUMBER_INVALID != ret) {
                swat_cxt_index_configure(&udpTxIndex[index]);
                break;
            }
        }
        if (CALC_STREAM_NUMBER_INVALID == ret) {
            SWAT_PTF("Warning udpTxIndex is full\n");
            return;
        }

        /* DataBase Initial */
        swat_database_initial(&cxtUdpTxPara[index]);

        /* Update Database */
        swat_database_set(&cxtUdpTxPara[index], ipAddress, &ip6Address, INADDR_ANY, NULL, NULL, port, protocol,
                          pktSize, mode, seconds, numOfPkts, TEST_DIR_TX,local_ipAddress,ip_hdr_inc,0,delay);

        /* Client */
        cxtUdpTxPara[index].index = index;
}

void
udp_rx_init(A_UINT32 protocol, A_UINT32 port, A_UINT32 localIpAddress, A_UINT32 mcastIpAddress,
                 IP6_ADDR_T* lIp6, IP6_ADDR_T* mIp6, A_UINT32 ip_hdr_inc, A_UINT32 rawmode)
{
    A_UINT8 index = 0;
    A_UINT8 ret = 0;

        /*Calc Handle */
    for (index = 0; index < CALC_STREAM_NUMBER; index++) {
            ret = swat_cxt_index_find(&udpRxIndex[index]);

            if (CALC_STREAM_NUMBER_INVALID != ret) {
                swat_cxt_index_configure(&udpRxIndex[index]);
                break;
            }
        }

    if (CALC_STREAM_NUMBER_INVALID == ret) {
            //SWAT_PTF("Warning udpRxIndex is full\n");
            return;
        }

        /* DataBase Initial */
    swat_database_initial(&cxtUdpRxPara[index]);

        /* Update DataBase */
    swat_database_set(&cxtUdpRxPara[index], INADDR_ANY, NULL, mcastIpAddress, lIp6, mIp6, port,
                          protocol, 1500, 0, 0, 0, TEST_DIR_RX,0,0,0,0);

        /* Server */
    cxtUdpRxPara[index].index = index;
}

int app_strrcl_recall(A_UINT8 *pbuf, A_UINT32 buf_strrcl_len)
{
   IP6_ADDR_T ip6addr;
   struct strrcl_buf *pStrrcl = (struct strrcl_buf *)pbuf;
   
   A_MEMSET(&ip6addr,0,sizeof(IP6_ADDR_T));
   qcom_printf("flag=%d\n", pStrrcl->flags);
   flag_new_console_task = 0;

   switch (pStrrcl->flags)
   {
   case	 CALL_TCP_RX:
        tcp_rx_init(TEST_PROT_TCP, 0, 0, 0, 0, 0, 0, 0);

	tcp_data_restore (cxtTcpRxPara, pbuf);
        swat_bench_rx_test(TEST_PROT_TCP, 0, 0, 0, 0, 0, 0, 0);

        break;
   case  CALL_TCP_TX:
        tcp_tx_init(0, ip6addr, 0, TEST_PROT_TCP, 0, 0, 0, 0, 0, 0, 0);

        tcp_data_restore (cxtTcpTxPara, pbuf);
        swat_bench_tx(0, ip6addr, 0, TEST_PROT_TCP, 0, 0, 0, 0, 0, 0, 0);

        break;
   case  CALL_UDP_RX:
        udp_rx_init (TEST_PROT_UDP, 0, 0, 0, 0, 0, 0, 0);

        udp_data_restore(cxtUdpRxPara, pbuf);
        swat_bench_rx_test(TEST_PROT_UDP, 0, 0, 0, 0, 0, 0, 0);

        break;
   case  CALL_UDP_TX:
        udp_tx_init(0, ip6addr, 0, TEST_PROT_UDP, 0, 0, 0, 0, 0, 0, 0);

        udp_data_restore(cxtUdpTxPara, pbuf);
        swat_bench_tx(0, ip6addr, 0, TEST_PROT_UDP, 0, 0, 0, 0, 0, 0, 0);
        break;
   }
}

A_UINT8  *app_strrcl_store(A_UINT32 buf_strrcl_len)
{
   struct strrcl_buf *pStrrcl = (struct strrcl_buf *)swat_mem_malloc(buf_strrcl_len);
   if (pStrrcl == NULL)
      return NULL;

   pStrrcl->flags = flag_strrcl_request;

/*
 *  this implementation only support one stream store/recall
 *  so overwritting flag_strrcl_request doesn't happen
 */
   switch (flag_strrcl_request)
   {
   case  CALL_TCP_RX:
        tcp_data_store (pStrrcl, cxtTcpRxPara);
        break;
   case  CALL_TCP_TX:
        tcp_data_store (pStrrcl, cxtTcpTxPara);
        break;
   case  CALL_UDP_RX:
        udp_data_store (pStrrcl, cxtUdpRxPara);
        break;
   case  CALL_UDP_TX:
        udp_data_store (pStrrcl, cxtUdpTxPara);
        break;
   }
   return (A_UINT8 *)pStrrcl;
}

static void
swat_bench_tcp_timer_handler(unsigned int alarm, void *data)
{
    benchTcpTxTimerStop[*(A_UINT32 *) data] = 1;
}

static void
swat_bench_udp_timer_handler(unsigned int alarm, void *data)
{
    benchUdpTxTimerStop[*(A_UINT32 *) data] = 1;
}


void
swat_bench_tcp_timer_clean(A_UINT32 index)
{
    swat_time_stop(&benchTcpTxTimer[index]);
    swat_time_delete(&benchTcpTxTimer[index]);
}

void
swat_bench_udp_timer_clean(A_UINT32 index)
{
    swat_time_stop(&benchUdpTxTimer[index]);
    swat_time_delete(&benchUdpTxTimer[index]);
}


#ifdef SWAT_BENCH_RAW
static void swat_bench_raw_timer_handler(unsigned int alarm, void *data)
{
    benchRawTxTimerStop[*(A_UINT32*)data] = 1;
}
#endif
void
swat_bench_timer_init(A_UINT32 seconds, A_UINT32 protocol, A_UINT32 index)
{
    if (TEST_PROT_TCP == protocol || TEST_PROT_SSL == protocol) {
        benchTcpTxTimerStop[index] = 0;
        benchTcpTxTimer[index].arg = index;
        swat_time_init(&benchTcpTxTimer[index], swat_bench_tcp_timer_handler,
                       (void *) &(benchTcpTxTimer[index].arg), seconds * 1000, ONESHOT);
        swat_time_start(&benchTcpTxTimer[index]);
    }
    if (TEST_PROT_UDP == protocol) {
        benchUdpTxTimerStop[index] = 0;
        benchUdpTxTimer[index].arg = index;
        swat_time_init(&benchUdpTxTimer[index], swat_bench_udp_timer_handler,
                       (void *) &(benchUdpTxTimer[index].arg), seconds * 1000, ONESHOT);
        swat_time_start(&benchUdpTxTimer[index]);
    }
#ifdef SWAT_BENCH_RAW
    if (TEST_PROT_RAW == protocol)    
    {   
        benchRawTxTimerStop[index] = 0;  
        benchRawTxTimer[index].arg = index;
        swat_time_init(&benchRawTxTimer[index], swat_bench_raw_timer_handler, 
                (void *)&(benchRawTxTimer[index].arg), seconds * 1000, ONESHOT);   
        swat_time_start(&benchRawTxTimer[index]);    
    }
#endif
}

void
swat_tcp_tx_handle(STREAM_CXT_t * pStreamCxt)
{
    A_INT32 sendBytes = 0 ;
    A_UINT32 sumBytes = 0;
//    A_UINT32 currentPackets = 0;
    A_UINT8 *pDataBuffer = NULL;
#ifdef SWAT_SSL
    A_INT32 result = 0;
    SSL_INST *ssl = &ssl_inst[SSL_CLIENT_INST];
#endif
    SWAT_PTR_NULL_CHK(pStreamCxt);

    /* Initial Bench Value */
    swat_bench_quit_init();

    /* Initial Calc & Time */
    pStreamCxt->calc.firstTime.milliseconds = CALC_TIME_DEF;
    pStreamCxt->calc.lastTime.milliseconds = CALC_TIME_DEF;
    pStreamCxt->calc.bytes = CALC_BYTES_DEF;
    pStreamCxt->calc.kbytes = CALC_KBYTES_DEF;

    qcom_printf("packet size=%d\n", pStreamCxt->param.pktSize);

    /* Malloc Packet Buffer Size */
    pDataBuffer = swat_mem_malloc(pStreamCxt->param.pktSize);
    if (NULL == pDataBuffer) {
        SWAT_PTF("TCP TX data buffer malloc error\r\n");
        /* Close Socket */
        swat_socket_close(pStreamCxt);
        return;
    }
    pStreamCxt->param.pktBuff = pDataBuffer;

    /* Initial Packet */
    SWAT_PTF("Sending...\r\n");

    /* Get First Time */
    swat_get_first_time(pStreamCxt);
    if (TEST_MODE_TIME == pStreamCxt->param.mode) {
        swat_bench_timer_init(pStreamCxt->param.seconds, pStreamCxt->param.protocol,
                              pStreamCxt->index);
    }
#ifdef SWAT_SSL
    if (pStreamCxt->param.protocol ==TEST_PROT_SSL)
    {
        if (ssl->ssl == NULL)
        {
            // Create SSL connection object
            ssl->ssl = qcom_SSL_new(ssl->sslCtx);
            if (ssl->ssl == NULL)
            {
                printf("ERROR: Unable to create SSL context\n");
                goto ERROR;
            }

            // configure the SSL connection
            if (ssl->config_set)
            {
                result = qcom_SSL_configure(ssl->ssl, &ssl->config);
                if (result < A_OK)
                {
                    printf("ERROR: SSL configure failed (%d)\n", result);
                   goto ERROR;
                }
            }
        }

        // Add socket handle to SSL connection
        result = qcom_SSL_set_fd(ssl->ssl, pStreamCxt->socketLocal);
        if (result < 0)
        {
            printf("ERROR: Unable to add socket handle to SSL (%d)\n", result);
           goto ERROR;
        }

        // SSL handshake with server
        result = qcom_SSL_connect(ssl->ssl);
        if (result < 0)
        {
            if (result == ESSL_TRUST_CertCnTime)
            {
                /** The peer's SSL certificate is trusted, CN matches the host name, time is valid */
                printf("The certificate is trusted\n");
            }
            else if (result == ESSL_TRUST_CertCn)
            {
                /** The peer's SSL certificate is trusted, CN matches the host name, time is expired */
                printf("ERROR: The certificate is expired\n");
                goto ERROR;
            }
            else if (result == ESSL_TRUST_CertTime)
            {
                /** The peer's SSL certificate is trusted, CN does NOT match the host name, time is valid */
                printf("ERROR: The certificate is trusted, but the host name is not valid\n");
                goto ERROR;
            }
            else if (result == ESSL_TRUST_Cert)
            {
                /** The peer's SSL certificate is trusted, CN does NOT match host name, time is expired */
                printf("ERROR: The certificate is expired and the host name is not valid\n");
                goto ERROR;
            }
            else if (result == ESSL_TRUST_None)
            {
                /** The peer's SSL certificate is NOT trusted */
                printf("ERROR: The certificate is NOT trusted\n");
                goto ERROR;
            }
            else
		    {
		        printf("ERROR: SSL connect failed (%d)\n", result);
		        goto ERROR;
		    }
        }
    }
#endif 
    qcom_printf("send ... socket=%d packet size=%d\n", pStreamCxt->socketLocal, pStreamCxt->param.pktSize);
    
    flag_strrcl_request = CALL_TCP_TX;

    while (1) {

       while (flag_strrcl_start)
       {
	  qcom_thread_msleep(100);
       }

       if (swat_bench_quit()) {
            /* Get Last Time For Pressure */
            //SWAT_PTF("Warning Bench Quit!!\n");
            swat_get_last_time(pStreamCxt);
            break;
       }
#ifdef SWAT_SSL
           if (pStreamCxt->param.protocol ==TEST_PROT_SSL)
           {
               sendBytes = qcom_SSL_write(ssl->ssl, (char *)pDataBuffer, pStreamCxt->param.pktSize);
           }
           else
#endif
       {
            sendBytes =
            swat_send(pStreamCxt->socketLocal, (char *) pDataBuffer,
                      pStreamCxt->param.pktSize, 0);
       }

        /* delay */
       if (gNumOfWlanDevices > 1)
       {
            A_UINT32 uidelay;

            /* 
            * For concurrency, delay 100ms. 
            * So the other device's chops have oppotunity to be schedulered. 
            */
            uidelay = (pStreamCxt->param.delay > 100) ? pStreamCxt->param.delay : 100;
            qcom_thread_msleep(pStreamCxt->param.delay);
        }
        else if (pStreamCxt->param.delay) {
            qcom_thread_msleep(pStreamCxt->param.delay);
        }
      
        if (sendBytes < 0) {
            if (TX_BUFF_FAIL == sendBytes) {
                /* buff full will send fail, it should be normal */
                SWAT_PTF("[bench id %d, port %d]buffer full\r\n", pStreamCxt->index,
                         (pStreamCxt->param.port));
            } else {
                SWAT_PTF
                    ("[bench id %d, port %d]TCP Socket send is error %d sumBytes = %d\r\n",
                     pStreamCxt->index, (pStreamCxt->param.port), sendBytes, sumBytes);

				swat_get_last_time(pStreamCxt);
                break;
            }
        } else {
            /* bytes & kbytes */
            sumBytes += sendBytes;
        }

	if (sendBytes >0){
	    swat_bytes_calc(pStreamCxt, sendBytes);
	}

        /* Packets Mode */
        if (TEST_MODE_PACKETS == pStreamCxt->param.mode) {
            pStreamCxt->param.current_packets++;
            if (0 != (sumBytes / pStreamCxt->param.pktSize)) {
                //currentPackets += (sumBytes / pStreamCxt->param.pktSize);
                sumBytes = sumBytes % pStreamCxt->param.pktSize;
            }

            if (pStreamCxt->param.current_packets >= pStreamCxt->param.numOfPkts) {
		swat_get_last_time(pStreamCxt);
                break;
            }
        }

        /* Time Mode */
        if (TEST_MODE_TIME == pStreamCxt->param.mode) 
        {
            if (0 != benchTcpTxTimerStop[pStreamCxt->index]) {
                swat_get_last_time(pStreamCxt);
                break;
            }
        }
    }


    swat_test_result_print(pStreamCxt);

    SWAT_PTF("IOT Throughput Test Completed.\n");

#ifdef SWAT_SSL
ERROR: 
    if (ssl_role == SSL_CLIENT &&  ssl->ssl != NULL)
    {
        qcom_SSL_shutdown(ssl->ssl);
        ssl->ssl = NULL;
    }
    if (ssl_role == SSL_CLIENT) {
        ssl_role = 0;
    }
#endif
       
   flag_strrcl_request = 0;

  /* Free Buffer */
    swat_buffer_free(&(pStreamCxt->param.pktBuff));
    /* Close Socket */
    swat_socket_close(pStreamCxt);
}

void
swat_tcp_rx_data(STREAM_CXT_t * pStreamCxt)
{
    A_INT32 recvBytes = 0;
    A_UINT8 *pDataBuffer = NULL;
    struct timeval tmo;
    A_INT32 fdAct = 0;
    q_fd_set sockSet,master;
#ifdef SWAT_SSL    
    SSL_INST *ssl = &ssl_inst[SSL_SERVER_INST];
#endif
    SWAT_PTR_NULL_CHK(pStreamCxt);
    /* Initial pBuffer */
    /* Malloc Packet Buffer Size */
    pDataBuffer = swat_mem_malloc(pStreamCxt->param.pktSize);
    if (NULL == pDataBuffer) {
        SWAT_PTF("TCP RX data buffer malloc error\r\n");
        return;
    }
    pStreamCxt->param.pktBuff = pDataBuffer;

    /* Initial Packet */
    swat_mem_set(pDataBuffer, 0, sizeof (A_UINT8) * pStreamCxt->param.pktSize);

    /* Get First Time */
    swat_get_first_time(pStreamCxt);
    swat_fd_zero(&master);
    swat_fd_set(pStreamCxt->clientFd, &master);
    if (swat_fd_isset(pStreamCxt->clientFd, &master) == 0)
        goto EXIT;

    pStreamCxt->pfd_set = (void *) &sockSet;
    tmo.tv_sec	= 1;
    tmo.tv_usec = 0;

    flag_strrcl_request = CALL_TCP_RX;

    while (1) {
       if (swat_bench_quit()) 
       {
            /* Get Last Time For Pressure */
            //SWAT_PTF("Bench quit!!\r\n");
            swat_get_last_time(pStreamCxt);
            break;
       }

        /* Wait for Input */
        /* Copy the master list everytime before calling select() because select
         * modifies sockSet */
       sockSet = master;
       fdAct = swat_select(pStreamCxt->clientFd + 1, &sockSet, NULL, NULL, &tmo);  //k_select()
       if (0 != fdAct) 
       {
            if (swat_fd_isset(pStreamCxt->clientFd, &sockSet)) 
            {
#ifdef SWAT_SSL                
                if (pStreamCxt->param.protocol == TEST_PROT_SSL){
                    recvBytes = qcom_SSL_read(ssl->ssl, (char*) pDataBuffer, pStreamCxt->param.pktSize);
                }
                else
#endif                    
                {
                    recvBytes = swat_recv(pStreamCxt->clientFd, (char *) pDataBuffer, pStreamCxt->param.pktSize, 0);
                }
                if (recvBytes <= 0) {
                    SWAT_PTF("swat_recv fail\n");
                    swat_get_last_time(pStreamCxt);
                    break;
                } else {
                    swat_bytes_calc(pStreamCxt, recvBytes);
                }
            }
            else{
                swat_get_last_time(pStreamCxt);
                SWAT_PTF("swat_fd_isset fail\n");
                goto EXIT;
            }
       }
    }
EXIT:    
    swat_test_result_print(pStreamCxt);
#ifdef SWAT_SSL
    if (ssl_role == SSL_SERVER && ssl->ssl != NULL)
    {
       qcom_SSL_shutdown(ssl->ssl);
       ssl->ssl = NULL;
    }
#endif

    SWAT_PTF("Waiting.\n");
    flag_strrcl_request = 0;

    /* Free Buffer */
    swat_buffer_free(&(pStreamCxt->param.pktBuff));
    swat_fd_zero(&sockSet);
    pStreamCxt->pfd_set = NULL;
}

void
swat_tcp_rx_handle(STREAM_CXT_t * pStreamCxt)
{
    A_INT32 ret = 0;
    A_UINT32 clientIp;
    A_UINT16 clientPort;
    A_INT32 clientFd = -1;
    A_UINT32 isFirst = 0;
    struct sockaddr_in  clientAddr;
    struct sockaddr_in6 client6Addr;
    A_INT32 len;
    struct timeval tmo;
    A_INT32 fdAct = 0;
    q_fd_set sockSet,master;
    char ip_str[48];
//    A_STATUS   status;
#ifdef SWAT_SSL    
    SSL_INST *ssl = &ssl_inst[SSL_SERVER_INST];
#endif
    A_BOOL  is_first;

    SWAT_PTR_NULL_CHK(pStreamCxt);

    /* Initial Bench Value */
    swat_bench_quit_init();

    swat_fd_zero(&master);
    swat_fd_set(pStreamCxt->socketLocal, &master);
    pStreamCxt->pfd_set = (void *) &sockSet;
    tmo.tv_sec = 2;
    tmo.tv_usec = 0;

    if(v6_enabled){
        len = sizeof(struct sockaddr_in6);
    }else{
        len = sizeof(struct sockaddr_in);
    }
    is_first = 1;

    while (1) {
        if  (!qcom_strrcl_in_recall() || is_first == 0 || app_resume_done == 1)
        {
       // if (0 == isFirst) {
           ret = swat_listen(pStreamCxt->socketLocal, 10);
           if (ret < 0) {
                /* Close Socket */
                //SWAT_PTF("Failed to listen socket %d.\n", pStreamCxt->socketLocal);
                //goto QUIT;
           }
           isFirst = 1;
        //}
           while (1) {
             if (swat_bench_quit()) {
                goto QUIT;
             }
            /* Copy the master list everytime before calling select() because select
             * modifies sockSet */
             sockSet = master;
             fdAct = swat_select(pStreamCxt->socketLocal, &sockSet, NULL, NULL, &tmo);   //k_select()
             if (fdAct != 0) {
                break;
             }
          }
          if(v6_enabled){
            if((clientFd = swat_accept(pStreamCxt->socketLocal, (struct sockaddr *) &client6Addr, &len)) < 0){
                /* Close Socket */
                SWAT_PTF("Failed to accept socket %d.\n", clientFd);
                goto QUIT;
            }
            //clientIp = client6Addr.sin6_addr;
            clientPort = ntohs(client6Addr.sin6_port);
            SWAT_PTF("Receiving from %s:%d\n",inet6_ntoa((char*)&client6Addr.sin6_addr,(char *)ip_str),clientPort);
          }
          else
          {
              clientFd = swat_accept(pStreamCxt->socketLocal, (struct sockaddr *) &clientAddr, &len);

              if(clientFd < 0)
              {
            /* Close Socket */
               SWAT_PTF("Failed to accept socket %d.\n", clientFd);
               goto QUIT;
              }
              clientIp = ntohl(clientAddr.sin_addr.s_addr);
              clientPort = ntohs(clientAddr.sin_port);

              SWAT_PTF("Fd=%d Receiving from 0x%x Remote port:%d \r\n",
                 clientFd, clientIp, clientPort);
          }
        pStreamCxt->clientFd = clientFd;

        /* Initial Calc & Time */
        pStreamCxt->calc.firstTime.milliseconds = CALC_TIME_DEF;
        pStreamCxt->calc.lastTime.milliseconds  = CALC_TIME_DEF;
        pStreamCxt->calc.bytes  = CALC_BYTES_DEF;
        pStreamCxt->calc.kbytes = CALC_KBYTES_DEF;
#ifdef SWAT_SSL
            if (pStreamCxt->param.protocol == TEST_PROT_SSL)
            {
                if (ssl->ssl == NULL)
                {
                    // Create SSL connection object
                    ssl->ssl = qcom_SSL_new(ssl->sslCtx);
                    if (ssl->ssl == NULL)
                    {
                        printf("ERROR: Unable to create SSL context\n");
                        goto QUIT;
                    }

                    // configure the SSL connection
                    if (ssl->config_set)
                    {
                        ret = qcom_SSL_configure(ssl->ssl, &ssl->config);
                        if (ret < A_OK)
                        {
                            printf("ERROR: SSL configure failed (%d)\n", ret);
                            goto QUIT;
                        }
                    }
                }

                // Add socket handle to SSL connection
                ret = qcom_SSL_set_fd(ssl->ssl, clientFd);
                if (ret < A_OK)
                {
                    printf("ERROR: Unable to add socket handle to SSL\n");
                    goto QUIT;
                }

                // SSL handshake with server
                ret = qcom_SSL_accept(ssl->ssl);
                if (ret < 0)
                {
                    printf("ERROR: SSL accept failed (%d)\n", ret);
                    goto QUIT;
                }
            }
#endif
        } 
        else
        {
           is_first = 0;
        }

        swat_tcp_rx_data(pStreamCxt);
        //ssl_role = 0;
        /* Close Client Socket */
        swat_close(pStreamCxt->clientFd);
        pStreamCxt->clientFd = -1;
    }
QUIT:
    swat_test_result_print(pStreamCxt);
//    SWAT_PTF("*************IOT Throughput Test Completed **************\n");
    SWAT_PTF("Shell> ");
    /* Init fd_set */
    swat_fd_zero(&sockSet);
    /* Close Client Socket */
    swat_close(pStreamCxt->clientFd);

#ifdef SWAT_SSL
    if (ssl_role == SSL_SERVER && ssl->ssl != NULL)
    {
       qcom_SSL_shutdown(ssl->ssl);
       ssl->ssl = NULL;
    }
    if (ssl_role == SSL_SERVER) {
        ssl_role = 0;
    }
#endif

    pStreamCxt->pfd_set = NULL;
    /* Close Socket */
    swat_socket_close(pStreamCxt);
    app_resume_done = 1;
}

void
swat_udp_tx_handle(STREAM_CXT_t * pStreamCxt)
{
    A_INT32 sendBytes = 0;
    A_INT32 fromSize = 0;
    A_UINT32 sumBytes = 0;
    A_UINT32 currentPackets = 0;
    A_UINT8 *pDataBuffer = NULL;
    struct sockaddr_in remoteAddr;
    struct sockaddr_in6 remote6Addr;
    EOT_PACKET_t eotPacket;
//    A_STATUS  status;

    A_INT32 sendTerminalCount = 0;

    SWAT_PTR_NULL_CHK(pStreamCxt);

    /* Initial Bench Value */
    swat_bench_quit_init();

    /* Initial Calc & Time */
    pStreamCxt->calc.firstTime.milliseconds = CALC_TIME_DEF;
    pStreamCxt->calc.lastTime.milliseconds = CALC_TIME_DEF;
    pStreamCxt->calc.bytes = CALC_BYTES_DEF;
    pStreamCxt->calc.kbytes = CALC_KBYTES_DEF;

    /* Malloc Packet Buffer Size */
    pDataBuffer = swat_mem_malloc(pStreamCxt->param.pktSize);
    if (NULL == pDataBuffer) {
        SWAT_PTF("UDP TX data buffer malloc error:%d\r\n", pStreamCxt->param.pktSize);
        /* Close Socket */
        swat_socket_close(pStreamCxt);
        return;
    }
    pStreamCxt->param.pktBuff = pDataBuffer;
    /* Prepare IP address & port */
    memset(&remoteAddr, 0, sizeof (struct sockaddr_in));
    memset(&remote6Addr, 0, sizeof (struct sockaddr_in6));

        remoteAddr.sin_addr.s_addr= HTONL(pStreamCxt->param.ipAddress);
        remoteAddr.sin_port = HTONS(pStreamCxt->param.port);
        remoteAddr.sin_family = AF_INET;
        fromSize = sizeof (struct sockaddr_in);

    /* Get First Time */
    swat_get_first_time(pStreamCxt);
    if (TEST_MODE_TIME == pStreamCxt->param.mode) {
        swat_bench_timer_init(pStreamCxt->param.seconds, pStreamCxt->param.protocol,
                              pStreamCxt->index);
    }

    qcom_printf("start send start=%d\n", pStreamCxt->param.current_packets);
    qcom_printf("UDP IP %d.%d.%d.%d prepare OK\r\n",
             (pStreamCxt->param.ipAddress) >> 24 & 0xFF, (pStreamCxt->param.ipAddress) >> 16 & 0xFF,
             (pStreamCxt->param.ipAddress) >> 8 & 0xFF, (pStreamCxt->param.ipAddress) & 0xFF);

    flag_strrcl_request = CALL_UDP_TX;

    while (1) {

        while (flag_strrcl_start)
        {
           qcom_thread_msleep(100);
        }

        if (swat_bench_quit()) {
            /* Get Last Time For Pressure */
            //SWAT_PTF("Bench quit!!\r\n");
            swat_get_last_time(pStreamCxt);
            break;
        }

//        if(v6_enabled){
//            sendBytes = swat_sendto(pStreamCxt->socketLocal, (char*) pDataBuffer,
//                                pStreamCxt->param.pktSize, 0,
//                                 (struct sockaddr*)&remote6Addr, sizeof (struct sockaddr_in6));
//        }else{
            sendBytes = swat_sendto(pStreamCxt->socketLocal, (char*) pDataBuffer,
                                pStreamCxt->param.pktSize, 0,
                                 (struct sockaddr*)&remoteAddr, sizeof (struct sockaddr_in));
//        }
		/*delay */
	if (pStreamCxt->param.delay){
	   qcom_thread_msleep(pStreamCxt->param.delay);
	}

        if (sendBytes < 0) {
            SWAT_PTF("UDP Socket send is error %d sumBytes = %d\r\n", sendBytes, sumBytes);
            /* Free Buffer */
            //swat_buffer_free(&(pStreamCxt->param.pktBuff));
            qcom_thread_msleep(100);
            //break;
        } else {
            /* bytes & kbytes */
            sumBytes += sendBytes;
        }

	if (sendBytes > 0){
	    swat_bytes_calc(pStreamCxt, sendBytes);
        }

        /* Packets Mode */
        if (TEST_MODE_PACKETS == pStreamCxt->param.mode) {
            pStreamCxt->param.current_packets++;
            if (0 != (sumBytes / pStreamCxt->param.pktSize)) {
                //currentPackets += (sumBytes / pStreamCxt->param.pktSize);
                sumBytes = sumBytes % pStreamCxt->param.pktSize;
            }

            if (pStreamCxt->param.current_packets >= pStreamCxt->param.numOfPkts) {
		swat_get_last_time(pStreamCxt);
                break;
            }
        }

        /* Time Mode */
        if (TEST_MODE_TIME == pStreamCxt->param.mode) {
			/* Get Last Time */
            if (0 != benchUdpTxTimerStop[pStreamCxt->index]) {
                swat_get_last_time(pStreamCxt);
                break;
            }
        }
    }


    int send_fail = 0;
    /* Tell ath_console TX is complete end mark is AABBCCDD */
    while (send_fail <= 10) {
        eotPacket.code = 0xAABBCCDD;
        eotPacket.packetCount = currentPackets;
        if(v6_enabled){
            sendBytes = swat_sendto(pStreamCxt->socketLocal, (char *) & eotPacket,
                                sizeof (EOT_PACKET_t), 0, (struct sockaddr *) &remote6Addr,
                                sizeof (struct sockaddr_in6));
        }else{
            sendBytes = swat_sendto(pStreamCxt->socketLocal, (char *) & eotPacket,
                                sizeof (EOT_PACKET_t), 0, (struct sockaddr *) &remoteAddr,
                                sizeof (struct sockaddr_in));
        }
        if (sendBytes < 0) {
            SWAT_PTF("UDP send terminate packet error %d , retry %d \r\n", sendBytes,
                     sendTerminalCount);

            send_fail ++;
            qcom_thread_msleep(100);
        } else {
            qcom_thread_msleep(100);

            sendTerminalCount++;
            if(sendTerminalCount > 2)
                break;
        }
    }

    flag_strrcl_request = 0;

    swat_test_result_print(pStreamCxt);
    SWAT_PTF("*************IOT Throughput Test Completed **************\n");

    /* Free Buffer */
    swat_buffer_free(&(pStreamCxt->param.pktBuff));

    /* Close Socket */
    swat_socket_close(pStreamCxt);
}

void
swat_udp_rx_data(STREAM_CXT_t * pStreamCxt)
{
    A_INT32 recvBytes = 0;
    A_INT32 fromSize = 0;
    A_UINT32 sumBytes = 0;
    A_UINT8 *pDataBuffer = NULL;
    struct sockaddr_in fromAddr;
    struct sockaddr_in6 from6Addr;
    q_fd_set sockSet, master;
    struct timeval tmo;
    A_INT32 fdAct = 0;
    A_UINT32 isFirst = 1;
    A_UINT32 clientIp = 0;
    A_UINT16 clientPort = 0;
    A_UINT32 totalInterval = 0;
    A_INT32 sendBytes = 0;
    STAT_PACKET_t StatPacket;
    A_UINT8 ip6_str[48];
//    A_STATUS   status;
    A_INT32 sendTerminalCount = 0;

    SWAT_PTR_NULL_CHK(pStreamCxt);


    /* Initial Bench Value */
    swat_bench_quit_init();

    /* Malloc Packet Buffer Size */
    pDataBuffer = swat_mem_malloc(pStreamCxt->param.pktSize);
    if (NULL == pDataBuffer) {
        SWAT_PTF("UDP RX data buffer malloc error\r\n");
        return;
    }
    pStreamCxt->param.pktBuff = pDataBuffer;

    memset(&StatPacket, 0, sizeof (StatPacket));

    /* Prepare IP address & port */
    if(v6_enabled){
        memset(&from6Addr, 0, sizeof (struct sockaddr_in6));
        fromSize = sizeof (struct sockaddr_in6);
    }else{
        memset(&fromAddr, 0, sizeof (struct sockaddr_in));
        fromSize = sizeof (struct sockaddr_in);
    }

    /* Init fd_set */
    swat_fd_zero(&master);
    swat_fd_set(pStreamCxt->socketLocal, &master);
    pStreamCxt->pfd_set = (void *)&master;
    tmo.tv_sec = 1;
    tmo.tv_usec = 0;

    /* Get First Time */
	//removed for EV138076 by zhengang
    //swat_get_first_time(pStreamCxt);
    flag_strrcl_request = CALL_UDP_RX;

    while (1) {

        if (swat_bench_quit()) {
            /* Get Last Time For Pressure */
	    if(!isFirst)
		swat_get_last_time(pStreamCxt);
            break;
        }

    /* Copy the master list everytime before calling select() because select
         * modifies sockSet */
        sockSet = master;        /* Wait for Input */
        fdAct = swat_select(pStreamCxt->socketLocal + 1, &sockSet, NULL, NULL, &tmo);   //k_select()
        if (0 != fdAct) {
            if (swat_fd_isset(pStreamCxt->socketLocal, &sockSet)) {
                if(v6_enabled){
                    recvBytes = swat_recvfrom(pStreamCxt->socketLocal, (char*) pDataBuffer,
                                          pStreamCxt->param.pktSize, 0,
                                          (struct sockaddr *) &from6Addr, &fromSize);

                }else{
                    recvBytes = swat_recvfrom(pStreamCxt->socketLocal, (char*) pDataBuffer,
                                          pStreamCxt->param.pktSize, 0,
                                          (struct sockaddr *) &fromAddr, &fromSize);
                }
                if (recvBytes <= 0) {
                    SWAT_PTF("UDP Socket receive is error %d, sumBytes = %d\r\n", recvBytes,
                             sumBytes);
                    break;
                
                }

                if (recvBytes >= sizeof (EOT_PACKET_t)) 
                {
                    if (isFirst) {
                        if (recvBytes > sizeof (EOT_PACKET_t)) {
                            clientPort = ntohs(fromAddr.sin_port);
                            if(v6_enabled){
                                clientPort = ntohs(from6Addr.sin6_port);
                                inet6_ntoa((char *)&from6Addr.sin6_addr,(char *)ip6_str);
                                SWAT_PTF("UDP receving from %s port:%d \r\n",ip6_str,clientPort);
                            }else{
                            clientIp = ntohl(fromAddr.sin_addr.s_addr);

                            SWAT_PTF("UDP receving from 0x%x port:%d \r\n",
                                     clientIp, clientPort);
                            }
                            isFirst = 0;
							/* Initial Calc & Time */
							//for EV 138076, only init the data when it is a new udp connection
			pStreamCxt->calc.firstTime.milliseconds = CALC_TIME_DEF;
			pStreamCxt->calc.lastTime.milliseconds = CALC_TIME_DEF;
			pStreamCxt->calc.bytes	= CALC_BYTES_DEF;
			pStreamCxt->calc.kbytes = CALC_KBYTES_DEF;

                            swat_get_first_time(pStreamCxt);
                        }
                        swat_bytes_calc(pStreamCxt, recvBytes);
                    } else {

                    	/*End packet is not count*/
                    	if (recvBytes > sizeof (EOT_PACKET_t)) {
							swat_bytes_calc(pStreamCxt, recvBytes);
                    	}
						else {
							/* Update Port */
                            if(v6_enabled){
                                from6Addr.sin6_port = HTONS(pStreamCxt->param.port);
                                from6Addr.sin6_family = AF_INET6;	/* End Of Transfer */
                            }else{
                                fromAddr.sin_port = HTONS(pStreamCxt->param.port);
                                fromAddr.sin_family = AF_INET;	/* End Of Transfer */
                            }
                            swat_get_last_time(pStreamCxt);

                            totalInterval =
                                (pStreamCxt->calc.lastTime.milliseconds -
                                 pStreamCxt->calc.firstTime.milliseconds);
                            StatPacket.kbytes = pStreamCxt->calc.kbytes;
                            StatPacket.bytes = pStreamCxt->calc.bytes;
                            StatPacket.msec = totalInterval;
                            /* Tell ath_console TX received end mark AABBCCDD with throughput value */
                            while (sendTerminalCount <= 10) {
                                if(v6_enabled){
                                    sendBytes = swat_sendto(pStreamCxt->socketLocal,
                                                            (char *) (&StatPacket),
                                                            sizeof (STAT_PACKET_t), 0,
                                                            (struct sockaddr*)&from6Addr,
                                                            sizeof (struct sockaddr_in6));

                                }else{
                                    sendBytes = swat_sendto(pStreamCxt->socketLocal,
                                                            (char *) (&StatPacket),
                                                            sizeof (STAT_PACKET_t), 0,
                                                            (struct sockaddr*)&fromAddr,
                                                            sizeof (struct sockaddr_in));
                                }
                                if (sendBytes < 0) {
                                    SWAT_PTF("UDP send throughput info packet error %d , retry %d \r\n",
                                                 sendBytes, sendTerminalCount);
                                    qcom_thread_msleep(100);
                                } else {
                                    /* Clean */
                                    tmo.tv_sec = 2;
                                    tmo.tv_usec = 0;
                                    while (recvBytes == sizeof (EOT_PACKET_t)) {
                                        /* Copy the master list everytime before calling select() because select
                                         * modifies sockSet */
                                        sockSet = master;        /* Wait for Input */

                                        fdAct = swat_select(pStreamCxt->socketLocal + 1, &sockSet, NULL, NULL, &tmo);   //k_select()
                                        if (0 == fdAct) {
                                            SWAT_PTF("UDP break\n");
                                            break;
                                        }

                                        if (swat_fd_isset(pStreamCxt->socketLocal, &sockSet)) {
                                            if(v6_enabled){
                                                recvBytes = swat_recvfrom(pStreamCxt->socketLocal, (char*) pDataBuffer,
                                                                          pStreamCxt->param.pktSize, 0,
                                                                          (struct sockaddr *) &from6Addr, &fromSize);

                                            }else{
                                                recvBytes = swat_recvfrom(pStreamCxt->socketLocal, (char*) pDataBuffer,
                                                                          pStreamCxt->param.pktSize, 0,
                                                                          (struct sockaddr *)&fromAddr, &fromSize);
                                           }
                                       }
                                   }
                                   break;
                               }
                               sendTerminalCount++;
                           }
                           break;
                       }
                   }
               }
            }
        }
    }

    flag_strrcl_request = 0;

    swat_test_result_print(pStreamCxt);

    SWAT_PTF("Waiting.\n");
    /* Free Buffer */
    swat_buffer_free(&(pStreamCxt->param.pktBuff));

}
#ifdef SWAT_BENCH_RAW
void swat_raw_rx_data(STREAM_CXT_t* pStreamCxt)
{
    A_INT32  recvBytes = 0;
    A_INT32  fromSize = 0;
    A_UINT32 sumBytes = 0;
    A_UINT8* pDataBuffer = NULL;
    struct sockaddr_in fromAddr;
    q_fd_set master;
    q_fd_set sockSet;
    struct timeval tmo;
    A_INT32  fdAct = 0;
    A_UINT32 isFirst = 1;
    A_UINT32 clientIp = 0;
    A_UINT16 clientPort = 0;
    A_UINT32 totalInterval = 0;
    A_UINT32 sendBytes = 0;
    STAT_PACKET_t StatPacket;
    ip_header *iphdr;
    unsigned short int ip_flags[4];
    int size;
    A_UINT8 ip_str[16];
    A_INT32 sendTerminalCount = 0;

    SWAT_PTR_NULL_CHK(pStreamCxt);

    /* Initial Bench Value */
    swat_bench_quit_init();

    /* Malloc Packet Buffer Size */
    pDataBuffer = swat_mem_malloc(pStreamCxt->param.pktSize);
    if (NULL == pDataBuffer)
    {
        SWAT_PTF("RAW RX data buffer malloc error\r\n");
        return;
    }
    pStreamCxt->param.pktBuff = pDataBuffer;

    memset(&StatPacket, 0, sizeof(StatPacket));

    /* Prepare IP address & port */
    memset(&fromAddr, 0, sizeof(struct sockaddr_in));
    fromSize = sizeof(struct sockaddr_in);

    /* Init fd_set */
    swat_fd_zero(&master);
    swat_fd_set(pStreamCxt->socketLocal, &master);
    pStreamCxt->pfd_set = (void *)&master;
    tmo.tv_sec  = 1;
    tmo.tv_usec = 0;

    while(1)
    {
        if (swat_bench_quit())
        {

            if(!isFirst)
                swat_get_last_time(pStreamCxt);
            break;
        }

        sockSet = master;

        /* Wait for Input */
        fdAct = swat_select(pStreamCxt->socketLocal + 1, &sockSet, NULL, NULL, &tmo); //k_select()
        if (0 != fdAct)
        {
            if (swat_fd_isset(pStreamCxt->socketLocal, &sockSet))
            {
                recvBytes = swat_recvfrom(pStreamCxt->socketLocal, (CHAR*)pDataBuffer, 
                        CFG_PACKET_SIZE_MAX_RX, 0, (struct sockaddr *)&fromAddr, &fromSize);
                if (recvBytes <= 0)
                {
                    SWAT_PTF("RAW Socket receive is error %d, sumBytes = %d\r\n", recvBytes, sumBytes);
                    continue;
                }
                else
                {
                    sumBytes += recvBytes;
                    //SWAT_PTF("RAW Socket receive %d, sumBytes = %d\r\n", recvBytes, sumBytes);
                }
            if(pStreamCxt->param.rawmode == ETH_RAW)
            {
                    qcom_printf("Received: %d\n",recvBytes);
                    int i;
                    for(i=0; i<recvBytes; i++){
                        qcom_printf("%02x ",*((CHAR*)pDataBuffer+i));
                        if( (i + 1)%16 == 0) //each line prints 16 numbers
                            qcom_printf("\n");
                    }
                    qcom_printf("\n");

                    char eapol_pkt[] = {
                        0x00, 0x21, 0x70, 0xaa, 0x87, 0x1a, 0x00, 0x03,  0x7F, 0x00, 0x40, 0x89, 0x88, 0x8E, 0x01, 0x00,
                        0x00, 0x05, 0x01, 0x01, 0x00, 0x05, 0x01, 0x00,  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
                        0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
                        0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 
                    };
                    A_UINT8* pkt_buffer = NULL;
                    int data_len = sizeof(eapol_pkt);
                    int send_result;
                    pkt_buffer = swat_mem_malloc(data_len);
                    if (NULL == pkt_buffer)
                    {
                        SWAT_PTF("pkt buffer malloc error for ETH RAW\r\n");
                        return;
                    }
                    memcpy(ip_str, &pDataBuffer[6], 6);
                    memcpy(pkt_buffer, eapol_pkt, data_len);
                    memcpy(pkt_buffer, ip_str, 6);
                    send_result = swat_sendto(pStreamCxt->socketLocal, 
                                  (CHAR*)(&pkt_buffer), data_len, 0, (struct sockaddr*)&fromAddr, sizeof(struct sockaddr_in));

                    if(send_result < 0)
                        SWAT_PTF("Eapol send error \n");
                    else
                        SWAT_PTF("Eapol sent: %d \n",data_len);
                    
                    swat_buffer_free(&(pkt_buffer));
           }
           else{    
                /* Decrementing the ip header size */
               if(pStreamCxt->param.ip_hdr_inc == 1)
                    recvBytes -= sizeof(ip_header);

                if (recvBytes >= sizeof(EOT_PACKET_t))
                {
                    if (isFirst)
                    {
                        if(recvBytes > sizeof(EOT_PACKET_t))
                        {
                            clientIp   = ntohl(fromAddr.sin_addr.s_addr);
                            clientPort = ntohs(fromAddr.sin_port);
                            SWAT_PTF("RAW receving from 0x%x port:%d \r\n", 
                                    clientIp, clientPort);
                            isFirst = 0;        

                            pStreamCxt->calc.firstTime.milliseconds = CALC_TIME_DEF;
                            pStreamCxt->calc.lastTime.milliseconds  = CALC_TIME_DEF;
                            pStreamCxt->calc.bytes  = CALC_BYTES_DEF;
                            pStreamCxt->calc.kbytes = CALC_KBYTES_DEF;
                            swat_get_first_time(pStreamCxt);
                            swat_bytes_calc(pStreamCxt, recvBytes);														
                        }
                    }
                    else
                    {
                        A_UINT8* bf;
                        fromAddr.sin_family = AF_INET;                    /* End Of Transfer */
                        if (recvBytes == sizeof(EOT_PACKET_t))
                        {
                            swat_get_last_time(pStreamCxt);

                            totalInterval = (pStreamCxt->calc.lastTime.milliseconds - pStreamCxt->calc.firstTime.milliseconds);
                            StatPacket.kbytes = pStreamCxt->calc.kbytes;
                            StatPacket.bytes  = pStreamCxt->calc.bytes;
                            StatPacket.msec   = totalInterval;
                            bf = (A_UINT8*)&StatPacket;
                            /* Tell ath_console TX received end mark AABBCCDD with throughput value */
                            while(sendTerminalCount <= 10)
                            {
                                 size = sizeof(STAT_PACKET_t);

                                /* Add IP header */
                                if(pStreamCxt->param.ip_hdr_inc == 1) //if raw_mode == iphdr
                                {
                                  size += sizeof(ip_header);
                                  iphdr = (ip_header *)pStreamCxt->param.pktBuff;
                                  iphdr->iph_ihl = IPV4_HEADER_LENGTH / sizeof(A_UINT32);
                                  iphdr->iph_ver = 4; /* IPv4 */
                                  iphdr->iph_tos = 0;
                                  iphdr->iph_len = htons(size);
                                  iphdr->iph_ident = htons(0);        
                                  /* Zero (1 bit) */
                                  ip_flags[0] = 0;
                                  /* Do not fragment flag (1 bit) */
                                  ip_flags[1] = 0;
                                  /* More fragments following flag * (1 bit) */
                                  ip_flags[2] = 0;
                                  /* Fragmentation * offset (13 bits)* */
                                  ip_flags[3] = 0;      
                                  iphdr->iph_offset = htons((ip_flags[0] << 15) | (ip_flags[1] << 14)| (ip_flags[2] << 13) | ip_flags[3]);
                                  /* Time-to-Live (8 bits): default to maximum value */
                                  iphdr->iph_ttl = 255;
                                  /* Transport layer protocol (8 bits): 17 for UDP ,255 for raw */
                                  iphdr->iph_protocol  = pStreamCxt->param.port; //ATH_IP_PROTO_RAW;
                                  iphdr->iph_sourceip  = NTOHL(pStreamCxt->param.local_ipAddress);
                                  iphdr->iph_destip    = NTOHL(fromAddr.sin_addr.s_addr);
                                  memcpy((A_UINT8*)iphdr + sizeof(ip_header),&StatPacket, sizeof(STAT_PACKET_t));
                                  bf = (A_UINT8*)iphdr;
                                }
                                                                
                                 /* Clean */
                                SWAT_PTF("SENDING\n");
			                    sendBytes = swat_sendto(pStreamCxt->socketLocal, 
			                                    (CHAR*)(bf/*&StatPacket*/), size, 0, 
			                                    (struct sockaddr *)&fromAddr, sizeof(struct sockaddr_in));
				                if (sendBytes < 0)
				                {
				                         SWAT_PTF("RAW send throughput info packet error %d , retry %d \r\n", sendBytes, sendTerminalCount);
				                         qcom_thread_msleep(100);
										 sendTerminalCount++;
				                }
								else
								{
                                	break;
								}
                            }	
                            break;
                        } /* EOT packet is received */
						else
						{								
							swat_bytes_calc(pStreamCxt, recvBytes); 													
						}
                    }     /* not the first packet   */
                }         /* received pkt >= EOT    */
              }           /* IP_RAW                 */
           }              /* swatfdisset            */
        }                 /* swat select !=0        */
    }                     /* end of while(1)        */

    swat_test_result_print(pStreamCxt);

    SWAT_PTF("Waiting.\n");
    /* Free Buffer */
    swat_buffer_free(&(pStreamCxt->param.pktBuff));

}
#endif

void
swat_udp_rx_handle(STREAM_CXT_t * pStreamCxt)
{
    q_fd_set sockSet;
    struct timeval tmo;
//    A_STATUS  status;

    /* Initial Bench Value */
    swat_bench_quit_init();

    /* Init fd_set */
    swat_fd_zero(&sockSet);
    swat_fd_set(pStreamCxt->socketLocal, &sockSet);
    tmo.tv_sec = 1;
    tmo.tv_usec = 0;
    pStreamCxt->pfd_set = (void *) &sockSet;

    while (1) {
        if (swat_bench_quit()) {
            goto QUIT;
        }
        swat_udp_rx_data(pStreamCxt);
    }
  QUIT:
    swat_test_result_print(pStreamCxt);
    SWAT_PTF("*************IOT Throughput Test Completed **************\n");
    SWAT_PTF("Shell> ");
    swat_fd_zero(&sockSet);
    pStreamCxt->pfd_set = NULL;
    /* Close Socket */
    swat_socket_close(pStreamCxt);
}

#ifdef SWAT_BENCH_RAW
void swat_raw_rx_handle(STREAM_CXT_t* pStreamCxt)
{
    q_fd_set sockSet;
    struct timeval tmo;
    
    /* Initial Bench Value */
    swat_bench_quit_init();

    /* Init fd_set */
    swat_fd_zero(&sockSet);
    swat_fd_set(pStreamCxt->socketLocal, &sockSet);    
    tmo.tv_sec = 2;   
    tmo.tv_usec = 0;
    pStreamCxt->pfd_set = (void *)&sockSet;
    
    while (1)
    {
            if (swat_bench_quit())   {
                goto QUIT;
            }            
        swat_raw_rx_data(pStreamCxt);
    }
QUIT:
    swat_test_result_print(pStreamCxt);
    SWAT_PTF("*************IOT Throughput Test Completed **************\n");
    SWAT_PTF("Shell> ");
    swat_fd_zero(&sockSet);
    pStreamCxt->pfd_set = NULL;
    /* Close Socket */
    swat_socket_close(pStreamCxt);
}
#endif

void
swat_bench_tcp_tx_task(STREAM_CXT_t * pStreamCxt)
{
    A_INT32 ret = 0;
    struct sockaddr_in remoteAddr;
    struct sockaddr_in6 remote6Addr;

    SWAT_PTR_NULL_CHK(pStreamCxt);

    if  (!qcom_strrcl_in_recall() || app_resume_done == 1)
    {

        if(v6_enabled) { 
            pStreamCxt->socketLocal = swat_socket(AF_INET6, SOCK_STREAM, 0);
        } else {
            pStreamCxt->socketLocal = swat_socket(AF_INET, SOCK_STREAM, 0);
        }

        if (pStreamCxt->socketLocal < 0) {
            SWAT_PTF("Open socket error...\r\n");
            goto QUIT;
        }

    /* Connect Socket */
        if(v6_enabled){
            swat_mem_set(&remote6Addr, 0, sizeof (struct sockaddr_in6));
            memcpy(&remote6Addr.sin6_addr, pStreamCxt->param.ip6Address.addr,sizeof(IP6_ADDR_T));
            remote6Addr.sin6_port = htons(pStreamCxt->param.port);
            remote6Addr.sin6_family = AF_INET6;
            ret = swat_connect(pStreamCxt->socketLocal, (struct sockaddr *) &remote6Addr,
                     sizeof (struct  sockaddr_in6));

        } else {
            swat_mem_set(&remoteAddr, 0, sizeof (struct sockaddr_in));
            remoteAddr.sin_addr.s_addr = htonl(pStreamCxt->param.ipAddress);
            remoteAddr.sin_port = htons(pStreamCxt->param.port);
            remoteAddr.sin_family = AF_INET;
            ret = swat_connect(pStreamCxt->socketLocal, (struct sockaddr *) &remoteAddr,
                     sizeof (struct  sockaddr_in));
        }

        SWAT_PTF("Connecting from socket %d.\n", pStreamCxt->socketLocal);

        if (ret < 0) {
        /* Close Socket */
            SWAT_PTF("Connect Failed\r\n");
            swat_socket_close(pStreamCxt);
            goto QUIT;
        }

        SWAT_PTF("Connect %lu.%lu.%lu.%lu OK\r\n",
             (remoteAddr.sin_addr.s_addr) & 0xFF, (remoteAddr.sin_addr.s_addr) >> 8 & 0xFF,
             (remoteAddr.sin_addr.s_addr) >> 16 & 0xF, (remoteAddr.sin_addr.s_addr) >> 24 & 0xFF);

        pStreamCxt->param.current_packets = 0;
    }
    else
    {
        qcom_printf("TCP TX recall start ...\n");
    }

    /* Packet Handle */
    swat_tcp_tx_handle(pStreamCxt);
  QUIT:
#ifdef SWAT_SSL
    if (ssl_role == SSL_CLIENT) {
        ssl_role = 0;
    }
#endif
    /* Free Index */
    swat_cxt_index_free(&tcpTxIndex[pStreamCxt->index]);
    swat_bench_tcp_timer_clean(pStreamCxt->index);
    swat_task_delete();

    app_resume_done = 1;
}

void
swat_bench_tcp_rx_task(STREAM_CXT_t * pStreamCxt)
{
    A_INT32 ret = 0;
    struct sockaddr_in remoteAddr;

    qcom_power_set_mode(0, MAX_PERF_POWER);

    SWAT_PTR_NULL_CHK(pStreamCxt);

    pStreamCxt->socketLocal = swat_socket(AF_INET, SOCK_STREAM, 0);
    if (pStreamCxt->socketLocal < 0) {
        SWAT_PTF("Open socket error...\r\n");
        goto QUIT;
    }

        /* Connect Socket */
    swat_mem_set(&remoteAddr, 0, sizeof (struct sockaddr_in));
    remoteAddr.sin_addr.s_addr = htonl(pStreamCxt->param.ipAddress);
    remoteAddr.sin_port = htons(pStreamCxt->param.port);
    remoteAddr.sin_family = AF_INET;
    ret = swat_bind(pStreamCxt->socketLocal, (struct sockaddr *) &remoteAddr,
                  sizeof (struct sockaddr_in));

    if (ret < 0) {
        /* Close Socket */
        SWAT_PTF("Bind Failed %d.\n", pStreamCxt->socketLocal);
        swat_socket_close(pStreamCxt);
        goto QUIT;
    }

   
    SWAT_PTF("****************************************************\n");
    if (pStreamCxt->param.protocol == TEST_PROT_SSL){
        SWAT_PTF(" SSL RX Test\n"); 
    }else{
        SWAT_PTF(" TCP RX Test\n");
    }
    SWAT_PTF("****************************************************\n");
    SWAT_PTF("Local port %d\n", pStreamCxt->param.port);
    SWAT_PTF("Type benchquit to terminate test\n");
    SWAT_PTF("****************************************************\n");

    SWAT_PTF("Waiting.\n");

    /* Packet Handle */
    swat_tcp_rx_handle(pStreamCxt); 
QUIT:
    /* Free Index */
    swat_cxt_index_free(&tcpRxIndex[pStreamCxt->index]);
    qcom_power_set_mode(0, REC_POWER);

    /* Thread Delete */
    swat_task_delete();
}

void
swat_bench_udp_tx_task(STREAM_CXT_t * pStreamCxt)
{
    SWAT_PTR_NULL_CHK(pStreamCxt);

    if  (!qcom_strrcl_in_recall() || app_resume_done == 1)
    {
        if(v6_enabled){
            pStreamCxt->socketLocal = swat_socket(PF_INET6, SOCK_DGRAM, 0);
        }else{
            pStreamCxt->socketLocal = swat_socket(PF_INET, SOCK_DGRAM, 0);
        }
        if (pStreamCxt->socketLocal < 0) {
            SWAT_PTF("Open socket error...\r\n");
            goto QUIT;
        }
    }

    /* Packet Handle */
    swat_udp_tx_handle(pStreamCxt);
QUIT:
    /* Free Index */
    swat_cxt_index_free(&udpTxIndex[pStreamCxt->index]);
    swat_bench_udp_timer_clean(pStreamCxt->index);
    /* Thread Delete */
    swat_task_delete();
    app_resume_done = 1;
}

void
swat_bench_udp_rx_task(STREAM_CXT_t * pStreamCxt)
{
    A_INT32 ret = 0;
    struct sockaddr_in remoteAddr;

    struct ip_mreq {
        A_UINT32 imr_multiaddr;   /* IP multicast address of group */
        A_UINT32 imr_interface;   /* local IP address of interface */
    } group;

    SWAT_PTR_NULL_CHK(pStreamCxt);
    qcom_power_set_mode(0, MAX_PERF_POWER);

    if  (!qcom_strrcl_in_recall() || app_resume_done == 1)
    {
        if(v6_enabled){
            pStreamCxt->socketLocal = swat_socket(PF_INET6, SOCK_DGRAM, 0);
        }else{
            pStreamCxt->socketLocal = swat_socket(PF_INET, SOCK_DGRAM, 0);
        }
        if (pStreamCxt->socketLocal < 0) {
            SWAT_PTF("Open socket error...\r\n");
            goto QUIT;
        }

        if((pStreamCxt->socketLocal = swat_socket(PF_INET, SOCK_DGRAM, 0)) < 0){
            SWAT_PTF("Open socket error...\r\n");
            goto QUIT;
        }

        /* Connect Socket */
        swat_mem_set(&remoteAddr, 0, sizeof (struct sockaddr_in));
        remoteAddr.sin_addr.s_addr = htonl(pStreamCxt->param.ipAddress);
        remoteAddr.sin_port = htons(pStreamCxt->param.port);
        remoteAddr.sin_family = AF_INET;

        if((ret = swat_bind(pStreamCxt->socketLocal, (struct sockaddr *) &remoteAddr,sizeof (struct sockaddr_in))) < 0){         
            /* Close Socket */
            SWAT_PTF("Failed to bind socket %d.\n", pStreamCxt->socketLocal);
            swat_socket_close(pStreamCxt);
            goto QUIT;
        }
        if(pStreamCxt->param.mcast_enabled){
            group.imr_multiaddr = HTONL(pStreamCxt->param.mcAddress);
            group.imr_interface = HTONL(pStreamCxt->param.ipAddress);

            if (group.imr_multiaddr != 0) {
                if((ret = swat_setsockopt(pStreamCxt->socketLocal, SOL_SOCKET, IP_ADD_MEMBERSHIP, (void *) &group,
                          sizeof (group))) < 0) {
                    /* Close Socket */
                    SWAT_PTF("Failed to set socket option %d.\n", pStreamCxt->socketLocal);
                    swat_socket_close(pStreamCxt);
                    goto QUIT;
                }
            }
        }
    }
    else
    {
//        qcom_printf("dup rx recall start ...\n");
//        qcom_strrcl_recall(udp_rx_app_strrcl_recall, (A_UINT8 *)&buf_strrcl, sizeof(buf_strrcl));
    }

    SWAT_PTF("****************************************************\n");
    SWAT_PTF(" UDP RX Test\n");
    SWAT_PTF("****************************************************\n");

    SWAT_PTF("Local port %d\n", pStreamCxt->param.port);
    SWAT_PTF("Type benchquit to termintate test\n");
    SWAT_PTF("****************************************************\n");

    SWAT_PTF("Waiting.\n");

    /* Packet Handle */
    swat_udp_rx_handle(pStreamCxt);
  QUIT:
    /* Free Index */
    swat_cxt_index_free(&udpRxIndex[pStreamCxt->index]);

    /* Thread Delete */
    swat_task_delete();

    app_resume_done = 1;
}
#ifdef SWAT_BENCH_RAW
void swat_bench_raw_rx_task(STREAM_CXT_t* pStreamCxt)
{
    A_INT32 ret = 0;
    struct sockaddr_in localAddr;
    struct ip_mreq
    {
       A_UINT32 imr_multiaddr; /* IP multicast address of group */
       A_UINT32 imr_interface; /* local IP address of interface */
    } group;

    SWAT_PTR_NULL_CHK(pStreamCxt);

    pStreamCxt->socketLocal = swat_socket(AF_INET, SOCK_RAW, pStreamCxt->param.port);
    if (pStreamCxt->socketLocal < 0)

    {
        SWAT_PTF("Open raw socket error...\r\n");
        goto QUIT;
    }

    /* Connect Socket */
    swat_mem_set(&localAddr, 0, sizeof(struct sockaddr_in));
    localAddr.sin_addr.s_addr = htonl(pStreamCxt->param.local_ipAddress);
    localAddr.sin_family      = AF_INET;

    SWAT_PTF("****************************************************\n");
    SWAT_PTF(" RAW RX Test\n\n");
    SWAT_PTF("Type benchquit to termintate test\n");
    SWAT_PTF("****************************************************\n");

    SWAT_PTF("Waiting....\n");

    ret = swat_bind(pStreamCxt->socketLocal, (struct sockaddr *)&localAddr, sizeof(struct sockaddr_in));
    if (ret < 0)
    {
        /* Close Socket */
        SWAT_PTF("Failed to bind socket %d.\n", pStreamCxt->socketLocal);
        swat_socket_close(pStreamCxt);
        goto QUIT;
    }

    group.imr_multiaddr = (pStreamCxt->param.mcAddress);
    group.imr_interface = (pStreamCxt->param.ipAddress);

    if (group.imr_multiaddr != 0) {
        ret = swat_setsockopt(pStreamCxt->socketLocal, SOL_SOCKET, IP_ADD_MEMBERSHIP, (void *)&group, sizeof(group));
        if (ret < 0) {
            /* Close Socket */
            SWAT_PTF("Failed to set socket option %d.\n", pStreamCxt->socketLocal);
            swat_socket_close(pStreamCxt);
            goto QUIT;
        }
    }
   #define  IP_HDRINCL        2  /* int; header is included with data */
   if(pStreamCxt->param.ip_hdr_inc == 1) // if raw mode == iphdr
   {
        ret = swat_setsockopt(pStreamCxt->socketLocal, SOL_SOCKET, IP_HDRINCL, (A_UINT8 *)(&pStreamCxt->param.ip_hdr_inc), sizeof(int));
        if (ret < 0) {
            /* Close Socket */
            SWAT_PTF("Failed to set socket option %d.\n", pStreamCxt->socketLocal);
            swat_socket_close(pStreamCxt);
            goto QUIT;
        }
   }
    SWAT_PTF("CALLING HANDLE\n");
    /* Packet Handle */
    swat_raw_rx_handle(pStreamCxt);
QUIT:    
    /* Free Index */
    swat_cxt_index_free(&rawRxIndex[pStreamCxt->index]);
    qcom_power_set_mode(0, REC_POWER);


    /* Thread Delete */
    swat_task_delete();
}
#endif

void
swat_bench_tcp_tx(A_UINT32 index)
{
    swat_bench_tcp_tx_task(&cxtTcpTxPara[index]);
}

void
swat_bench_udp_tx(A_UINT32 index)
{
    qcom_printf("udp tx index=%d\n", index);

    swat_bench_udp_tx_task(&cxtUdpTxPara[index]);
}

void
swat_bench_tcp_rx(A_UINT32 index)
{
    swat_bench_tcp_rx_task(&cxtTcpRxPara[index]);
}

void
swat_bench_udp_rx(A_UINT32 index)
{
    swat_bench_udp_rx_task(&cxtUdpRxPara[index]);
}

#ifdef SWAT_BENCH_RAW
void swat_bench_raw_tx(A_UINT32 index)
{
    swat_bench_raw_tx_task(&cxtRawTxPara[index]);
}

void swat_bench_raw_rx(A_UINT32 index)
{
    qcom_power_set_mode(0, MAX_PERF_POWER);
    swat_bench_raw_rx_task(&cxtRawRxPara[index]);
    qcom_power_set_mode(0, REC_POWER);
}
#endif

void
swat_bench_tx(A_UINT32 ipAddress, IP6_ADDR_T ip6Address, A_UINT32 port, A_UINT32 protocol, A_UINT32 pktSize,
							A_UINT32 mode, A_UINT32 seconds, A_UINT32 numOfPkts,
							A_UINT32 local_ipAddress, A_UINT32 ip_hdr_inc, A_UINT32 delay)
{

    A_UINT8 index = 0;
    A_UINT8 ret = 0;

    /* TCP */
    if (TEST_PROT_TCP == protocol || TEST_PROT_SSL == protocol) {
        /*Calc Handle */
       if (!qcom_strrcl_in_recall() || flag_new_console_task == 1)
       {
	   tcp_tx_init(ipAddress, ip6Address, port, protocol, pktSize, mode, seconds, numOfPkts,
                                                        local_ipAddress, ip_hdr_inc, delay);
       }

        ret = swat_task_create(swat_bench_tcp_tx, index, 2048, 50);
        if (0 == ret) {
            SWAT_PTF("TCP TX session %d Success\n", index);
        } else {
            swat_cxt_index_free(&tcpTxIndex[index]);
        }
        /* Thread Delete */
    }

    /* UDP */
    if (TEST_PROT_UDP == protocol) {
        if (!qcom_strrcl_in_recall() || flag_new_console_task == 1)
        {
           udp_tx_init(ipAddress, ip6Address, port, protocol, pktSize, mode, seconds, numOfPkts,
                                                        local_ipAddress, ip_hdr_inc, delay);
        }

        /* Client */
        ret = swat_task_create(swat_bench_udp_tx, index, 2048, 50);
        if (0 == ret) {
            SWAT_PTF("UDP TX session %d Success\n", index);
        } else {
            swat_cxt_index_free(&udpTxIndex[index]);
        }
    }
 /* RAW */
#ifdef SWAT_BENCH_RAW

    if (TEST_PROT_RAW == protocol)
    {
        /*Calc Handle */
        for (index = 0; index < CALC_STREAM_NUMBER; index ++)
        {
            ret = swat_cxt_index_find(&rawTxIndex[index]);

            if (CALC_STREAM_NUMBER_INVALID != ret)
            {
                swat_cxt_index_configure(&rawTxIndex[index]);
                break;
            }
        }
        if (CALC_STREAM_NUMBER_INVALID == ret)
        {
            SWAT_PTF("Warning rawTxIndex is full\n");
            return;
        }

        /* DataBase Initial */
        swat_database_initial(&cxtRawTxPara[index]);

        /* Update Database*/
        swat_database_set(&cxtRawTxPara[index], ipAddress, &ip6Address, INADDR_ANY, NULL, NULL, port, protocol, 
            pktSize, mode, seconds, numOfPkts, TEST_DIR_TX,local_ipAddress,ip_hdr_inc,1,delay); 

        /* Client */
        cxtRawTxPara[index].index = index;
        /* Client */
        SWAT_PTF("Creating task swat_bench_raw_tx \n");
        ret = swat_task_create(swat_bench_raw_tx, index, 2048, 50);
        if (0 == ret)  
        {
            SWAT_PTF("RAW TX session %d Success\n",index);        

        }
        else
        {
            swat_cxt_index_free(&rawTxIndex[index]);
        }
    }
#endif /*SWAT_BENCH_RAW*/
}

void
swat_bench_rx_test(A_UINT32 protocol, A_UINT32 port, A_UINT32 localIpAddress, A_UINT32 mcastIpAddress, IP6_ADDR_T* lIp6, IP6_ADDR_T* mIp6, A_UINT32 ip_hdr_inc, A_UINT32 rawmode)
{
    A_UINT8 index = 0;
    A_UINT8 ret = 0;

    /* TCP */
    if (TEST_PROT_TCP == protocol || TEST_PROT_SSL == protocol) {
        /*Calc Handle */
        if (!qcom_strrcl_in_recall() || flag_new_console_task == 1)
        {
              tcp_rx_init(protocol, port, localIpAddress, mcastIpAddress, lIp6, mIp6, ip_hdr_inc, rawmode);
        }

        ret = swat_task_create(swat_bench_tcp_rx, index, 2048, 50);
        if (0 == ret) {
            SWAT_PTF("TCP RX session %d Success\n", index);
        } else {
            swat_cxt_index_free(&tcpRxIndex[index]);
        }
    }
    /* UDP */
    if (TEST_PROT_UDP == protocol) {
        if (!qcom_strrcl_in_recall() || flag_new_console_task == 1)
        {
            udp_rx_init(protocol, port, localIpAddress, mcastIpAddress, lIp6, mIp6, ip_hdr_inc, rawmode);
        }

        /* Server */
        ret = swat_task_create(swat_bench_udp_rx, index, 2048+512, 50);
        if (0 == ret) {
            //SWAT_PTF("UDP RX session %d Success\n", index);
        } else {
            swat_cxt_index_free(&udpRxIndex[index]);
        }
    }
#ifdef SWAT_BENCH_RAW
    if (TEST_PROT_RAW == protocol)
    {
        /*Calc Handle */
        for (index = 0; index < CALC_STREAM_NUMBER; index ++)
        {
            ret = swat_cxt_index_find(&rawRxIndex[index]);

            if (CALC_STREAM_NUMBER_INVALID != ret)
            {
                swat_cxt_index_configure(&rawRxIndex[index]);
                break;
            }
        }

        if (CALC_STREAM_NUMBER_INVALID == ret)
        {
            //SWAT_PTF("Warning rawRxIndex is full\n");
            return;
        }

        /* DataBase Initial */
        swat_database_initial(&cxtRawRxPara[index]);

        /* Update DataBase */
        swat_database_set(&cxtRawRxPara[index], INADDR_ANY, NULL, mcastIpAddress, NULL, NULL, port, 
            protocol, 1500, 0, 0, 0, TEST_DIR_RX,localIpAddress,ip_hdr_inc,rawmode,0);

        /* Server */
        cxtRawRxPara[index].index =index;
        /* Server */
	ret = swat_task_create(swat_bench_raw_rx, index, 2048, 50);
	if (0 == ret)
	{
		SWAT_PTF("RAW RX session %d Success\n",index);
	}
    else
       {
           swat_cxt_index_free(&rawRxIndex[index]);
       }
    }

#endif 
}

void swat_socket_close_all(void)
{
    int i;

    for (i = 0; i < CALC_STREAM_NUMBER; i++)
    {
        if (TRUE == tcpTxIndex[i].isUsed)
        {
            swat_cxt_index_free(&tcpTxIndex[i]);
            swat_socket_close(&cxtTcpTxPara[i]);
            swat_buffer_free(&(cxtTcpTxPara[i].param.pktBuff));
        }

        if (TRUE == tcpRxIndex[i].isUsed)
        {
            swat_cxt_index_free(&tcpRxIndex[i]);
            swat_socket_close(&cxtTcpRxPara[i]);
            swat_buffer_free(&(cxtTcpRxPara[i].param.pktBuff));
        }

        if (TRUE == udpTxIndex[i].isUsed)
        {
            swat_cxt_index_free(&udpTxIndex[i]);
            swat_socket_close(&cxtUdpTxPara[i]);
            swat_buffer_free(&(cxtUdpTxPara[i].param.pktBuff));
        }

        if (TRUE == udpRxIndex[i].isUsed)
        {
            swat_cxt_index_free(&udpRxIndex[i]);
            swat_socket_close(&cxtUdpRxPara[i]);
            swat_buffer_free(&(cxtUdpRxPara[i].param.pktBuff));
        }

#ifdef SWAT_BENCH_RAW
        if (TRUE == rawTxIndex[i].isUsed)
        {
            swat_cxt_index_free(&rawTxIndex[i]);
            swat_socket_close(&cxtRawTxPara[i]);
            swat_buffer_free(&(cxtRawTxPara[i].param.pktBuff));
        }

        if (TRUE == rawRxIndex[i].isUsed)
        {
            swat_cxt_index_free(&rawRxIndex[i]);
            swat_socket_close(&cxtRawRxPara[i]);
            swat_buffer_free(&(cxtRawRxPara[i].param.pktBuff));
        }
#endif
    }
    

    return ;
}

A_BOOL swat_sock_close_all_status()
{
    int i;
    A_BOOL ret = TRUE;

    for (i = 0; i < CALC_STREAM_NUMBER; i++)
    {
        if (TRUE == tcpTxIndex[i].isUsed)
        {
	    ret = FALSE;
	    break;
        }

        if (TRUE == tcpRxIndex[i].isUsed)
        {
	    ret = FALSE;
	    break;
        }

        if (TRUE == udpTxIndex[i].isUsed)
        {
	    ret = FALSE;
	    break;
        }

        if (TRUE == udpRxIndex[i].isUsed)
        {
	    ret = FALSE;
	    break;
        }

#ifdef SWAT_BENCH_RAW
        if (TRUE == rawTxIndex[i].isUsed)
        {
	    ret = FALSE;
	    break;
        }

        if (TRUE == rawRxIndex[i].isUsed)
        {
	    ret = FALSE;
	    break;
        }
#endif
    }
    return ret;
    
}
#ifdef SWAT_BENCH_RAW
void swat_raw_tx_handle(STREAM_CXT_t* pStreamCxt)
{
    A_INT32  sendBytes = 0;
    A_UINT32 sumBytes = 0;
    A_UINT32 currentPackets = 0;
    A_UINT8* pDataBuffer = NULL;
    struct sockaddr_in remoteAddr;
    A_UINT32    temp_addr;
    ip_header *iphdr;
    unsigned short int ip_flags[4];
    EOT_PACKET_t eotPacket;
    A_INT32 ret;

    A_INT32 sendTerminalCount = 0;
    A_UINT32 packet_size = pStreamCxt->param.pktSize;

    SWAT_PTR_NULL_CHK(pStreamCxt);

    /* Initial Bench Value */
    swat_bench_quit_init();

    /* Initial Calc & Time */
    pStreamCxt->calc.firstTime.milliseconds = CALC_TIME_DEF;
    pStreamCxt->calc.lastTime.milliseconds  = CALC_TIME_DEF;
    pStreamCxt->calc.bytes  = CALC_BYTES_DEF;
    pStreamCxt->calc.kbytes = CALC_KBYTES_DEF;
    
    /* Malloc Packet Buffer Size */
    pDataBuffer = swat_mem_malloc(pStreamCxt->param.pktSize);
    if (NULL == pDataBuffer)
    {
        SWAT_PTF("malloc error\r\n");
        /* Close Socket */
        swat_socket_close(pStreamCxt);
        return;
    }
    pStreamCxt->param.pktBuff = pDataBuffer;

    temp_addr = pStreamCxt->param.ipAddress; 
    /* Add IP header */
    if(pStreamCxt->param.ip_hdr_inc == 1) 
    {
        pStreamCxt->param.pktSize += 20;
        packet_size += 20;
        if(pStreamCxt->param.pktSize > CFG_PACKET_SIZE_MAX_TX)
        {
                packet_size = CFG_PACKET_SIZE_MAX_TX;
        }
        temp_addr = pStreamCxt->param.local_ipAddress;

    }

    /* Prepare IP address & port */
    memset(&remoteAddr, 0, sizeof( struct sockaddr_in));
    remoteAddr.sin_addr.s_addr = (pStreamCxt->param.ipAddress);
    remoteAddr.sin_port        = (pStreamCxt->param.port);    
    remoteAddr.sin_family      =  AF_INET;

    /* Initial Packet */
    SWAT_PTF("RAW Remote IP %d.%d.%d.%d prepare OK\r\n",
            (pStreamCxt->param.ipAddress) >> 24 & 0xFF, (pStreamCxt->param.ipAddress) >> 16 & 0xFF, 
            (pStreamCxt->param.ipAddress) >> 8 & 0xFF, (pStreamCxt->param.ipAddress) & 0xFF);
   SWAT_PTF("Sending...\r\n");
    
    /* Get First Time */   
    swat_get_first_time(pStreamCxt);
    if (TEST_MODE_TIME == pStreamCxt->param.mode)
    {
        swat_bench_timer_init(pStreamCxt->param.seconds, pStreamCxt->param.protocol, pStreamCxt->index);
    }
    while(1)
    {
        if (swat_bench_quit())
        {
            //SWAT_PTF("Bench quit!!\r\n");  
            swat_get_last_time(pStreamCxt);
            break;
        }
        
        /* Add IP header */
            if(pStreamCxt->param.ip_hdr_inc == 1) //if raw_mode == iphdr
            {
                  iphdr = (ip_header *)pStreamCxt->param.pktBuff;
                  iphdr->iph_ihl = IPV4_HEADER_LENGTH / sizeof(A_UINT32);
                  iphdr->iph_ver = 4; /* IPv4 */
                  iphdr->iph_tos = 0;
                  iphdr->iph_len = htons(packet_size);
                  iphdr->iph_ident = htons(0);        
                  /* Zero (1 bit) */
                  ip_flags[0] = 0;
                  /* Do not fragment flag (1 bit) */
                  ip_flags[1] = 0;
                  /* More fragments following flag * (1 bit) */
                  ip_flags[2] = 0;
                  /* Fragmentation * offset (13 bits)* */
                  ip_flags[3] = 0;      
                  iphdr->iph_offset = htons((ip_flags[0] << 15) | (ip_flags[1] << 14)| (ip_flags[2] << 13) | ip_flags[3]);
                  /* Time-to-Live (8 bits): default to maximum value */
                  iphdr->iph_ttl = 255;
                  /* Transport layer protocol (8 bits): 17 for UDP ,255 for raw */
                  iphdr->iph_protocol  = pStreamCxt->param.port;//ATH_IP_PROTO_RAW;
                  iphdr->iph_sourceip  = NTOHL(pStreamCxt->param.local_ipAddress);
                  iphdr->iph_destip    = NTOHL(pStreamCxt->param.ipAddress);
                      
              }/*end of ip_hdr_inc == 1 */

                sendBytes = swat_sendto(pStreamCxt->socketLocal, (CHAR*)pDataBuffer, pStreamCxt->param.pktSize,
                                 0, (struct sockaddr *)&remoteAddr, sizeof(struct sockaddr_in));

                if (sendBytes < 0)
                {
                    SWAT_PTF("RAW TX Error %d sumBytes = %d\r\n", sendBytes, sumBytes);
                    /* Free Buffer */
                    qcom_thread_msleep(100);
                }
                else
                {
                    /* bytes & kbytes */
                    sumBytes += sendBytes;
                }

								if (sendBytes > 0){
									swat_bytes_calc(pStreamCxt,sendBytes);
								}

                /* Packets Mode */
                if (TEST_MODE_PACKETS == pStreamCxt->param.mode)
                {      currentPackets++; 
                    if (0 != (sumBytes / pStreamCxt->param.pktSize))                
                    {
                        //currentPackets += (sumBytes / pStreamCxt->param.pktSize);
                        sumBytes = sumBytes % pStreamCxt->param.pktSize;
                    }

                    if (currentPackets >= pStreamCxt->param.numOfPkts)
                    {           
                        break;
                    }
                }
                
                /* Time Mode */
                if (TEST_MODE_TIME == pStreamCxt->param.mode)
                {   
                    /* Get Last Time */   
                    if (0 != benchRawTxTimerStop[pStreamCxt->index])
                    {
                        swat_get_last_time(pStreamCxt);
                        break;
                    }
                }
                      
    } /* end of while(1) */
        
    pStreamCxt->param.ip_hdr_inc = 0;
    #define  IP_HDRINCL        2  /* int; header is included with data */
    ret = swat_setsockopt(pStreamCxt->socketLocal, SOL_SOCKET, IP_HDRINCL, (A_UINT8 *)(&pStreamCxt->param.ip_hdr_inc), sizeof(int));
    if (ret < 0) {
        /* Close Socket */
        SWAT_PTF("SETSOCKOPT err %d.\n", pStreamCxt->socketLocal);
        swat_socket_close(pStreamCxt);
    }
    
    /* Tell ath_console TX is complete end mark is AABBCCDD */
    while(sendTerminalCount <= 10)
    {
        eotPacket.code = 0xAABBCCDD;
        eotPacket.packetCount = currentPackets;
        /* Wait for Output */
        sendBytes = swat_sendto(pStreamCxt->socketLocal, (CHAR*)&eotPacket, 
                  sizeof(EOT_PACKET_t), 0, (struct sockaddr *)&remoteAddr, 
                  sizeof(struct sockaddr_in));
        if (sendBytes < 0)
        {
             SWAT_PTF("RAW TX error %d , retry %d \r\n", sendBytes,sendTerminalCount);
             qcom_thread_msleep(100);
             sendTerminalCount++;
        }
        else
        {
            break;
        }
    }
		swat_test_result_print(pStreamCxt);
    SWAT_PTF("*************IOT Throughput Test Completed **************\n");

    /* Free Buffer */
    swat_buffer_free(&(pStreamCxt->param.pktBuff));
    /*Close socket*/
    swat_socket_close(pStreamCxt);
}
#endif

#ifdef SWAT_BENCH_RAW
void swat_bench_raw_tx_task(STREAM_CXT_t* pStreamCxt)
{

    A_INT32 ret = 0;
    SWAT_PTR_NULL_CHK(pStreamCxt);

    pStreamCxt->socketLocal = swat_socket(AF_INET, SOCK_RAW, pStreamCxt->param.port);
    if (pStreamCxt->socketLocal < 0)
    {
        SWAT_PTF("Open socket error...\r\n");
        goto QUIT;
    }

    #define  IP_HDRINCL        2  /* int; header is included with data */
    /* Set IP_HDRINCL socket option if we need to receive the packet with IP header */ 
   if(pStreamCxt->param.ip_hdr_inc == 1) // if raw mode == iphdr
   {
        ret = swat_setsockopt(pStreamCxt->socketLocal, SOL_SOCKET, IP_HDRINCL, (A_UINT8 *)(&pStreamCxt->param.ip_hdr_inc), sizeof(int));
        if (ret < 0) {
            /* Close Socket */
            SWAT_PTF("Failed to set socket option %d.\n", pStreamCxt->socketLocal);
            swat_socket_close(pStreamCxt);
            goto QUIT;
        }
   }
    /* Packet Handle */
    swat_raw_tx_handle(pStreamCxt);
QUIT:
    /* Free Index */
    swat_cxt_index_free(&rawTxIndex[pStreamCxt->index]);
   // swat_bench_raw_timer_clean(pStreamCxt->index);
    /* Thread Delete */
    swat_task_delete();
}
#endif /* swat_bench_raw_tx_task */

#endif    


