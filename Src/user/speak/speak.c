#include "speak.h"
#include "usart.h"
#include "gpio.h"
#include "wir433.h"
#include "display.h"
#include "timer.h"
#include "../src/user/type.h"
extern  para_struct_type para_struct;
  unsigned int timenum;      //必须要加volatile！！！  
  unsigned long time_delay; // 延时时间，注意定义为全局变量  
  
//SysTick实现延时n_ms，中断方式  
void delay_ms(volatile unsigned long nms)  
{  
    //SYSTICK分频--1ms的系统时钟中断  
    if (SysTick_Config(HAL_RCC_GetHCLKFreq()/1000))  
    {  
     
        while (1);  
    }  
    time_delay=nms;//读取定时时间  
    while(time_delay);  
    SysTick->CTRL=0x00; //关闭计数器  
    SysTick->VAL =0X00; //清空计数器  
}  
  
  

//  //SysTick实现延时n_us，中断方式  
//void delay_us(volatile unsigned long nus)  
//{  
// //SYSTICK分频--1us的系统时钟中断  
//    if (SysTick_Config(HAL_RCC_GetHCLKFreq()/1000000))  
//    {  
//     
//        while (1);  
//    }  
//    time_delay=nus;//读取定时时间  
//    while(time_delay);  
//    SysTick->CTRL=0x00; //关闭计数器  
//    SysTick->VAL =0X00; //清空计数器  
//}
//SysTick实现延时n_us，中断方式  
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
;模块名称:Line_1A_WTN6(UC8 SB_DATA)
;功 能:实现一线串口通信函数
;入 参: SB_DATA 为发送数据
;出 参:
;P_DATA 为数据口
;-------------------------------------*/
void Line_1A_WTN6( uint8_t SB_DATA)
	{
		uint8_t S_DATA;
		uint8_t B_DATA;
		S_DATA = SB_DATA;
		
		HAL_GPIO_WritePin(SPK_DATA_GPIO_Port,SPK_DATA_Pin,GPIO_PIN_RESET);
		__disable_irq();
		delay_us(50); //延时 5ms
	
		for(uint8_t j=0;j<8;j++) 
		{
			B_DATA = S_DATA&0X01;
			if(B_DATA == 1)
			{
				HAL_GPIO_WritePin(SPK_DATA_GPIO_Port,SPK_DATA_Pin,GPIO_PIN_SET);
				delay_us(60); //延时 600us
				HAL_GPIO_WritePin(SPK_DATA_GPIO_Port,SPK_DATA_Pin,GPIO_PIN_RESET);
				delay_us(20); //延时 200us
			}
			else
			{
				HAL_GPIO_WritePin(SPK_DATA_GPIO_Port,SPK_DATA_Pin,GPIO_PIN_SET);
				delay_us(20); //延时 200us
				HAL_GPIO_WritePin(SPK_DATA_GPIO_Port,SPK_DATA_Pin,GPIO_PIN_RESET);
				delay_us(60); //延时 600us
			} 
			S_DATA = S_DATA>>1;
			
		} 
			  __enable_irq();
				HAL_GPIO_WritePin(SPK_DATA_GPIO_Port,SPK_DATA_Pin,GPIO_PIN_SET);
	}
//100ms调用一次   节约资源
uint8_t spkTask(void)
{
	static  uint8_t statusSpeak = 0;
	static uint8_t playFlag=0;
		static uint8_t timer=0,step=0;
	static uint16_t stressLast=1000;
	timer++;
		switch(statusSpeak)
		{
			case 0: //气压大于10
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
			case 1: //气压小于10
					
							
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
			case 2: //气压小于6
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
			case 3: //气压小于5
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