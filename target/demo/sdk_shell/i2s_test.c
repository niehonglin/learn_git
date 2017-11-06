/*
 * Copyright (c) 2011 Qualcomm Atheros, Inc.
 * All Rights Reserved.
 * Qualcomm Atheros Confidential and Proprietary.
 */


#include <fwconfig_AR6006.h>
#include <qcom/qcom_common.h>
#include <qcom/socket_api.h>
#include <qcom/select_api.h>
#include <qcom/qcom_i2s.h>
#include <qcom/qcom_i2c.h>
#include <qcom/qcom_mem.h>
#include "threadx/tx_api.h"
#include "threadx/tx_thread.h"

#include "swat_parse.h"

#if 1 //def SWAT_I2S_TEST
short a2ltable[256] = 
{
      -5504, -5248, -6016, -5760, -4480, -4224, -4992, -4736,
      -7552, -7296, -8064, -7808, -6528, -6272, -7040, -6784,
      -2752, -2624, -3008, -2880, -2240, -2112, -2496, -2368,
      -3776, -3648, -4032, -3904, -3264, -3136, -3520, -3392,
     -22016,-20992,-24064,-23040,-17920,-16896,-19968,-18944,
     -30208,-29184,-32256,-31232,-26112,-25088,-28160,-27136,
     -11008,-10496,-12032,-11520, -8960, -8448, -9984, -9472,
     -15104,-14592,-16128,-15616,-13056,-12544,-14080,-13568,
       -344,  -328,  -376,  -360,  -280,  -264,  -312,  -296,
       -472,  -456,  -504,  -488,  -408,  -392,  -440,  -424,
                -88,   -72,  -120,  -104,   -24,    -8,   -56,   -40,
       -216,  -200,  -248,  -232,  -152,  -136,  -184,  -168,
      -1376, -1312, -1504, -1440, -1120, -1056, -1248, -1184,
      -1888, -1824, -2016, -1952, -1632, -1568, -1760, -1696,
       -688,  -656,  -752,  -720,  -560,  -528,  -624,  -592,
       -944,  -912, -1008,  -976,  -816,  -784,  -880,  -848,
       5504,  5248,  6016,  5760,  4480,  4224,  4992,  4736,
       7552,  7296,  8064,  7808,  6528,  6272,  7040,  6784,
       2752,  2624,  3008,  2880,  2240,  2112,  2496,  2368,
       3776,  3648,  4032,  3904,  3264,  3136,  3520,  3392,
      22016, 20992, 24064, 23040, 17920, 16896, 19968, 18944,
      30208, 29184, 32256, 31232, 26112, 25088, 28160, 27136,
      11008, 10496, 12032, 11520,  8960,  8448,  9984,  9472,
      15104, 14592, 16128, 15616, 13056, 12544, 14080, 13568,
                344,   328,   376,   360,   280,   264,   312,   296,
                472,   456,   504,   488,   408,   392,   440,   424,
                88,    72,   120,   104,    24,     8,    56,    40,
                216,   200,   248,   232,   152,   136,   184,   168,
       1376,  1312,  1504,  1440,  1120,  1056,  1248,  1184,
       1888,  1824,  2016,  1952,  1632,  1568,  1760,  1696,
                688,   656,   752,   720,   560,   528,   624,   592,
                944,   912,  1008,   976,   816,   784,   880,   848
};

struct ringb {
    int rd;             /* read access offset in buf */
    int wr;             /* write access offset in buf */
    int size;           /* size of buffer used */
    char *buf;          /* buffer used */
    int disable;        /* nonzero to throway input */
};

extern A_INT32 uda1380_read(A_UINT8 addr, A_UINT16 * val);
extern A_INT32 uda1380_write(A_UINT8 addr, A_UINT16 val);

//extern void qcom_thread_msleep(unsigned long ms);
extern int qcom_task_start(void (*fn) (unsigned int), unsigned int arg, int stk_size, int tk_ms);
extern void qcom_task_exit();
extern void i2s_rx_intr_cb(void *ctx ,A_UINT8 * bufp, A_UINT32 size);

#define I2S_BUF_SIZE 1024
#define I2S_ERR 0
#define I2S_OK 1

#define UDA1380_CNT (9)
#define TCP_RX_BUF_LEN  I2S_BUF_SIZE
#define TCP_TX_BUF_LEN  (I2S_BUF_SIZE + 16)

