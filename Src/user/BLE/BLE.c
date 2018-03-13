#include "ble.h"
#include "usart.h"
#include "timer.h"
#include "display.h"
#include "../src/user/type.h"
uint8_t recFlag=0;
extern  para_struct_type para_struct;
uint8_t bleSendBuff[40]={0};
uint8_t bleRecBuff[40]={0};
void mencpy(uint8_t *src,uint8_t *des,uint8_t len);
void menclr(uint8_t *str1,uint8_t len);
int8_t menFind(uint8_t *src,uint8_t *des,uint8_t len1,uint8_t len2);
uint8_t mencpr(uint8_t *str1,uint8_t *str2,uint8_t len);
uint8_t initBLE(void)
{
	uint8_t try_cnt=0;
	HAL_StatusTypeDef statuble;
	HAL_GPIO_WritePin(POWER_EN_GPIO_Port,POWER_EN_Pin,GPIO_PIN_SET);
	HAL_GPIO_WritePin(BLE_RST_GPIO_Port,BLE_RST_Pin,GPIO_PIN_SET);
	HAL_GPIO_WritePin(BLE_WKUP_GPIO_Port,BLE_WKUP_Pin,GPIO_PIN_RESET);	
	DelayTimer(100);
	mencpy("AT\r\n",bleSendBuff,sizeof("AT\r\n"));
	while(try_cnt<5)
	{	   DelayTimer(20);
			 HAL_UART_Transmit_DMA(&huart3,bleSendBuff,sizeof("AT\r\n")-1);
			 statuble = HAL_UART_Receive(&huart3,bleRecBuff,7,100);
				if(mencpr("AT+OK",bleRecBuff,sizeof("AT+OK")-1)==1)
				{
					try_cnt=0;
					menclr(bleRecBuff,40);
					break;
				}
				try_cnt++;
	}
	if(try_cnt==5)
	{
			para_struct.pageOneInfo.BLEflag=BLE_NO;
			HAL_GPIO_WritePin(BLE_WKUP_GPIO_Port,BLE_WKUP_Pin,GPIO_PIN_SET);	
		  return 0;
	}
	else
	{
		mencpy("AT+DEVID=?\r\n",bleSendBuff,sizeof("AT+DEVID=?\r\n"));
		while(HAL_UART_Transmit_DMA(&huart3,bleSendBuff,sizeof("AT+DEVID=?\r\n")-1)==HAL_OK);
		statuble = HAL_UART_Receive(&huart3,bleRecBuff,32,500);
		if(statuble==HAL_OK)
		{
			mencpy(&bleRecBuff[18],para_struct.pageTwoInfo.BHTMAC,12);
			STMFLASH_Write( FLASH_BASE+60*1024, (uint16_t *) &para_struct, sizeof(para_struct)/2 ); 
			para_struct.pageOneInfo.BLEflag=BLE_UNCONN;
		}
		menclr(bleRecBuff,40);
 	HAL_GPIO_WritePin(BLE_WKUP_GPIO_Port,BLE_WKUP_Pin,GPIO_PIN_SET);	
	}
	
return 1;
}
void bleGetDate(void)
{
	int8_t t=-1,ch=0x7e;
	menclr(bleRecBuff,40);
	HAL_GPIO_WritePin(BLE_WKUP_GPIO_Port,BLE_WKUP_Pin,GPIO_PIN_RESET);	
	HAL_UART_Receive(&huart3,bleRecBuff,20,50);	
	t=menFind(bleRecBuff,&ch,40,1);
	if(t!=-1)
	{		if(bleRecBuff[t+6]==getCheck(&bleRecBuff[t+1],5))
					para_struct.pageOneInfo.BLE_SendTotal = bleRecBuff[t+3];
			para_struct.pageOneInfo.BLE_SendInterval =  bleRecBuff[t+4]*256+ bleRecBuff[t+5];
	}
	menclr(bleRecBuff,40);
 
}
uint8_t bleCheckConn(void)
{
	int8_t t;
		HAL_GPIO_WritePin(BLE_WKUP_GPIO_Port,BLE_WKUP_Pin,GPIO_PIN_RESET);	
		 
		menclr(bleRecBuff,40);
		mencpy("AT+SYS_STATE=?\r\n",bleSendBuff,sizeof("AT+SYS_STATE=?\r\n"));
	  DelayTimer(2);
		HAL_UART_Transmit_DMA(&huart3,bleSendBuff,sizeof("AT+SYS_STATE=?\r\n")-1);
		HAL_UART_Receive(&huart3,bleRecBuff,20,50);
		HAL_GPIO_WritePin(BLE_WKUP_GPIO_Port,BLE_WKUP_Pin,GPIO_PIN_SET);	
		t=menFind(bleRecBuff,"AT+OK",40,5);
		if(t==-1)
			return 0;
			if( bleRecBuff[t+7]==0x35)
		{ 
			menclr(bleRecBuff,40);
			return 1;
		}
		else 
		{ 
			menclr(bleRecBuff,40);
			return 0;
		}
 
}
 

