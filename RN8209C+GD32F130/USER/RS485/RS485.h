#ifndef __RS485_H_
#define __RS485_H_
#include "gd32f1x0.h"

#define RS485_RX_EN  gpio_bit_set(GPIOB , GPIO_PIN_1)
#define RS485_TX_EN  gpio_bit_reset(GPIOB , GPIO_PIN_1)

void RS485_init(uint16_t  bound);
void RS485_gpio_init(void);
void RS485_EN_GPIO_init(void);
void RS485_Send_Data(uint8_t *buf,uint8_t len);

#endif