#define IPv4_STR_PRT(addr) qcom_printf("%d.%d.%d.%d", (addr)>>24&0xff, (addr)>>16&0xff, (addr)>>8&0xff, (addr)&0xff)

static int gl4_port_for_i2s;
static unsigned int gip_for_i2s;

int i2s_ctl_flag;
#define I2S_INIT 			0x8
#define I2S_SEND_FINI 	0x4
#define I2S_SEND_STOP 	0x2
#define I2S_REC_STOP 	0x1

typedef enum {
    UDA1380_SPEAKER = 1,
    UDA1380_LINEIN	 =2
} CODEC_MODE;

void tcp_recv_from_i2s(unsigned char * bufp, unsigned int size);


#define RING_BUFFER_SIZE 20480
#define SEND_BUF_SIZE 512

/*-F- ringbInit -- Set up ring data structure
*/
void ringbInit(
        struct ringb *ring, 
        int size, 
        void *buf)
{
    ring->rd = 0;
    ring->wr = 0;
    ring->size = size;
    ring->buf = buf;
    ring->disable = 0;
    return;
}

/*-F- ringbHave -- Returns how much data is in ring 
*/
int ringbHave(struct ringb *ring, int *p_have1, int *p_have2)
{
    int have1 = 0;
    int have2 = 0;
    if (ring->disable)
        return 0;
    if (ring->wr >= ring->rd) {
        have1 = ring->wr - ring->rd;
    } else {
        have1 = ring->size - ring->rd;
        have2 = ring->wr;
    }
    if (p_have1) *p_have1 = have1;
    if (p_have2) *p_have2 = have2;
    return have1 + have2;
}

/*-F- ringbRoom --  Returns how much free space in ring (free to write to)
*       Note that one byte must be left unused (and thus is never free).
*/
int ringbRoom(struct ringb *ring, int *p_room1, int *p_room2)
{
    int room1 = 0;
    int room2 = 0;
    if (ring->disable)
        return 0;
    if (ring->rd > ring->wr) {
        room1 = ring->rd - ring->wr - 1;
    } else {
        room1 = ring->size - ring->wr;
        if (ring->rd > 0) {
            room2 = ring->rd-1;
        } else {
            room1--;
        }
    }
    if (p_room1) *p_room1 = room1;
    if (p_room2) *p_room2 = room2;
    return room1 + room2;
}

/*-F- ringbEat -- Throw away next <n> bytes in ring buffer
*       nbytes is reduced if bigger than contents of ring.
*/
void ringbEat(struct ringb *ring, int nbytes)
{
    int have1=0, have2=0;
    int nmax = ringbHave(ring, &have1, &have2);
    if (nbytes > nmax)
        nbytes = nmax;
    if (nbytes <= 0) 
        return;
    if (nbytes >= have1) {
        /* wrap around the ring */
        ring->rd = nbytes - have1;
    } else {
        ring->rd += nbytes;
    }

    return;
}

/*-F- ringbWrite -- Copy to ring, returning appropriate count or error.
*/
int ringbWrite(struct ringb *ring,
        const void *from, size_t count)
{
    const char *buf = from;
    int room1;
    int room2;
    int max = ringbRoom(ring, &room1, &room2);

    if (count == 0)
        return 0;
    if (ring->disable)
        return 0;
    if (count > max)
        count = max;
    if (count > room1)
        room2 = count - room1;
    else
        room2 = 0, room1 = count;   

    /*Eric work aroud for PCM verify on O5*/
    if(NULL == ring->buf)
    {
	    printf("NULL == ring->buf\n");
        return 0;
    }
    memcpy(ring->buf+ring->wr, buf, room1);

    buf += room1;
    if (ring->wr+room1 >= ring->size)
        ring->wr = 0;
    else
        ring->wr += room1;     /* note, room2 will be zero */
    if (room2) {
        memcpy(ring->buf, buf, room2);
        ring->wr = room2;
    }

    return count;
}

