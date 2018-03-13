#include "speak.h"
#include "usart.h"
#include "gpio.h"
#include "wir433.h"
#include "display.h"
#include "timer.h"
#include "../src/user/type.h"
extern  para_struct_type para_struct;
  unsigned int timenum;      //����Ҫ��volatile������  
  unsigned long time_delay; // ��ʱʱ�䣬ע�ⶨ��Ϊȫ�ֱ���  
  
//SysTickʵ����ʱn_ms���жϷ�ʽ  
void delay_ms(volatile unsigned long nms)  
{  
    //SYSTICK��Ƶ--1ms��ϵͳʱ���ж�  
    if (SysTick_Config(HAL_RCC_GetHCLKFreq()/1000))  
    {  
     
        while (1);  
    }  
    time_delay=nms;//��ȡ��ʱʱ��  
    while(time_delay);  
    SysTick->CTRL=0x00; //�رռ�����  
    SysTick->VAL =0X00; //��ռ�����  
}  
  
  

//  //SysTickʵ����ʱn_us���жϷ�ʽ  
//void delay_us(volatile unsigned long nus)  
//{  
// //SYSTICK��Ƶ--1us��ϵͳʱ���ж�  
//    if (SysTick_Config(HAL_RCC_GetHCLKFreq()/1000000))  
//    {  
//     
//        while (1);  
//    }  
//    time_delay=nus;//��ȡ��ʱʱ��  
//    while(time_delay);  
//    SysTick->CTRL=0x00; //�رռ�����  
//    SysTick->VAL =0X00; //��ռ�����  
//}
//SysTickʵ����ʱn_us���жϷ�ʽ  
void delay_us(volatile unsigned long nus)  
{  
  int a;
  while(nus)
  {
    a=35;
    while(a--);
    nus--;
  }
  return; 
}  
 void HAL_SYSTICK_Callback(void)
{
  /* NOTE : This function Should not be modified, when the callback is needed,
            the HAL_SYSTICK_Callback could be implemented in the user file
   */
	
	   if(time_delay)  
       time_delay--;   
}
/*--------------------------------------
;ģ������:Line_1A_WTN6(UC8 SB_DATA)
;�� ��:ʵ��һ�ߴ���ͨ�ź���
;�� ��: SB_DATA Ϊ��������
;�� ��:
;P_DATA Ϊ���ݿ�
;-------------------------------------*/
void Line_1A_WTN6( uint8_t SB_DATA)
	{
		uint8_t S_DATA;
		uint8_t B_DATA;
		S_DATA = SB_DATA;
		
		HAL_GPIO_WritePin(SPK_DATA_GPIO_Port,SPK_DATA_Pin,GPIO_PIN_RESET);
		__disable_irq();
		delay_us(50); //��ʱ 5ms
	
		for(uint8_t j=0;j<8;j++) 
		{
			B_DATA = S_DATA&0X01;
			if(B_DATA == 1)
			{
				HAL_GPIO_WritePin(SPK_DATA_GPIO_Port,SPK_DATA_Pin,GPIO_PIN_SET);
				delay_us(60); //��ʱ 600us
				HAL_GPIO_WritePin(SPK_DATA_GPIO_Port,SPK_DATA_Pin,GPIO_PIN_RESET);
				delay_us(20); //��ʱ 200us
			}
			else
			{
				HAL_GPIO_WritePin(SPK_DATA_GPIO_Port,SPK_DATA_Pin,GPIO_PIN_SET);
				delay_us(20); //��ʱ 200us
				HAL_GPIO_WritePin(SPK_DATA_GPIO_Port,SPK_DATA_Pin,GPIO_PIN_RESET);
				delay_us(60); //��ʱ 600us
			} 
			S_DATA = S_DATA>>1;
			
		} 
			  __enable_irq();
				HAL_GPIO_WritePin(SPK_DATA_GPIO_Port,SPK_DATA_Pin,GPIO_PIN_SET);
	}
//100ms����һ��   ��Լ��Դ
uint8_t spkTask(void)
{
	static  uint8_t statusSpeak = 0;
	static uint8_t playFlag=0;
		static uint8_t timer=0,step=0;
	static uint16_t stressLast=1000;
	timer++;
		switch(statusSpeak)
		{
			case 0: //��ѹ����10
				if(step == 0)
				{
					if(timer>100)
					{
						step=1;timer=0;
					}
				}
					else if(step ==1) 
					{
							if(para_struct.pageOneInfo.airPress<2000)
							{
								spkRunPlay(2);
								timer=0;
							}
							else
							{
								timer=30;
							}
							step=2;
					}
					else if(step == 2)
					{
						if(timer>30)
							{timer=0;
								step=0;
								statusSpeak=1;
							}
					}
				break;
			case 1: //��ѹС��10
					
							
					if(playFlag ==1)
					{
						if(step==0)
						{ 
							spkRunPlay(0);
							step++;timer=0;}
						else
						{
							if(timer>30)
							{
								spkRunPlay(0);
								playFlag=0;
								step=0;
								timer=0;
								statusSpeak=2;
							}
						}
					}
					else
					{	
							if(para_struct.pageOneInfo.airPress<1000)
							{
								playFlag = 1;step=0;
							}
					}
				break;
			case 2: //��ѹС��6
					if(playFlag ==1)
					{
						if(step==0)
						{ 
							spkRunPlay(1);
							step++;timer=0;}
						else
						{
							if(timer>30)
							{
								spkRunPlay(1);
								playFlag=0;
								step=0;
								timer=0;
								statusSpeak=3;
							}
						}
					}
					else
					{	
							if(para_struct.pageOneInfo.airPress<600)
							{
								playFlag = 1;step=0;
							}
					}
				break;
			case 3: //��ѹС��5
					if(para_struct.pageOneInfo.airPress>2000)
					{
						statusSpeak=0;
						playFlag=0;
						step=1;
						timer=100;
					}
				break;
		}
	
 

}	
void spkRunPlay(uint8_t playNum)
{
	Line_1A_WTN6(playNum);Line_1A_WTN6(playNum);
}
void setVoice(uint8_t voice)
{
		Line_1A_WTN6(0xe0+voice*4);
//		DelayTimer(50);
//		spkRunPlay(1);
//		DelayTimer(50);
}