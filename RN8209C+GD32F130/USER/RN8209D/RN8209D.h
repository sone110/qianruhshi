#ifndef __RN8209D_H_
#define __RN8209D_H_
#include "gd32f1x0.h"
#include "stdio.h"
#include "stdlib.h"
#include "systick.h"
#include "usart.h"


#define USART_REC_LEN 200
extern uint8_t 		USART2_RxBuf[USART_REC_LEN] ;        //���ͻ�����
extern uint8_t		USART2_TxBuf[USART_REC_LEN] ;        //���ջ�����
extern   uint8_t   USART2_RX_STA  ;
/*****
*BIT7:����������ɱ�־
*BIT6-0���������ݳ���
**/
 extern uint8_t   USART2_RX_STA ;


//----------------------start RN8209  ��ַ����---------------------------------------------------//
#define					ADSYSCON        0x00       //ϵͳ���ƼĴ�����д����
#define        	ADEMUCON        0x01       //�������ƼĴ���
#define        	ADHFConst     	0x02       //����Ƶ�ʼĴ���
#define        	ADPStart      	0x03       //�й��𶯹�������
#define        	ADDStart      	0x04       //�Զ�������𶯹�������
#define					ADGPQA        	0x05       //ͨ��A��������У���Ĵ���
#define        	ADGPQB        	0x06       //ͨ��B��������У���Ĵ���
#define        	ADPhsA        	0x07       //ͨ��A��λУ���Ĵ���
#define        	ADPhsB        	0x08       //ͨ��B��λУ���Ĵ���
#define					ADQPHSCAL				0x09       //�޹���λ����
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
#define					ADPFCnt       	0x20         //�����й��������    
#define        	ADDFcnt       	0x21         //�Զ�����ܿ����������
#define        	ADIARMS       	0x22         //ͨ��A������Чֵ
#define        	ADIBRMS       	0x23         //ͨ��B������Чֵ
#define        	ADURMS        	0x24         //��ѹ��Чֵ
#define					ADUFreq       	0x25         //��ѹƵ��
#define        	ADPowerPA     	0x26         //�й�����A
#define        	ADPowerPB     	0x27         //�й�����B
#define         ADEnergyP  	    0x29         //�޹�����
#define         ADEnergyP2 	    0x2a          //�й�����
#define         ADEnergyD     	0x2b          
#define         ADEnergyD2    	0x2c
#define         ADEMUStatus   	0x2d
#define         ADSPL_IA      	0x30         //IAͨ��ADC����
#define         ADSPL_IB      	0x31         //IBͨ��ADC����
#define         ADSPL_U       	0x32         //Uͨ��ADC����
#define         ADIE  		      0x40         //�ж�����Ĵ���
#define         ADIF  		      0x41         //�жϱ�־�Ĵ���
#define         ADRIF         	0x42         //��λ�жϱ�־�Ĵ���
#define         ADSysStatus  	  0x43         //ϵͳ״̬�Ĵ���
#define         ADRData      	  0x44         //��һ��SPI/USART����������
#define         ADWData        	0x45         //��һ��SPI/USARTд�������
#define         ADDeviceID    	0x7f         //Device ID
#define         WriteEn   	    0xea          //��������Ĵ���    
//----------------------end RN8209  ��ַ����-----------------------------------------------//
//---start У������ļ�-------
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
	float		KUrms;								// ��ѹϵ��
	float		KIArms;								// Aͨ������ϵ��
	float		KIBrms;								// Bͨ������ϵ��
	float		KPrms;								// ����ϵ��
	uint16_t			RealUI[2];						// ������ʾֵ�����ʴ��ڴ�ֵʱ��ʾ 0.2%
	uint32_t			RealPw;								// ������ʾֵ���������ڴ�ֵʱ��ʾ 0.2%
	uint32_t			ChkSum;             	
	uint16_t			RTCDota0;							// RTCУ���Ĵ���
	uint8_t			TemperAdj[2];					// �ߵ��²���ֵ
	uint8_t			RTCAdj[4];						// RTC�ߵ���ʱ����ֵ
	uint8_t			CurAdj;								// ����Ӱ�첹��ֵ
	uint8_t 			OfsetAdjAcVolt[2]; 		//���ݵ�ѹ����OFFSETֵ
	uint16_t			CorrectionTemper;  		//У��ʱ�̱�Ƶ��¶�
} sDl645FirmParaFile_TypeDef;		//58 Byte
//---end У������ļ�-------