/*-F- ringbWrite_v2 -- Copy to ring, returning appropriate count or error.
*/
int ringbWrite_v2(struct ringb *ring,
        const void *from, size_t count)
{
    const char *buf = from;
    int room1;
    int room2;
    int max = ringbRoom(ring, &room1, &room2);


    if (count == 0)
        return 0;
    if (ring->disable)
        return 0;
    if (count > max)
        return 0;
    if (count > room1)
        room2 = count - room1;
    else
        room2 = 0, room1 = count;   

    /*Eric work aroud for PCM verify on O5*/
    if(NULL == ring->buf)
    {
	    printf("NULL == ring->buf\n");
        return 0;
    }
    memcpy(ring->buf+ring->wr, buf, room1);

    buf += room1;
    if (ring->wr+room1 >= ring->size)
        ring->wr = 0;
    else
        ring->wr += room1;     /* note, room2 will be zero */
    if (room2) {
        memcpy(ring->buf, buf, room2);
        ring->wr = room2;
    }

    return count;
}

int ringbRead_v2(struct ringb *ring,
        void *to, size_t count)
{
    char *buf = to;
    int have1=0;
    int have2=0;
    int max = ringbHave(ring, &have1, &have2);

    if (count == 0)
        return 0;
    if (count > max)
        return 0;
    else
    if (count > have1)
        have2 = count - have1;
    else
        have2 = 0, have1 = count;

    memcpy(buf, ring->buf+ring->rd, have1);

    buf += have1;
    if (ring->rd+have1 >= ring->size)
        ring->rd = 0;
    else
        ring->rd += have1;      /* note, have2 will be zero */
    if (have2 > 0) {
        memcpy(buf, ring->buf, have2);
        ring->rd = have2;
    }

    return count;
}

extern void _i2s_interface_enable(A_UINT32 portnum);
extern char qcomI2sInitFlag;
int uda1380_config(CODEC_MODE mode)
{
	static unsigned char uda1380_reg[UDA1380_CNT] =
	{0x7F,0x01,0x13,0x14,0x00,0x02,0x22,0x23,0x21};

	static unsigned short int uda1380_cfg1[UDA1380_CNT] =
	{0,0,0,0,0x0f02,0xa5df,0x040e,0x1,0x0303 };

	static unsigned short int uda1380_cfg2[UDA1380_CNT] =
	{0,0,0,0,0x0f02,0xa5df,0x0401,0x1,0x0303};

	if(!(i2s_ctl_flag&I2S_INIT)){
		if(!qcomI2sInitFlag){

			  	if(mode == UDA1380_SPEAKER)
				{
					if(I2S_ERR==(qcom_i2s_init(1, I2S_FREQ_32K,16,8,1,2048,NULL)))
					{
						QCOM_DEBUG_PRINTF("%s: qcom_i2s_init error!\n", __func__);
						return I2S_ERR;
					}
				}
				else 
				{   
					if(I2S_ERR==(qcom_i2s_init(1, I2S_FREQ_32K,16,1,8,1024,(void *)i2s_rx_intr_cb)))
					{
						QCOM_DEBUG_PRINTF("%s: qcom_i2s_init error!\n", __func__);
						return I2S_ERR;
					}
				}
		}
		else{
				//in R3.2 I2S design, it does not support dynamic change of I2S bus configuration, when init is done, no config can be updated.
				printf("i2s is already initialized, don't init again!\n");
		}

		qcom_i2c_init(I2C_FREQ_400K);

		i2s_ctl_flag|=I2S_INIT;
	}
   printf("re-enable i2s.\n");
  _i2s_interface_enable(1);
	
	int i;
	if(UDA1380_SPEAKER == mode){
		for(i=0;i<(UDA1380_CNT);i++){
			if(I2S_ERR == uda1380_write(uda1380_reg[i], uda1380_cfg1[i]))
				return I2S_ERR;
		}
	}
	else if(UDA1380_LINEIN == mode){
		for(i=0;i<(UDA1380_CNT);i++){
			if(I2S_ERR == uda1380_write(uda1380_reg[i], uda1380_cfg2[i]))
				return I2S_ERR;
		}
	}

#if 0
	printf("==============================\n");
	unsigned short val;
		for(i=0;i<(UDA1380_CNT);i++){
			if(I2S_ERR == uda1380_read(uda1380_reg[i], &val))
				return I2S_ERR;
			if(uda1380_cfg1[i] != val)
				printf("%s: i2c check error, index %d, original 0x%08x, fail 0x%08x\n", __func__, i, uda1380_reg[i], val);
		}
#endif

	qcom_thread_msleep(100);
	return I2S_OK;
}

