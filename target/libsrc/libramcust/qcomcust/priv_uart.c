/*
 * Copyright (c) 2011 Qualcomm Atheros, Inc.
 * All Rights Reserved.
 * Qualcomm Atheros Confidential and Proprietary.
 * $ATH_LICENSE_TARGET_C$
 */
#include <osapi.h>
#include <qcom/priv_uart.h>
#include <serialport_api.h>
#include <serialport_ioctl.h>
#include <uart_serphw_api.h>
#include <qcom/select_api.h>

#define DEBUG_UART                      0
#define CONSOLE_FLAGS_RX_SUSPENDED     (1 << 0)
#define CONSOLE_FLAGS_ALLOW_RX_SUSPEND (1 << 1)
#define CONSOLE_FLAGS_USE_HW_RX_SUSP   (1 << 2)
#define CONSOLE_FLAGS_RX_WAKE_ENABLED  (1 << 3)

#ifdef UARTCB_SUPPORT
#define CONSOLE_RECV_BUFFER_SIZE        (3*1024)
#else
#define CONSOLE_RECV_BUFFER_SIZE        (2*1024)
#endif
#define CONSOLE_RECV_BUFFER_THRESHOLD   (1024)
#ifdef UARTCB_SUPPORT
#define CONSOLE_SEND_BUFFER_SIZE        (3*1024)
#else
#define CONSOLE_SEND_BUFFER_SIZE        (2*1024)
#endif
#define CONSOLE_SLOW_POLL_INTERVAL_MS   1000
#define DEFAULT_BAUD                    115200

#define TX_SYS_DECL_PROTECT(flag)
#define TX_SYS_PROTECT(flag)
#define TX_SYS_UNPROTECT(flag)

typedef struct _CONSOLE_INFO {
    void            *port;
    A_UINT32        flags;
} CONSOLE_INFO;

#ifdef UARTCB_SUPPORT
typedef void (*uart_app_cb)(void *pContext);

typedef struct _uart_info_t {
    unsigned int    uart_fd;
    void            *handle;
    unsigned char   name[6];
    uart_app_cb     pUartCallback;
    void            *pUartContext;
} uart_info_t;

#define MAX_NUM_UART    2
#define GET_IDX_FROM_FD(x)  ((x != 0) ? (((x) <= MAX_NUM_UART) ? ((x) - 1) : -1) : -1)

//static CONSOLE_INFO *Console_uart_info[2] = {NULL,NULL};
//static A_BOOL        Console_serial_restored = FALSE;

static uart_info_t  uart_port_info[MAX_NUM_UART] = 
{
    {
        1, 0, "UART0", NULL, NULL
    },
    {
        2, 0, "UART1", NULL, NULL
    }
};
#else
static CONSOLE_INFO *Console_uart_info[2] = {NULL,NULL};
static A_BOOL        Console_serial_restored = FALSE;
static int uart_print_id=1;
#endif
static void* uart_handle[2];

static UART_CFG_T uart_data[2];
int uart_init = 0;

int Uart_read_null(int fd);
extern UART_RX_DETECT uart_rx_detect;


#ifndef UARTCB_SUPPORT
#if DEBUG_UART
static void console_putc_replacement(A_UINT8 ch);
#if __XCC__ < 8000
    #include "stdarg.h"
    #define va_list __gnuc_va_list
#else
    #define va_list __builtin_va_list
    #define va_arg __builtin_va_arg
    #define va_start __builtin_va_start
    #define va_end __builtin_va_end
    #define va_copy __builtin_va_copy
#endif

extern int cmnos_vprintf(void (*)(char ), const char *, va_list );

static void console_putc_replacement(A_UINT8 ch);

static void console_putc(unsigned char ch) 
{
    if (ch == '\n') {
        console_putc_replacement('\r');
        console_putc_replacement('\n');
    } else if (ch == '\r') {
    } else {
        console_putc_replacement(ch);
    }
}

static void (*_putc)(char c) = (void (*)(char))console_putc;

int
uart_dbg(const char *fmt, ...)
{
    va_list ap;
    int ret;

    va_start(ap, fmt);

    ret = cmnos_vprintf(_putc, fmt, ap);

    va_end(ap);

    return (ret);
}

static int console_getc_replacement(A_UINT8 *pch)
{
    return 0;
}

static void console_putc_replacement(A_UINT8 ch)
{
    int len = 1;
    
    if (Console_serial_restored || (Console_uart_info[1]->flags & CONSOLE_FLAGS_RX_SUSPENDED)) {
            /* direct UART method */
        SERP_Ioctl(Console_uart_info[1]->port,
                   SER_PORT_HW_IOCTL_WRITE_BYTES_DIRECT,
                   &ch,
                   1); 
    } else {
            /* buffered UART */
        SERP_Write(Console_uart_info[1]->port,&ch,&len);
    }
}

