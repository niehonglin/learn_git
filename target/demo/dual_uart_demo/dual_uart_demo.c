/*
  * Copyright (c) 2014 Qualcomm Atheros, Inc..
  * All Rights Reserved.
  * Qualcomm Atheros Confidential and Proprietary.
  */

#include "qcom_common.h"
#include "qcom_p2p_api.h"
#include "qcom_wps.h"
#include "threadx/tx_api.h"
#include "threadxdmn_api.h"
#include "qcom_uart.h"
#include "select_api.h"

TX_THREAD host_thread;
#define BYTE_POOL_SIZE (5*1024)
#define PSEUDO_HOST_STACK_SIZE (4 * 1024)   /* small stack for pseudo-Host thread */
TX_BYTE_POOL pool;

#define uart_test_str0 "hello uart0\r\n"
#define uart_test_str1 "hello uart1\r\n"
#define uart_buffer_size 1024

/*
* User should configure the appropriate tx/rx pins 
* for uart0 and uart1 based on their HW design. 
* The two uart will be opened for read and write, 
* receive data from each uart and echo the data to user. 
*/
void uart_demo(void)
{
    A_UINT32 uart_length, remain_len;
    A_CHAR uart_buff[uart_buffer_size];
    q_fd_set fd;
    struct timeval tmo;
    A_INT32 uart0_fd, uart1_fd;
    int ret;

    A_PRINTF("uart_demo in...\n");
    qcom_uart_init();
    uart0_fd = qcom_uart_open((A_CHAR *)"UART0");
    if (uart0_fd == -1) {
        A_PRINTF("qcom_uart_open uart0 failed...\n");
        return;
    }
    uart1_fd = qcom_uart_open((A_CHAR *)"UART1");
    if (uart1_fd == -1) {
        A_PRINTF("qcom_uart_open uart1 failed...\n");
        return;
    }
    tx_thread_sleep(1000);
    A_PRINTF("uart0_fd %d uart1_fd %d\n", uart0_fd, uart1_fd);
    tx_thread_sleep(10000);

    uart_length = strlen((A_CHAR *)uart_test_str0);
    qcom_uart_write(uart0_fd, (A_CHAR *)uart_test_str0, &uart_length);
    uart_length = strlen((A_CHAR *)uart_test_str1);
    qcom_uart_write(uart1_fd, (A_CHAR *)uart_test_str1, &uart_length);
    
    while (1)
    {
        FD_ZERO(&fd);
        FD_SET(uart0_fd, &fd);
        FD_SET(uart1_fd, &fd);
        tmo.tv_sec = 30;
        tmo.tv_usec = 0;

        ret = qcom_select(uart1_fd + 1, &fd, NULL, NULL, &tmo);
        if (ret == 0) {
            A_PRINTF("UART receive timeout\n");
        } else {
            if (FD_ISSET(uart0_fd, &fd)) {
                uart_length = uart_buffer_size;
                remain_len = qcom_uart_read(uart0_fd, uart_buff, &uart_length);
                if (uart_length) {
                    remain_len = qcom_uart_write(uart0_fd, uart_buff, &uart_length);
                }
            }

            if (FD_ISSET(uart1_fd, &fd)) {
                uart_length = uart_buffer_size;
                remain_len = qcom_uart_read(uart1_fd, uart_buff, &uart_length);
                if (uart_length) {
                    A_PRINTF("receive length:%d\n", uart_length);
                    remain_len = qcom_uart_write(uart1_fd, uart_buff, &uart_length);
                }
            }
        }
    }
}

