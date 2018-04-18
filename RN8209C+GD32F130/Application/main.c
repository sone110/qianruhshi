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

uint8_t SendData[2] = {'a','b'};




/*!
    \brief      main function
    \param[in]  none
    \param[out] none
    \retval     none
*/
int main(void)
{
    uint32_t data = 0;
    systick_config();
    RS485_init(9600);    
    RS485_TX_EN;
    printf("≤‚ ‘");
    RN8209_Init();
    

    while (1)
    {	
        
         RN8209D_ReadData(ADDeviceID ,(uint8_t* )&data ,3);
 	     printf("data: 0x%x---------------------------  \n" , data);
         delay_1ms(1800);
                 
    }
        
}





