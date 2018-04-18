#ifndef __RN8209D_H_
#define __RN8209D_H_
#include "gd32f1x0.h"
#include "stdio.h"
#include "stdlib.h"
#include "systick.h"
#include "usart.h"


#define USART_REC_LEN 200
extern uint8_t 		USART2_RxBuf[USART_REC_LEN] ;        //发送缓冲区
extern uint8_t		USART2_TxBuf[USART_REC_LEN] ;        //接收缓冲区
extern   uint8_t   USART2_RX_STA  ;
/*****
*BIT7:接收数据完成标志
*BIT6-0：接收数据长度
**/
 extern uint8_t   USART2_RX_STA ;


//----------------------start RN8209  地址定义---------------------------------------------------//
#define					ADSYSCON        0x00       //系统控制寄存器，写保护
#define        	ADEMUCON        0x01       //计量控制寄存器
#define        	ADHFConst     	0x02       //脉冲频率寄存器
#define        	ADPStart      	0x03       //有功起动功率设置
#define        	ADDStart      	0x04       //自定义电能起动功率设置
#define					ADGPQA        	0x05       //通道A功率增益校正寄存器
#define        	ADGPQB        	0x06       //通道B功率增益校正寄存器
#define        	ADPhsA        	0x07       //通道A相位校正寄存器
#define        	ADPhsB        	0x08       //通道B相位校正寄存器
#define					ADQPHSCAL				0x09       //无功相位补偿
#define					ADAPOSA 				0x0a       //
#define        	ADAPOSB 				0x0b
#define        	ADRPOSA 				0x0c
#define        	ADRPOSB 				0x0d
#define        	ADIARMSOS     	0x0e
#define        	ADIBRMSOS     	0x0f
#define        	ADIBGain      	0x10
#define					ADD2FPL       	0x11
#define        	ADD2FPH       	0x12
#define        	ADDCIAH       	0x13
#define        	ADDCIBH       	0x14
#define         ADDCUH		      0x15   
#define         ADDCL   	      0x16 
#define         ADEMUCON2	      0x17
#define					ADPFCnt       	0x20         //快速有功脉冲计数    
#define        	ADDFcnt       	0x21         //自定义电能快速脉冲计数
#define        	ADIARMS       	0x22         //通道A电流有效值
#define        	ADIBRMS       	0x23         //通道B电流有效值
#define        	ADURMS        	0x24         //电压有效值
#define					ADUFreq       	0x25         //电压频率
#define        	ADPowerPA     	0x26         //有功功率A
#define        	ADPowerPB     	0x27         //有功功率B
#define         ADEnergyP  	    0x29         //无功功率
#define         ADEnergyP2 	    0x2a          //有功能量
#define         ADEnergyD     	0x2b          
#define         ADEnergyD2    	0x2c
#define         ADEMUStatus   	0x2d
#define         ADSPL_IA      	0x30         //IA通道ADC采样
#define         ADSPL_IB      	0x31         //IB通道ADC采样
#define         ADSPL_U       	0x32         //U通道ADC采样
#define         ADIE  		      0x40         //中断允许寄存器
#define         ADIF  		      0x41         //中断标志寄存器
#define         ADRIF         	0x42         //复位中断标志寄存器
#define         ADSysStatus  	  0x43         //系统状态寄存器
#define         ADRData      	  0x44         //上一次SPI/USART读出的数据
#define         ADWData        	0x45         //上一次SPI/USART写入的数据
#define         ADDeviceID    	0x7f         //Device ID
#define         WriteEn   	    0xea          //特殊命令寄存器    
//----------------------end RN8209  地址定义-----------------------------------------------//
//---start 校表参数文件-------
typedef struct 
{		
	uint16_t			SYSCON;
	uint16_t			EMUCON;
	uint16_t			HFConst;
	uint16_t			PStart;	
	uint16_t			QStart;  			//10
	uint16_t			GPQA;    	
	uint16_t			GPQB;    	
	uint16_t			IAGain;  	
	uint16_t			UGain;   	
	uint16_t			IBGain;  	
	uint16_t			PhsA;	   	
	uint16_t			PhsB;    	
	uint16_t			QPhsCal; 			//22
	uint16_t			APOSA;   	
	uint16_t			APOSB;	 	
	uint16_t			RPOSA;   	
	uint16_t			RPOSB;   	
	uint16_t			IARMSOS; 			//32
	uint16_t			IBRMSOS;			//34
	uint16_t			EMUCON2; 	
	float		KUrms;								// 电压系数
	float		KIArms;								// A通道电流系数
	float		KIBrms;								// B通道电流系数
	float		KPrms;								// 功率系数
	uint16_t			RealUI[2];						// 功率显示值，功率大于此值时显示 0.2%
	uint32_t			RealPw;								// 电流显示值，电流大于此值时显示 0.2%
	uint32_t			ChkSum;             	
	uint16_t			RTCDota0;							// RTC校正寄存器
	uint8_t			TemperAdj[2];					// 高低温补偿值
	uint8_t			RTCAdj[4];						// RTC高低温时补偿值
	uint8_t			CurAdj;								// 自热影响补偿值
	uint8_t 			OfsetAdjAcVolt[2]; 		//根据电压调整OFFSET值
	uint16_t			CorrectionTemper;  		//校正时刻表计的温度
} sDl645FirmParaFile_TypeDef;		//58 Byte
//---end 校表参数文件-------


