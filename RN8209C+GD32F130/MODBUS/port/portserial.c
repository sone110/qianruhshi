/*
 * FreeModbus Libary: MSP430 Port
 * Copyright (C) 2006 Christian Walter <wolti@sil.at>
 *
 * This library is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Lesser General Public
 * License as published by the Free Software Foundation; either
 * version 2.1 of the License, or (at your option) any later version.
 *
 * This library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public
 * License along with this library; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA  02110-1301  USA
 *
 * File: $Id: portserial.c,v 1.3 2006/11/19 03:57:49 wolti Exp $
 */

/* ----------------------- Platform includes --------------------------------*/
#include "port.h"

/* ----------------------- Modbus includes ----------------------------------*/
#include "mb.h"
#include "mbport.h"

#include "RS485.h"
/* ----------------------- Static variables ---------------------------------*/
UCHAR           ucGIEWasEnabled = FALSE;
UCHAR           ucCriticalNesting = 0x00;

/* ----------------------- Start implementation -----------------------------*/
void
vMBPortSerialEnable( BOOL xRxEnable, BOOL xTxEnable )
{
    ENTER_CRITICAL_SECTION(  );
    if( xRxEnable )
    {
			RS485_RX_EN;
			usart_receive_config(USART0, USART_RECEIVE_ENABLE);
			usart_interrupt_disable(USART0,USART_INT_TBE);
			
    }
    else
    {
			RS485_TX_EN;
			usart_receive_config(USART0, USART_RECEIVE_DISABLE);
			
    }
    if( xTxEnable )
    {
			RS485_TX_EN;
			usart_interrupt_enable(USART0,USART_INT_TBE);
			usart_transmit_config(USART0, USART_TRANSMIT_ENABLE);
			
    }
    else
    {
			RS485_RX_EN;
			usart_interrupt_disable(USART0,USART_INT_TBE);
			usart_transmit_config(USART0, USART_TRANSMIT_DISABLE);
			
    }
    EXIT_CRITICAL_SECTION(  );
}

BOOL
xMBPortSerialInit( UCHAR ucPort, ULONG ulBaudRate, UCHAR ucDataBits, eMBParity eParity )
{
	BOOL    bInitialized = TRUE;
	if( bInitialized )
	{
		ENTER_CRITICAL_SECTION(  );
		RS485_init(ulBaudRate);
		EXIT_CRITICAL_SECTION(  );
	}
	return bInitialized;
}

BOOL
xMBPortSerialPutByte( CHAR ucByte )
{
	usart_data_transmit(USART0, ucByte);
	while(usart_flag_get(USART0, USART_FLAG_TC) == RESET) ;	   //等待发送完成
    return TRUE;
}

BOOL
xMBPortSerialGetByte( CHAR * pucByte )
{
	//USART_ClearFlag(USART1, USART_IT_RXNE) ;				 //中断接收
//	while(USART_GetFlagStatus(USART1, USART_IT_RXNE) == 0);  //等待接收标志	
    *pucByte = (uint8_t)usart_data_receive(USART0);	//获取接收BUFFER的数据
    return TRUE;
}

//void USART0_IRQHandler(void)
//{

//	if(usart_flag_get(USART1,USART_FLAG_TBE))
//	{
//	    pxMBFrameCBTransmitterEmpty(  );
//	}
//	else if(usart_flag_get(USART1,USART_FLAG_RBNE))
//	{
//		 pxMBFrameCBByteReceived(  );
//	}
//}

/* 
 * Create an interrupt handler for the transmit buffer empty interrupt
 * (or an equivalent) for your target processor. This function should then
 * call pxMBFrameCBTransmitterEmpty( ) which tells the protocol stack that
 * a new character can be sent. The protocol stack will then call 
 * xMBPortSerialPutByte( ) to send the character.
 */
void prvvUARTTxReadyISR(void)
{
    pxMBFrameCBTransmitterEmpty();
}

/* 
 * Create an interrupt handler for the receive interrupt for your target
 * processor. This function should then call pxMBFrameCBByteReceived( ). The
 * protocol stack will then call xMBPortSerialGetByte( ) to retrieve the
 * character.
 */
void prvvUARTRxISR(void)
{
    pxMBFrameCBByteReceived();
}


void
EnterCriticalSection( void )
{
  __disable_irq();
}

void
ExitCriticalSection( void )
{
  __enable_irq();
}
