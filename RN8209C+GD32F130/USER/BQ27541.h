#include "config.h"

#define DEVICE_ADD   0xaa


#define IIC_INIT()  PINSEL1 &= 0XFFFFFF0F
#define SCLK_OUT()  IO0DIR |= (1 << 18)        
#define SCLK_HIGH()	IO0SET |= (1 << 18)
#define SCLK_LOW()    IO0CLR |= (1 << 18)  
#define SDAT_IN()   IO0DIR &= ~(1<< 19)
#define SDAT_OUT()  IO0DIR |= (1 << 19)
#define SDAT_SET()  IO0SET |= (1 << 19)
#define SDAT_CLR()  IO0CLR |= (1 << 19)
#define SDAT_READ()  ((IO0PIN & (0x00080000)) == 0x00080000)


void Port_TWI_Init(void);

void EMULATOR_TWI_START( void );

void EMULATOR_TWI_STOP( void );
void EMULATOR_TWI_SEND_BIT_H( void );
void EMULATOR_TWI_SEND_BIT_L( void );
INT8U EMULATOR_TWI_RECV_BIT( INT8U bitofget );
void EMULATOR_TWI_RECV_ACK( void );
void EMULATOR_TWI_SEND_ACK( void );
void EMULATOR_TWI_SEND_NACK( void );
void EMULATOR_TWI_SEND_BYTE_H2L( INT8U chrsend );
void EMULATOR_TWI_SEND_BYTE_L2H( INT8U chrsend );
INT8U EMULATOR_TWI_RECV_BYTE_H2L( void );
INT8U EMULATOR_TWI_RECV_BYTE_L2H( void );

unsigned char Read_bq(unsigned char RomAddress);
unsigned char Write_bq(unsigned char RomAddress,unsigned char Wdata);
void  initialBQ(void);
void  bqreadall(void);

