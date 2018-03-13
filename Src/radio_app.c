#include "./radio_app.h"
#include "./radio.h"
#include "display.h"
#include "led.h"
uint8_t statusRadio=RADIO_RUN;
Hud1Typede Hud1Struct;
Hud2Typede Hud2Struct;
extern  para_struct_type para_struct;
//根远传一起 100ms调用一次
uint32_t addrLocal ;
uint32_t addrPromote ;
uint8_t radioAppInit(void)
{
	si446x_part_info();
 	vRadio_Init();
 	vRadio_StartRX(2,9);
	//获取地址数据
	addrLocal=strToInt( para_struct.pageTwoInfo.SN,8);
	addrPromote=strToInt( para_struct.pageTwoInfo.HUD,8);
}
void packHUD2(uint8_t headet)
	{
		Hud2Struct.header = headet;
		Hud2Struct.addrLocal[0]= (addrLocal>>16)&0XFF;
		Hud2Struct.addrLocal[1]= (addrLocal>>8)&0XFF;
		Hud2Struct.addrLocal[2]= (addrLocal>>0)&0XFF;
	  Hud2Struct.addrPromote[0]= (addrPromote>>16)&0XFF;
		Hud2Struct.addrPromote[1]= (addrPromote>>8)&0XFF;
		Hud2Struct.addrPromote[2]= (addrPromote>>0)&0XFF;
		Hud2Struct.stress[0] = 	para_struct.pageOneInfo.airPress / 256;
		Hud2Struct.stress[1] = 	para_struct.pageOneInfo.airPress % 256;
		if(para_struct.pageOneInfo.temperature>=0)
		{
			Hud2Struct.temperature = para_struct.pageOneInfo.temperature/10;
		}
		else
		{
			Hud2Struct.temperature = (-para_struct.pageOneInfo.temperature)/10;
			Hud2Struct.temperature |= 0x80;
		}
		Hud1Struct.voltage = para_struct.pageOneInfo.batt;	
		Hud2Struct.checkSum = getCheck((uint8_t *)&Hud2Struct,11);
}
void packHUD1(uint8_t headet)
	{
		Hud1Struct.header = headet;
		Hud1Struct.addrLocal[0]= (addrLocal>>16)&0XFF;
		Hud1Struct.addrLocal[1]= (addrLocal>>8)&0XFF;
		Hud1Struct.addrLocal[2]= (addrLocal>>0)&0XFF;
		Hud1Struct.stress[0] = 	para_struct.pageOneInfo.airPress / 256;
		Hud1Struct.stress[1] = 	para_struct.pageOneInfo.airPress % 256;
		if(para_struct.pageOneInfo.temperature>=0)
		{
			Hud1Struct.temperature = para_struct.pageOneInfo.temperature/10;
		}
		else
		{
			Hud1Struct.temperature = (-para_struct.pageOneInfo.temperature)/10;
			Hud1Struct.temperature |= 0x80;
		}
		Hud1Struct.voltage = para_struct.pageOneInfo.batt/20;	
		Hud1Struct.checkSum = getCheck((uint8_t *)&Hud1Struct,8);
}
void radioPair(void)
{
	statusRadio= RADIO_PAIR;
}
uint8_t getPairStatus(void)
{
	if(statusRadio== RADIO_PAIR)
		return 0;
	else
		return 1;
}
uint8_t radioTask(void)
{
	static uint16_t timer =0 ;
 	static uint8_t step=0;
	timer++;
	static uint8_t cnt = 0;
		switch(statusRadio)
		{
			case  RADIO_IDLE:
						
				break;
			case  RADIO_RUN:
							if(timer>20)
							{
								//2秒发送一次数据
								timer=0;
								if(para_struct.pageOneInfo.HUDflag==HUD1)
								{
										packHUD1(0x5a);
										vRadio_StartTx_Variable_Packet(2, (uint8_t *)&Hud1Struct, 9);
								}
								else if(para_struct.pageOneInfo.HUDflag==HUD2)
								{
										packHUD2(0x5a);
										vRadio_StartTx_Variable_Packet(2, (uint8_t *)&Hud2Struct, 12);
								}
							}
				break;
			case  RADIO_PAIR:
							if(timer>5) 
							{
								
								timer=0;
								cnt++;
								if(para_struct.pageOneInfo.HUDflag==HUD1)
								{
										packHUD1(0x6a);
										vRadio_StartTx_Variable_Packet(2, (uint8_t *)&Hud1Struct, 9);
								}
								else if(para_struct.pageOneInfo.HUDflag==HUD2)
								{
										packHUD2(0x6a);
										vRadio_StartTx_Variable_Packet(2, (uint8_t *)&Hud2Struct, 12);
								}
								//0.5秒发送一次数据配对
								if(cnt>15)
								{
									cnt=0;
									statusRadio=RADIO_RUN;
									stopLed(LED_BLE);
									
								}
							}
				break;
			case  RADIO_ERR:
				break;
			default:break;
		}
}