void audio_socket_addr_init(struct sockaddr_in *psrv_add)
{
    	memset(psrv_add, 0, sizeof(struct sockaddr_in));
    	psrv_add->sin_addr.s_addr = htonl(gip_for_i2s);
    	psrv_add->sin_port = htons(gl4_port_for_i2s);
    	psrv_add->sin_family = AF_INET;
	return;
}

void audio_play(unsigned int no_used)
{
    char file_name[32];
    int ret = -1, nRecv, nSend,file_size, len, nTime=0;	
    q_fd_set sockSet, masterSet;
    int fd_act=0, err_times, nBytes=0, nCount;
    struct timeval tmo;
    int socket_serv = -1;
    int socket_clnt = -1;
    struct sockaddr_in sock_add;
    struct sockaddr_in clnt_addr;
    unsigned int reglist[5];
    int i;
    int audio_play_start = 0;
    int recv_count = 0;
    char *ringbuffer;
    short *sendBuf;
    char *recvBuf;
    struct ringb rb;
    char ack[3]= "OK";

    /*init ringb*/
    if ((ringbuffer = (char *)qcom_mem_alloc(RING_BUFFER_SIZE)) == NULL)
    {
        printf("!!!!!!!!!!Malloc fault to apply %d Bytes ringbuffer Memeory\n", RING_BUFFER_SIZE);
        qcom_task_exit();
        return;
    }

    if ((sendBuf = (short *)qcom_mem_alloc(SEND_BUF_SIZE*2*2)) == NULL)
    {
        printf("!!!!!!!!!!Malloc fault to apply %d Bytes sendBuf Memeory\n", SEND_BUF_SIZE*2*2);
        qcom_task_exit();
        return;
    }  

    if ((recvBuf = (char *)qcom_mem_alloc(TCP_RX_BUF_LEN)) == NULL)
    {
        printf("!!!!!!!!!!Malloc fault to apply %d Bytes recvBuf Memeory\n", TCP_RX_BUF_LEN);
        qcom_task_exit();
        return;
    }

    memset(ringbuffer, 0, sizeof(ringbuffer));
    ringbInit(&rb, RING_BUFFER_SIZE, ringbuffer);

    //save gpio1-5
    for(i=0; i<5; i++) {
        reglist[i] = *(unsigned int *)(0x1402c+i*4);
    }

    if(I2S_ERR == uda1380_config(UDA1380_SPEAKER)){
        printf("%s: config error!\n", __func__);
        qcom_task_exit();
        return ;
    }

    memset(recvBuf, 0, sizeof(recvBuf));
    /* create TCP socket */
    socket_serv = qcom_socket(AF_INET, SOCK_STREAM, 0);
    if(socket_serv <=0)
    {
        //printf("%s: socket error!\n", __func__);
        goto done;
    }
    /* bind to a port */
    audio_socket_addr_init(&sock_add);
    ret = qcom_bind(socket_serv, (struct sockaddr *)&sock_add, sizeof(struct sockaddr_in));
    //if(ret<0)
    //printf("%s: bind error!\n", __func__);


    /* wait for connection */
    FD_ZERO(&masterSet);
    FD_SET(socket_serv, &masterSet);
    tmo.tv_sec = 120;
    tmo.tv_usec = 0;


    /* listen on the port */
    ret = qcom_listen(socket_serv, 10);
    if(ret<0)
    {
        ;
        //printf("%s: listen error!\n", __func__);
    }

    qcom_printf("Tcp server is listenning on port %d...\n", gl4_port_for_i2s);

    sockSet = masterSet;
    fd_act = qcom_select(2, &sockSet, NULL, NULL, &tmo);
    if(fd_act <= 0)
    {
        QCOM_DEBUG_PRINTF("No connection in %ld seconds\n", tmo.tv_sec);
        goto done;
    }

    /* accept connection from client */
    socket_clnt = qcom_accept(socket_serv, (struct sockaddr *)&clnt_addr, &len);
    if(socket_clnt<=0)
        goto done;

    qcom_printf("Accept connection from ");
    IPv4_STR_PRT((int)(ntohl(clnt_addr.sin_addr.s_addr)));
    qcom_printf(": %d\n", ntohs(clnt_addr.sin_port));

    /* recv first packet */
    qcom_thread_msleep(500);
    nRecv = qcom_recv(socket_clnt, recvBuf, TCP_RX_BUF_LEN, 0);
    qcom_printf("nRecv = %d\n", nRecv);
    file_size = ntohl(*(int*)recvBuf);
    memcpy(file_name, recvBuf+4, nRecv-4);
    file_name[nRecv-4] = 0;
    qcom_printf("Sending file %s to i2s, size = %d\n", file_name, file_size);
    qcom_thread_msleep(500);

    /* send ACK */
    nSend = qcom_send(socket_clnt, ack, 3, 0);
    qcom_thread_msleep(800);

    /* start the loop */
    nTime = time_ms();
    while(1)
    {  
        /* init fd_set */
        FD_ZERO(&sockSet);
        FD_SET(socket_clnt, &sockSet);
        tmo.tv_sec = 10; /*1; //10;*/
        tmo.tv_usec = 4000; // 4msec

        fd_act = qcom_select(socket_clnt+1, &sockSet, NULL, NULL, &tmo);
        if(fd_act <0)
        {
            printf("No packet received in %ld seconds\n", tmo.tv_sec);
            err_times++;
        }
        /* Timeout*/
        else if (fd_act == 0)
        {
        }	
        else
        {
             if(FD_ISSET(socket_clnt, &sockSet))
             {
                  nRecv = qcom_recv(socket_clnt, recvBuf, TCP_RX_BUF_LEN, 0);
                  if(nRecv > 0)
                  {
                       nSend = qcom_send(socket_clnt, ack, 3, 0);

                       /*write audio data to ringb*/
                       do
                       {
                            ret = ringbWrite_v2(&rb, recvBuf, nRecv);
                            if(ret==0)
                            {
                                 //printf("F-Eat\n");		            			  
                                 ringbEat(&rb, RING_BUFFER_SIZE/2);
                            }
                       }while(audio_play_start == 1 && ret == 0);
                       recv_count++;	
                      
                       nCount++;
                       if((nCount%200) == 0)
                       qcom_printf("* ");
                       if((nCount%4000) == 0)
                       qcom_printf("\n");
                  }	
                  else
                  {
                      qcom_printf("\nRecv %s done\n", file_name);
                      break;
                  }
            }
            else
            {
                printf("%s: rcv NULL ?\n", __func__);
                goto done;
            }
        }
       
        if(audio_play_start == 0 && recv_count == 8)
        {
             int i = 7;
             while(i--)
             {
                 int count = ringbRead_v2(&rb, recvBuf, SEND_BUF_SIZE);
                 if(count != SEND_BUF_SIZE)
                 {
                      printf("Line=%d, count=%d\n", __LINE__, count);
                 }
                 ret = qcom_i2s_xmt_data(1, (A_UINT8 *)recvBuf,SEND_BUF_SIZE*2);
                 if(ret)
                 {
                      printf("i=%d, ret=%d\n", i, ret);
                 }	
             }
             audio_play_start = 1;
         }            
  
         if(audio_play_start)
         {
             int   count = ringbRead_v2(&rb, recvBuf, SEND_BUF_SIZE);
             ret = 1;
             if(!count)
             {
                 #if 1
                 int have1, have2;
                 /*write audio data to ringb*/
                 int cnt = ringbRoom(&rb, &have1, &have2);
                 ret = 0;
                 if(cnt > (RING_BUFFER_SIZE - SEND_BUF_SIZE))
                 { 
                       memset(recvBuf, 0, sizeof(recvBuf));
                       while(cnt > 0)
                       {
                           cnt -= ringbWrite(&rb, recvBuf, sizeof(recvBuf));
                           //printf("Write 1/2\n");
                       }
                 }
                 #endif
             }
             else
             {
                 do{
                      for(i =0; i< count; i++)
                      {
                           int tmp =  recvBuf[i];
                           sendBuf[2*i]   = a2ltable[tmp];
                           sendBuf[2*i+1] = a2ltable[tmp];                   
                      }
                      ret = qcom_i2s_xmt_data(1, (A_UINT8 *)sendBuf,count*4);

                      if(ret)
                      {
                          qcom_thread_msleep(10);
                      }

                  }while(ret);
              }
         }     
    }

    nTime = time_ms()-nTime;
    qcom_printf("\ntcp_2_i2s test done. Write %d bytes to i2s in %d seconds\n", nBytes, nTime/1000);

done:

    /* c1ose TCP sockets */
    if (socket_clnt > 0)
        qcom_socket_close(socket_clnt);
    
    if (socket_serv > 0)
        qcom_socket_close(socket_serv);

    if(recvBuf != NULL)
    {
        qcom_mem_free(recvBuf);
        recvBuf = NULL;
    }

    if(sendBuf != NULL)
    {
        qcom_mem_free(sendBuf);
        sendBuf = NULL;
    }

    if(ringbuffer != NULL)
    {
        qcom_mem_free(ringbuffer);
        ringbuffer = NULL;
    }
    
    //restore gpio1-5
    for(i=0; i<5; i++) {
     *(unsigned int *)(0x1402c+i*4) = reglist[i];
    }

    qcom_task_exit();
    return;
}

