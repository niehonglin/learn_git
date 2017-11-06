/*
 * Copyright (c) 2013 Qualcomm Atheros, Inc.
 * All Rights Reserved.
 * Qualcomm Atheros Confidential and Proprietary.
 * $ATH_LICENSE_TARGET_C$
 */

#include "ipport.h"
#include "libport.h"
#include "osport.h"

#include "tcpport.h"

#include "protosw.h"
#include "in_pcb.h"
#include "if_athvar.h"
#include "txrx_internal.h"
#include "qcom/select_internal.h"
#include "qcom/priv_uart.h"

extern TXRX_CONTEXT *g_pTXRX;
#define  SOREAD      1
#define  SOWRITE     2

extern int patch_sock_selscan(fd_set * ibits, fd_set * obits);
extern void uart_select_patch_install(void);
int
patch_sock_selscan(fd_set * ibits, fd_set * obits)
{
   fd_set *in, *out;
   int   which;
   int   sock;
   int   flag  =  0;
   int   num_sel  =  0;

   for (which = 0; which < 3; which++)
   {
      switch (which)
      {
      case 0:
         flag = SOREAD;
         break;

      case 1:
         flag = SOWRITE;
         break;

      case 2:
         flag = 0;
         break;
      }
      in = &ibits[which];
      out = &obits[which];

      for (sock = 0; sock < (int)in->fd_count; sock++)
      {
        //Not UART FD
        if (in->fd_array[sock] > 2)
        {
          if (sock_select(in->fd_array[sock], flag))
          {
            FD_SET(in->fd_array[sock], out);	
            num_sel++;
          }
        }
        else    /* UART */
        {
 #if 1       
              int remain=0, length=0, fd_uart;  
              fd_uart = in->fd_array[sock];
              if(SOREAD == flag)  
                     remain=Uart_Read(fd_uart, NULL, &length);
              if(SOWRITE == flag)  
                     remain=Uart_Write(fd_uart, NULL, &length);
              if(remain > 0)
              {
                    FD_SET(fd_uart, out);
                    num_sel++;
              }
#endif			  
        }

      }
   }

   return (num_sel);
}

void uart_select_patch_install()
{
    soselect_internal_api._sock_selscan = patch_sock_selscan;
}

