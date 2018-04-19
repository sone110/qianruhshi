#include "usart.h"
#include  "RN8209D.h"
//
void usart_init(uint32_t  bound)
{
    
    usart_gpio_init();
    
    usart_deinit(USART1);
    usart_baudrate_set(USART1,bound);  //波特率设置
    usart_word_length_set(USART1 , USART_WL_9BIT) ;  //字长设置
    usart_stop_bit_set(USART1, USART_STB_1BIT ) ;    //停止位设置
    usart_parity_config(USART1 , USART_PM_EVEN);     //偶校验设置
    
    usart_transmit_config(USART1, USART_TRANSMIT_ENABLE);

    usart_receive_config(USART1, USART_RECEIVE_ENABLE);
    usart_enable(USART1);
    
    usart_interrupt_enable(USART1, USART_INT_RBNE);  //开启接收中断
    
    nvic_priority_group_set(NVIC_PRIGROUP_PRE1_SUB3);  //中断组设置
    nvic_irq_enable(USART1_IRQn, 1, 1);    
    
    USART2_RX_STA=0;

    
    
}


void usart_gpio_init(void)
{
    rcu_periph_clock_enable( RCU_GPIOA );
    
    rcu_periph_clock_enable( RCU_GPIOB );
    
    rcu_periph_clock_enable( RCU_USART1 ) ;
    //TX 复用定义
    gpio_af_set(GPIOA , GPIO_AF_4 ,GPIO_PIN_8) ; 
    //RX 复用定义
    gpio_af_set(GPIOB , GPIO_AF_4 ,GPIO_PIN_0) ;
    
    
    //usart2 TX 定义
      /* configure USART Tx as alternate function push-pull */
    gpio_mode_set(GPIOA , GPIO_MODE_AF , GPIO_PUPD_PULLUP,GPIO_PIN_8 );
    gpio_output_options_set(GPIOA, GPIO_OTYPE_PP, GPIO_OSPEED_50MHZ,GPIO_PIN_8);
    
    /* configure USART Rx as alternate function push-pull */
    gpio_mode_set(GPIOB, GPIO_MODE_AF, GPIO_PUPD_PULLUP,GPIO_PIN_0);
    gpio_output_options_set(GPIOB, GPIO_OTYPE_PP, GPIO_OSPEED_50MHZ,GPIO_PIN_0);
    


//    rcu_periph_clock_enable(  RCU_GPIOA ); 
//    gpio_mode_set(GPIOA , GPIO_MODE_OUTPUT , GPIO_PUPD_PULLUP, GPIO_PIN_2| GPIO_PIN_3);
//    gpio_output_options_set(GPIOA, GPIO_OTYPE_PP, GPIO_OSPEED_50MHZ,GPIO_PIN_2| GPIO_PIN_3);
//    gpio_bit_set(GPIOA , GPIO_PIN_2| GPIO_PIN_3);
    
    
    
    
}