void cli_audio_paly(void)
{

	gip_for_i2s = INADDR_ANY;
	gl4_port_for_i2s = 6000;
	qcom_task_start(audio_play, 0, 2048, 50);
    return;
}



/**********************************************************
 **********************************************************
 ********************   recorder  process   ********************
 **********************************************************
 **********************************************************/
char *record_ringbuffer;
char *record_recvbuffer;
char *record_sendbuffer;
struct ringb record_rb;
unsigned int record_count = 0;

void i2s_rx_intr_cb(void *ctx ,A_UINT8 * bufp, A_UINT32 size)
{
	unsigned int data_size=0;
	int ret=-1;
	short *p1, *p2;
	int i;

	ret = qcom_i2s_rcv_data(1, (A_UINT8 *)record_recvbuffer, 1024, &data_size);
	
	/*2 channels to 1 channel*/
	p1 = (short *)record_recvbuffer;
	p2 = p1;
	for(i = 0; i < data_size/4; i++)
	{
		*p2 = *p1;
		p1 += 2;
		p2++;
	}
	
	/*put data to ringb*/
	if(data_size > 0)
	{
        ret = ringbWrite(&record_rb, record_recvbuffer, data_size/2);
        if(ret==0)
        {		            			  
             ringbEat(&record_rb, RING_BUFFER_SIZE/2);
        }
	}

	record_count++;
}

