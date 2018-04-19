/*!
    \file  gd32f1x0_it.c
    \brief interrupt service routines
*/

/*
    Copyright (C) 2017 GigaDevice

    2014-12-26, V1.0.0, platform GD32F1x0(x=3,5)
    2016-01-15, V2.0.0, platform GD32F1x0(x=3,5,7,9)
    2016-04-30, V3.0.0, firmware update for GD32F1x0(x=3,5,7,9)
    2017-06-19, V3.1.0, firmware update for GD32F1x0(x=3,5,7,9)
*/

#include "gd32f1x0_it.h"
#include "main.h"
#include "systick.h"
#include "RN8209D.h"
#include "RS485.h"

extern uint8_t 		USART2_RxBuf[USART_REC_LEN] ;        //���ͻ�����
extern uint8_t		USART2_TxBuf[USART_REC_LEN] ;        //���ջ�����
extern   uint8_t   USART2_RX_STA  ;
/*!
    \brief      this function handles NMI exception
    \param[in]  none
    \param[out] none
    \retval     none
*/
void NMI_Handler(void)
{
}

/*!
    \brief      this function handles HardFault exception
    \param[in]  none
    \param[out] none
    \retval     none
*/
void HardFault_Handler(void)
{
    /* if Hard Fault exception occurs, go to infinite loop */
    while(1){
    }
}

/*!
    \brief      this function handles MemManage exception
    \param[in]  none
    \param[out] none
    \retval     none
*/
void MemManage_Handler(void)
{
    /* if Memory Manage exception occurs, go to infinite loop */
    while(1){
    }
}

/*!
    \brief      this function handles BusFault exception
    \param[in]  none
    \param[out] none
    \retval     none
*/
void BusFault_Handler(void)
{
    /* if Bus Fault exception occurs, go to infinite loop */
    while(1){
    }
}

/*!
    \brief      this function handles UsageFault exception
    \param[in]  none
    \param[out] none
    \retval     none
*/
void UsageFault_Handler(void)
{
    /* if Usage Fault exception occurs, go to infinite loop */
    while(1){
    }
}

/*!
    \brief      this function handles SVC exception
    \param[in]  none
    \param[out] none
    \retval     none
*/
void SVC_Handler(void)
{
}

/*!
    \brief      this function handles DebugMon exception
    \param[in]  none
    \param[out] none
    \retval     none
*/
void DebugMon_Handler(void)
{
}

/*!
    \brief      this function handles PendSV exception
    \param[in]  none
    \param[out] none
    \retval     none
*/
void PendSV_Handler(void)
{
}

/*!
    \brief      this function handles SysTick exception
    \param[in]  none
    \param[out] none
    \retval     none
*/
void SysTick_Handler(void)
{
//    led_spark();
    delay_decrement();
}


//void TIMER2_IRQHandler(void)
//{
//    if(SET == timer_interrupt_flag_get(TIMER2,TIMER_INT_FLAG_UP))
//    {
//        USART2_RX_STA|=1<<7;	//��ǽ������
//		
//        printf("TIM�ж� \n");        
//		timer_disable(TIMER2);  //�ر�TIM2 
//        timer_interrupt_flag_clear(TIMER2, TIMER_INT_FLAG_UP);  //���TIM7�����жϱ�־          
//    }
//   
//}


//void USART1_IRQHandler(void)
//{
//	uint8_t data  ;
//	 if(RESET != usart_interrupt_flag_get(USART1, USART_INT_FLAG_RBNE))//�����ж�
//	{		
//		data = usart_data_receive(USART1);
//		printf("rec:0x%x ",data);
//		if((USART2_RX_STA &(1<<7))==0) //�жϽ�����ɱ�־λ�Ƿ���գ���պ�����ٴν���
//		{
//			if(USART2_RX_STA<USART_REC_LEN)	//�����Խ�������
//		//	if(USART2_RX_STA<1)	//�����Խ�������
//			{
//				timer_counter_value_config(TIMER2,0);        //���������
//				if(USART2_RX_STA==0) 				//�����ճ���Ϊ0ʱ,������ʱ�� 
//				{
//					 timer_enable(TIMER2);//ʹ�ܶ�ʱ��7,����ʱ10msǿ�ƽ������
//				}
//				USART2_RxBuf[USART2_RX_STA++]=data;	//��¼���յ���ֵ	 
//			}else 
//			{
//				USART2_RX_STA|=1<<7;				//ǿ�Ʊ�ǽ������	
//			}	
//		}  	
//	}
//	usart_interrupt_flag_clear(USART1 , USART_INT_FLAG_RBNE);
//	//USART_ClearITPendingBit(USART1 , USART_INT_FLAG_RBNE);
//}
//void USART1_IRQHandler()
//{
//    uint8_t data ;
//    if(RESET != usart_interrupt_flag_get(USART1, USART_INT_FLAG_RBNE))
//      {
//        /* receive data */
//        data = usart_data_receive(USART1);
//        printf("�������ݣ�0x%x \n",data);
//       usart_flag_clear(USART1 , USART_FLAG_ORERR);
//          usart_interrupt_flag_clear(USART1 ,  USART_INT_FLAG_RBNE);
//        
//      }
//      if(RESET != usart_interrupt_flag_get(USART1, USART_INT_FLAG_RBNE_ORERR))
//      {
//        /* receive data */
//        data = usart_data_receive(USART1);
//      //  printf("�������ݣ�0x%x \n",data);
//       //usart_flag_clear(USART1 , USART_FLAG_ORERR);
//        usart_interrupt_flag_clear(USART1 ,  USART_FLAG_ORERR);
//        
//      }
//     
//  
//    // printf("sdfds");
//   
//}
