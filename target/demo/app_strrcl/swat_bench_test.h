/*
  * Copyright (c) 2015 Qualcomm Atheros, Inc..
  * All Rights Reserved.
  * Qualcomm Atheros Confidential and Proprietary.
  */

#ifndef _SWAT_BENCH_INICHE_1_1_2_H_
#define _SWAT_BENCH_INICHE_1_1_2_H_

extern volatile A_BOOL flag_new_console_task;

void swat_bench_tcp_rx_task(STREAM_CXT_t* pStreamCxt);
void swat_bench_tcp_tx_task(STREAM_CXT_t* pStreamCxt);
void swat_bench_udp_rx_task(STREAM_CXT_t* pStreamCxt);
void swat_bench_udp_tx_task(STREAM_CXT_t* pStreamCxt);
void swat_bench_raw_rx_task(STREAM_CXT_t* pStreamCxt);
void swat_bench_raw_tx_task(STREAM_CXT_t* pStreamCxt);

void swat_bench_tx(A_UINT32 ipAddress, IP6_ADDR_T ip6Address, A_UINT32 port, A_UINT32 protocol, A_UINT32 pktSize, 
                        A_UINT32 mode, A_UINT32 seconds, A_UINT32 numOfPkts,
                        A_UINT32 local_ipAddress, A_UINT32 ip_hdr_inc,A_UINT32 delay);
void swat_bench_rx_test(A_UINT32 protocol, A_UINT32 port, A_UINT32 localIpAddress, 
                               A_UINT32 mcastIpAddress,IP6_ADDR_T* lIp6, IP6_ADDR_T* mIp6,
                               A_UINT32 ip_hdr_inc, A_UINT32 rawmode);

#endif


