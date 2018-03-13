#include "WIR433.H"
#include "usart.h"
#include "timer.h"
#include "display.h"
#include "ble.h"
#include "speak.h"
#include "oled.h"
////////////////////类型

//变量
paraDataType paraDataTemp;
extern  para_struct_type para_struct;
framType framStruct;
framType cmdBuff[5]={0};
uint8_t cmdBuffIndex =0;
uint8_t wir433mode =0;
uint8_t wir433SendBuff[30]={0};
uint8_t wir433RecBuff[30]={0};
void returnDate(uint8_t *retBuff ,uint8_t *gpSrc);
uint8_t connCnt=0;
uint8_t statusWir433=STATUS_WIR_IDLE;
uint8_t cmdRecevied=0;
//内部函数
framType *cmdSolve(framType *frame);
void writeGPToFlash(framType *cmdTemp);
void writeGPToRam(framType *cmdTemp);
uint8_t * IntTostr(uint32_t gp, uint8_t len );
void reply(uint8_t cmd);
void wirTurnOff(void);
void replySet2( framType *cmdTemp);
void setMode(uint8_t mode);
uint32_t strToInt(uint8_t * buff, uint8_t len );
void upLoad(void);
void replyGroup( uint8_t *gp);
uint8_t pushCMD(framType *framStruct)
{
	if(cmdBuffIndex<MAX_CMD)
	{
		cmdBuff[cmdBuffIndex]=*framStruct;
		cmdBuffIndex++;
		return cmdBuffIndex;
	}
	return 0;
}
framType *  popCMD(void)
{
	if(cmdBuffIndex>0)
	{
		cmdBuffIndex--;
		return &cmdBuff[cmdBuffIndex];
	}
	else
	{
		return NULL;
	}
}
void init_WIR433(uint8_t channal)
{
	
	if(para_struct.pageThreeInfo.DateSwitch==2)
		{
			para_struct.pageOneInfo.DATAflag = DATA_NO;
			return ;
		}
	uint8_t cnt= 1;
	(&huart2)->Init.BaudRate = 9600;
	(&huart2)->Instance->BRR = UART_BRR_SAMPLING16(HAL_RCC_GetPCLK1Freq(), (&huart2)->Init.BaudRate);
 
	setMode(3);
	wir433SendBuff[0]=0XC1;
	wir433SendBuff[1]=0Xc1;	wir433SendBuff[2]=0Xc1;	
	HAL_UART_Transmit(&huart2,wir433SendBuff,3,200);	
	HAL_UART_Receive(&huart2,wir433RecBuff,6,200);
	
	HAL_UART_AbortReceive(&huart2);
	wir433SendBuff[0]=0XC0;
	wir433SendBuff[1]=0Xff;	wir433SendBuff[2]=0Xff;	
  wir433SendBuff[3] =	((para_struct.pageThreeInfo.serialRate-1)<<3) + para_struct.pageThreeInfo.airRate;  
	wir433SendBuff[4]=channal;
	wir433SendBuff[5]=0X40;
	if(	para_struct.pageThreeInfo.FFC == 0)
	{
		wir433SendBuff[5]&=~0x04;
	}
	else
	{
		wir433SendBuff[5]|=0x04;
	}
	wir433SendBuff[5]+=para_struct.pageThreeInfo.poweSet;
	
	
	HAL_UART_Transmit(&huart2,wir433SendBuff,6,500);
	HAL_UART_Receive(&huart2,wir433RecBuff,6,500);
	if(mencpr(wir433SendBuff,wir433RecBuff,6)==1)
	{
		para_struct.pageOneInfo.DATAflag=DATA_UNCONN;
	}
	else
	{
		para_struct.pageOneInfo.DATAflag = DATA_NO;
	}
	wir433SendBuff[0]=0XC1;
	wir433SendBuff[1]=0Xc1;	wir433SendBuff[2]=0Xc1;	
	HAL_UART_Transmit(&huart2,wir433SendBuff,3,200);	
	HAL_UART_Receive(&huart2,wir433RecBuff,6,200);
	(&huart2)->Init.BaudRate = 1200;
	while(para_struct.pageThreeInfo.serialRate>cnt)
	{
		(&huart2)->Init.BaudRate*=2;
		cnt++;
	}
	(&huart2)->Instance->BRR = UART_BRR_SAMPLING16(HAL_RCC_GetPCLK1Freq(), (&huart2)->Init.BaudRate);
	setMode(0);
  HAL_UART_Receive_DMA(&huart2,wir433RecBuff,2);
	framStruct.head[0]=0x5a;
	framStruct.head[1]=0xa5;
}

