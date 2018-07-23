/*!
    \file  main.c
    \brief led spark with systick 
*/

/*
    Copyright (C) 2017 GigaDevice

    2014-12-26, V1.0.0, platform GD32F1x0(x=3,5)
    2016-01-15, V2.0.0, platform GD32F1x0(x=3,5,7,9)
    2016-04-30, V3.0.0, firmware update for GD32F1x0(x=3,5,7,9)
    2017-06-19, V3.1.0, firmware update for GD32F1x0(x=3,5,7,9)
*/

#include "gd32f1x0.h"
#include "systick.h"
#include <stdio.h>
#include "main.h"
#include "gd32f1x0_eval.h"
#include "RS485.h"
#include "usart.h"
#include "RN8209D.h"


/* ----------------------- Modbus includes ----------------------------------*/
#include "mb.h"
#include "mbport.h"
#include "mbutils.h"
/* ----------------------- Defines ------------------------------------------*/
#define REG_COILS_START  0
#define REG_COILS_NREGS  4
#define REG_INPUT_START 0
#define REG_INPUT_NREGS 255
#define REG_HOLDING_START 	0
#define REG_HOLDING_NREGS 	255
/* ----------------------- Static variables ---------------------------------*/
static USHORT   usRegCoilsStart = REG_COILS_START;
static UCHAR    usRegCoilsData = 0x00;            //定义8个线圈，对应1个字节
static UCHAR *   usRegCoilsPoint = &usRegCoilsData;
static USHORT   usRegInputStart = REG_INPUT_START;
static USHORT   usRegInputBuf[REG_INPUT_NREGS]={0x147b,0x3f8e,0x147b,0x400e,0x1eb8,0x4055,0x147b,0x408e,1255,2555,1238};
static USHORT   usRegHoldingStart = REG_HOLDING_START;
static USHORT   usRegHoldingBuf[REG_HOLDING_NREGS]={0x147b,0x3f8e,0x147b,0x400e,0x1eb8,0x4055,0x147b,0x408e,1234,5678};

USHORT   usSCoilStart                                 = REG_COILS_START;
#if REG_COILS_NREGS%8     //判断是否为8的整数倍
UCHAR    ucSCoilBuf[REG_COILS_NREGS/8+1] ={0x00}               ;
#else
UCHAR    ucSCoilBuf[REG_COILS_NREGS/8]                   ;
#endif

void Relay_init()
{
	  rcu_periph_clock_enable(RCU_GPIOA);
	  gpio_mode_set(GPIOA , GPIO_MODE_OUTPUT, GPIO_PUPD_PULLUP,GPIO_PIN_4);
    gpio_output_options_set(GPIOA, GPIO_OTYPE_PP, GPIO_OSPEED_50MHZ,GPIO_PIN_4);
}

int main(void)
{
	  uint32_t data = 0 , frequent=0,ia=0  ,ADC_U=0,powera=0,id =0;
	  SystemInit();
    systick_config();
	  Relay_init();
	  RN8209_Init();
	  
	  nvic_priority_group_set(NVIC_PRIGROUP_PRE1_SUB3);
    nvic_irq_enable(USART0_IRQn, 1, 3); 
		eMBInit(MB_RTU, 0x01, 0, 9600, MB_PAR_NONE);
	  eMBEnable();

    while (1)
    {	
        eMBPoll();
			  gpio_bit_write(GPIOA , GPIO_PIN_4 ,  ucSCoilBuf[0]);  
        RN8209D_ReadData(ADURMS ,(uint8_t* )&ADC_U ,3);
        usRegHoldingBuf[0] = (USHORT)ADC_U ;
        
        delay_1ms(200);			
    }
        
}





/****************************************************************************
* 名	  称：eMBRegInputCB 
* 功    能：读取输入寄存器，对应功能码是 04 eMBFuncReadInputRegister
* 入口参数：pucRegBuffer: 数据缓存区，用于响应主机   
*						usAddress: 寄存器地址
*						usNRegs: 要读取的寄存器个数
* 出口参数：
* 注	  意：上位机发来的 帧格式是: SlaveAddr(1 Byte)+FuncCode(1 Byte)
*								+StartAddrHiByte(1 Byte)+StartAddrLoByte(1 Byte)
*								+LenAddrHiByte(1 Byte)+LenAddrLoByte(1 Byte)+
*								+CRCAddrHiByte(1 Byte)+CRCAddrLoByte(1 Byte)
*							3 区
****************************************************************************/
eMBErrorCode
eMBRegInputCB( UCHAR * pucRegBuffer, USHORT usAddress, USHORT usNRegs )
{
    eMBErrorCode    eStatus = MB_ENOERR;
    int             iRegIndex;

	  usAddress--;
    if( ( usAddress >= REG_INPUT_START )
        && ( usAddress + usNRegs <= REG_INPUT_START + REG_INPUT_NREGS ) )
    {
        iRegIndex = ( int )( usAddress - usRegInputStart );
        while( usNRegs > 0 )
        {
            *pucRegBuffer++ = ( UCHAR )( usRegInputBuf[iRegIndex] >> 8 );
            *pucRegBuffer++ = ( UCHAR )( usRegInputBuf[iRegIndex] & 0xFF );
            iRegIndex++;
            usNRegs--;
        }
    }
    else
    {
        eStatus = MB_ENOREG;
    }

    return eStatus;
}

