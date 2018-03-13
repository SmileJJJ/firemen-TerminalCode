#include "display.h"
#include "oled.h"
#include "../src/user/type.h"
extern  para_struct_type para_struct;
void showPageOne(pageOneInfoType *pageOneInfo)
{
	static uint8_t blinkCnt=0;
	blinkCnt++;
	if(pageOneInfo->HUDflag==HUD1)
	{
		LCD_P8x16Str(0,0,"HUD1");
	}
	else if(pageOneInfo->HUDflag==HUD2)
	{
			LCD_P8x16Str(0,0,"HUD2");
	}
	if(pageOneInfo->BLEflag==BLE_CONN)
	{
		Draw_BMP(56,0,63,1,BLE);  	
	}
	else if(pageOneInfo->BLEflag==BLE_UNCONN)
	{
		
			if(((blinkCnt/10)%2)==0)
			{
				Draw_BMP(56,0,63,1,BLE); 
			}
			else
			{
				Draw_BMP(56,0,63,1,NO_BLE); 
			}
	}
	else if(pageOneInfo->BLEflag==BLE_NO)
	{
			Draw_BMP(56,0,63,1,NO_BLE);  	
	}
	if(pageOneInfo->DATAflag==DATA_CONN)
	{
		LCD_P8x16Str(80,0,"DATA");
	}
	else if(pageOneInfo->DATAflag==DATA_UNCONN)
	{
			if(((blinkCnt/10)%2)==0)
			{
					LCD_P8x16Str(80,0,"DATA");
			}
			else
			{
					LCD_P8x16Str(80,0,"    ");
			}
	}
	else if(pageOneInfo->DATAflag==DATA_NO)
	{
		LCD_P8x16Str(80,0,"    ");
	}
	LCD_Print(0,2,"温度");
	LCD_Print(92,2,"C");
	LCD_Print(72,2,".");
	LCD_Print(0,4,"气压");	
  LCD_Print(72,4,".");	
	LCD_Print(96,4,"Mpa");	
	LCD_Print(0,6,"电量");
//		LCD_Print(72,6,"%");
	LCD_Print(102,6,"V");

 	Dis_Num(2,7,pageOneInfo->temperature/10,2);
	Dis_Num(2,10,pageOneInfo->temperature%10,1);
	
	 
	
 	Dis_Num(4,7,pageOneInfo->airPress/100,2);
	Dis_Num(4,10,pageOneInfo->airPress%100/10,1);
	
	
	
 	Dis_Num(6,14,pageOneInfo->voiceLevel,1);
	Draw_Power(pageOneInfo->batt/10,0);
	 
}

void showPageTwo(pageTwoInfoType *pageTwoInfo)
{
		LCD_P8x16Str(0,0,"SN");LCD_P8x16Str(48,0,pageTwoInfo->SN);
		LCD_P8x16Str(0,2,"HUD2");
		if(para_struct.pageOneInfo.HUDflag==2)
			LCD_P8x16Str(48,2,pageTwoInfo->HUD);
		else
			LCD_P8x16Str(48,2,"00000000");
		LCD_P8x16Str(0,4,"BTH");LCD_P8x16Str(32,4,pageTwoInfo->BHTMAC);
		LCD_P8x16Str(0,6,"GP");LCD_P8x16Str(32,6,pageTwoInfo->GP);
	  
}

void showPageThree(pageThreeInfoType *pageThreeInfo)
{
		LCD_P8x16Str(0,0,"Ver");LCD_P8x16Str(48,0,pageThreeInfo->Version);
		LCD_P8x16Str(0,2,"DATE");LCD_P8x16Str(48,2,pageThreeInfo->DATE);
}
void showPageSetSel(uint8_t selNum)
{
	LCD_Print(100,(selNum-1)*2,"《=");
	LCD_Print(0,0,"1.HUD模式选择");
	LCD_Print(0,2,"2.蓝牙开关");
	LCD_Print(0,4,"3.数据通信");
	LCD_Print(0,6,"4.气压标定");
}
void showPageSetHUD(uint8_t selNum)
{
	LCD_Print(100,(selNum-1)*2,"《=");
	if(para_struct.pageOneInfo.HUDflag==1)
	{LCD_P8x16Str(90,0,"*");		LCD_P8x16Str(90,2," ");}
	else
	{LCD_P8x16Str(90,2,"*");LCD_P8x16Str(90,0," ");}
	LCD_Print(0,0,"1.HUD1");
	LCD_Print(0,2,"2.HUD2");
}
void showPageSetDATA(uint8_t selNum)
{
	LCD_Print(100,(selNum-1)*2,"《=");
	if(para_struct.pageThreeInfo.DateSwitch==1)
	{LCD_P8x16Str(90,0,"*");		LCD_P8x16Str(90,2," ");}
	else
	{LCD_P8x16Str(90,2,"*");LCD_P8x16Str(90,0," ");}
	LCD_Print(0,0,"1.数据通信开");
	LCD_Print(0,2,"2.数据通信关");
}
void showPageSetBLE(uint8_t selNum)
{
	LCD_Print(100,(selNum-1)*2,"《=");
	if(para_struct.pageThreeInfo.BLE_Switch==1)
	{LCD_P8x16Str(90,0,"*");		LCD_P8x16Str(90,2," ");}
	else
	{LCD_P8x16Str(90,2,"*");LCD_P8x16Str(90,0," ");}
 
	LCD_Print(0,0,"1.蓝牙开");
	LCD_Print(0,2,"2.蓝牙关");
}
void showPageSetCal(uint8_t selNum)
{
	LCD_Print(100,(selNum-1)*2,"《=");
	LCD_Print(0,0,"F0 0Mpa 标定");
	LCD_Print(0,2,"F1 10Mpa 标定");
	LCD_Print(0,4,"F2 30Mpa 标定");
	LCD_Print(0,6,"F3");
 	LCD_Print(24,6,para_struct.pageTwoInfo.SN);
}
void changeSN(char *SN,uint8_t lightNum)
{

	LCD_Print(24,6,SN);
	Dis_Num_Inv(6,11-lightNum,SN[8-lightNum]-0x30,1);
	//LCD_P8x16Str_Invers(24+(8-lightNum)*8,6,SN[8-lightNum]);
}
void CLR_Arr(void)
{
		LCD_Print(100,0,"   ");
			LCD_Print(100,2,"   ");
			LCD_Print(100,4,"   ");
			LCD_Print(100,6,"   ");
}