unsigned int reglist_rec[5];
void audio_recorder_main(unsigned int no_used)
{
	int ret = -1;
	int socket_serv = -1;
	int socket_conn = -1;
	int i2s2tcp_buf_len = 0;
	struct sockaddr_in sock_add;
	struct sockaddr_in clnt_addr;	
	char szbuf[32];
	q_fd_set sockSet, masterSet;
    int fd_act=0;
    struct timeval tmo;
    int get_count;
    q_fd_set fd_set_i2s2tcp;
	int fd_act_i2s2tcp=0;
	struct timeval timeout;
	timeout.tv_sec = 10; 
	timeout.tv_usec = 10000;
	int i;

    /*init ringb*/
    if ((record_ringbuffer = (char *)qcom_mem_alloc(RING_BUFFER_SIZE)) == NULL)
    {
        printf("!!!!!!!!!!Malloc fault to apply %d Bytes ringbuffer Memeory\n", RING_BUFFER_SIZE);
        qcom_task_exit();
        return;
    }
    memset(record_ringbuffer, 0, RING_BUFFER_SIZE);
    ringbInit(&record_rb, RING_BUFFER_SIZE, record_ringbuffer);

    if ((record_recvbuffer = (char *)qcom_mem_alloc(1024)) == NULL)
    {
        printf("!!!!!!!!!!Malloc fault to apply %d Bytes recvbuffer Memeory\n", 1024);
        qcom_task_exit();
        return;
    }

    if ((record_sendbuffer = (char *)qcom_mem_alloc(1024)) == NULL)
    {
        printf("!!!!!!!!!!Malloc fault to apply %d Bytes sendbuffer Memeory\n", 1024);
        qcom_task_exit();
        return;
    }

	i2s_ctl_flag &= I2S_INIT;
	
	socket_serv = qcom_socket(AF_INET, SOCK_STREAM, 0);
	if (socket_serv < 0)
	{
		goto done;
	}

	audio_socket_addr_init(&sock_add);
	ret = qcom_bind(socket_serv, (struct sockaddr *)&sock_add, sizeof(struct sockaddr_in));
	if (ret < 0)
	{
		QCOM_DEBUG_PRINTF("Failed to bind socket %d.\n", socket_serv);
		goto done;
	}

	ret = qcom_listen(socket_serv, 10);
	if (ret < 0)
	{
		;
	}

	qcom_printf("Tcp server is listenning on port %d...\n", gl4_port_for_i2s);

	/* wait for connection */
	FD_ZERO(&masterSet);
	FD_SET(socket_serv, &masterSet);
	tmo.tv_sec = 120;
   	tmo.tv_usec = 0;

	sockSet = masterSet;
	fd_act = qcom_select(2, &sockSet, NULL, NULL, &tmo);
	if(fd_act <= 0)
	{
    	QCOM_DEBUG_PRINTF("No connection in %ld seconds\n", tmo.tv_sec);
    	goto done;
	}

	socket_conn = qcom_accept(socket_serv, (struct sockaddr *)&clnt_addr, &i2s2tcp_buf_len);
	if (socket_conn < 0)
	{
		goto done;
	}

	qcom_printf("Accept connection from ");
	IPv4_STR_PRT((int)(ntohl(clnt_addr.sin_addr.s_addr)));
	qcom_printf(": %d\n", ntohs(clnt_addr.sin_port));
	qcom_printf("please start speaking, 1 minute later, recorder will be stopped!\n");

	qcom_thread_msleep(1000);
	qcom_i2s_rcv_control(1,0);
	qcom_thread_msleep(100);

	int sTime = 0;
	int nTime = 0;
	sTime = time_ms();
	while(1)
	{   
		if(record_count > 16)
		{
			FD_ZERO(&fd_set_i2s2tcp);
			FD_SET(socket_conn, &fd_set_i2s2tcp);

			//get data from ringb
			get_count = ringbRead_v2(&record_rb, record_sendbuffer, 1024);
			if(get_count > 0)
			{
				
				ret = qcom_send(socket_conn, (char *)record_sendbuffer, get_count, 0);
				if(ret == TX_BUFF_FAIL)
				{  
					qcom_thread_msleep(1);
					qcom_send(socket_conn, (char *)record_sendbuffer, get_count, 0);
				}
			}
			
			fd_act_i2s2tcp = qcom_select(socket_conn + 1, &fd_set_i2s2tcp, NULL, NULL, &timeout);
			if(fd_act_i2s2tcp != 0)
			{
				if(FD_ISSET(socket_conn, &fd_set_i2s2tcp))
				{   
					ret = qcom_recv(socket_conn, szbuf, 16, 0);
				}
			}
		}

		nTime = time_ms();
		qcom_thread_msleep(15);
		
		if(nTime - sTime >= 60000)
		{
			i2s2tcp_buf_len = strlen(szbuf);
			qcom_send(socket_conn, szbuf, i2s2tcp_buf_len, 0);
			qcom_printf("recorder timeout and stopped! \n");
			break;
		}
	}
	
	qcom_thread_msleep(2000);
	for(i = 0; i < 10; i++)
	{
		fd_act_i2s2tcp = qcom_select(socket_conn + 1, &fd_set_i2s2tcp, NULL, NULL, &timeout);
		if(fd_act_i2s2tcp != 0)
		{
			if(FD_ISSET(socket_conn, &fd_set_i2s2tcp))
			{   
				ret = qcom_recv(socket_conn, szbuf, 32, 0);
			}
		}
	}
	
done:
	qcom_i2s_rcv_control(1,1);
	qcom_thread_msleep(1000);

	if (socket_conn > 0)
	{
		qcom_socket_close(socket_conn);
	}

	if (socket_serv > 0)
	{
		qcom_socket_close(socket_serv);
	}

    if(record_sendbuffer != NULL)
    {
        qcom_mem_free(record_sendbuffer);
        record_sendbuffer = NULL;
    }

    if(record_recvbuffer != NULL)
    {
        qcom_mem_free(record_recvbuffer);
        record_recvbuffer = NULL;
    }

	if(record_ringbuffer != NULL)
    {
        qcom_mem_free(record_ringbuffer);
        record_ringbuffer = NULL;
    }

    //restore gpio1-5
    for(i=0; i<5; i++) {
     *(unsigned int *)(0x1402c+i*4) = reglist_rec[i];
    }
    
    qcom_thread_msleep(1000);
	qcom_task_exit();
	return;
}


