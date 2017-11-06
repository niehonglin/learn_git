
// Copyright (c) 2015 Qualcomm Atheros Communications Inc.
// All rights reserved.
// $ATH_LICENSE_TARGET_C$
//#include "cmnos_api.h"

#include "base.h"
#include "basetypes.h"
#include "queue.h"
#include "malloc_api.h"
#include "cdr.h"
#include "proxy_recv.h"
#include "select_api.h"
#include "qcom_lsock.h"


#define MAX_LMSG_NUM 8
#define  is_lsock(sock) ((sock >= 0) && (sock < MAX_LSCOK_SUPPORTED))
#define lsock_to_fd(sock) (sock+3)


struct lsock_msg lmsg_q[MAX_LMSG_NUM];
lsock_ctx_t lsocket[MAX_LSCOK_SUPPORTED];

extern LSOCK_RX_DETECT lsock_rx_detect ;

struct lsock_msg* alloc_lmsg(void)
{
    int i;

    for (i=0; i<MAX_LMSG_NUM; i++) {
       if (!lmsg_q[i].taken) {
           lmsg_q[i].taken = 1;
           return & lmsg_q[i];
       }
   }

   return  NULL;
}

void free_lmsg(struct lsock_msg* lmsg)
{
    lmsg->taken = 0;
}

void clear_lmsg_q(void)
{
    int i;

    for (i=0; i<MAX_LMSG_NUM; i++) {
       if (!lmsg_q[i].taken) {
           if (lmsg_q[i].p && (lmsg_q[i].blen>0)) {
               free(lmsg_q[i].p);
               lmsg_q[i].p = NULL;
               lmsg_q[i].blen = 0;
           }  
       }
   }
}

int get_lsock_tlen(int lsock)
{
    return lsocket[lsock].tlen;
}

int qcom_lsock_open(int  lsock)
{
    if (!is_lsock(lsock)) {
        //A_PRINTF("ERROR : local socket should be 0~3.\n");
        return A_ERROR;
    }

    /*local sock is taken*/
    if (lsocket[lsock].fd) {
        return A_ERROR;
    }

    STAILQ_INIT(&lsocket[lsock].msg);
    lsocket[lsock].fd = lsock_to_fd(lsock);
    if (!lsock_rx_detect) {
        lsock_rx_detect = get_lsock_tlen;
    }
    
    return lsocket[lsock].fd;    
}

A_STATUS qcom_lsock_write(int  lsock , char *p, short len)
{
    int index = 0;
    struct lsock_msg *lmsg = NULL;

    if (!is_lsock(lsock)) {
        return A_EINVAL;
    }

    /*local sock not opened*/
    if (!lsocket[lsock].fd) {
        return A_ERROR;
    }

    lmsg = alloc_lmsg();
    if (!lmsg)
        return A_NO_MEMORY;

    /* reuse the buffer if possible, it's best to send the fix lenth msg.*/
    if (lmsg->blen < len) {
        /*if current buf is not big enough, free it*/
        if (lmsg->blen > 0) {
            free(lmsg->p);
            lmsg->p = NULL;
            lmsg->blen = 0;
        }
        /*alloc a buffer*/
        lmsg->p = malloc(len);
        if (!lmsg->p) {
            free_lmsg(lmsg);
            return A_NO_MEMORY;
        }
        /*update the lmsg len.*/
        lmsg->blen = len;
    }

    lmsg->cc = len;
    memcpy (lmsg->p, p, len); 
    lsocket[lsock].tlen += lmsg->cc;
    STAILQ_INSERT_TAIL(&lsocket[lsock].msg, lmsg, next);

    if((index = find_socket_context(lsocket[lsock].fd, TRUE)) != SOCKET_NOT_FOUND){
        tx_event_flags_set(&proxy_recv_event, (1 << index), TX_OR);
    } 
    return A_OK;
}

short qcom_lsock_read(int  lsock , char *rbuf, short rlen)
{
    struct lsock_msg *lmsg = NULL;

    if (!is_lsock(lsock)) {
        return A_ERROR;
    }

    /*local sock not opened*/
    if (!lsocket[lsock].fd) {
        return A_ERROR;
    }

    if (!rbuf) {
        return A_ERROR;
    }

   lmsg = STAILQ_FIRST(&lsocket[lsock].msg);
    if (lmsg) {
        /*every lmsg shuold be copyed in one times*/
        if (lmsg->cc > rlen) {
            //A_PRINTF("rbuf len is small than msg len.\n");
            return A_ERROR;
        } else {
           STAILQ_REMOVE_HEAD(&lsocket[lsock].msg, next);
        }
    } else {
          return A_ERROR;
    }
    
    
    memcpy (rbuf, lmsg->p,lmsg->cc);
    lsocket[lsock].tlen -= lmsg->cc;
    
    free_lmsg(lmsg);
    //free(lmsg->p);

    return lmsg->cc;
}

A_STATUS qcom_lsock_close(int  lsock)
{
    struct lsock_msg *lmsg = NULL;
    int i = 0;

    if (!is_lsock(lsock)) {
        //A_PRINTF("ERROR : local socket should be 0~3.\n");
        return A_EINVAL;
    }

    /*local sock not opened*/
    if (!lsocket[lsock].fd) {
        return A_EINVAL;
    }

    lsocket[lsock].fd = 0;
    while(!STAILQ_EMPTY(&lsocket[lsock].msg)){
        /*Dequeue the packet*/
        lmsg = STAILQ_FIRST(&lsocket[lsock].msg);

        if(lmsg != NULL){
            /*Remove the node*/
            STAILQ_REMOVE_HEAD(&lsocket[lsock].msg,next);
        } 
    }

    clear_lmsg_q();
    /*if one lsock is open , don't clean the callback function*/
    for (i=0; i<MAX_LSCOK_SUPPORTED; i++) {
        if (lsocket[i].fd) {
            return A_OK;
        }
    }

    lsock_rx_detect = NULL;
    
    return A_OK;
}


