 
#include "DS18B20.h"
#include "timer.h"
/**
  ******************************************************************************
  * @file    18B20\main.c
  * @author  MCD Application Team
  * @version V2.0.0
  * @date    25-February-2011
  * @brief   This file contains the main function for 18B20 example.
  ******************************************************************************
*/
  GPIO_InitTypeDef GPIO_InitStruct;
	uint32_t sign,interger,decimal;
	float get_tempeture;
//=========================DS18b20.c================================//
/******************************************
�������ƣ�GPIO_DQ_Out_Mode
��    �ܣ�����DQ����Ϊ��©���ģʽ
��    ������
����ֵ  ����
*******************************************/
void GPIO_DQ_Out_Mode(void)
{

//	DQ_GPIO->CRH&=0X0FFFFFFF;
//	DQ_GPIO->CRH|=0X20000000;
  GPIO_InitStruct.Pin = DQ_GPIO_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(DQ_GPIO, &GPIO_InitStruct);
}
/******************************************
�������ƣ�GPIO_DQ_Input_Mode
��    �ܣ�����DQ����Ϊ��������ģʽ
��    ������
����ֵ  ����
*******************************************/
void GPIO_DQ_Input_Mode(void)
{
	DQ_GPIO->CRH&=0X0FFFFFFF;
	DQ_GPIO->CRH|=0X40000000;
//  GPIO_InitStruct.Pin = DQ_GPIO_Pin;
//  GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
//  GPIO_InitStruct.Pull = GPIO_NOPULL;
//  HAL_GPIO_Init(DQ_GPIO, &GPIO_InitStruct);
}
/******************************************
�������ƣ�Tx_ResetPulse
��    �ܣ����͸�λ����
��    ������
����ֵ  ����
*******************************************/
void Tx_ResetPulse(void)
{
  GPIO_DQ_Out_Mode() ;
  DQ_Write_0() ;  //��λ����
  Delay_Nus(500) ; //���ٱ���480us
  DQ_Write_1() ;  //�����������ٶ�
  Delay_Nus(2) ;
}
/******************************************
�������ƣ�Rx_PresencePulse
��    �ܣ�����Ӧ���ź�
��    ������
����ֵ  ����
*******************************************/
void Rx_PresencePulse(void)
{
		uint8_t cnt=0;
    GPIO_DQ_Input_Mode() ;
    while( DQ_ReadBit()) 
		{
			Delay_Nus(2);
			cnt++;
			if(cnt>250)
			{
				break;
			}
		}  //�ȴ�DS18b20Ӧ��
    while( DQ_ReadBit() == 0)
		{
			Delay_Nus(2);
			cnt++;
			if(cnt>250)
			{
				break;
			}			
		}//DS18b20����������60~240us ,Ȼ��������������������
    Delay_Nus(100) ;
    GPIO_DQ_Out_Mode() ;    //������ɣ��������¿�������
}
/******************************************
�������ƣ�Write_OneByte_ToDS18b20
��    �ܣ�дһ���ֽڵ�DS18b20
��    ������
����ֵ  ����
*******************************************/
void Write_OneByte_ToDS18b20(unsigned char data)
{
    unsigned char i ;
    GPIO_DQ_Out_Mode() ;
    for(i=0 ;i<8 ;i++)
    {
        if(data&0x01)    //��λ��ǰ
        {
            //д1
            DQ_Write_0() ; //дʱ���϶���Ǵ����ߵĵ͵�ƽ��ʼ
            Delay_Nus(10) ;  //15us������
            DQ_Write_1() ;
            Delay_Nus(70) ; //����д1ʱ϶������60us
        }
        else
        {
            //д0
            DQ_Write_0() ;
            Delay_Nus(80) ; //������60us��120us֮��
            DQ_Write_1() ;
            Delay_Nus(5) ;
        }
        data >>= 1 ;
    }
}
/******************************************
�������ƣ�Read_OneByte_FromDS18b20
��    �ܣ���DS18b20��һ���ֽ�
��    ������
����ֵ  ������������
*******************************************/
unsigned char Read_OneByte_FromDS18b20(void)
{
    unsigned char i ,data = 0 ;
    
    for(i=0 ;i<8 ;i++)
    {
        GPIO_DQ_Out_Mode() ;
        data >>= 1 ;
        DQ_Write_0() ;
        Delay_Nus(2) ;
        GPIO_DQ_Input_Mode() ;
        Delay_Nus(10) ;
        if(DQ_ReadBit())
        {
            data |= 0x80 ;
        }
        Delay_Nus(40) ;   //�ȴ���һλ������ɴ���
    }
    
    return data ;
}
/******************************************
�������ƣ�Read_Temperature
��    �ܣ���ȡ�¶���Ϣ
��    ����*sign - ������ţ����ϻ����£�
          *integer - ������������
          *decimal - ����С������
����ֵ  ����
*******************************************/

