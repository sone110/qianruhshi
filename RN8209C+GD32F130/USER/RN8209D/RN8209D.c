#include "RN8209D.h"

uint8_t 		USART2_RxBuf[USART_REC_LEN] ;        //���ͻ�����
uint8_t		USART2_TxBuf[USART_REC_LEN] ;        //���ջ�����
uint8_t   USART2_RX_STA =0 ;
 volatile  uint8_t  Data_Len ;

//void TIM4_IRQHandler(void)
//{ 	
//	if (TIM_GetITStatus(TIM4, TIM_IT_Update) != RESET)//�Ǹ����ж�
//	{	 			   
//		USART2_RX_STA|=1<<7;	//��ǽ������
//		TIM_ClearITPendingBit(TIM4, TIM_IT_Update  );  //���TIM7�����жϱ�־    
//		TIM_Cmd(TIM4, DISABLE);  //�ر�TIM7 
//	}	    
//}




void TIMER2_IRQHandler(void)
{
  
    if(SET == timer_interrupt_flag_get(TIMER2,TIMER_INT_FLAG_UP))
    {
        USART2_RX_STA|=1<<7;	//��ǽ������		
       // printf("TIM�ж� \n");
        //printf("0x%x \n",USART2_RX_STA);        		
        timer_interrupt_flag_clear(TIMER2, TIMER_INT_FLAG_UP);  //���TIM7�����жϱ�־ 
        timer_disable(TIMER2);  //�ر�TIM2 
       // usart_interrupt_disable(USART1, USART_INT_RBNE);
          
                
    }
     
   
}

/***************************************************************
*Function name:
*
*Description: 
*
*Parameters:
*
*Returned value: NONE
*
*Version :  v1.0
*
*Data&Author: 2018/1/10&SR
*****************************************************************/

void USART1_IRQHandler(void)
{
		uint8_t data=0  ;
	 if(SET == usart_flag_get(USART1, USART_FLAG_RBNE))//�����ж�
	{		
		data = usart_data_receive(USART1);

		if((USART2_RX_STA &(1<<7))==0) //�жϽ�����ɱ�־λ�Ƿ���գ���պ�����ٴν���
		{

			if(USART2_RX_STA <Data_Len)	//�����Խ�������
			{
                 USART2_RxBuf[USART2_RX_STA++]=data;	//��¼���յ���ֵ.                              
			}
            if(USART2_RX_STA == Data_Len) 
			{
				USART2_RX_STA  =  USART2_RX_STA|0x80;				//ǿ�Ʊ�ǽ������	
			}	
		}  	       
	}
     usart_flag_clear(USART1 , USART_FLAG_RBNE);
}


void nvic_configuration(void)
{
    nvic_priority_group_set(NVIC_PRIGROUP_PRE1_SUB3);
    nvic_irq_enable(TIMER2_IRQn, 1, 2);
}
 
//ͨ�ö�ʱ��7�жϳ�ʼ��
//����ʱ��ѡ��ΪAPB1��2������APB1Ϊ42M
//arr���Զ���װֵ��
//psc��ʱ��Ԥ��Ƶ��
//��ʱ�����ʱ����㷽��:Tout=((arr+1)*(psc+1))/Ft us.
//Ft=��ʱ������Ƶ��,��λ:Mhz 
//ͨ�ö�ʱ���жϳ�ʼ��
//����ʼ��ѡ��ΪAPB1��2������APB1Ϊ36M
//arr���Զ���װֵ��
//psc��ʱ��Ԥ��Ƶ��		 
void TIM2_Int_Init(uint16_t arr,uint16_t psc)
{	
//	NVIC_InitTypeDef NVIC_InitStructure;
//	TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;

//	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM4, ENABLE);//TIM7ʱ��ʹ��    
//	
//	//��ʱ��TIM7��ʼ��
//	TIM_TimeBaseStructure.TIM_Period = arr; //��������һ�������¼�װ�����Զ���װ�ؼĴ������ڵ�ֵ	
//	TIM_TimeBaseStructure.TIM_Prescaler =psc; //����������ΪTIMxʱ��Ƶ�ʳ�����Ԥ��Ƶֵ
//	TIM_TimeBaseStructure.TIM_ClockDivision = TIM_CKD_DIV1; //����ʱ�ӷָ�:TDTS = Tck_tim
//	TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;  //TIM���ϼ���ģʽ
//	TIM_TimeBaseInit(TIM4, &TIM_TimeBaseStructure); //����ָ���Ĳ�����ʼ��TIMx��ʱ�������λ
// 
//	TIM_ITConfig(TIM4,TIM_IT_Update,ENABLE ); //ʹ��ָ����TIM7�ж�,��������ж�
//	
//	TIM_Cmd(TIM4,ENABLE);//������ʱ��7
//	
//	NVIC_InitStructure.NVIC_IRQChannel = TIM4_IRQn;
//	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority=2 ;//��ռ���ȼ�0
//	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 2;		//�����ȼ�2
//	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;			//IRQͨ��ʹ��
//	NVIC_Init(&NVIC_InitStructure);	//����ָ���Ĳ�����ʼ��VIC�Ĵ���	


    
    timer_ic_parameter_struct timer_icinitpara;
    timer_parameter_struct timer_initpara;

    rcu_periph_clock_enable(RCU_TIMER2);

    timer_deinit(TIMER2);

    /* TIMER2 configuration */
    timer_initpara.prescaler         = psc;
    timer_initpara.alignedmode       = TIMER_COUNTER_EDGE;
    timer_initpara.counterdirection  = TIMER_COUNTER_UP;
    timer_initpara.period            = arr;
    timer_initpara.clockdivision     = TIMER_CKDIV_DIV1;
    timer_initpara.repetitioncounter = 0;
    timer_init(TIMER2,&timer_initpara);
    
    timer_auto_reload_shadow_enable(TIMER2);
    /* clear channel 0 interrupt bit */
    timer_interrupt_flag_clear(TIMER2, TIMER_INT_FLAG_UP);   //�����ж�
    /* channel 0 interrupt enable */
    timer_interrupt_enable(TIMER2, TIMER_INT_FLAG_UP);

    /* TIMER2 counter enable */
   // timer_enable(TIMER2);
}
	 