/****************************************************************************
* 名	  称：eMBRegHoldingCB 
* 功    能：对应功能码有：06 写保持寄存器 eMBFuncWriteHoldingRegister 
*													16 写多个保持寄存器 eMBFuncWriteMultipleHoldingRegister
*													03 读保持寄存器 eMBFuncReadHoldingRegister
*													23 读写多个保持寄存器 eMBFuncReadWriteMultipleHoldingRegister
* 入口参数：pucRegBuffer: 数据缓存区，用于响应主机   
*						usAddress: 寄存器地址
*						usNRegs: 要读写的寄存器个数
*						eMode: 功能码
* 出口参数：
* 注	  意：4 区
****************************************************************************/
eMBErrorCode
eMBRegHoldingCB( UCHAR * pucRegBuffer, USHORT usAddress, USHORT usNRegs, eMBRegisterMode eMode )
{
	eMBErrorCode    eStatus = MB_ENOERR;
	int             iRegIndex;

	usAddress--;
	if((usAddress >= REG_HOLDING_START)&&\
		((usAddress+usNRegs) <= (REG_HOLDING_START + REG_HOLDING_NREGS)))
	{
		iRegIndex = (int)(usAddress - usRegHoldingStart);
		switch(eMode)
		{                                       
			case MB_REG_READ://读 MB_REG_READ = 0
        while(usNRegs > 0)
				{
					*pucRegBuffer++ = (uint8_t)(usRegHoldingBuf[iRegIndex] >> 8);            
					*pucRegBuffer++ = (uint8_t)(usRegHoldingBuf[iRegIndex] & 0xFF); 
          iRegIndex++;
          usNRegs--;					
				}                            
        break;
			case MB_REG_WRITE://写 MB_REG_WRITE = 0
				while(usNRegs > 0)
				{         
					usRegHoldingBuf[iRegIndex] = *pucRegBuffer++ << 8;
          usRegHoldingBuf[iRegIndex] |= *pucRegBuffer++;
          iRegIndex++;
          usNRegs--;
        }				
			}
	}
	else//错误
	{
		eStatus = MB_ENOREG;
	}	
	
	return eStatus;
}

/****************************************************************************
* 名	  称：eMBRegCoilsCB 
* 功    能：对应功能码有：01 读线圈 eMBFuncReadCoils
*													05 写线圈 eMBFuncWriteCoil
*													15 写多个线圈 eMBFuncWriteMultipleCoils
* 入口参数：pucRegBuffer: 数据缓存区，用于响应主机   
*						usAddress: 线圈地址
*						usNCoils: 要读写的线圈个数
*						eMode: 功能码
* 出口参数：
* 注	  意：如继电器 
*						0 区
****************************************************************************/
eMBErrorCode
eMBRegCoilsCB( UCHAR * pucRegBuffer, USHORT usAddress, USHORT usNCoils, eMBRegisterMode eMode )
{
	 eMBErrorCode    eStatus = MB_ENOERR;
	 USHORT          iRegIndex , iRegBitIndex , iNReg;
   UCHAR *         pucCoilBuf;
	 pucCoilBuf = ucSCoilBuf ;
   iNReg =  usNCoils / 8 + 1;   //判断寄存器个数
   usAddress--;
	if((usAddress >= REG_COILS_START)&&\
		((usAddress+usNCoils) <= (REG_COILS_START + REG_COILS_NREGS)))
	{
			
		 iRegIndex = (USHORT) (usAddress - usRegCoilsStart) / 8;  //计算字节数
     iRegBitIndex = (USHORT) (usAddress - usRegCoilsStart) % 8;
		 		  
		switch(eMode)
		{                                       
			case MB_REG_READ://读 MB_REG_READ = 0
				
        while(iNReg > 0)
				{
					 *pucRegBuffer = pucCoilBuf[0];
					//*pucRegBuffer++ = xMBUtilGetBits(&pucCoilBuf[iRegIndex++],iRegBitIndex, 4);
           iNReg-- ;
           					
         			            					
				}   
        break;
				
				
				
			case MB_REG_WRITE://写 MB_REG_WRITE = 1
				while (iNReg > 0)
            {							
                iNReg--;
            }
            /* last coils */
            usNCoils = usNCoils % 8;
            /* xMBUtilSetBits has bug when ucNBits is zero */
            if (usNCoils != 0)
            {
                xMBUtilSetBits(&pucCoilBuf[iRegIndex++], iRegBitIndex, usNCoils,
                        *pucRegBuffer++);
            }
            break;
						
			}
	}
	else//错误
	{
		eStatus = MB_ENOREG;
	}	
	
	return eStatus;
}
/****************************************************************************
* 名	  称：eMBRegDiscreteCB 
* 功    能：读取离散寄存器，对应功能码有：02 读离散寄存器 eMBFuncReadDiscreteInputs
* 入口参数：pucRegBuffer: 数据缓存区，用于响应主机   
*						usAddress: 寄存器地址
*						usNDiscrete: 要读取的寄存器个数
* 出口参数：
* 注	  意：1 区
****************************************************************************/
eMBErrorCode
eMBRegDiscreteCB( UCHAR * pucRegBuffer, USHORT usAddress, USHORT usNDiscrete )
{
    ( void )pucRegBuffer;
    ( void )usAddress;
    ( void )usNDiscrete;
    return MB_ENOREG;
}





