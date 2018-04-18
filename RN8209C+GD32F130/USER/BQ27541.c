#include "BQ27541.h"
volatile INT32U  emulator_twi_delay = 0;
volatile INT32U  emulator_twi_ack_error = 0;
volatile INT32U  emulator_twi_i=0;
const INT8U emulator_twi_getbit[]={0x01,0x02,0x04,0x08,0x10,0x20,0x40,0x80};
unsigned char read[32] = {0,0,0,0,0,  0,0,0,0,0,   0,0,0,0,0,  0,0,0,0,0,   0,0,0,0,0,  0,0,0,0,0,  0,0 }; 
void Port_TWI_Init(void)
{
	IIC_INIT();
	SCLK_OUT();
	
	
	SCLK_HIGH();
	//SCLK_LOW();
	SDAT_OUT();
	SDAT_SET();
	//SDAT_CLR();
}
/* 启动IIC总线 */
void EMULATOR_TWI_START( void )
{
		emulator_twi_delay++;
		SDAT_SET();
		emulator_twi_delay++;
		SCLK_HIGH();
		emulator_twi_delay++;
		SDAT_CLR();
		emulator_twi_delay++;
		SCLK_LOW();
}
/* 关闭 IIC 总线 */
void EMULATOR_TWI_STOP( void )
{
		emulator_twi_delay++;
		SCLK_LOW();
		emulator_twi_delay++;
		SDAT_CLR();
		emulator_twi_delay++;
		SCLK_HIGH();
		emulator_twi_delay++;
		SDAT_SET();
		emulator_twi_delay++;
}

/* 发送一位数据：高 */
void EMULATOR_TWI_SEND_BIT_H( void )
{
		SDAT_SET();
		emulator_twi_delay++;
		SCLK_HIGH();
		emulator_twi_delay++;emulator_twi_delay++;emulator_twi_delay++;
		emulator_twi_delay++;emulator_twi_delay++;emulator_twi_delay++;
		SCLK_LOW();
}

/* 发送一位数据：低 */
void EMULATOR_TWI_SEND_BIT_L( void )
{
		SDAT_CLR();
		emulator_twi_delay++;
		SCLK_HIGH();
		emulator_twi_delay++;emulator_twi_delay++;emulator_twi_delay++;
		emulator_twi_delay++;emulator_twi_delay++;emulator_twi_delay++;
		SCLK_LOW();
}

/* 读取一位数据 */
INT8U EMULATOR_TWI_RECV_BIT( INT8U bitofget )
{
		INT8U chrread = 0;
		SDAT_SET();
		SDAT_IN();
		SCLK_HIGH();
		emulator_twi_delay++;
		if( SDAT_READ() )
		{
			chrread |= emulator_twi_getbit[bitofget];
		}
		SCLK_LOW();
		SDAT_OUT();
		return chrread;
}

/* 接收ACK */
void EMULATOR_TWI_RECV_ACK( void )
{
		SDAT_SET();
		SDAT_IN();
		emulator_twi_ack_error = 0;
		SCLK_HIGH();
		emulator_twi_delay++;
		if( SDAT_READ() )
		{
			emulator_twi_ack_error = 1;
		}
		SCLK_LOW();
		emulator_twi_delay++;
		SDAT_OUT();
}

/* 发送ACK*/
void EMULATOR_TWI_SEND_ACK( void )
{
		emulator_twi_delay++;
		SDAT_CLR();
		emulator_twi_delay++;
		SCLK_HIGH();
		emulator_twi_delay++;
		SCLK_LOW();
}

/* 发送NACK*/
void EMULATOR_TWI_SEND_NACK( void )
{
		emulator_twi_delay++;
		SDAT_SET();
		emulator_twi_delay++;
		SCLK_HIGH();
		emulator_twi_delay++;
		SCLK_LOW();
}