void cli_audio_recorder(void)
{
	int i;
    //save gpio1-5
    for(i=0; i<5; i++) {
        reglist_rec[i] = *(unsigned int *)(0x1402c+i*4);
    }
	uda1380_config(UDA1380_LINEIN);
	qcom_thread_msleep(2000);
	gip_for_i2s = INADDR_ANY;
	gl4_port_for_i2s = 6000;
	qcom_task_start(audio_recorder_main, 0, 4096, 10);
}


/**********************************************************
 **********************************************************
 **********************     loopback test      *******************
 **********************************************************
 **********************************************************/

void audio_loopback_test(unsigned int unused)
{

	int i, ret;
	A_UINT32 len;

	A_UINT8 audio_pattern_written[1024];
	A_UINT8 audio_pattern_read[1024];

	//A_UINT8 *p=audio_pattern_read;

	for(i=0; i<1024;i++)
		audio_pattern_written[i] = 0x5a;

	i=0;

#if 0
	qcom_i2s_init(1, 1, 16, 3, 3, 1024, NULL);
#else
	qcom_i2s_init(1, 1, 16, 5, 5, 1024, NULL);
#endif

	ret = qcom_i2s_xmt_data(1, (A_UINT8 *)audio_pattern_written,1024);

	qcom_i2s_rcv_control(1,0);
	while(1)
	{
		ret = qcom_i2s_xmt_data(1, (A_UINT8 *)audio_pattern_written,1024);
		if(!ret)
		{
			A_MEMSET(audio_pattern_read, 0, 1024);
			ret = qcom_i2s_rcv_data(1, audio_pattern_read, 1024, &len);

			if(len)
			{
				i++;
				ret = A_MEMCMP(audio_pattern_written, audio_pattern_read, 1024);
                #if 0
				if(ret)
					printf("%s: not equal!0x%08x\n", __func__, *(A_UINT32 *)p);
				else
				{
					if(!(i%10))
						printf("%s: ==> 0x%08x\n", __func__, *(A_UINT32 *)p);
				}
                #endif

			}
			else
			{
				qcom_thread_msleep(5);
//				qcom_thread_msleep(2);
			}
		}
		else
		{
			qcom_thread_msleep(5);
		}

//		if (i>=40)
//			break;
	}

//	qcom_task_exit();

}


