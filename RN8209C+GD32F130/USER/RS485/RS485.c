#include "RS485.h"
#include <stdio.h>

void RS485_init(uint16_t  bound)
{
    
    
    RS485_EN_GPIO_init();  
    RS485_gpio_init();  
    usart_deinit(USART0);
    usart_baudrate_set(USART0,bound);
    usart_word_length_set(USART0 , USART_WL_8BIT) ;  //字长设置
    usart_stop_bit_set(USART0, USART_STB_1BIT ) ;    //停止位设置
    //usart_parity_config(USART0 , USART_PM_NONE);     //偶校验设置
    usart_transmit_config(USART0, USART_TRANSMIT_ENABLE);
//    usart_receive_config(USART0, USART_RECEIVE_ENABLE);
    usart_enable(USART0);
    usart_interrupt_enable(USART0, USART_INT_RBNE );  //开启接收中断
//	  usart_interrupt_enable(USART0, USART_INT_TBE ); 
//	  usart_flag_clear(USART0,USART_FLAG_TBE);
    //中断设置
  
	  
}

void RS485_gpio_init(void)
{
    rcu_periph_clock_enable( RCU_GPIOA );   
    rcu_periph_clock_enable( RCU_USART0 ) ;
    //TX 复用定义
    gpio_af_set(GPIOA , GPIO_AF_1 ,GPIO_PIN_9) ; 
    //RX 复用定义
    gpio_af_set(GPIOA , GPIO_AF_1 ,GPIO_PIN_10) ;
    
    //usart1 TX 定义
    gpio_mode_set(GPIOA , GPIO_MODE_AF , GPIO_PUPD_PULLUP,GPIO_PIN_9 );
    gpio_output_options_set(GPIOA, GPIO_OTYPE_PP, GPIO_OSPEED_10MHZ,GPIO_PIN_9);
    //usart1 RX 定义
    gpio_mode_set(GPIOA , GPIO_MODE_AF , GPIO_PUPD_PULLUP,GPIO_PIN_10 );
    gpio_output_options_set(GPIOA, GPIO_OTYPE_PP, GPIO_OSPEED_10MHZ,GPIO_PIN_10);
       
}

void RS485_EN_GPIO_init(void)
{
    rcu_periph_clock_enable(  RCU_GPIOB ); 
    gpio_mode_set(GPIOB , GPIO_MODE_OUTPUT , GPIO_PUPD_PULLUP, GPIO_PIN_1 );
    gpio_output_options_set(GPIOB, GPIO_OTYPE_PP, GPIO_OSPEED_50MHZ,GPIO_PIN_1);
}



int fputc(int ch, FILE *f)
{
    usart_data_transmit(USART0, (uint8_t)ch);
    while(RESET == usart_flag_get(USART0, USART_FLAG_TBE));
    return ch;
}


void RS485_Send_Data(uint8_t *buf,uint8_t len)
{
	uint8_t t;
	RS485_TX_EN;			//设置为发送模式
  	for(t=0;t<len;t++)		//循环发送数据
	{		   
		while(usart_flag_get(USART0, USART_FLAG_TC) == RESET);	  
		usart_data_transmit(USART0,buf[t]);

	}	 
 
	while(usart_flag_get(USART0, USART_FLAG_TC) == RESET) ;		   
	RS485_RX_EN;				//设置为接收模式	
}


