/****************************************************************************
 *   $Id:: uart.h 5751 2010-11-30 23:56:11Z usb00423                        $
 *   Project: NXP LPC17xx UART example
 *
 *   Description:
 *     This file contains UART code header definition.
 *
****************************************************************************/
#ifndef __UART_H 
#define __UART_H

#if defined __cplusplus
extern "C" {
#endif

#define UART_BUFSIZE		0x200

#define IER_RBR		0x01
#define IER_THRE	0x02
#define IER_RLS		0x04

#define IIR_PEND	0x01
#define IIR_RLS		0x03
#define IIR_RDA		0x02
#define IIR_CTI		0x06
#define IIR_THRE	0x01

#define LSR_RDR		0x01
#define LSR_OE		0x02
#define LSR_PE		0x04
#define LSR_FE		0x08
#define LSR_BI		0x10
#define LSR_THRE	0x20
#define LSR_TEMT	0x40
#define LSR_RXFE	0x80


void UART_printf(char port, char *arg_list, ...);

int UARTnInit(int nport, int baudrate );
void UART0_IRQHandler( void );
void UART1_IRQHandler( void );
void UARTSend( uint32_t portNum, uint8_t *BufferPtr, uint32_t Length );
void UARTNSend(int nport, char *msg, int n);


extern int AddUART_RX_to_buffer(int nport, char byte);
extern int AddUART_TX_to_buffer(int nport, char byte);
extern int UART_RX_bufferLength(int nport);
extern int UART_Get_buffer(int nport);
extern void UART_Init_buffer(int nport);
extern void * UART_Get_bIn(int nport);

#ifdef __cplusplus
}
#endif

#endif /* end __UART_H */
/*****************************************************************************
**                            End Of File
******************************************************************************/