void sendInfo(void)
{
  HAL_GPIO_WritePin(BLE_WKUP_GPIO_Port,BLE_WKUP_Pin,GPIO_PIN_RESET);	
	DelayTimer(2);
	bleSendBuff[0]=0x7e;bleSendBuff[1]=0x0A;bleSendBuff[2]=0x11;
	bleSendBuff[3]=para_struct.pageOneInfo.airPress/256;	bleSendBuff[4]=para_struct.pageOneInfo.airPress%256;
	bleSendBuff[5]=(para_struct.pageOneInfo.batt/10)/256;	bleSendBuff[6]=(para_struct.pageOneInfo.batt/10)%256;
	bleSendBuff[7]=para_struct.pageOneInfo.temperature/10;
	bleSendBuff[8]=0;bleSendBuff[9]=0;bleSendBuff[10]=0;
	bleSendBuff[11]=getCheck(&bleSendBuff[1],10);
	HAL_UART_Transmit(&huart3,bleSendBuff,12,100);
 HAL_GPIO_WritePin(BLE_WKUP_GPIO_Port,BLE_WKUP_Pin,GPIO_PIN_SET);	
	
}
uint8_t getCheck(uint8_t * str,uint8_t len)
{
	uint8_t check=0;
	while(len)
	{
		check^=*str;
		str++;
		len--;
	}
	return check;
}
//蓝牙进程  不阻塞 100ms调用一次
void bleTask(void)
{
		static uint8_t bleTempStatus=0;
 
		if(para_struct.pageThreeInfo.BLE_Switch==2)
					{
							StartTimer(TIMER_BLE,2000);
							para_struct.pageOneInfo.BLEflag=BLE_NO;	
							return;
					}
		para_struct.pageOneInfo.BLEflag=BLE_UNCONN;	
		if(recFlag==1)
		{
			bleGetDate();
			recFlag=0;
		}
		if((bleTempStatus)==0)//断开
		{
			bleTempStatus=bleCheckConn();
			DelayTimer(2);
			para_struct.pageOneInfo.BLEflag=BLE_UNCONN;
			StartTimer(TIMER_BLE,2000);
		}
		else if((bleTempStatus)==1)//连接
		{
								
	  	para_struct.pageOneInfo.BLEflag=BLE_CONN;
	  	//发送协
	  	if(para_struct.pageOneInfo.BLE_SendTotal>0)
	  	{
	  		para_struct.pageOneInfo.BLE_SendTotal--;
	  		sendInfo();StartTimer(TIMER_BLE,para_struct.pageOneInfo.BLE_SendInterval);
	  	}
	  	else
	  	{
	  		  bleTempStatus=bleCheckConn();
					if((bleTempStatus)==1)
						sendInfo();
	  			StartTimer(TIMER_BLE,2000);
	  	}
								 
		}
 
}