float runTemperature(void)
{
    unsigned char a=0;
    unsigned char b=0;
    
    unsigned int tmp ;
    static uint8_t step=0;
	if(step==0)
	{
    DS18B20_Init();
		
		__disable_irq();
    Write_OneByte_ToDS18b20(ROM_Skip_Cmd);//���������кŲ���
    Write_OneByte_ToDS18b20(Convert_T); //�����¶�ת��
		__enable_irq();
		StartTimer(TIMER_DS18B20,1000);
		step=1;
	}
	else if(step==1)
	{
		if(ReadTimer(TIMER_DS18B20)==OK)
		{
			step=2;
		}
	}
	else if(step==2)
	{
		 __disable_irq();
    DS18B20_Init();
    Write_OneByte_ToDS18b20(ROM_Skip_Cmd);
    Write_OneByte_ToDS18b20(Read_Scratchpad); //��ȡ�Ĵ������ݣ����ԴӼĴ���0�����Ĵ���8��
    
    a= Read_OneByte_FromDS18b20();     //�¶ȵ�8λ
    b= Read_OneByte_FromDS18b20();     //�¶ȸ�8λ
    //c= Read_OneByte_FromDS18B20();   //TH
    //d= Read_OneByte_FromDS18B20();   //TL
    //e= Read_OneByte_FromDS18B20();   //Configuration Register
    
    //Tx_ResetPulse();  //�ж����ݶ�ȡ
    tmp = (b<<8) | a ;

    if(b & 0xF0)
    {
    sign = 1 ;              //���Ų���
    tmp = ~tmp+1 ;
    }
    else 
    {
    sign = 0 ;
    }
    interger = (tmp>>4) & 0x00FF;  //��������
    decimal = (tmp & 0x000F) * 625 ; //С������ 
    get_tempeture=interger*10+decimal/1000;
		step=0;
			__enable_irq();
		return get_tempeture;
		
	}
	return 10000;
}
/******************************************
�������ƣ�Write_EEPROM
��    �ܣ�д���ò���
��    ����Th - �����¶�����
          Tl - �����¶�����
          Register_Con - ���ƼĴ�����ֵ
����ֵ  ������������
*******************************************/
void Write_EEPROM(unsigned char Th,unsigned char Tl,unsigned char Register_Con )
{
  
    DS18B20_Init();
    Write_OneByte_ToDS18b20(ROM_Skip_Cmd);
    Write_OneByte_ToDS18b20(Write_Scratchpad);
    
    Write_OneByte_ToDS18b20(Th);//Th=7F
    Write_OneByte_ToDS18b20(Tl);//Tl=FF ���λ����λ
    Write_OneByte_ToDS18b20(Register_Con);//12λģʽ
    
    Delay_Nms(700);
    DS18B20_Init();
    Write_OneByte_ToDS18b20(ROM_Skip_Cmd);
    Write_OneByte_ToDS18b20(Copy_Scratchpad);//���Ĵ���������ֵд��EEPROM
    
    Delay_Nms(300);
 
}
/******************************************
�������ƣ�DS18B20_Init
��    �ܣ���ʼ��DS18b20
��    ������
����ֵ  ����
*******************************************/
void DS18B20_Init(void)
{
//	while(1)
	{
		  Tx_ResetPulse();
	}
  
    Rx_PresencePulse(); 
}

void Delay_Nus(uint16_t us)
{us*=3;
	while(--us);
}

void Delay_Nms(uint16_t ms)
{
	while(ms--)
		{
		Delay_Nus(1000);
	}
}

float get_data(void)
{
	return get_tempeture;
}
