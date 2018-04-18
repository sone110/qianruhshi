#ifndef _iic_H
#define _iic_H
#include "gd32f1x0.h"
#include "systick.h"
//�����ֲ����ʱֻҪ��һ�������ط�������
/* ��ʱʹ�õ�IO�� */
#define I2C_SCL GPIO_Pin_10	  //PB10
#define I2C_SDA GPIO_Pin_11	  //PB11
#define GPIO_I2C GPIOB

#define I2C_SCL_H GPIO_SetBits(GPIO_I2C,I2C_SCL)
#define I2C_SCL_L GPIO_ResetBits(GPIO_I2C,I2C_SCL)

#define I2C_SDA_H GPIO_SetBits(GPIO_I2C,I2C_SDA)
#define I2C_SDA_L GPIO_ResetBits(GPIO_I2C,I2C_SDA)

/* ����ȫ�ֺ��� */
void I2C_INIT(void);
void I2C_SDA_OUT(void);
void I2C_SDA_IN(void);
void I2C_Start(void);
void I2C_Stop(void);
void I2C_Ack(void);
void I2C_NAck(void);
uint8_t   I2C_Wait_Ack(void);
void I2C_Send_Byte(uint8_t txd);
uint8_t   I2C_Read_Byte(uint8_t ack);

#endif