void setMode(uint8_t mode)
{
	wir433mode=mode;
	DelayTimer(200);
	HAL_GPIO_WritePin(WIR433_M0_GPIO_Port,WIR433_M0_Pin,(GPIO_PinState)(mode&0x01));
	HAL_GPIO_WritePin(WIR433_M1_GPIO_Port,WIR433_M1_Pin,(GPIO_PinState)(mode&0x02));
	DelayTimer(200);
}
	//无线任务进程  100ms一次 不阻塞
	static uint16_t timeCounter=0;
	static uint8_t step=0;
void wirReg(void);
//外部调用程序
void turnOnWir433(void)
{
				statusWir433=STATUS_WIR_REG;timeCounter=1;step=0;
}
void turnOffWir433(void){
				statusWir433=STATUS_WIR_OFF;timeCounter=1;step=0;
}

uint8_t getWirStatus()
{
	return statusWir433;
}
uint8_t wir433Task(void)
{
		framType *cmdTemp;
		timeCounter++;
		cmdRecevied=0;
		static uint8_t failCounter= 0;
		if(para_struct.pageThreeInfo.DateSwitch==2)
		{
			return 0;
		}
		if(para_struct.pageOneInfo.DATAflag==DATA_NO)
		{
			return 0;
		}
		cmdTemp= cmdSolve(popCMD());
		
		switch(statusWir433)
		{
			case STATUS_WIR_OFF: 
					if(step==0)
						{
							wirTurnOff();
							step=1;
						}
						else if(step==1)
						{
							wirTurnOff();
							step=2;timeCounter=0;
						}
						else if(step==2)
						{
							if(timeCounter%50==0)
							{
								step=0;
								failCounter++;
								if(failCounter == 2)
								{
									statusWir433=STATUS_WIR_IDLE;
								}
							}
							if(cmdRecevied==CMD_REGISIT_RE)
							{
								step=0;
								statusWir433=STATUS_WIR_IDLE;
								timeCounter=0;
							}
						}
				break;
			case STATUS_WIR_REG:
					if(step==0)
					{
						wirReg();
						step=1;
					}
					else if(step==1)
					{
						wirReg();
						step=2;timeCounter=0;
					}
					else if(step==2)
					{
						if(timeCounter>=50)//5s
						{
							step=0;
						}
						if(cmdRecevied==CMD_REGISIT_RE)
						{
							step=0;
							statusWir433=STATUS_WIR_RUN;
							para_struct.pageOneInfo.DATAflag = DATA_CONN;
							timeCounter=0;
						}
						if(cmdTemp->paraData.downLoadOutGroup_Group.deviceType==0x12){
							 if(cmdRecevied==CMD_GROUP)//临时组队
								{
									replyGroup(cmdTemp->paraData.downLoadOutGroup_Group.serialNumCtr);
									writeGPToRam(cmdTemp);
									step=0;
									statusWir433=STATUS_WIR_REG;
									timeCounter=0;
								}
							else if(cmdRecevied==CMD_SLAVE_EXCHANGE)//切换
								{
									reply(CMD_SLAVE_EXCHANGE_RE);
									init_WIR433(cmdTemp->paraData.downLoadOutGroup_Exc.desChannal);
									step=0;
									timeCounter=0;
								}
						}
					}
				break;
			case STATUS_WIR_RUN: 
					 
						if(step==0)
						{
								if(para_struct.pageThreeInfo.DateSwitch==1&&(timeCounter>=80))//数据开  10秒上传一次
								{
									 timeCounter=0;
									 step=1;
									 upLoad();
								}
						}
						else if(step==1)
						{
							if(cmdRecevied==CMD_UPLOAD_RE)
								{
									timeCounter=0;
									step=0;
									para_struct.pageOneInfo.DATAflag = DATA_CONN;
								}
							if(timeCounter>20)//等待响应500ms
							{	 
								para_struct.pageOneInfo.DATAflag = DATA_UNCONN;
								timeCounter=0;
								step=0;
							}
						}
							if(cmdRecevied==CMD_SET1)
								{
									statusWir433=STATUS_WIR_SET_ADD;
									step=0;
								}
							if(cmdRecevied==CMD_SET2)
								{
									statusWir433=STATUS_WIR_SET_PARA;
									step=0;
								}					
						 
				break;
			case STATUS_WIR_SET_ADD: 
						if(step==0)
						{
							timeCounter=0;
							step=1;
						}
						else  
						{
							if(cmdRecevied==CMD_SET1)
								{
									step++;
									if(step==3)
									{
										statusWir433=STATUS_WIR_RUN;
										step=0;timeCounter=0;
										//接收三次 
										writeGPToFlash(cmdTemp);
										reply(CMD_SET1_RE);
									}
								}
							if(timeCounter>10)
									{
										statusWir433=STATUS_WIR_RUN;
										step=0;timeCounter=0;
									}
						}
				break;
			case STATUS_WIR_SET_PARA: 
						if(step==0)
						{
							timeCounter=0;
							step=1;
						}
						else  
						{
							if(cmdRecevied==CMD_SET2)
								{
									step++;
									if(step==3)
									{
										statusWir433=STATUS_WIR_RUN;
										step=0;timeCounter=0;
										//接收三次  返回保存
								  	replySet2(cmdTemp);
									 
									}
								}
							if(timeCounter>10)
									{
										statusWir433=STATUS_WIR_RUN;
										step=0;timeCounter=0;
									}
						}
				break;
			case STATUS_WIR_IDLE: 
				break;
			case STATUS_WIR_ERR: 
				break;		
		}
}
void wirTurnOff(void)
{
	framStruct.cmd = CMD_TURNOFF;
	framStruct.direct=0x20;
	framStruct.path =0xc0;
	returnDate((uint8_t *)&framStruct.paraData,para_struct.pageTwoInfo.GP);
	framStruct.verify =  getCheck((uint8_t *)& framStruct,sizeof(framStruct)-1);
	HAL_UART_Transmit_DMA(&huart2,(uint8_t *)&framStruct,sizeof(framStruct));
}
void wirReg(void)
{
	framStruct.cmd = CMD_REGISIT;
	framStruct.direct=0x20;
	framStruct.path =0xc0;
	returnDate((uint8_t *)&framStruct.paraData,para_struct.pageTwoInfo.GP);
	framStruct.verify =  getCheck((uint8_t *)& framStruct,sizeof(framStruct)-1);
	HAL_UART_Transmit_DMA(&huart2,(uint8_t *)&framStruct,sizeof(framStruct));
}
framType *cmdSolve(framType *frame)
{
 
	if(frame==NULL)
	{
		return NULL;
	}
	//数据校验
	
	//校对是否本组数据包
	uint32_t gp = (frame->paraData.downLoadInGroup.serialNum[0]<<16)+(frame->paraData.downLoadInGroup.serialNum[1]<<8)+(frame->paraData.downLoadInGroup.serialNum[2]);
	
	
	if(gp!=strToInt( para_struct.pageTwoInfo.GP,8))
	{
		return NULL;
	}
		switch (frame->direct)
		{
			case 0x10:
				cmdRecevied=frame->cmd;
				switch(frame->cmd)
				{
					case CMD_PLAY1:spkRunPlay(1);
								reply(CMD_PLAY1_RE);
						break;//播放1
					case CMD_PLAY2:spkRunPlay(2);
								reply(CMD_PLAY2_RE);
						break;//播放2
					case CMD_PLAY3:spkRunPlay(3);
								reply(CMD_PLAY3_RE);
						break;//播放3
					case CMD_PLAY4:spkRunPlay(4);
								reply(CMD_PLAY4_RE);
						break;//播放4
					case CMD_PLAY5:spkRunPlay(5);
								reply(CMD_PLAY5_RE);
						break;//CMD_UPLOAD
					case CMD_CHECK:
								reply(CMD_CHECK_RE);
						break;//主机查询
					case CMD_GROUP:
							statusWir433 =  STATUS_WIR_REG;step=2;timeCounter=0;//直接跳到组队
						break;//组队
					case CMD_SET1:
						//  statusWir433 = STATUS_WIR_SET_ADD;
						break;//设置1
					case CMD_SET2:
						//	statusWir433 = STATUS_WIR_SET_PARA;step=0;
						break;//设置2
					default:break;
				}
			
				
				break;//主机往终端
			case 0x20:
				break;//终端往主机
		}
		return frame;
}