void cli_audio_loopback_test(void)
{
	qcom_task_start(audio_loopback_test, 0, 2048, 50);
}


char *i2s_help="i2s { speak | recorder} { start | stop }\n";

int cmd_i2s_test(int argc, char *argv[])
{
	if(argc <2)
    	{
	        qcom_printf("%s", i2s_help);
	        return -1;
   	}
	if(!A_STRCMP(argv[1], "speak"))
	{
		if (argc<3)
		{
			qcom_printf("%s", i2s_help);
			return -1;
		}

		if(!A_STRCMP(argv[2], "start"))
		{
			extern void cli_audio_paly(void);
			cli_audio_paly();
			qcom_printf("i2s speaker test start...\n");
		}
		else if (!A_STRCMP(argv[2], "stop"))
		{
			qcom_printf("i2s speaker test stop...\n");
		}
	}
#if 1
	else if(!A_STRCMP(argv[1], "recorder"))
	{
		if (argc<3)
		{
			qcom_printf("%s", i2s_help);
			return -1;
		}

		if(!A_STRCMP(argv[2], "start"))
		{
			extern void cli_audio_recorder(void);

			qcom_printf("for sdi pin confilict, please inser the sdi pin after 10 seconds...\n");
			qcom_thread_msleep(10000);

			qcom_printf("i2s recorder test start...\n");
			cli_audio_recorder();
		}
		else if (!A_STRCMP(argv[2], "stop"))
		{
			extern int i2s_ctl_flag;
			i2s_ctl_flag|=0x2;
			qcom_printf("i2s recorder test stop...\n");
		}
	}

#endif
	else if(!A_STRCMP(argv[1], "loopback"))
		{
			extern void cli_audio_loopback_test(void);

			cli_audio_loopback_test();
			qcom_printf("i2s loopback test ...\n");

		}
	else{
		qcom_printf("%s", i2s_help);
	}

	return;
}
#endif //SWAT_I2S_TEST