/* 发送一个字节，从高位到低位 */
void EMULATOR_TWI_SEND_BYTE_H2L( INT8U chrsend )
{
		for( emulator_twi_i=0; emulator_twi_i<8; emulator_twi_i++ )
		{
			if( chrsend & emulator_twi_getbit[7-emulator_twi_i] )
			{
				EMULATOR_TWI_SEND_BIT_H();
			}
			else
			{
				EMULATOR_TWI_SEND_BIT_L();
			}
		}																
}

/* 发送一个字节，从低位到高位 */
void EMULATOR_TWI_SEND_BYTE_L2H( INT8U chrsend )
{
		for( emulator_twi_i=0; emulator_twi_i<8; emulator_twi_i++ )
		{
			if( chrsend & emulator_twi_getbit[emulator_twi_i] )
			{
				EMULATOR_TWI_SEND_BIT_H();
			}
			else
			{
				EMULATOR_TWI_SEND_BIT_L();
			}
		}																
}

/* 读入一个字节，从高位到低位 */
INT8U EMULATOR_TWI_RECV_BYTE_H2L( void )
{
		INT8U chrread = 0;
		for( emulator_twi_i=0; emulator_twi_i<8; emulator_twi_i++ )
		{
			chrread |= EMULATOR_TWI_RECV_BIT( 7-emulator_twi_i );
		}
		return chrread;
}

/* 读入一个字节，从低位到高位 */
INT8U EMULATOR_TWI_RECV_BYTE_L2H( void )
{
		INT8U chrread = 0;
		for( emulator_twi_i=0; emulator_twi_i<8; emulator_twi_i++ )
		{
			chrread |= EMULATOR_TWI_RECV_BIT( emulator_twi_i );
		}
		return chrread;
}

unsigned char Read_bq(unsigned char RomAddress)
{
	unsigned char ret;
	EMULATOR_TWI_START();
	EMULATOR_TWI_SEND_BYTE_H2L(DEVICE_ADD + 1);
	EMULATOR_TWI_RECV_ACK();
	if( emulator_twi_ack_error )
	{
		ret = 0xFE;
		goto _ReadStop;
	}
	EMULATOR_TWI_SEND_BYTE_H2L(RomAddress);
	EMULATOR_TWI_RECV_ACK();
	if( emulator_twi_ack_error )
	{
		ret = 0xFD;
		goto _ReadStop;
	}
	ret = EMULATOR_TWI_RECV_BYTE_H2L();
    EMULATOR_TWI_SEND_ACK();
_ReadStop:
	EMULATOR_TWI_STOP();
	return (ret);
}
unsigned char Write_bq(unsigned char RomAddress,unsigned char Wdata) 
{
	unsigned char ret;
	EMULATOR_TWI_START();
	EMULATOR_TWI_SEND_BYTE_H2L(DEVICE_ADD);// ((slaveID<<1) | EMULATOR_TWI_WRITE) );
	EMULATOR_TWI_RECV_ACK();
	if( emulator_twi_ack_error )
	{
		ret = 0xFE;
		goto _WriteStop;
	}
	EMULATOR_TWI_SEND_BYTE_H2L( RomAddress );
	EMULATOR_TWI_RECV_ACK();
	if( emulator_twi_ack_error )
	{
		ret = 0xFD;
		goto _WriteStop;
	}
	EMULATOR_TWI_SEND_BYTE_H2L( Wdata );
	EMULATOR_TWI_RECV_ACK();
	if( emulator_twi_ack_error )
	{
		ret = 0xFC;
		goto _WriteStop;
	}
	ret = 1;
_WriteStop:
	EMULATOR_TWI_STOP();
	return ret;
    
}
void  initialBQ(void)
{

	Write_bq(0x61,0); //地址，数据  
 
	Write_bq(0x3e,0x3a); 
 
	Write_bq(0x3F,0x00); 
  
}
 void  bqreadall(void)
{       
	unsigned char i ;   
	for(i= 0x40 ;i<=0x5f ;i++ )  
	{                     	  
	     read[i-0x40] = Read_bq(i);  //read to RD               		       
	} 
}