void returnDate(uint8_t *retBuff,uint8_t *gpSrc)
{
	uint32_t gp = strToInt( gpSrc,8);
	retBuff[0]=0x11;//设备号
	retBuff[1]=gp>>16;	retBuff[2]=gp>>8;	retBuff[3]=gp>>0;//组号
	retBuff[4]=strToInt( gpSrc+9,2);//终端号
	retBuff[5]=para_struct.pageOneInfo.airPress/256;	retBuff[6]=para_struct.pageOneInfo.airPress%256;//气压
	retBuff[7]=para_struct.pageOneInfo.batt/2560;	retBuff[8]=(para_struct.pageOneInfo.batt/10)%256;//电压
	retBuff[9]=para_struct.pageOneInfo.temperature/10;//温度
	retBuff[10]=para_struct.pageThreeInfo.runTime/2560;	retBuff[11]=para_struct.pageThreeInfo.runTime/10%256;//开机时间
	retBuff[12]=0x55;	retBuff[13]=0x55;	retBuff[14]=0x55;//预留
	
	retBuff[15]=0;//状态
	if(para_struct.pageOneInfo.HUDflag==HUD1)
	{
		retBuff[15]&=~0X80;
	}
	else
	{
		retBuff[15]|=0X80;
	}
	
	if(para_struct.pageOneInfo.BLEflag==BLE_CONN)
	{
		retBuff[15]&=~0X40;
	}
	else
	{
		retBuff[15]|=0X40;
	}
		 
	
}
uint32_t strToInt(uint8_t * buff, uint8_t len )
{
	int32_t numer=0;
	uint8_t i=0;
	while(i<len)
	{
		numer *=10;
		numer += (buff[i]-0x30);
		i++;
	}
	return numer;
}

