#include "RN8209D.h"

uint8_t 		USART2_RxBuf[USART_REC_LEN] ;        //发送缓冲区
uint8_t		USART2_TxBuf[USART_REC_LEN] ;        //接收缓冲区
uint8_t   USART2_RX_STA =0 ;
 volatile  uint8_t  Data_Len ;

//void TIM4_IRQHandler(void)
//{ 	
//	if (TIM_GetITStatus(TIM4, TIM_IT_Update) != RESET)//是更新中断
//	{	 			   
//		USART2_RX_STA|=1<<7;	//标记接收完成
//		TIM_ClearITPendingBit(TIM4, TIM_IT_Update  );  //清除TIM7更新中断标志    
//		TIM_Cmd(TIM4, DISABLE);  //关闭TIM7 
//	}	    
//}




void TIMER2_IRQHandler(void)
{
  
    if(SET == timer_interrupt_flag_get(TIMER2,TIMER_INT_FLAG_UP))
    {
        USART2_RX_STA|=1<<7;	//标记接收完成		
       // printf("TIM中断 \n");
        //printf("0x%x \n",USART2_RX_STA);        		
        timer_interrupt_flag_clear(TIMER2, TIMER_INT_FLAG_UP);  //清除TIM7更新中断标志 
        timer_disable(TIMER2);  //关闭TIM2 
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
	 if(SET == usart_flag_get(USART1, USART_FLAG_RBNE))//接收中断
	{		
		data = usart_data_receive(USART1);

		if((USART2_RX_STA &(1<<7))==0) //判断接收完成标志位是否清空，清空后才能再次接收
		{

			if(USART2_RX_STA <Data_Len)	//还可以接收数据
			{
                 USART2_RxBuf[USART2_RX_STA++]=data;	//记录接收到的值.                              
			}
            if(USART2_RX_STA == Data_Len) 
			{
				USART2_RX_STA  =  USART2_RX_STA|0x80;				//强制标记接收完成	
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
 
//通用定时器7中断初始化
//这里时钟选择为APB1的2倍，而APB1为42M
//arr：自动重装值。
//psc：时钟预分频数
//定时器溢出时间计算方法:Tout=((arr+1)*(psc+1))/Ft us.
//Ft=定时器工作频率,单位:Mhz 
//通用定时器中断初始化
//这里始终选择为APB1的2倍，而APB1为36M
//arr：自动重装值。
//psc：时钟预分频数		 
void TIM2_Int_Init(uint16_t arr,uint16_t psc)
{	
//	NVIC_InitTypeDef NVIC_InitStructure;
//	TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;

//	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM4, ENABLE);//TIM7时钟使能    
//	
//	//定时器TIM7初始化
//	TIM_TimeBaseStructure.TIM_Period = arr; //设置在下一个更新事件装入活动的自动重装载寄存器周期的值	
//	TIM_TimeBaseStructure.TIM_Prescaler =psc; //设置用来作为TIMx时钟频率除数的预分频值
//	TIM_TimeBaseStructure.TIM_ClockDivision = TIM_CKD_DIV1; //设置时钟分割:TDTS = Tck_tim
//	TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;  //TIM向上计数模式
//	TIM_TimeBaseInit(TIM4, &TIM_TimeBaseStructure); //根据指定的参数初始化TIMx的时间基数单位
// 
//	TIM_ITConfig(TIM4,TIM_IT_Update,ENABLE ); //使能指定的TIM7中断,允许更新中断
//	
//	TIM_Cmd(TIM4,ENABLE);//开启定时器7
//	
//	NVIC_InitStructure.NVIC_IRQChannel = TIM4_IRQn;
//	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority=2 ;//抢占优先级0
//	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 2;		//子优先级2
//	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;			//IRQ通道使能
//	NVIC_Init(&NVIC_InitStructure);	//根据指定的参数初始化VIC寄存器	


    
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
    timer_interrupt_flag_clear(TIMER2, TIMER_INT_FLAG_UP);   //更新中断
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
//		//USART2_TX	  GPIOA.2初始化
//	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_2 ;
//	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP ;
//	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz ;
//	GPIO_Init(GPIOA, &GPIO_InitStructure);
//	
//	//USART2_RX	  GPIOA.3初始化
//  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_3;//PA10
//  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;//浮空输入
//  GPIO_Init(GPIOA, &GPIO_InitStructure);//初始化 
//	
//	
//	NVIC_InitStructure.NVIC_IRQChannel = USART2_IRQn;
//	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority=3 ;//抢占优先级3
//	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 3;		//子优先级3
//	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;			//IRQ通道使能
//	NVIC_Init(&NVIC_InitStructure);	//根据指定的参数初始化VIC寄存器
//	
//	
//	USART_InitStructure.USART_BaudRate = 4800;//串口波特率
//	USART_InitStructure.USART_WordLength = USART_WordLength_9b;//字长为8位数据格式
//	USART_InitStructure.USART_StopBits = USART_StopBits_1;//一个停止位
//	USART_InitStructure.USART_Parity = USART_Parity_Even;//偶校验位
//	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;//无硬件数据流控制
//	USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;	//收发模式

//  USART_Init(USART2, &USART_InitStructure); //初始化串口2
//  USART_ITConfig(USART2, USART_IT_RXNE, ENABLE);//开启串口接受中断
//  USART_Cmd(USART2, ENABLE);   
//	
//	
//	TIM4_Int_Init(10000,7200);		//TIMER-7 10ms中断
//	USART2_RX_STA=0;		//清零
//	TIM_Cmd(TIM4,DISABLE);			//关闭定时器7
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
	
	uint8_t Write_EN[3]={0xEA,0xE5,0x30};     //写使能命令
	uint8_t Write_DISABLE[3]={0xEA,0xDC,0x39};     //写使能关闭命令
   tempp = datas;
	
  if( (ucLen == 0) || (ucLen > 4) ) return(ERROR);//发送数据长度判断 
	USART1_SendData(Write_EN , 3);        //发送写使能
	for( Repeat =2; Repeat != 0 ; Repeat--)	//循环两次
	{
		err = SUCCESS;		
		//写数据命令，命令字节bit[7]=0:读;1:写;bit[6:0]寄存器地址		
		temp =wReg|0x80;//寄存器地址最高位或1
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
	USART1_SendData(Write_DISABLE , 3);  //发送写保护
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
		//发送命令字，即需要读数据的寄存器地址
       
        while(usart_flag_get(USART1, USART_FLAG_TC) == RESET) ;	  
		    usart_data_transmit(USART1,temp);
  
    do
      {	      
        delay_1ms(100);
        timeout++;			
		}while( ((timeout == 0xFF) ||(USART2_RX_STA & (1<<7)) == 0)  ); //当超时次数等于0xFF或者接收标志位为1时跳出
		//判断是否超时，若超时，跳出循环
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
			//将接收到的数据调整高字节位和低字节位，并计算校验值
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
                  
					break;  //接收数据校验正确，跳出
			   
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
*Parameters: data: data address ，length :  data length
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
	
  for(t=0;t<length;t++)		//循环发送数据
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