static void console_restore_replacement(void)
{
        /* system encountered an exception and restored the serial port,
         * set the flag to handle A_PRINTF outputs */
    Console_serial_restored = TRUE;    
}
#else
static void console_putc_replacement(A_UINT8 ch);
static void console_putc_replacement(A_UINT8 ch)
{
#if 0
    int len = 1;
    if (Console_serial_restored || (Console_uart_info[1]->flags & CONSOLE_FLAGS_RX_SUSPENDED)) {
            /* direct UART method */
        SERP_Ioctl(Console_uart_info[1]->port,
                   SER_PORT_HW_IOCTL_WRITE_BYTES_DIRECT,
                   &ch,
                   1); 
    } else {
            /* buffered UART */
        SERP_Write(Console_uart_info[1]->port,&ch,&len);
    }
#else
    if (!A_SERIAL_ENABLED()) 
        return;

    Console_serial_restored = TRUE;
    SERP_Ioctl(Console_uart_info[uart_print_id]->port, SER_PORT_HW_IOCTL_WRITE_BYTES_DIRECT, &ch, 1); 
#endif
}

#endif  //DEBUG_UART

static void _console_process_char(void)
{
#if 0
    A_UINT8  character;
    int      length;
    int      bytes_avail;
    
    while (TRUE) {
        
        bytes_avail = SERP_Read(Console_uart_info->port,(A_UINT8 *)&character,&length);
                
        if (length == 0) {
            break;    
        } else {
            uart_dbg("recv -- %d\n", length);
	    break;
	}
        
    }  
#endif    
}
#endif

static void console_serial_event_callback(void *context, A_UINT16 event, A_UINT32 arg0, A_UINT32 arg1)
{
#ifdef UARTCB_SUPPORT
    uart_info_t     *pUartInfo = (uart_info_t *)context;
#endif
    switch (event) {
        case SERIAL_EVENT_RX_AVAIL :
#ifdef UARTCB_SUPPORT
            if (pUartInfo->pUartCallback)
            {
                pUartInfo->pUartCallback(pUartInfo->pUartContext);
            }
#else			
            _console_process_char();   
#endif
            break;
        case SERIAL_EVENT_RX_WAKE :
                /* this is a one-shot event, it will not occur until we
                 * re-enable RX wake in the uart driver  */
#ifdef UARTCB_SUPPORT
            if (pUartInfo->pUartCallback)
            {
                pUartInfo->pUartCallback(pUartInfo->pUartContext);
            }
#else				 
            Console_uart_info[1]->flags &= ~CONSOLE_FLAGS_RX_WAKE_ENABLED;
            _console_process_char();
#endif
            break;
        default:
            break;    
    }
    
}

static void uart_mod_init(int phys_uart_id)
{
    UART_HW_CONFIG  uart_config;
    void            *phys_port;

#ifdef UARTCB_SUPPORT
    if (uart_port_info[phys_uart_id].handle != NULL) {
        return;    
    }
#else    
    if (Console_uart_info[phys_uart_id] != NULL) {
        return;    
    }
    
    Console_uart_info[phys_uart_id] = (CONSOLE_INFO *)A_ALLOCRAM(sizeof(CONSOLE_INFO));
#endif    
        /* setup the physical modem */
    phys_port = UARTSERP_HW_Init(phys_uart_id);
    A_ASSERT(phys_port != NULL);  
    A_MEMZERO(&uart_config,sizeof(uart_config));        
        /* for the console, set UART buffering parameters
         * for recv we can use a fixed value to accommodate very fast typists */
    uart_config.RxBufferSize = CONSOLE_RECV_BUFFER_SIZE;
        /* use a value that is an override */
    uart_config.TxBufferSize = CONSOLE_SEND_BUFFER_SIZE;
    UARTSERP_HW_Start(phys_port,&uart_config);
}