//---start 计量值瞬时值中转-------
typedef struct
{
  uint8_t		ChkErrCnt;
  uint32_t 	Pw[2];   		    //pa,pb   
  uint32_t 	UI[3];          // Ia=UI[0] Inal U         
  uint16_t 	Frequency;   		//频率                            	
  uint32_t		Pulse;		    	//前台脉冲
  uint16_t     Pstart;
  //---电能脉冲---	
  uint32_t		Pulse_Eg;	    	//脉冲个数
  uint32_t 	PDirect;				//功率方向
  uint32_t 	ChkSum1;				//读出EMU的校表参数校验
  // 校表使用参数
  uint16_t		RatintU;				// 额定电压
	uint16_t		RatingI;				// 额定电流
  uint32_t		TempU;					// 当前校正点电压
  uint32_t		TempI;					// 当店校正点电流
  uint32_t		TempPw;					// 当前校正点功率
} sDl645FrontTmp_TypeDef;
//---end 计量值瞬时值中转-------

//---start 计量值瞬时值文件-------
typedef struct
{	
	struct sDl645FrontPubData_TypeDef  
	{
		uint16_t			U;			    		//---电压---NNN.N
		uint32_t	    Ia;			    		//---电流NNNN.NNNN(电流值要求3整3小，整定值要求2整4小，最高位表示方向)---		
		uint32_t	    In;         		//---零线电流
		uint32_t	Pw;			    		    //---瞬时有功p
		uint16_t			Pf;			    		//---功率因素N.NNN---	最高位表示方向{Pf Pfa Pfb Pfc}	
		uint16_t			Angle;		  		//---相角NNN.N---		
		uint16_t			Frequency;			//---频率NN.NN
		uint32_t			PPwave;					//---一分钟平均功率NN.NNNN
		uint8_t      Chnsel;     		
		uint16_t			Temperature;		//---NNN.N  温度
		uint16_t			ClockBat;				//---NN.NN  电池电压
		uint32_t			tWorkBat;				//---NNNN   时钟工作时间（分钟）
		uint8_t			PDirect;				//---原功率方向
		                    		
	  uint16_t    	CfIn; 					//脉冲输入电平判断
	  uint8_t    	CfTime;					//
	  uint8_t    	Step;       		
	  uint16_t   	FrontStamp; 		
	  uint16_t			tMaxI;					// 最大电流持续时间
	  uint8_t			SafeCurFlag;		// 电流门限不为零标志
	} PubData;
	
	struct sDl645FrontPriData_TypeDef  
	{		
		uint8_t			Flag;						//---工作异常标志---
	} PriData;	
	
	struct sDl645FrontPriPara_TypeDef  
	{		
		uint32_t	 PConstE;						//---有功常数---
		uint16_t	 Crc;
	} PriPara;		
} sDl645Front_TypeDef;	
//---end 计量值瞬时值-------

//---start 串口通信变量文件-------
typedef struct
{
	uint16_t		EFlag;							//通讯状态
	
  uint16_t 	RxLen;							//接收数据长度
  uint16_t  	TxLen;              // 发送数据长度
  uint32_t		TimeOutStamp;				//超时
  uint8_t 		*pTx;
  
  uint8_t		fBps;								//波特率变更标志
  uint8_t		NewBps;							
  uint32_t 	NewBpsStamp;				
//uint8_t 		TxAddr;
  

}sComPack_TypeDef;
//---end 串口通信变量文件-------
ErrStatus RN8209D_WriteData(uint8_t wReg,uint8_t *pBuf,uint8_t ucLen) ;
void  USART1_SendData( uint8_t* data , int length) ;
void RN8209_Init(void);
ErrStatus RN8209D_ReadData(uint8_t wReg,uint8_t *pBuf,uint8_t ucLen);
void nvic_configuration(void);
void TIM2_Int_Init(uint16_t arr,uint16_t psc);

#endif