uint8_t * IntTostr(uint32_t gp, uint8_t len )
{
	uint8_t buff[20];
	int32_t numer=0;
	uint8_t i=0;
	while(i<len)
	{
		buff[len-i-1]=(gp%10+0x30);
		gp/=10;
 
		i++;
	}
	return buff;
}

void upLoad(void)
{
	if(	para_struct.pageOneInfo.DATAflag !=DATA_NO)
	{
	framStruct.cmd = CMD_UPLOAD;
	framStruct.direct=0x20;
	framStruct.path =0xc0;
	returnDate((uint8_t *)&framStruct.paraData,para_struct.pageTwoInfo.GP);
	framStruct.verify =  getCheck((uint8_t *)& framStruct,sizeof(framStruct)-1);
	HAL_UART_Transmit_DMA(&huart2,(uint8_t *)&framStruct,sizeof(framStruct));
	}
}
void replyGroup( uint8_t *gp)
{
	if(	para_struct.pageOneInfo.DATAflag !=DATA_NO)
	{
		para_struct.pageOneInfo.DATAflag = DATA_CONN;
		framStruct.cmd = CMD_GROUP_RE;
		framStruct.direct=0x80;
		framStruct.paraData.upLoadInGroup.deviceType=0x11;
		framStruct.paraData.upLoadInGroup.serialNum[0]=gp[0];framStruct.paraData.upLoadInGroup.serialNum[1]=gp[1];
		framStruct.paraData.upLoadInGroup.serialNum[2]=gp[2];framStruct.paraData.upLoadInGroup.serialNum[3]=gp[3];
		returnDate((uint8_t *)&framStruct.paraData,gp);
		*(uint16_t *)framStruct.paraData.upLoadInGroup.airStress=para_struct.pageOneInfo.airPress;
		*(uint16_t *)framStruct.paraData.upLoadInGroup.voltage=para_struct.pageOneInfo.batt;
		framStruct.paraData.upLoadInGroup.temperature=para_struct.pageOneInfo.temperature/10;
		*(uint16_t *)framStruct.paraData.upLoadInGroup.runTime=para_struct.pageThreeInfo.runTime/10;
		framStruct.paraData.upLoadInGroup.status=0;//状态
		if(para_struct.pageOneInfo.HUDflag==HUD1)
		{
			framStruct.paraData.upLoadInGroup.status&=~0X80;
		}
		else
		{
			framStruct.paraData.upLoadInGroup.status|=0X80;
		}
		
		if(para_struct.pageOneInfo.BLEflag==BLE_CONN)
		{
			framStruct.paraData.upLoadInGroup.status&=~0X40;
		}
		else
		{
			framStruct.paraData.upLoadInGroup.status|=0X40;
		}		
		framStruct.path =0xc0;
		framStruct.verify =  getCheck((uint8_t *)&framStruct+2,sizeof(framStruct)-1);
		HAL_UART_Transmit_DMA(&huart2,(uint8_t *)&framStruct,sizeof(framStruct));
	}
}

void reply(uint8_t cmd )
{
	if(	para_struct.pageOneInfo.DATAflag !=DATA_NO)
	{
		para_struct.pageOneInfo.DATAflag = DATA_CONN;
		framStruct.cmd = cmd;
		if(cmd==CMD_SLAVE_EXCHANGE)
		{		
				framStruct.direct=0x80;
			
		}
 		else
		{	framStruct.direct=0x20;
				
		}
		returnDate((uint8_t *)&framStruct.paraData,para_struct.pageTwoInfo.GP);
		framStruct.path =0xc0;
		framStruct.verify =  getCheck((uint8_t *)&framStruct+2,sizeof(framStruct)-1);
		HAL_UART_Transmit_DMA(&huart2,(uint8_t *)&framStruct,sizeof(framStruct));
	}
}