void uart_demo_921600(void)
{
    A_UINT32 uart_length;
    A_UINT32 uart0_length;
    A_CHAR uart_buff[uart_buffer_size];
    A_CHAR uart0_buff[6] = {0x55, 0xaa, 0x55, 0xaa, 0x55, 0xaa};
    q_fd_set fd;
    struct timeval tmo;
    A_INT32 uart0_fd, uart1_fd;
    int ret;

    qcom_uart_para com_uart_cfg;

    //com_uart_cfg.BaudRate=     115200;
    com_uart_cfg.BaudRate=     921600; /* 1Mbits */
    com_uart_cfg.number=       8;
    com_uart_cfg.StopBits =    1;
    com_uart_cfg.parity =      0;
    com_uart_cfg.FlowControl = 0;

    A_PRINTF("uart_demo in...\n");
    qcom_uart_init();
    uart0_fd = qcom_uart_open((A_CHAR *)"UART0");
    if (uart0_fd == -1) {
        A_PRINTF("qcom_uart_open uart0 failed...\n");
        return;
    }
    uart1_fd = qcom_uart_open((A_CHAR *)"UART1");
    if (uart1_fd == -1) {
        A_PRINTF("qcom_uart_open uart1 failed...\n");
        return;
    }

    int i;
    for(i=0; i<15; i++) {
        A_PRINTF("sleep %d seconds...\n", i);
        tx_thread_sleep(1000);
    }
    qcom_set_uart_config((A_CHAR *)"UART0",&com_uart_cfg);

    tx_thread_sleep(1000);
    A_PRINTF("uart0_fd %d uart1_fd %d\n", uart0_fd, uart1_fd);
    tx_thread_sleep(10000);

    uart_length = strlen((A_CHAR *)uart_test_str0);
    qcom_uart_write(uart0_fd, (A_CHAR *)uart_test_str0, &uart_length);
    uart_length = strlen((A_CHAR *)uart_test_str1);
    qcom_uart_write(uart1_fd, (A_CHAR *)uart_test_str1, &uart_length);

    while (1)
    {
        FD_ZERO(&fd);
        FD_SET(uart0_fd, &fd);
        FD_SET(uart1_fd, &fd);
        tmo.tv_sec = 30;
        tmo.tv_usec = 0;

        ret = qcom_select(uart1_fd + 1, &fd, NULL, NULL, &tmo);
        if (ret == 0) {
            A_PRINTF("UART receive timeout\n");
        } else {
            if (FD_ISSET(uart0_fd, &fd)) {
                qcom_uart_read(uart0_fd, uart_buff, &uart_length);
                tx_thread_sleep(100);
                if (uart_length) {
                    A_PRINTF("uart0 receive length:%d\n", uart_length);
                    qcom_uart_write(uart0_fd, uart_buff, &uart_length);
                    tx_thread_sleep(100);
                }
            } else if (FD_ISSET(uart1_fd, &fd)) {
                qcom_uart_read(uart1_fd, uart_buff, &uart_length);
                tx_thread_sleep(100);
                if (uart_length) {
                    A_PRINTF("uart1 receive length:%d\n", uart_length);
                    qcom_uart_write(uart1_fd, uart_buff, &uart_length);
                    A_PRINTF("Send to UART0: {0x55, 0xaa, 0x55, 0xaa, 0x55, 0xaa}\n");
                    uart0_length = 6;
                    qcom_uart_write(uart0_fd, uart0_buff, &uart_length);
                    tx_thread_sleep(100);
                }
            } else {
                A_PRINTF("UART something is error!\n");
            }
        }
    }
}

void
uart_demo_entry(ULONG which_thread)
{
    extern void user_pre_init(void);
    user_pre_init();
    qcom_enable_print(1);
    A_PRINTF("uart_demo_entry in...\n");
    uart_demo();
}

void user_main(void)
{    
    extern void task_execute_cli_cmd();
    tx_byte_pool_create(&pool, "dual_uart_demo pool", TX_POOL_CREATE_DYNAMIC, BYTE_POOL_SIZE);

    {
        CHAR *pointer;
        tx_byte_allocate(&pool, (VOID **) & pointer, PSEUDO_HOST_STACK_SIZE, TX_NO_WAIT);

        tx_thread_create(&host_thread, "dual_uart_demo thread", uart_demo_entry,
                         0, pointer, PSEUDO_HOST_STACK_SIZE, 16, 16, 4, TX_AUTO_START);
    }

#if defined(AR6002_REV74)
    cdr_threadx_thread_init();
#endif
}