//void RN8209D_USART_Init()
//{
//	GPIO_InitTypeDef GPIO_InitStructure ;
//	USART_InitTypeDef USART_InitStructure;
//	NVIC_InitTypeDef  NVIC_InitStructure ;
//	
//	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA  ,ENABLE );
//	RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART2  ,ENABLE ); 
//	
//		//USART2_TX	  GPIOA.2��ʼ��
//	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_2 ;
//	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP ;
//	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz ;
//	GPIO_Init(GPIOA, &GPIO_InitStructure);
//	
//	//USART2_RX	  GPIOA.3��ʼ��
//  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_3;//PA10
//  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;//��������
//  GPIO_Init(GPIOA, &GPIO_InitStructure);//��ʼ�� 
//	
//	
//	NVIC_InitStructure.NVIC_IRQChannel = USART2_IRQn;
//	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority=3 ;//��ռ���ȼ�3
//	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 3;		//�����ȼ�3
//	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;			//IRQͨ��ʹ��
//	NVIC_Init(&NVIC_InitStructure);	//����ָ���Ĳ�����ʼ��VIC�Ĵ���
//	
//	
//	USART_InitStructure.USART_BaudRate = 4800;//���ڲ�����
//	USART_InitStructure.USART_WordLength = USART_WordLength_9b;//�ֳ�Ϊ8λ���ݸ�ʽ
//	USART_InitStructure.USART_StopBits = USART_StopBits_1;//һ��ֹͣλ
//	USART_InitStructure.USART_Parity = USART_Parity_Even;//żУ��λ
//	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;//��Ӳ������������
//	USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;	//�շ�ģʽ

//  USART_Init(USART2, &USART_InitStructure); //��ʼ������2
//  USART_ITConfig(USART2, USART_IT_RXNE, ENABLE);//�������ڽ����ж�
//  USART_Cmd(USART2, ENABLE);   
//	
//	
//	TIM4_Int_Init(10000,7200);		//TIMER-7 10ms�ж�
//	USART2_RX_STA=0;		//����
//	TIM_Cmd(TIM4,DISABLE);			//�رն�ʱ��7
//}





/***************************************************************
*Function name:ERR_STATUS RN8209D_WriteData(u8 wReg,u8 *pBuf,u8 ucLen)
*
*Description: Write data to 8209
*
*Parameters: wReg: Register address , pBuf: address store the data wirte  ucLen  : data length 
*
*Returned value:  ErrorStatus: ERROR , SUCCESS 
*
*Version :  v1.0
*
*Data&Author: 2018/1/10&SR
*****************************************************************/
ErrStatus RN8209D_WriteData(uint8_t wReg,uint8_t *pBuf,uint8_t ucLen)
{
	uint8_t i,j,temp,chksum,Repeat;
	uint32_t data=0;
	uint8_t* tempp;
	ErrStatus err ;
	uint8_t* datas = (uint8_t*)malloc(sizeof(uint8_t)*(ucLen+2));
	
	uint8_t Write_EN[3]={0xEA,0xE5,0x30};     //дʹ������
	uint8_t Write_DISABLE[3]={0xEA,0xDC,0x39};     //дʹ�ܹر�����
   tempp = datas;
	
  if( (ucLen == 0) || (ucLen > 4) ) return(ERROR);//�������ݳ����ж� 
	USART1_SendData(Write_EN , 3);        //����дʹ��
	for( Repeat =2; Repeat != 0 ; Repeat--)	//ѭ������
	{
		err = SUCCESS;		
		//д������������ֽ�bit[7]=0:��;1:д;bit[6:0]�Ĵ�����ַ		
		temp =wReg|0x80;//�Ĵ�����ַ���λ��1
		*datas++  = temp ;
		chksum = temp;		
// 		for(i = ucLen; i > 0;i-- )
// 		{		
// 			*datas++=pBuf[i-1];	
// 			chksum +=pBuf[i-1];
// 		}
// 		chksum = ~chksum;
		for( i=0 ; i<ucLen;i++)
		{
			*datas++ = pBuf[i];
			chksum +=pBuf[i];

    }
		chksum = ~chksum;
		*datas++=chksum;
		USART1_SendData(tempp , ucLen+2);
		delay_1ms(180);
// 	 RN8209D_ReadData(wReg,(uint8_t* )&data,2);
// 	 printf("Read reg data: 0x%x \n" , data);
	}
	USART1_SendData(Write_DISABLE , 3);  //����д����
  return(err);
	
	
}