void writeGPToRam(framType *cmdTemp)
{
	mencpy(IntTostr((cmdTemp->paraData.downLoadOutGroup_Group.serialNumNew[0]<<16)+(cmdTemp->paraData.downLoadOutGroup_Group.serialNumNew[1]<<8)+\
	(cmdTemp->paraData.downLoadOutGroup_Group.serialNumNew[2]),8), para_struct.pageTwoInfo.GP,8);
	mencpy(IntTostr((cmdTemp->paraData.downLoadOutGroup_Group.serialNumNew[3]),2),(uint8_t *) &para_struct.pageTwoInfo.GP[9],2);
	 
}


void replySet2( framType *cmdTemp)
{
	if(	para_struct.pageOneInfo.DATAflag !=DATA_NO)
	{
		para_struct.pageOneInfo.DATAflag = DATA_CONN;
		framStruct.cmd = CMD_SET2_RE;
		framStruct.direct=0x20;
		framStruct.paraData.downLoadInGroup_Set2.deviceType=0x11;
		framStruct.paraData.downLoadInGroup_Set2.serialNumDs[0]=cmdTemp->paraData.downLoadInGroup_Set2.serialNumDs[0];
		framStruct.paraData.downLoadInGroup_Set2.serialNumDs[1]=cmdTemp->paraData.downLoadInGroup_Set2.serialNumDs[1];
		framStruct.paraData.downLoadInGroup_Set2.serialNumDs[2]=cmdTemp->paraData.downLoadInGroup_Set2.serialNumDs[2];
		framStruct.paraData.downLoadInGroup_Set2.serialNumDs[3]=cmdTemp->paraData.downLoadInGroup_Set2.serialNumDs[3];
		
		framStruct.paraData.downLoadInGroup_Set2.serialRate=para_struct.pageThreeInfo.serialRate;
		framStruct.paraData.downLoadInGroup_Set2.FFC=para_struct.pageThreeInfo.FFC;
		framStruct.paraData.downLoadInGroup_Set2.poweSet=para_struct.pageThreeInfo.poweSet;
		framStruct.paraData.downLoadInGroup_Set2.airRate=para_struct.pageThreeInfo.airRate;
	
		para_struct.pageThreeInfo.serialRate=cmdTemp->paraData.downLoadInGroup_Set2.serialRate;
		para_struct.pageThreeInfo.FFC=cmdTemp->paraData.downLoadInGroup_Set2.FFC;
		para_struct.pageThreeInfo.poweSet=cmdTemp->paraData.downLoadInGroup_Set2.poweSet;
		para_struct.pageThreeInfo.airRate=cmdTemp->paraData.downLoadInGroup_Set2.airRate;	
		
		framStruct.paraData.downLoadInGroup_Set2.serialRateNew=para_struct.pageThreeInfo.serialRate;
		framStruct.paraData.downLoadInGroup_Set2.FFCNew=para_struct.pageThreeInfo.FFC;
		framStruct.paraData.downLoadInGroup_Set2.poweSetNew=para_struct.pageThreeInfo.poweSet;
		framStruct.paraData.downLoadInGroup_Set2.airRateNew=para_struct.pageThreeInfo.airRate;	 
		STMFLASH_Write( FLASH_BASE+60*1024, (uint16_t *) &para_struct, sizeof(para_struct)/2 ); 
		framStruct.path =0x40;
		framStruct.verify =  getCheck((uint8_t *)&framStruct+2,sizeof(framStruct)-1);
		HAL_UART_Transmit_DMA(&huart2,(uint8_t *)&framStruct,sizeof(framStruct));
	}
}

 
 

 
void writeGPToFlash(framType *cmdTemp)
{
	mencpy(IntTostr((cmdTemp->paraData.downLoadInGroup_Set1.serialNumNew[0]<<16)+(cmdTemp->paraData.downLoadInGroup_Set1.serialNumNew[1]<<8)+\
	(cmdTemp->paraData.downLoadInGroup_Set1.serialNumNew[2]),8), para_struct.pageTwoInfo.GP,8);
	mencpy(IntTostr((cmdTemp->paraData.downLoadInGroup_Set1.serialNumNew[3]),2),(uint8_t *) &para_struct.pageTwoInfo.GP[9],2);
	STMFLASH_Write( FLASH_BASE+60*1024, (uint16_t *) &para_struct, sizeof(para_struct)/2 ); 
}