LOCAL void uart_mod_start(char* name, int phys_uart_id)
{   
    SERIAL_PORT_PROPERTIES  port_properties;
    SER_PORT_UART_CONFIG    port_config;
    A_STATUS                status;

#ifdef UARTCB_SUPPORT
        /* open the logical port */
    uart_port_info[phys_uart_id].handle = SERP_Open((A_CHAR*)name,
                                   &uart_port_info[phys_uart_id],
                                   console_serial_event_callback,
                                   &port_properties);
   
    A_MEMZERO(&port_config,sizeof(port_config));

    uart_handle[phys_uart_id] = uart_port_info[phys_uart_id].handle;
    port_config.Baud = DEFAULT_BAUD;
    port_config.DataBits = 8;
    port_config.StopBits = 1;
    
    /* configure port */
    status = SERP_Ioctl(uart_port_info[phys_uart_id].handle,
                        SER_PORT_IOCTL_CONFIG_UART_PORT,
                        &port_config,
                        sizeof(port_config));
    A_ASSERT(A_SUCCESS(status));
    
    /* set receiver threshold to 1 character */
    SERP_Ioctl(uart_port_info[phys_uart_id].handle,SER_PORT_IOCTL_SET_RX_THRESH,NULL,1);
#else    
        /* open the logical port */
    Console_uart_info[phys_uart_id]->port = SERP_Open((A_CHAR*)name,
                                   NULL,
                                   console_serial_event_callback,
                                   &port_properties);
   
    A_MEMZERO(&port_config,sizeof(port_config));

    uart_handle[phys_uart_id] = Console_uart_info[phys_uart_id]->port;
    port_config.Baud = DEFAULT_BAUD;
    port_config.DataBits = 8;
    port_config.StopBits = 1;
    
        /* configure port */
    status = SERP_Ioctl(Console_uart_info[phys_uart_id]->port,
                        SER_PORT_IOCTL_CONFIG_UART_PORT,
                        &port_config,
                        sizeof(port_config));
    A_ASSERT(A_SUCCESS(status));
    
        /* set receiver threshold to 1 character */
    SERP_Ioctl(Console_uart_info[phys_uart_id]->port,SER_PORT_IOCTL_SET_RX_THRESH,NULL,CONSOLE_RECV_BUFFER_THRESHOLD);
#endif          
#if DEBUG_UART
    _A_OS_INDIRECTION_TABLE->cmnos.serial._serial_putc = console_putc_replacement;    
    _A_OS_INDIRECTION_TABLE->cmnos.serial._serial_getc = console_getc_replacement;
    _A_OS_INDIRECTION_TABLE->cmnos.serial._serial_restore_funcs = console_restore_replacement;
#endif
}

void uart_rx_start(int phys_uart_id)
{
    extern void UartConfigureRxPolling(int id, int en);
    UartConfigureRxPolling(phys_uart_id, 1);
}

/* Return number of UART */
int Uart_Enumerate(void)
{
    //int uart_num = 1;
    int uart_num = 2;
    if (uart_init != 0) {
    	return uart_num;
    }
    
    uart_init = 1; 
    
    SERP_MODULE_INSTALL();
    SERP_Init();
    UARTSERP_HW_MODULE_INSTALL();
    uart_mod_init(0);
    uart_mod_init(1);
    uart_mod_start((char*)"UART0", 0);
    uart_mod_start((char*)"UART1", 1);
    return uart_num;
}

/* Uart_Open - open UART and get a file descritor (fd) 
   name(input): UART0 or UART1
*/
int Uart_Open(char* name)
{
#ifdef UARTCB_SUPPORT
    int id = 0;

    for (id = 0; id < 2; id++)
    {
        if (A_STRCMP(name, uart_port_info[id].name) == 0) {
            break;
        }
    }

    if (id == 2)
        return -1;

    return uart_port_info[id].uart_fd;
#else
    int phys_uart_id;

    if (A_STRCMP(name, "UART0") == 0) {
        phys_uart_id = 0;
       //uart_rx_start(0);
    } else if (A_STRCMP(name, "UART1") == 0) {
        phys_uart_id = 1;
       //uart_print_id = 0;
        uart_rx_start(1);
    } else {
         //A_ASSERT(FALSE);
         return -1;
    }

    uart_rx_detect = Uart_read_null;
//    uart_mod_init(phys_uart_id);
//    uart_mod_start(name, phys_uart_id);
    _A_OS_INDIRECTION_TABLE->cmnos.serial._serial_putc = console_putc_replacement;    

    return (phys_uart_id + 1);
#endif
}

/* Set parameter of UART */
int Uart_SetPara(int fd, UART_CFG_T uart_cfg) 
{
    A_STATUS                status;
    SER_PORT_UART_CONFIG   port_config; 
#ifdef UARTCB_SUPPORT
    void *handle = (void *)uart_handle[GET_IDX_FROM_FD(fd)];

    A_MEMCPY(&(uart_data[GET_IDX_FROM_FD(fd)]), &uart_cfg, sizeof(UART_CFG_T));
#else
    void *handle = (void *)uart_handle[fd-1];

    A_MEMCPY(&(uart_data[fd-1]), &uart_cfg, sizeof(UART_CFG_T));
#endif	
    A_MEMCPY(&port_config, &uart_cfg, sizeof(port_config));
        /* configure port */
    status = SERP_Ioctl(handle,
                        SER_PORT_IOCTL_CONFIG_UART_PORT,
                        &port_config,
                        sizeof(port_config));
    return status;
}