/***************************************************************
*Function name:ErrorStatus RN8209D_ReadData(u8 wReg,u8 *pBuf,u8 ucLen)
*
*Description: Read 8209 Register data
*
*Parameters:  wReg: Register address , pBuf: address store the data read from RN8209 
*
*Returned value: ERR_STATUS: ERROR , SUCCESS 
*
*Version :  v1.0
*
*Data&Author: 2018/1/10&SR
*****************************************************************/

ErrStatus RN8209D_ReadData(uint8_t wReg,uint8_t *pBuf,uint8_t ucLen)
{
	uint8_t i,temp,Repeat,k;
	uint8_t j=0,timeout=0;
	uint8_t chksum=0;	
	ErrStatus	err;
	if(ucLen == 0) return(ERROR);

	for( Repeat=2; Repeat != 0 ; Repeat--)	
	{
		err  = SUCCESS;	    
		temp = wReg ;
		chksum=wReg;
		j = 0;
        timeout=0;	
        Data_Len = ucLen +1;
		//���������֣�����Ҫ�����ݵļĴ�����ַ
       
        while(usart_flag_get(USART1, USART_FLAG_TC) == RESET) ;	  
		    usart_data_transmit(USART1,temp);
  
    do
      {	      
        delay_1ms(100);
        timeout++;			
		}while( ((timeout == 0xFF) ||(USART2_RX_STA & (1<<7)) == 0)  ); //����ʱ��������0xFF���߽��ձ�־λΪ1ʱ����
		//�ж��Ƿ�ʱ������ʱ������ѭ��
		if(timeout == 0xFF )
		{
			err = ERROR;
            break;			
		}
		
       else if((USART2_RX_STA & (1<<7)) != 0)	  //
       {
//			
			for(k=0;k<(USART2_RX_STA&0x7F);k++)
			{
				
                printf("USART2_RxBuf[%d] = 0x%x \n",k,USART2_RxBuf[k]);
				
			}
			//�����յ������ݵ������ֽ�λ�͵��ֽ�λ��������У��ֵ
			for(i = (ucLen ); i > 0;i--)
				{
					pBuf[i-1] = USART2_RxBuf[j++]; //
					chksum += pBuf[i-1];//
				}
			chksum = ~(chksum);
			temp=USART2_RxBuf[j++];
			
			if(temp!=chksum)  
				{
					err = ERROR;
					for(i = ucLen; i > 0;i--) 
					    pBuf[i-1] = 0;
					 USART2_RX_STA=0;
				}
			if(err == SUCCESS)
			{   
				  Repeat =0;
				  USART2_RX_STA=0;
                  
					break;  //��������У����ȷ������
			   
			}
       			
			
		}			 	  	
	}     
  Data_Len = 0;	
  USART2_RX_STA=0;
	return(err);
	
}
/***************************************************************
*Function name:void  USART2_SendData( u8* data , int length)
*
*Description: usart2 send data to RN8209D
*
*Parameters: data: data address ��length :  data length
*
*Returned value: NONE
*
*Version :  v1.0
*
*Data&Author: 2018/1/10&SR
*****************************************************************/
void  USART1_SendData( uint8_t* data , int length)
{
	
	uint8_t t;
	
  for(t=0;t<length;t++)		//ѭ����������
		{		   
			while(usart_flag_get(USART1, USART_FLAG_TC) == RESET) ;	  
		    usart_data_transmit(USART1,data[t]);
		}	 
	while(usart_flag_get(USART1, USART_FLAG_TC) == RESET) ;		 	
}

/***************************************************************
*Function name: RN8209_Init(void)
*
*Description: Init RN8209
*
*Parameters:  NONE
*
*Returned value: NONE
*
*Version :  v1.0
*
*Data&Author: 2018/1/10&SR
*****************************************************************/


void RN8209_Init(void)
{
	uint32_t data=0;
	uint8_t pbuf[]={0x00,0x02};
    nvic_configuration();
    usart_init(4800);
	RN8209D_WriteData(ADIBGain, pbuf, 2);
    
    RN8209D_ReadData(ADIBGain ,(uint8_t* )&data ,2);
 	printf("ADIBGain: 0x%x---------------------------  \n" , data);
    delay_1ms(1000);
}

