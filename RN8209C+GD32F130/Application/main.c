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

/*!
    \brief      toggle the led every 500ms
    \param[in]  none
    \param[out] none
    \retval     none
*/

uint8_t SendData[4] = {0x82,0x09,0x00,0xF5};




/*!
    \brief      main function
    \param[in]  none
    \param[out] none
    \retval     none
*/
int main(void)
{
    uint32_t data = 0 , frequent=0,ia=0  ,ADC_U=0,powera=0,id =0;
    SystemInit();
    systick_config();
    RS485_init(9600);    
    RS485_TX_EN;
    RN8209_Init();
    RN8209D_ReadData(ADDeviceID ,(uint8_t* )&id ,3);
	printf("DeviceID: 0x%x---------------------------  \n" , id);

    while (1)
    {	
   
         delay_1ms(1000);
         RN8209D_ReadData(ADURMS ,(uint8_t* )&ADC_U ,3);
	     printf("电压有效值: 0x%x---------------------------  \n" , ADC_U);
         delay_1ms(1000);
         RN8209D_ReadData(ADUFreq ,(uint8_t* )&frequent ,2);
	     printf("频率: 0x%x---------------------------  \n" , frequent);        
         delay_1ms(5000);
                 
    }
        
}





