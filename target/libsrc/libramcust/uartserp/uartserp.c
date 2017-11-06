/*
 * Copyright (c) 2011-2015 Qualcomm Atheros, Inc.
 * All Rights Reserved.
 * Qualcomm Atheros Confidential and Proprietary.
 * $ATH_LICENSE_TARGET_C$
 */

//
// @File:
//
// @Abstract: WLAN UART (aka HCI UART) serial port implementation
//
// @Notes:
//
//

#include <osapi.h>
#include "uartserp_internal.h"
#include <hw/uart_reg.h>
#include "select_handler_api.h"
#include "qcom_gpio.h"

#undef A_WUART_REG_READ
#undef A_WUART_REG_WRITE

UART_HW_PIN_CONFIG g_Uart0DefaultPins[UART_HW_PIN_MAX] =
{
    {.GpioPin      = UART0_RX_PIN,
     .ConfigValue  = 1,
     .ConfigAsGpio = UART0_GPIO_CFG},
    {.GpioPin      = UART0_CTS_PIN,
     .ConfigValue  = UART0_CFG, 
     .ConfigAsGpio = UART0_GPIO_CFG},
#if 0    
    {.GpioPin      = UART0_TX_PIN,
     .ConfigValue  = 1, 
#else
    /* The GPIO19 is connected with UART0_TXD pin(GPIO PIN7) on SP140. 
       Then in Quad mode, UART TX will affect the SPI operation.
       Change the UART0_TXD(GPIO PIN7) to GPIO PIN11 of UART1.
       */
    {.GpioPin      = UART0_TX_PIN_HACK,
     .ConfigValue  = UART0_CFG_HACK, 
#endif
     .ConfigAsGpio = UART0_GPIO_CFG},
    {.GpioPin      = UART0_RTS_PIN,
     .ConfigValue  = UART0_CFG, 
     .ConfigAsGpio = UART0_GPIO_CFG},          
};

UART_HW_PIN_CONFIG g_Uart1DefaultPins[UART_HW_PIN_MAX] =
{
    {.GpioPin      = UART1_RX_PIN,
     .ConfigValue  = UART1_CFG, 
     .ConfigAsGpio = UART1_GPIO_CFG},
    {.GpioPin      = UART1_CTS_PIN,
     .ConfigValue  = UART1_CFG, 
     .ConfigAsGpio = UART1_GPIO_CFG},
#if 0     
    {.GpioPin      = UART1_TX_PIN,
#else
    /* PIN conflict in SP140/SP144 in Quad SPI mode. 
       GPIO24 is not available on QCA4004.
    */
    {.GpioPin      = UART1_TX_PIN_HACK,
#endif
     .ConfigValue  = UART1_CFG, 
     .ConfigAsGpio = UART1_GPIO_CFG},
    {.GpioPin      = UART1_RTS_PIN,
     .ConfigValue  = UART1_CFG, 
     .ConfigAsGpio = UART1_GPIO_CFG}, 
};

#if defined(AR6002_REV6) || defined(AR6002_REV7)

UART_HW_PIN_CONFIG g_Uart2DefaultPins[UART_HW_PIN_MAX] =
{
    {.GpioPin      = UART2_RX_PIN,
     .ConfigValue  = UART2_CFG, 
     .ConfigAsGpio = UART2_GPIO_CFG},
    {.GpioPin      = UART2_CTS_PIN,
     .ConfigValue  = UART2_CFG, 
     .ConfigAsGpio = UART2_GPIO_CFG},
    {.GpioPin      = UART2_TX_PIN,
     .ConfigValue  = UART2_CFG, 
     .ConfigAsGpio = UART2_GPIO_CFG},
    {.GpioPin      = UART2_RTS_PIN,
     .ConfigValue  = UART2_CFG, 
     .ConfigAsGpio = UART2_GPIO_CFG}, 
};

#endif

UART_DEVICE *g_pUartDevices[UART_SERP_MAX_INSTANCES];
extern int uart_init;
#if 0
void Uart_Rx_Pin_19(void)
{
    g_Uart1DefaultPins[0].GpioPin=19;
    g_Uart1DefaultPins[0].ConfigValue = 11;
    g_Uart0DefaultPins[0].GpioPin=19;
    g_Uart0DefaultPins[0].ConfigValue = 2;
}
#endif
#if 1 /* Deleted according to QCA4004 GPIO Power Optimization.docx */
void
Uart_Rx_Pin_Set(int pin0, int pin1)
{
    int ConfigValue;
    /* Config RX pin for UART0 */
    ConfigValue = -1;
    switch (pin0) {
    /* Config RX pin for UART1 */
    case 2:
        ConfigValue = 1;
        break;
    case 6:
        ConfigValue = 3;
        break;
    case 10:
        ConfigValue = 4;
        break;
    case 15:
        ConfigValue = 5;
        break;
    case 19:
        ConfigValue = 11;
        break;
    case 21:
        ConfigValue = 9;
        break;
    case 25:
        ConfigValue = 4;
        break;
    default:
        break;
    }
    if (ConfigValue >= 0) {
        g_Uart0DefaultPins[0].GpioPin = pin0;
        g_Uart0DefaultPins[0].ConfigValue = ConfigValue;
    }
    ConfigValue = -1;
    switch (pin1) {
        case 6:
            ConfigValue = 4;
            break;
        case 10:
            ConfigValue = 8;
            break;
        case 15:
            ConfigValue = 2;
            break;
        case 19:
            ConfigValue = 2;
            break;
        case 21:
            ConfigValue = 8;
            break;
        case 25:
            ConfigValue = 3;
            break;
        default:
            break;
    }
    if (ConfigValue >= 0) {
        g_Uart1DefaultPins[0].GpioPin = pin1;
        g_Uart1DefaultPins[0].ConfigValue = ConfigValue;
    }
}

void
Uart_Tx_Pin_Set(int pin0, int pin1)
{
    int ConfigValue;
    /* Config TX pin for UART0 */
    ConfigValue = -1;
    switch (pin0) {
    /* Config TX pin for UART1 */
    case 4:
        ConfigValue = 1;
        break;
    case 7:
        ConfigValue = 1;
        break;
    case 11:
        ConfigValue = 4;
        break;
    case 16:
        ConfigValue = 5;
        break;
    case 20:
        ConfigValue = 5;
        break;
    case 24:
        ConfigValue = 4;
            break;
        default:
            break;
    }
    if (ConfigValue >= 0) {
        g_Uart0DefaultPins[2].GpioPin = pin0;
        g_Uart0DefaultPins[2].ConfigValue = ConfigValue;
    }
    ConfigValue = -1;
    switch (pin1) {
        case 7:
            ConfigValue = 3;
            break;
        case 11:
            ConfigValue = 8;
            break;
        case 16:
            ConfigValue = 2;
            break;
        case 20:
            ConfigValue = 2;
            break;
        case 24:
            ConfigValue = 3;
        break;
    default:
        break;
    }
    if (ConfigValue >= 0) {
        g_Uart1DefaultPins[2].GpioPin = pin1;
        g_Uart1DefaultPins[2].ConfigValue = ConfigValue;
    }
}
#endif

USAPI void _StartStopInterruptPolling(UART_DEVICE *pUartDevice, A_BOOL Start)
{
    if (Start) {
        if (!pUartDevice->TimerQueued) {
            pUartDevice->TimerQueued = TRUE;
            A_TIMEOUT_MS(&pUartDevice->PollingTimer,pUartDevice->PollTimeoutMS,0);        
        }   
    } else {
        if (pUartDevice->TimerQueued) {
            pUartDevice->TimerQueued = FALSE;
            A_UNTIMEOUT(&pUartDevice->PollingTimer);    
        } 
    }
}

USAPI void _InterruptPollCallback(A_HANDLE alarm, void *data)
{ 
    UART_DEVICE *pUartDevice = (UART_DEVICE *)data;
    
    pUartDevice->TimerQueued = FALSE;
 
    UartDevDsrHandler(pUartDevice);
    
    if (A_IX_UART_REG_READ(pUartDevice,UART_INT_EN_ADDRESS) != 0) {
            /* keep polling */    
        StartStopInterruptPolling(pUartDevice,TRUE);
    }
}
    
static void DisableUartInterrupts(UART_DEVICE *pUartDevice, A_UINT32 Ints)
{
    A_UINT32 val = A_IX_UART_REG_READ(pUartDevice,UART_INT_EN_ADDRESS); 
    val &= ~Ints;
    A_IX_UART_REG_WRITE(pUartDevice,UART_INT_EN_ADDRESS,val);
    (void)A_IX_UART_REG_READ(pUartDevice,UART_INT_EN_ADDRESS); 
}

static void EnableUartInterrupts(UART_DEVICE *pUartDevice, A_UINT32 Ints)
{
    A_UINT32 val = A_IX_UART_REG_READ(pUartDevice,UART_INT_EN_ADDRESS); 
    val |= Ints;
    A_IX_UART_REG_WRITE(pUartDevice,UART_INT_EN_ADDRESS,val);
    (void)A_IX_UART_REG_READ(pUartDevice,UART_INT_EN_ADDRESS);  
}

#define  A_INTR_DISABLE_FOR_UART(pOld)                \
    do {                                                \
        A_UINT32 tmp;                        \
        asm volatile("rsr.ps %0" : "=r" (tmp)); \
        *(pOld) = (A_old_intr_t)(tmp & 0x0F);      \
        if ((tmp & 0x0F) < 3) {          \
            tmp |= 0x03;                           \
        }                                          \
        asm volatile("wsr.ps %0" :: "r" (tmp) : "ps"); \
    } while (0)

extern void athos_restore_intrlvl(A_old_intr_t old_intr);
#define  A_INTR_RESTORE_FOR_UART(old)    athos_restore_intrlvl((old))

USAPI void _UartTryTransmit(UART_DEVICE *pUartDevice)
{
    A_UINT8     byte;
    A_UINT32    data;
    int         bytesPopped,bytesTransmitted;
    A_BOOL      noRoom = FALSE;
    A_old_intr_t old_intr;
    
    bytesTransmitted = 0;
    
    while  (TRUE) {
        
        (void)A_IX_UART_REG_READ(pUartDevice,UART_DATA_ADDRESS);
        
            /* check if there is room in the TX FIFO */
        data = A_IX_UART_REG_READ(pUartDevice,UART_DATA_ADDRESS);             
        if (!(data & UART_DATA_TX_CSR_MASK)) {
            noRoom = TRUE;
            break;    
        }        
            /* TODO, this will be a bit slow */
        bytesPopped = SERPHW_PopTXBytes(pUartDevice->hPortDev,&byte,1);
        if (!bytesPopped) {
                /* ring buffer in upper layer is now drained */
            break;    
        }           
        
            /* push byte onto the FIFO */
        data = byte | UART_DATA_TX_CSR_SET(1);
        A_IX_UART_REG_WRITE(pUartDevice,UART_DATA_ADDRESS, data);
            /* flush before looping around */
        (void)A_IX_UART_REG_READ(pUartDevice,UART_DATA_ADDRESS);
          
        bytesTransmitted++;
        
    }   
    
    A_INTR_DISABLE_FOR_UART(&old_intr);
    //A_INTR_DISABLE(&old_intr);
    if (bytesTransmitted || noRoom) {
        UART_STAT_INCR(pUartDevice,TxBytes,(A_UINT32)bytesTransmitted); 
        if (!pUartDevice->Polling) {
            pUartDevice->EnableInterrupts |= UART_INT_EN_TX_EMPTY_INT_EN_MASK;
            if (!pUartDevice->RxIsr)
                EnableUartInterrupts(pUartDevice, UART_INT_EN_TX_EMPTY_INT_EN_MASK);
        } else
        EnableUartInterrupts(pUartDevice,UART_INT_EN_TX_EMPTY_INT_EN_MASK);    
    } else {
            /* transmitter is now idle */
        if (!pUartDevice->Polling) {
            pUartDevice->EnableInterrupts &= ~UART_INT_EN_TX_EMPTY_INT_EN_MASK;
            if (!pUartDevice->RxIsr)
                DisableUartInterrupts(pUartDevice,UART_INT_EN_TX_EMPTY_INT_EN_MASK);       
        } else
            DisableUartInterrupts(pUartDevice,UART_INT_EN_TX_EMPTY_INT_EN_MASK);      
    }
    A_INTR_RESTORE_FOR_UART(old_intr);
    //A_INTR_RESTORE(old_intr);
}

extern void SERHWAPI_PushRXBytes_Notify(SER_PORT_INSTANCE Instance);
extern int SERHWAPI_PushRXBytes_Push(SER_PORT_INSTANCE Instance, A_UINT8 *pBuffer, int Length);
USAPI void _UartReceive(UART_DEVICE *pUartDevice)
{
    A_UINT32 data;
    A_UINT8  byte;
    int      bytesPushed;

    while (TRUE) {
     
            /* read the FIFO port, this will not pop the actual byte out of the FIFO */    
        data = A_IX_UART_REG_READ(pUartDevice,UART_DATA_ADDRESS);        
            /* check if the byte is valid */
        if (!(data & UART_DATA_RX_CSR_MASK)) {
            break;    
        }
        
        byte = (A_UINT8)(data & 0xff);

            /* advance the RX FIFO */
        data = UART_DATA_RX_CSR_SET(1);
        A_IX_UART_REG_WRITE(pUartDevice,UART_DATA_ADDRESS, data);
            /* flush */
        (void)A_IX_UART_REG_READ(pUartDevice,UART_DATA_ADDRESS);
         
            /* TODO : this will be slow */
        bytesPushed = SERPHW_PushRXBytes(pUartDevice->hPortDev,&byte,1);
        
#if 0
        if (!bytesPushed) {
                /* upper layer ring buffer can't take anymore data */
            pUartDevice->SavedByte = (A_UINT16)byte | SAVED_BYTE_VALID;
                /* disable interrupt and wait */
            DisableUartInterrupts(pUartDevice,UART_INT_EN_RX_VALID_INT_EN_MASK);  
            break;    
        }   
#endif
        
        UART_STAT_INCR(pUartDevice,RxBytes,(A_UINT32)bytesPushed);                                    
    }
    
}

    /* callback from serial layer when the application writes bytes to the TX buffer,
     * this is to kick start the transmitter */
static void UartDevHWTransmit(void *pPortContext, int TxBufferDepth)
{
    UART_DEVICE *pUartDevice = (UART_DEVICE *)pPortContext;
    UartTryTransmit(pUartDevice); 
}


    /* drain handler, called by serial port layer when there is room in receiver to queue more bytes*/
static void UartDevDrainRX(void *pPortContext)
{
#if 0
    UART_DEVICE *pUartDevice = (UART_DEVICE *)pPortContext;
    if (pUartDevice->SavedByte) {
        A_UINT8 byte = (A_UINT8)(pUartDevice->SavedByte & 0xFF);
        pUartDevice->SavedByte = 0;
        SERPHW_PushRXBytes(pUartDevice->hPortDev,&byte,1);    
    }    
        /* re-enable recv interrupts */
    EnableUartInterrupts(pUartDevice,UART_INT_EN_RX_VALID_INT_EN_MASK);  
#endif
}

USAPI void _UartDevSetBaud(UART_DEVICE *pUartDevice, A_UINT32 Baud, A_UINT32 Scale, A_UINT32 Step)
{
    int      scale;
    int      step;
    
    if (Baud != 0) {
            /* this is a rough calculation that works for lower baud rates */
        int divisor = A_UART_FREQUENCY() / 16 / Baud;
        step = 8192;
        while (divisor > 0xff) {
            divisor = divisor >> 1;
            step = step >> 1;
        }
        scale = divisor;
    } else  {
            /* use explicit values */
        scale = Scale;
        step =  Step;  
    }     
    
        /* write baud value */
    A_IX_UART_REG_WRITE(pUartDevice,UART_CLKDIV_ADDRESS, 
                      (UART_CLKDIV_CLK_SCALE_SET(scale) | UART_CLKDIV_CLK_STEP_SET(step)));
    
}

#if 1 //#ifdef RTOS_TX_BLOCKING
void trigger_dsr1(void)
{
    unsigned long v;
#if 0
    __asm__ __volatile__("movi %0, 1<<QCA_DSR_INTNUM\n\t" "wsr %0, INTSET\n\t"::"a"(v)
            );
#endif
    __asm__ __volatile__("movi %0, 1\n\t" "wsr %0, INTSET\n\t"::"a"(v));
}
#endif

void UartRxFifo(UART_DEVICE *pUartDevice)
{
    A_UINT32 data;
    A_UINT8  byte;

    while (TRUE) {
        /* read the FIFO port, this will not pop the actual byte out of the FIFO */    
        data = A_IX_UART_REG_READ(pUartDevice,UART_DATA_ADDRESS);        
        /* check if the byte is valid */
        if (!(data & UART_DATA_RX_CSR_MASK)) {
            break;    
        }
        
        byte = (A_UINT8)(data & 0xff);
        /* advance the RX FIFO */
        data = UART_DATA_RX_CSR_SET(1);
        A_IX_UART_REG_WRITE(pUartDevice,UART_DATA_ADDRESS, data);
        if (pUartDevice->BufferLength < UART_RX_BUFFER_SIZE) {
            pUartDevice->Buffer[pUartDevice->BufferLength++] = byte;
        } else {
            UART_STAT_INCR(pUartDevice,RxDropped,1); 
       }
    }
}

static A_UINT32 UartDevIsrHandler(void *unused)
{
    UART_DEVICE *pUartDevice = (UART_DEVICE *)unused;
    
    /* all work is deferred to the DSR */
    #if 1 //#ifdef RTOS_TX_BLOCKING
    trigger_dsr1();
    #endif
	
    {
    A_UINT32 interrupts;
    A_UINT32 enable_interrupts = A_IX_UART_REG_READ(pUartDevice,UART_INT_EN_ADDRESS);
    interrupts = A_IX_UART_REG_READ(pUartDevice,UART_INT_ADDRESS);    
    DisableUartInterrupts(pUartDevice, ~UART_INT_RX_FULL_INT_MASK);

    if (interrupts & UART_INT_RX_FULL_INT_MASK)  {
        UartRxFifo(pUartDevice);
        UART_STAT_INCR(pUartDevice,RxFullCount,1); 
        A_IX_UART_REG_WRITE(pUartDevice,UART_INT_ADDRESS,UART_INT_RX_FULL_INT_MASK);
    }

    if (!pUartDevice->RxIsr) {
        pUartDevice->RxIsr = TRUE;
        pUartDevice->EnableInterrupts = enable_interrupts;
    }
    }
    return A_ISR_CALL_DSR;
}

USAPI void _CatchTxOverFlowError(UART_DEVICE *pUartDevice)
{
    A_ASSERT(FALSE);    
}

USAPI void _CatchRxOverFlowError(UART_DEVICE *pUartDevice)
{
    /* TODO */
    
}

USAPI int _UartDevDsrHandler(void *data)
{
    UART_DEVICE *pUartDevice = (UART_DEVICE *)data;
    A_UINT32 interrupts;
    
    while (TRUE) {
    
        interrupts = A_IX_UART_REG_READ(pUartDevice,UART_INT_ADDRESS);    
        /* check only those enabled */
        if (!pUartDevice->Polling)
            interrupts &= pUartDevice->EnableInterrupts;
        else
            interrupts &= A_IX_UART_REG_READ(pUartDevice,UART_INT_EN_ADDRESS);
                
        if (!interrupts) {
            break;    
        }
        
        if (interrupts & UART_INT_RX_VALID_INT_MASK) {
            A_BOOL wakeup = FALSE;
            A_UINT32 length;
            UART_STAT_INCR(pUartDevice,RxIrqs,1); 
            if (!pUartDevice->Polling) {
                DisableUartInterrupts(pUartDevice, UART_INT_RX_FULL_INT_MASK);
                UartRxFifo(pUartDevice);
                length = pUartDevice->BufferLength;
                if (length) {
                    A_UINT32 bytesPushed = 0;
                    
                    bytesPushed = SERHWAPI_PushRXBytes_Push(pUartDevice->hPortDev, pUartDevice->Buffer, length);
                    
                    UART_STAT_INCR(pUartDevice,RxDropped,(length - bytesPushed)); 
                    pUartDevice->BufferLength = 0; 

                    if (bytesPushed) {
                        wakeup = TRUE;
                        UART_STAT_INCR(pUartDevice,RxBytes,bytesPushed);
                    }
                }
                
                EnableUartInterrupts(pUartDevice, UART_INT_RX_FULL_INT_MASK);
                if (wakeup) {
                    SERHWAPI_PushRXBytes_Notify(pUartDevice->hPortDev);
                }
            } else { 
               UartReceive(pUartDevice);
            }
#ifndef UARTCB_SUPPORT
            extern void patch_uart_wakeup_select(char* name);
            if (uart_init) {
			    //A_PRINTF("%s  dsr\n",(char* )pUartDevice->SerialDev.pPortName);
                patch_uart_wakeup_select((char* )pUartDevice->SerialDev.pPortName);
			}
#endif			
        }
        
        if (interrupts & UART_INT_TX_EMPTY_INT_MASK) {
            UART_STAT_INCR(pUartDevice,TxIrqs,1); 
            UartTryTransmit(pUartDevice); 
        }
        
        if (interrupts & UART_INT_RX_OFLOW_ERR_INT_MASK) {
            UART_STAT_INCR(pUartDevice,RxOverFlowErrors,1); 
            CatchRxOverFlowError(pUartDevice);  
        }
        
        if (interrupts & UART_INT_RX_FRAMING_ERR_INT_MASK) {
            /* TODO */   
            UART_STAT_INCR(pUartDevice,RxFramingErrors,1);
        }
        
        if (interrupts & UART_INT_RX_PARITY_ERR_INT_MASK) {
            /* TODO */
            UART_STAT_INCR(pUartDevice,RxParityErrors,1); 
        }
        
        if (interrupts & UART_INT_TX_OFLOW_ERR_INT_MASK) {
                /* this should never happen since software always checks if the FIFO
                 * has room for a byte */
            CatchTxOverFlowError(pUartDevice);
        }

        /* According to the HW datasheet:
           1.RX_VALID interrupt can be cleared if there is no more data in the receive FIFO. 
           2.TX_READY interrupt, setting this bit will clear this interrupt
           if there is room for more data in the transmit FIFO. 
        */
        /* ack interrupts */
        A_IX_UART_REG_WRITE(pUartDevice,UART_INT_ADDRESS,interrupts); 
        (void)A_IX_UART_REG_READ(pUartDevice,UART_INT_ADDRESS); 
        
    }

    if (!pUartDevice->Polling) {
        DisableUartInterrupts(pUartDevice, UART_INT_RX_FULL_INT_MASK);
        pUartDevice->RxIsr = FALSE;
        EnableUartInterrupts(pUartDevice, (pUartDevice->EnableInterrupts | UART_INT_RX_FULL_INT_MASK));
    }  
    
    return A_HANDLER_DONE; 
}

USAPI void _UartDevPortPinsEnableDisable(UART_DEVICE *pUartDevice, A_BOOL Enable)
{
#if 0
    int i;
    
    if (Enable) {
        /* disable pin hold circuits and reconfigure GPIOs */
        for (i = 0; i < UART_HW_PIN_MAX; i++) {
            if (pUartDevice->HwPins.PinList[i].GpioPin > 0) {
                gpio_configure_pin(pUartDevice->HwPins.PinList[i].GpioPin, 
                                   pUartDevice->HwPins.PinList[i].ConfigValue, 
                                   pUartDevice->PinIsInput[i],
                                   GPIO_PIN_PULLNONE);
            }              
        }               
    } else {

        /* when port is disabled, we pulldown all pins and set them as inputs */               
        for (i = 0; i < UART_HW_PIN_MAX; i++) {
            if (pUartDevice->HwPins.PinList[i].GpioPin > 0) {
                if (i == UART_HW_TX) {
                    /* TODO-FIX ME, there is an issue in the FPGA where converting 
                     * the TX output as a GPIO input takes down debug UART as well */
                } else {
                    gpio_configure_pin(pUartDevice->HwPins.PinList[i].GpioPin, 
                                       pUartDevice->HwPins.PinList[i].ConfigAsGpio, /* revert to gpio */
                                       TRUE,  /* configure as an input */
                                       GPIO_PIN_PULLDOWN);
                }
            }              
        }         
    }
#endif

#if 1 /* QCA4004 GPIO Power Optimization.docx */

    int uart_id = pUartDevice->InstanceId;
    A_UINT32    peripheral_id = 0;

    peripheral_id = (uart_id == UART_SERP_UART0_INSTANCE_ID) ?
                            QCOM_PERIPHERAL_ID_UART_0 : QCOM_PERIPHERAL_ID_UART_1;
    if (A_OK != qcom_gpio_apply_peripheral_configuration( peripheral_id,
                                                (Enable) ? FALSE : TRUE))
    {
        A_UINT32    peripheral_map = 0;
        A_STATUS    status = A_OK;

        status = qcom_gpio_peripheral_pin_conflict_check(peripheral_id, &peripheral_map);
        A_PRINTF("UART %d failed on %s,  map %x, status %d\n", uart_id, (Enable) ? "Enable" : "Disable", 
                                                        peripheral_map, status);
        return;
    }
#endif

}

USAPI void _UartDevShutDownUart(UART_DEVICE *pUartDevice)
{   
    if (pUartDevice->Polling) {
        StartStopInterruptPolling(pUartDevice,FALSE);    
    }
    DisableUartInterrupts(pUartDevice,ALL_UART_INTS); 
    UartDevPortPinsEnableDisable(pUartDevice,FALSE);  
}

USAPI void _UartDevSetupUart(UART_DEVICE *pUartDevice, SER_PORT_UART_CONFIG *pConfig)
{
    A_UINT32 regVal,baud;

        /* reset UART block */
    regVal = A_RTC_SOC_REG_READ(SOC_RESET_CONTROL_ADDRESS);
    if(pUartDevice->BaseAddress == WLAN_UART_BASE_ADDRESS) {
        regVal |= RESET_CONTROL_UART_RST_SET(1);
    }
    else if(pUartDevice->BaseAddress ==WLAN_DBG_UART_BASE_ADDRESS)    {
        regVal |= RESET_CONTROL_DEBUG_UART_RST_SET(1);
    }
    else{
        A_ASSERT(FALSE);
    }
    A_RTC_SOC_REG_WRITE(SOC_RESET_CONTROL_ADDRESS,regVal);
    if(pUartDevice->BaseAddress == WLAN_UART_BASE_ADDRESS) {
        regVal &= ~RESET_CONTROL_UART_RST_MASK;
    }
    else if(pUartDevice->BaseAddress ==WLAN_DBG_UART_BASE_ADDRESS)    {
        regVal &= ~RESET_CONTROL_DEBUG_UART_RST_MASK;
    }
    else{
        A_ASSERT(FALSE);
    }
    A_RTC_SOC_REG_WRITE(SOC_RESET_CONTROL_ADDRESS,regVal);

        /* Disable interrupts */
    DisableUartInterrupts(pUartDevice,ALL_UART_INTS);   
    
        /* we can enable the port pins now */
    UartDevPortPinsEnableDisable(pUartDevice,TRUE);
    
        /* enable the interface, set for default DTE operation */
    regVal = UART_CONTROL_IFC_ENABLE_SET(1) | UART_CONTROL_HOST_INT_ENABLE_SET(1);
    
    if (pConfig->ConfigFlags & SER_PORT_CONFIG_RTS_CTS_FLOW) {
            /* flow control enabled */
        A_ASSERT(pUartDevice->HwPins.PinList[UART_HW_CTS].GpioPin >= 0);
        A_ASSERT(pUartDevice->HwPins.PinList[UART_HW_RTS].GpioPin >= 0);
        regVal |= UART_CONTROL_FLOW_ENABLE_SET(1);
    } else {
        regVal |= UART_CONTROL_RX_READY_ORIDE_SET(1);
        regVal |= UART_CONTROL_TX_READY_ORIDE_SET(1);     
    }
    
    if (pConfig->ConfigFlags & SER_PORT_CONFIG_PARITY_CHECK) {
        regVal |= UART_CONTROL_PARITY_ENABLE_SET(1);
        if (!(pConfig->ConfigFlags & SER_PORT_CONFIG_ODD_PARITY)) {
                /* even parity */
            regVal |= UART_CONTROL_PARITY_EVEN_SET(1);    
        }    
    }
   
    A_IX_UART_REG_WRITE(pUartDevice,UART_CONTROL_ADDRESS, regVal); 

    baud = pConfig->Baud;
    if (0 == baud) {
            /* caller will set a more precise setting for the baud rate, for now
             * just use a default */
        baud = DEFAULT_UART_BAUD_RATE; 
    }
    
    UartDevSetBaud(pUartDevice,baud,0,0);  
        
        /* enable interrupts */
    EnableUartInterrupts(pUartDevice,
                         UART_INT_EN_TX_OFLOW_ERR_INT_EN_MASK   | 
                         UART_INT_EN_RX_FRAMING_ERR_INT_EN_MASK | 
                         UART_INT_EN_RX_OFLOW_ERR_INT_EN_MASK);  

    if (pConfig->ConfigFlags & SER_PORT_CONFIG_PARITY_CHECK) {
        EnableUartInterrupts(pUartDevice,UART_INT_RX_PARITY_ERR_INT_MASK);    
    }
    
    UART_RESET_STATS(pUartDevice);
    
    EnableUartInterrupts(pUartDevice,UART_INT_EN_RX_VALID_INT_EN_MASK | UART_INT_EN_RX_FULL_INT_EN_MASK); 
    
#if 0   //karl
    if (pUartDevice->Polling) {
        StartStopInterruptPolling(pUartDevice,TRUE);
    }       
#endif    
}

void UartConfigureRxPolling(int id, int en)
{
    UART_DEVICE * pUartDevice = g_pUartDevices[id];
    if (pUartDevice->Polling) {
        StartStopInterruptPolling(pUartDevice,en);
    }       
}

USAPI void _UartConfigureRxWake(UART_DEVICE *pUartDevice, A_BOOL Enable)
{

    if (Enable) {
            /* switch RX as a GPIO input */
        /* gpio_configure_pin(pUartDevice->HwPins.PinList[UART_HW_RX].GpioPin,
                           pUartDevice->HwPins.PinList[UART_HW_RX].ConfigAsGpio,
                           GPIO_PIN_DIR_INPUT,
                           GPIO_PIN_PULLNONE); */
        if (A_OK != gpio_register_pin_interrupt(&pUartDevice->RxPinInterrupt))
        {
            A_PRINTF("Unable to register UART RX Interrupt\n");
            return;
        }
        gpio_set_interrupt_mode(&pUartDevice->RxPinInterrupt,GPIO_PIN_INT_FALLING_EDGE);
        gpio_arm_interrupt(&pUartDevice->RxPinInterrupt);
        if (pUartDevice->Polling) {
                /* stop any polling */
            StartStopInterruptPolling(pUartDevice,FALSE);
        } else {
                /* disable RX interrupts */
            DisableUartInterrupts(pUartDevice,UART_INT_EN_RX_VALID_INT_EN_MASK);
        } 
    } else {
        gpio_deregister_pin_interrupt(&pUartDevice->RxPinInterrupt);
            /* return RX pin back to normal */
        /* gpio_configure_pin(pUartDevice->HwPins.PinList[UART_HW_RX].GpioPin,
                           pUartDevice->HwPins.PinList[UART_HW_RX].ConfigValue,
                           GPIO_PIN_DIR_INPUT,
                           GPIO_PIN_PULLNONE); */
        if (pUartDevice->Polling) {
                /* start up polling */
            StartStopInterruptPolling(pUartDevice,TRUE);
        } else {
                /* re-enable RX interrupts */
            EnableUartInterrupts(pUartDevice,UART_INT_EN_RX_VALID_INT_EN_MASK);
        }             
    }
    
}

    /* direct write without using interrupts, this is to accommodate console operation 
     * that needs to allow exception printing through the current console port */
static void UartDeviceDirectByteWrite(UART_DEVICE *pUartDevice, A_UINT8 *pBuffer, int Count)
{
    int      i;
    A_UINT32 data;
        
    for (i = 0; i < Count; i++) {
        while (TRUE) {
            data = A_IX_UART_REG_READ(pUartDevice,UART_DATA_ADDRESS);
            if (data & UART_DATA_TX_CSR_MASK) {
                break;
            }
        } 

        data = *pBuffer | UART_DATA_TX_CSR_SET(1);
        A_IX_UART_REG_WRITE(pUartDevice,UART_DATA_ADDRESS,data);
        
        pBuffer++;
        
        while (TRUE) {
            data = A_IX_UART_REG_READ(pUartDevice,UART_CONTROL_ADDRESS);
            if (!(data & UART_CONTROL_TX_BUSY_MASK)) {
                break;
            }
        }
    }    
        
}
    
    void testUart1(char *str)
    {
      //static int dbg='A';
      UartDeviceDirectByteWrite(g_pUartDevices[0],(A_UINT8 *)str,3);
      UartDeviceDirectByteWrite(g_pUartDevices[1],(A_UINT8 *)str,3);
    }


static A_STATUS UartDevIoctl(void       *pPortContext, 
                             A_UINT16   Code, 
                             void       *pBuffer,
                             int        BufferLengthOrArg)
{
    UART_DEVICE *pUartDevice = (UART_DEVICE *)pPortContext;
    A_STATUS status = A_OK;
    
    switch (Code) {
        case SER_PORT_IOCTL_FLUSH :
        
            if ((BufferLengthOrArg == SER_PORT_FLUSH_ALL) ||
                (BufferLengthOrArg == SER_PORT_FLUSH_RX)) {
                    
            } 
                
            /* TODO: flush pending TX? */
            break;  
        case SER_PORT_IOCTL_CONFIG_UART_PORT :
            {
                SER_PORT_UART_CONFIG *pConfig = (SER_PORT_UART_CONFIG *)pBuffer;
                if ((pConfig->DataBits != 8) || (pConfig->StopBits != 1)) {
                    status = A_ENOTSUP;
                    break;    
                }
                UartDevSetupUart(pUartDevice,pConfig);
            }
            break;   
        case SER_PORT_HW_IOCTL_DIRECT_BAUD :
            UartDevSetBaud(pUartDevice,
                           0, 
                           ((SER_PORT_HW_BAUD_CONFIG *)pBuffer)->Scale,
                           ((SER_PORT_HW_BAUD_CONFIG *)pBuffer)->Step);
            break;
        case SER_PORT_HW_IOCTL_STOP :
            UartDevShutDownUart(pUartDevice);
            break;
        case SER_PORT_HW_IOCTL_SET_POLL_INTERVAL :
            pUartDevice->PollTimeoutMS = (A_UINT32)BufferLengthOrArg;
            break;
        case SER_PORT_HW_IOCTL_CONFIG_RX_WAKEUP :
            if (BufferLengthOrArg) {
                UartConfigureRxWake(pUartDevice,TRUE);   
            } else {
                UartConfigureRxWake(pUartDevice,FALSE);         
            }
            break;
        case SER_PORT_HW_IOCTL_WRITE_BYTES_DIRECT :
            UartDeviceDirectByteWrite(pUartDevice,(A_UINT8 *)pBuffer,(int)BufferLengthOrArg);
            break;
        default:
            break;    
    }
    
    return status;   
}

struct cmnos_intr_service_s {
    A_handler_t dsr;            /* Delayed Service Routine */
    void       *dsr_arg;

    A_isr_t     isr;            /* Interrupt Service Routine */
    void       *isr_arg;
    A_UINT32    maskoff;        /* Which intrs to mask after ISR runs and before DSR completes */
};
extern struct cmnos_intr_service_s cmnos_intr_service[NUM_DIRECT_INTR]; 
static void UartDev_Start(void *pInstance, UART_HW_CONFIG *pConfig)
{
    UART_DEVICE         *pUartDevice;
    UART_HW_PIN_CONFIG  *pDefConfig;
    
    A_ASSERT(pInstance != NULL);
    
    pUartDevice = (UART_DEVICE *)pInstance; 
    
    pDefConfig = pUartDevice->pDefPinConfigTable;
    
    A_MEMCPY(&pUartDevice->HwPins,pConfig,sizeof(UART_HW_CONFIG));
    
    if (pUartDevice->HwPins.PinList[UART_HW_TX].GpioPin == 0) {
        pUartDevice->HwPins.PinList[UART_HW_TX] = pDefConfig[UART_HW_TX];
    }
    
    if (pUartDevice->HwPins.PinList[UART_HW_RX].GpioPin == 0) {
        pUartDevice->HwPins.PinList[UART_HW_RX] = pDefConfig[UART_HW_RX];
    }
    
    if (pUartDevice->HwPins.PinList[UART_HW_CTS].GpioPin == 0) {
        pUartDevice->HwPins.PinList[UART_HW_CTS] = pDefConfig[UART_HW_CTS];
    }
    
    if (pUartDevice->HwPins.PinList[UART_HW_RTS].GpioPin == 0) {
        pUartDevice->HwPins.PinList[UART_HW_RTS] = pDefConfig[UART_HW_RTS];
    }
  
        /* set directions */
    pUartDevice->PinIsInput[UART_HW_RX] = TRUE;
    pUartDevice->PinIsInput[UART_HW_CTS] = TRUE;
    pUartDevice->PinIsInput[UART_HW_TX] = FALSE;
    pUartDevice->PinIsInput[UART_HW_RTS] = FALSE;
    
    if (pConfig->pPortName != NULL) {
            /* user caller supplied name instead */
        pUartDevice->SerialDev.pPortName = pConfig->pPortName;   
    }
        
        /* disable port pins for now to prevent floating inputs */
    UartDevPortPinsEnableDisable(pUartDevice,FALSE);
    
        /* setup the serial device */                                     
    pUartDevice->SerialDev.Properties.Flags = SER_PORT_PROP_FLAGS_RX_NO_DROP;
        /* the serial layer provides ring buffers for us */     
    if (pConfig->TxBufferSize != 0) {                                                                                          
        pUartDevice->SerialDev.Properties.TXBufferSize = pConfig->TxBufferSize;
    } else {
        pUartDevice->SerialDev.Properties.TXBufferSize = SERIAL_LAYER_RING_BUFFER_SIZE;    
    }
    
    if (pConfig->RxBufferSize != 0) { 
        pUartDevice->SerialDev.Properties.RXBufferSize = pConfig->RxBufferSize;
    } else {
        pUartDevice->SerialDev.Properties.RXBufferSize = SERIAL_LAYER_RING_BUFFER_SIZE;    
    }
    
    pUartDevice->BufferLength = 0;
    pUartDevice->RxIsr = FALSE;
    pUartDevice->SerialDev.HWTransmit = UartDevHWTransmit;
    pUartDevice->SerialDev.DrainRX = UartDevDrainRX;
    pUartDevice->SerialDev.Ioctl = UartDevIoctl;
    pUartDevice->SerialDev.pPortContext = pUartDevice;
    
    DisableUartInterrupts(pUartDevice,ALL_UART_INTS);

    switch (pUartDevice->InstanceId) {
        case UART_SERP_UART0_INSTANCE_ID:
                pUartDevice->Polling = FALSE;
            /* setup dsr service routine */
            A_WUART_INTR_ATTACH(GET_UART_SERP_INDIR_FN(_UartDevDsrHandler), pUartDevice);
            /* register ISR handler */
            A_WUART_ISR_ATTACH(UartDevIsrHandler, pUartDevice);
            cmnos_intr_service[A_INUM_HCI_UART].maskoff |= 1 << A_INUM_HCI_UART;
            A_WUART_INTR_UNMASK();
            break;
        case UART_SERP_UART1_INSTANCE_ID:
            pUartDevice->Polling = TRUE;
            break;
#if defined(AR6002_REV6) || defined(AR6002_REV7)
        case UART_SERP_UART2_INSTANCE_ID :
            pUartDevice->Polling = TRUE;
            pUartDevice->BaseAddress = WLAN_UART2_BASE_ADDRESS;      
            break;  
#endif              
        default:
            A_ASSERT(FALSE);
            break;    
        
    }

    if (pUartDevice->Polling) {
            /* setup polling timer */
        A_INIT_TIMER(&pUartDevice->PollingTimer,
                     GET_UART_SERP_INDIR_FN(_InterruptPollCallback),
                     pUartDevice);  
        A_TIMER_SET_FLAGS(&pUartDevice->PollingTimer,TIMER_FLAGS_NO_FORCE_DISARM);
        pUartDevice->PollTimeoutMS = INT_POLL_INTERVAL_MS;
    }

    pUartDevice->RxPinInterrupt.pin = pUartDevice->HwPins.PinList[UART_HW_RX].GpioPin;
    pUartDevice->RxPinInterrupt.gpio_pin_int_handler_fn = GET_UART_SERP_INDIR_FN(_UartRxPinInterruptHandler);
    pUartDevice->RxPinInterrupt.arg = pUartDevice;
    
        /* register the port */
    pUartDevice->hPortDev = SERPHW_RegisterPort(&pUartDevice->SerialDev);
    A_ASSERT(pUartDevice->hPortDev != NULL);

}

USAPI void _UartRxPinInterruptHandler(void *data)
{  
    UART_DEVICE  *pUartDevice = (UART_DEVICE *)data;
    
        /* disable UART wake */
    UartConfigureRxWake(pUartDevice,FALSE);   
        /* notify application, application will decide to re-enable RX wake */
    SERPHW_IndicatePortEvent(pUartDevice->hPortDev,SERIAL_EVENT_RX_WAKE,0,0);
}


static void *UartDev_Init(int InstanceId)
{
    UART_DEVICE *pUartDevice;
    
    pUartDevice = (UART_DEVICE *)A_ALLOCRAM(sizeof(UART_DEVICE));
    pUartDevice->InstanceId = InstanceId;
    
    switch (InstanceId) {   
        case UART_SERP_UART0_INSTANCE_ID :
            pUartDevice->BaseAddress = WLAN_UART_BASE_ADDRESS;
            pUartDevice->SerialDev.pPortName = (A_CHAR *)UART_PORT0_NAME;
            pUartDevice->pDefPinConfigTable = g_Uart0DefaultPins;
            break;         
        case UART_SERP_UART1_INSTANCE_ID :
            pUartDevice->BaseAddress = WLAN_DBG_UART_BASE_ADDRESS;
            pUartDevice->SerialDev.pPortName = (A_CHAR *)UART_PORT1_NAME;
            pUartDevice->pDefPinConfigTable = g_Uart1DefaultPins;
            break;
#if defined(AR6002_REV6) || defined(AR6002_REV7)
        case UART_SERP_UART2_INSTANCE_ID :
            pUartDevice->BaseAddress = WLAN_UART2_BASE_ADDRESS;
            pUartDevice->SerialDev.pPortName = (A_CHAR *)UART_PORT2_NAME;
            pUartDevice->pDefPinConfigTable = g_Uart2DefaultPins;
            break;
#endif            
        default:
            A_ASSERT(FALSE);
            break;    
        
    }
    
//    A_ASSERT(g_pUartDevices[InstanceId] == NULL);
    g_pUartDevices[InstanceId] = pUartDevice;
    
    return (void *)pUartDevice;  
}

void UARTSERP_HW_module_install(struct uart_serphw_apis *pAPIs)
{
    pAPIs->_Init = UartDev_Init;
    pAPIs->_Start = UartDev_Start;  
    pAPIs->pReserved = &g_pUartDevices; 
}

UART_SERP_INTERNAL_APIS g_UartSerpIndirectionTable = {
    _StartStopInterruptPolling,
    _InterruptPollCallback,
    _UartTryTransmit,
    _UartReceive,
    _UartDevSetBaud,
    _CatchTxOverFlowError,
    _CatchRxOverFlowError,
    _UartDevDsrHandler,
    _UartDevPortPinsEnableDisable,
    _UartDevShutDownUart,
    _UartDevSetupUart,
    _UartRxPinInterruptHandler,
    _UartConfigureRxWake
};