/* Get parameter of UART */
int Uart_GetPara(int fd, UART_CFG_T* uart_cfg)
{
    if(fd == 1 || fd == 2)
    {
#ifdef UARTCB_SUPPORT
        A_MEMCPY(uart_cfg, &(uart_data[GET_IDX_FROM_FD(fd)]), sizeof(UART_CFG_T));
#else	
        A_MEMCPY(uart_cfg, &(uart_data[fd-1]), sizeof(UART_CFG_T));
#endif		
        return 0;
    }
    else
        return -1;
}

/* fd (Input), file descriptor 
   buff (Input/output), address of the buff, the buffer should big enough.
   len (Input/output), Input length of the buff, output length of bytes has been   written.
   returns, bytes remain in the TX buffer.
*/
int Uart_Write(int fd, unsigned char* buff, int* len)
{
#ifdef UARTCB_SUPPORT
    void *handle = (void *)uart_handle[GET_IDX_FROM_FD(fd)];
#else
    void *handle = (void *)uart_handle[fd-1];
#endif	
    int retVal = -1;
    //A_PRINTF("Uart_Write, fd=%d, handle %p\n", fd, handle);
    TX_SYS_DECL_PROTECT(prot_flag);

    TX_SYS_PROTECT(prot_flag);
    retVal =  SERP_Write(handle,buff,len);
    TX_SYS_UNPROTECT(prot_flag);
    return retVal;
}

/* fd (Input), file descriptor 
   buff (Input/output), address of the buff, the buffer should big enough.
   len (Input/output), Input length of the buff, output length of bytes read.
   returns, bytes remain in the RX buffer.
*/
int Uart_Read(int fd, unsigned char* buff, int* len)
{
#ifdef UARTCB_SUPPORT
    void *handle = (void *)uart_handle[GET_IDX_FROM_FD(fd)];
#else
    void *handle = (void *)uart_handle[fd-1];
#endif	
    int retVal = -1;
    //uart_dbg("Uart_Read,port %p,handle %p fd=%d\n", Console_info->port, handle, fd);
    TX_SYS_DECL_PROTECT(prot_flag);

    TX_SYS_PROTECT(prot_flag);
    retVal = SERP_Read(handle, buff, len);
    TX_SYS_UNPROTECT(prot_flag);
    return retVal;
}

int Uart_read_null(int fd)
{
    int lenth = 0;
    return Uart_Read(fd, (unsigned char*)NULL, &lenth);
}

/* Uart_Close -close UART with given fd */
int Uart_Close(int fd)
{
#ifdef UARTCB_SUPPORT
//    void *handle = (void *)uart_handle[GET_IDX_FROM_FD(fd)];
#else
//    void *handle = (void *)uart_handle[fd-1];
#endif    
//    SERP_Close(handle);
    return 0;
}

int Uart_GetFd(char* name)
{
   int fd = 0;
   if (A_STRCMP(name, "UART0") == 0) 
   {
	fd = 1;
    } 
   else if (A_STRCMP(name, "UART1") == 0)
   {
	fd = 2;
    } 

    return fd;
}

#ifdef UARTCB_SUPPORT
int Uart_Interrupt_Config(int fd, void *cb, void *pContext, int enable)
{
    int phys_id = GET_IDX_FROM_FD(fd);
    //int retval = -1;

    if (-1 == phys_id)
        return -1;

    if (!uart_port_info[phys_id].pUartCallback) {
        uart_port_info[phys_id].pUartCallback = (uart_app_cb )cb;
        uart_port_info[phys_id].pUartContext = pContext;
    }

#if 0
    retval = SERP_Ioctl(uart_port_info[phys_id].handle,
               SER_PORT_HW_IOCTL_CONFIG_RX_WAKEUP, NULL, enable);
    
    if (enable)
        retval = SERP_Ioctl(uart_port_info[phys_id].handle,
            SER_PORT_IOCTL_SET_EVENT_MASK, NULL, (1 << SERIAL_EVENT_RX_WAKE));
    else
        retval = SERP_Ioctl(uart_port_info[phys_id].handle,
            SER_PORT_IOCTL_CLEAR_EVENT_MASK, NULL, (1 << SERIAL_EVENT_RX_WAKE));

    return retval;
#endif
    return 0;
}
#endif