//---start ����ֵ˲ʱֵ��ת-------
typedef struct
{
  uint8_t		ChkErrCnt;
  uint32_t 	Pw[2];   		    //pa,pb   
  uint32_t 	UI[3];          // Ia=UI[0] Inal U         
  uint16_t 	Frequency;   		//Ƶ��                            	
  uint32_t		Pulse;		    	//ǰ̨����
  uint16_t     Pstart;
  //---��������---	
  uint32_t		Pulse_Eg;	    	//�������
  uint32_t 	PDirect;				//���ʷ���
  uint32_t 	ChkSum1;				//����EMU��У�����У��
  // У��ʹ�ò���
  uint16_t		RatintU;				// ���ѹ
	uint16_t		RatingI;				// �����
  uint32_t		TempU;					// ��ǰУ�����ѹ
  uint32_t		TempI;					// ����У�������
  uint32_t		TempPw;					// ��ǰУ���㹦��
} sDl645FrontTmp_TypeDef;
//---end ����ֵ˲ʱֵ��ת-------

//---start ����ֵ˲ʱֵ�ļ�-------
typedef struct
{	
	struct sDl645FrontPubData_TypeDef  
	{
		uint16_t			U;			    		//---��ѹ---NNN.N
		uint32_t	    Ia;			    		//---����NNNN.NNNN(����ֵҪ��3��3С������ֵҪ��2��4С�����λ��ʾ����)---		
		uint32_t	    In;         		//---���ߵ���
		uint32_t	Pw;			    		    //---˲ʱ�й�p
		uint16_t			Pf;			    		//---��������N.NNN---	���λ��ʾ����{Pf Pfa Pfb Pfc}	
		uint16_t			Angle;		  		//---���NNN.N---		
		uint16_t			Frequency;			//---Ƶ��NN.NN
		uint32_t			PPwave;					//---һ����ƽ������NN.NNNN
		uint8_t      Chnsel;     		
		uint16_t			Temperature;		//---NNN.N  �¶�
		uint16_t			ClockBat;				//---NN.NN  ��ص�ѹ
		uint32_t			tWorkBat;				//---NNNN   ʱ�ӹ���ʱ�䣨���ӣ�
		uint8_t			PDirect;				//---ԭ���ʷ���
		                    		
	  uint16_t    	CfIn; 					//���������ƽ�ж�
	  uint8_t    	CfTime;					//
	  uint8_t    	Step;       		
	  uint16_t   	FrontStamp; 		
	  uint16_t			tMaxI;					// ����������ʱ��
	  uint8_t			SafeCurFlag;		// �������޲�Ϊ���־
	} PubData;
	
	struct sDl645FrontPriData_TypeDef  
	{		
		uint8_t			Flag;						//---�����쳣��־---
	} PriData;	
	
	struct sDl645FrontPriPara_TypeDef  
	{		
		uint32_t	 PConstE;						//---�й�����---
		uint16_t	 Crc;
	} PriPara;		
} sDl645Front_TypeDef;	
//---end ����ֵ˲ʱֵ-------

//---start ����ͨ�ű����ļ�-------
typedef struct
{
	uint16_t		EFlag;							//ͨѶ״̬
	
  uint16_t 	RxLen;							//�������ݳ���
  uint16_t  	TxLen;              // �������ݳ���
  uint32_t		TimeOutStamp;				//��ʱ
  uint8_t 		*pTx;
  
  uint8_t		fBps;								//�����ʱ����־
  uint8_t		NewBps;							
  uint32_t 	NewBpsStamp;				
//uint8_t 		TxAddr;
  

}sComPack_TypeDef;
//---end ����ͨ�ű����ļ�-------
ErrStatus RN8209D_WriteData(uint8_t wReg,uint8_t *pBuf,uint8_t ucLen) ;
void  USART1_SendData( uint8_t* data , int length) ;
void RN8209_Init(void);
ErrStatus RN8209D_ReadData(uint8_t wReg,uint8_t *pBuf,uint8_t ucLen);
void nvic_configuration(void);
void TIM2_Int_Init(uint16_t arr,uint16_t psc);

#endif
