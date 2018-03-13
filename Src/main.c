/**
  ******************************************************************************
  * File Name          : main.c
  * Description        : Main program body
  ******************************************************************************
  ** This notice applies to any and all portions of this file
  * that are not between comment pairs USER CODE BEGIN and
  * USER CODE END. Other portions of this file, whether 
  * inserted by the user or by software development tools
  * are owned by their respective copyright owners.
  *
  * COPYRIGHT(c) 2018 STMicroelectronics
  *
  * Redistribution and use in source and binary forms, with or without modification,
  * are permitted provided that the following conditions are met:
  *   1. Redistributions of source code must retain the above copyright notice,
  *      this list of conditions and the following disclaimer.
  *   2. Redistributions in binary form must reproduce the above copyright notice,
  *      this list of conditions and the following disclaimer in the documentation
  *      and/or other materials provided with the distribution.
  *   3. Neither the name of STMicroelectronics nor the names of its contributors
  *      may be used to endorse or promote products derived from this software
  *      without specific prior written permission.
  *
  * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
  * AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
  * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
  * DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE
  * FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
  * DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR
  * SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER
  * CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY,
  * OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
  * OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
  *
  ******************************************************************************
  */
/* Includes ------------------------------------------------------------------*/
#include "main.h"
#include "stm32f1xx_hal.h"
#include "adc.h"
#include "dma.h"
#include "rtc.h"
#include "spi.h"
#include "tim.h"
#include "usart.h"
#include "gpio.h"

/* USER CODE BEGIN Includes */
#include "oled.h"
#include "led.h"
#include "display.h"
#include "ds18b20.h"
#include "ble.h"
#include "key.h"
#include "speak.h"
#include "stm_flash.h"
#include "wir433.h"
#include "./radio.h"
#include "./radio_app.h"
#include "../src/user/type.h"
#include "../src/user/stress.h"
/* USER CODE END Includes */

/* Private variables ---------------------------------------------------------*/

/* USER CODE BEGIN PV */
/* Private variables ---------------------------------------------------------*/
uint8_t sleepFlag=0;
uint16_t adcRawDate[50][3];
uint8_t status=STATUS_OFF;
para_struct_type para_struct;
uint8_t showPageNum=PAGE_RUN_ONE;
uint8_t selectedNum=1;
uint8_t keyPressStatus;
uint8_t keyDownStatus;
uint8_t keyHoldStatus;
uint16_t offTime=0; 
uint32_t index_cnt;
uint16_t lcdCloseCount = 1200;//50ms单位
uint16_t sysCloseCount =0;

/* RTC handler declaration */
RTC_HandleTypeDef RtcHandle;


/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);

/* USER CODE BEGIN PFP */
/* Private function prototypes -----------------------------------------------*/
uint16_t getAdcAfterFilt(uint8_t chanal ,uint8_t filt);
/* USER CODE END PFP */

/* USER CODE BEGIN 0 */
int8_t menFind(uint8_t *src,uint8_t *des,uint8_t len1,uint8_t len2)
{
	uint8_t index=0,i;
	
	for(index=0;index<len1;index++)
	{
		if(src[index]==des[0])
		{
			for(i=1;i<len2;i++)
			{
				if(src[index+i]!=des[i])
				{
					break;
				}
			}
			if(i==len2)
				break;
		}
		 
	}
	if(index!=len1)
	return index;
	else
		return -1;
	
}
void menclr(uint8_t *str1,uint8_t len)
{
	uint8_t i = 0;
	while(i<len)
	{
		str1[i]=0;
		i++;
	}
}
void mencpy(uint8_t *src,uint8_t *des,uint8_t len)
{ 
	uint8_t i = 0;
	while(i<len)
	{
		des[i]=src[i];
		i++;
	}
}
uint8_t mencpr(uint8_t *str1,uint8_t *str2,uint8_t len)
{ 
	uint8_t i = 0;
	while(i<len)
	{
		if(*str1!=*str2)
		{
				break;
			
		}
			str1++;
			str2++;
		i++;
	}
	return i==len?1:0;
}
uint16_t getAdcAfterFilt(uint8_t chanal , uint8_t filt)
{
	

	uint32_t adcDate=0;
	uint8_t i;
	if(filt == 0)
	{
	for(i = 0;i<50;i++)
	adcDate+=adcRawDate[i][chanal];
	return adcDate/50;
	}
	else{
			return adcRawDate[0][chanal];
		}
}

 //初始化参数
void paraInit(void)
{
	para_struct.pageOneInfo.airPress=0;
	para_struct.pageOneInfo.batt=0;
	para_struct.pageOneInfo.BLEflag=BLE_UNCONN;
	para_struct.pageOneInfo.HUDflag=HUD1;
	para_struct.pageOneInfo.BLE_SendInterval=2000;
	para_struct.pageOneInfo.BLE_SendTotal=0;
	para_struct.pageOneInfo.DATAflag=DATA_NO;
	para_struct.pageOneInfo.temperature=0;
	para_struct.pageOneInfo.voiceLevel=1;
	mencpy("************",para_struct.pageTwoInfo.BHTMAC,sizeof("************"));
	mencpy("00000001.22",para_struct.pageTwoInfo.GP,sizeof("00000001.22"));
	mencpy("00112233",para_struct.pageTwoInfo.SN,sizeof("00112233"));
	mencpy("00000000",para_struct.pageTwoInfo.HUD,sizeof("00000000"));
	para_struct.pageThreeInfo.AirF0=0;
	para_struct.pageThreeInfo.AirF1=100;
	para_struct.pageThreeInfo.AirF2=300;
	para_struct.pageThreeInfo.BLE_Switch=2;
	para_struct.pageThreeInfo.DateSwitch=2;
	para_struct.pageThreeInfo.serialRate=4;
	para_struct.pageThreeInfo.airRate=3;
	para_struct.pageThreeInfo.FFC=1;
	para_struct.pageThreeInfo.poweSet=0;
	para_struct.pageThreeInfo.serialRate = 4;
	para_struct.pageThreeInfo.airRate = 3;
	para_struct.pageThreeInfo.FFC = 1;
	para_struct.pageThreeInfo.poweSet = 0;
	mencpy("20180101",para_struct.pageThreeInfo.DATE,sizeof("20180101"));
  mencpy("1.0.1",para_struct.pageThreeInfo.Version,sizeof("1.0.1"));
}

/**
  * @brief  Configure the current time and date.
  * @param  None
  * @retval None
  */
  void RTC_AlarmConfig(void)
{
  RTC_DateTypeDef  sdatestructure_set = {0};
  RTC_TimeTypeDef  stimestructure = {0};
  RTC_AlarmTypeDef salarmstructure = {{0}, 0};
 
	  /*##-1- Configure the RTC peripheral #######################################*/
  RtcHandle.Instance = RTC;
  /*##-1- Configure the Date #################################################*/
  /* Set Date: October 31th 2014 */
  sdatestructure_set.Year = 0x14;
  sdatestructure_set.Month = RTC_MONTH_OCTOBER;
  sdatestructure_set.Date = 0x31;
  
  if(HAL_RTC_SetDate(&RtcHandle,&sdatestructure_set,RTC_FORMAT_BCD) != HAL_OK)
  {
    /* Initialization Error */
     Error_Handler(); 
  } 
  
  /*##-2- Configure the Time #################################################*/
  /* Set Time: 23:59:55 */
  stimestructure.Hours = 0x23;
  stimestructure.Minutes = 0x59;
  stimestructure.Seconds = 0x55;
  if(HAL_RTC_SetTime(&RtcHandle,&stimestructure,RTC_FORMAT_BCD) != HAL_OK)
  {
    /* Initialization Error */
     Error_Handler(); 
  }  

  /*##-3- Configure the RTC Alarm peripheral #################################*/
  /* Set Alarm to 00:00:10 
     RTC Alarm Generation: Alarm on Hours, Minutes and Seconds */
  salarmstructure.Alarm = RTC_ALARM_A;
  salarmstructure.AlarmTime.Hours = 0x00;
  salarmstructure.AlarmTime.Minutes = 0x00;
  salarmstructure.AlarmTime.Seconds = 0x10;
  
  if(HAL_RTC_SetAlarm_IT(&RtcHandle,&salarmstructure,RTC_FORMAT_BCD) != HAL_OK)
  {
    /* Initialization Error */
     Error_Handler(); 
  }
}
void beforSleep(void)
{
	  GPIO_InitTypeDef GPIO_InitStructure;

  /* Enable GPIOs clock */
  __HAL_RCC_GPIOA_CLK_ENABLE();
  __HAL_RCC_GPIOB_CLK_ENABLE();
  __HAL_RCC_GPIOC_CLK_ENABLE();
  __HAL_RCC_GPIOD_CLK_ENABLE();
  __HAL_RCC_GPIOE_CLK_ENABLE();

  /* Configure all GPIO port pins in Analog Input mode (floating input trigger OFF) */
  /* Note: Debug using ST-Link is not possible during the execution of this   */
  /*       example because communication between ST-link and the device       */
  /*       under test is done through UART. All GPIO pins are disabled (set   */
  /*       to analog input mode) including  UART I/O pins.           */
  GPIO_InitStructure.Pin = GPIO_PIN_All;
  GPIO_InitStructure.Mode = GPIO_MODE_ANALOG;
  GPIO_InitStructure.Pull = GPIO_NOPULL;

  HAL_GPIO_Init(GPIOA, &GPIO_InitStructure); 
  HAL_GPIO_Init(GPIOB, &GPIO_InitStructure);
  HAL_GPIO_Init(GPIOC, &GPIO_InitStructure);
  HAL_GPIO_Init(GPIOD, &GPIO_InitStructure);
  HAL_GPIO_Init(GPIOE, &GPIO_InitStructure);
//  GPIO_InitStructure.Pin = AMP_SD_Pin;
//  GPIO_InitStructure.Mode = GPIO_MODE_OUTPUT_PP;
//	GPIO_InitStructure.Speed=GPIO_SPEED_FREQ_LOW;
//  HAL_GPIO_Init(AMP_SD_GPIO_Port, &GPIO_InitStructure);
//	HAL_GPIO_WritePin(AMP_SD_GPIO_Port,AMP_SD_Pin,SET);
  /* Disable GPIOs clock */
  __HAL_RCC_GPIOA_CLK_DISABLE();
  __HAL_RCC_GPIOB_CLK_DISABLE();
  __HAL_RCC_GPIOC_CLK_DISABLE();
  __HAL_RCC_GPIOD_CLK_DISABLE();
  __HAL_RCC_GPIOE_CLK_DISABLE();
	RTC_AlarmConfig();
 
}



 
/* USER CODE END 0 */

int main(void)
{

  /* USER CODE BEGIN 1 */
   uint8_t turnOffStep = 0;uint8_t temp_i ;
  /* USER CODE END 1 */

  /* MCU Configuration----------------------------------------------------------*/

  /* Reset of all peripherals, Initializes the Flash interface and the Systick. */
  HAL_Init();

  /* USER CODE BEGIN Init */

  /* USER CODE END Init */

  /* Configure the system clock */
  SystemClock_Config();

  /* USER CODE BEGIN SysInit */

  /* USER CODE END SysInit */

  /* Initialize all configured peripherals */
  MX_GPIO_Init();
  MX_DMA_Init();
  MX_ADC1_Init();
  MX_SPI1_Init();
  MX_USART2_UART_Init();
  MX_TIM1_Init();
  MX_RTC_Init();
  MX_USART3_UART_Init();

  /* USER CODE BEGIN 2 */
 	 HAL_GPIO_WritePin(POWER_EN_GPIO_Port,POWER_EN_Pin,GPIO_PIN_SET);
	 
	 register_led(LED_BLE,LED_BLUE_GPIO_Port,LED_BLUE_Pin,GPIO_PIN_SET);
	 register_led(LED_HIPOWER,LED_YELLOW_GPIO_Port,LED_YELLOW_Pin,GPIO_PIN_SET);
	 register_led(LED_LOPOWER,LED_RED_GPIO_Port,LED_RED_Pin,GPIO_PIN_SET);
	 HAL_TIM_Base_Start_IT(&htim1); 
	 HAL_ADC_Start_DMA(&hadc1,(uint32_t*)&adcRawDate,150);
	 StartTimer(TIMERSTATUS_Machine,10);
	 radio_hal_SetNsel();
	 HAL_SPI_Transmit(&hspi1,&selectedNum,1,100);
		 

  /* USER CODE END 2 */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
  while (1)
  {
  /* USER CODE END WHILE */

  /* USER CODE BEGIN 3 */
	bRadio_Check_Tx_RX();
		if(sleepFlag)
		{
			   	 HAL_PWR_EnterSLEEPMode(0, PWR_SLEEPENTRY_WFI);
		}
		if(ReadTimer(TIMERSTATUS_Machine)==OK)
		{
			StartTimer(TIMERSTATUS_Machine,100);
			para_struct.pageThreeInfo.runTime++;
			// vRadio_StartTx_Variable_Packet(23,"zhoukundsadadas",sizeof("zhoukundsadadas"));
			switch(status)
			{
				case STATUS_IDLE:
					status = STATUS_ON;
				  StartTimer(TIMER_DS18B20,1000);
					StartTimer(TIMER_LCD,50);
					StartTimer(TIMER_BLE,200);
			  	StartTimer(TIMER_WIR433,100);
					StartTimer(TIMER_BATT_Test,200);
					STMFLASH_Read( FLASH_BASE+60*1024, (uint16_t *) &para_struct, sizeof(para_struct)/2 );           //从指定地址开始读出指定长度的数据	
				if(para_struct.pageThreeInfo.Version[0]==0xff)
					{
						paraInit();
						STMFLASH_Write( FLASH_BASE+60*1024, (uint16_t *) &para_struct, sizeof(para_struct)/2 ); 
					}
					HAL_GPIO_WritePin(POWER_EN_GPIO_Port,POWER_EN_Pin,GPIO_PIN_SET);
					HAL_GPIO_WritePin(AMP_SD_GPIO_Port,AMP_SD_Pin,GPIO_PIN_SET);
					OLED_Init();
					
					init_WIR433(20);
					turnOnWir433();
				  initBLE();
					para_struct.pageThreeInfo.runTime=0;

					radioAppInit();
					HAL_GPIO_WritePin(SPK_DATA_GPIO_Port,SPK_DATA_Pin,GPIO_PIN_SET);
					
					para_struct.pageOneInfo.airPress =getStress(getAdcAfterFilt(AIR_ADC_CH,0));
					para_struct.pageOneInfo.batt=para_struct.pageOneInfo.batt*0.9+(getAdcAfterFilt(BATT_ADC_CH,0)*660/4096);
					break;
				case STATUS_OFF:	//goto low power mode and wakeup every 10 secends
								STMFLASH_Read( FLASH_BASE+60*1024, (uint16_t *) &para_struct, sizeof(para_struct)/2 );      
								DelayTimer(50);
							  set_Init(getAdcAfterFilt(AIR_ADC_CH,1));
								para_struct.pageOneInfo.airPress =getStress( getAdcAfterFilt(AIR_ADC_CH,1));
						    para_struct.pageOneInfo.batt=getAdcAfterFilt(BATT_ADC_CH,1)*660/4096;
				
								deal_key_event();//预处理按键
								if(para_struct.pageOneInfo.airPress>150||((keyDownStatus&KEY5)==KEY5))
								{
									keyDownStatus=0;keyHoldStatus=0;
									HAL_GPIO_WritePin(POWER_EN_GPIO_Port,POWER_EN_Pin,GPIO_PIN_RESET);
									status=STATUS_IDLE;
								}
								else
								{
									 beforSleep();
									/* Clear all related wakeup flags */
								 	__HAL_PWR_CLEAR_FLAG(PWR_FLAG_WU);
									/* Enter the Standby mode */
								  	HAL_PWR_EnterSTANDBYMode();
								}
					break;
				case STATUS_ON:
					

				if(ReadTimer(TIMER_WIR433)==OK)
					{
						wir433Task();
						StartTimer(TIMER_WIR433,100);
					  spkTask();
						radioTask();
					}
					if(ReadTimer(TIMER_BLE)==OK)
					{
					 	bleTask();
					}
					if(ReadTimer(TIMER_BATT_Test)==OK)
					{
						StartTimer(TIMER_BATT_Test,200);
						para_struct.pageOneInfo.airPress =getStress(getAdcAfterFilt(AIR_ADC_CH,0));
						para_struct.pageOneInfo.batt=para_struct.pageOneInfo.batt*0.9+(getAdcAfterFilt(BATT_ADC_CH,0)*660/4096);
						if(para_struct.pageOneInfo.airPress<150)
						{
							offTime++; // 调试取消关机
							if(offTime>900)//气压低关机
							{
								if(turnOffStep==0)
								{
									turnOffStep++;
									turnOffWir433();
								}
								else
								{
								if( getWirStatus() == STATUS_WIR_IDLE ||getWirStatus()==STATUS_WIR_OFF )//等待关机完成
									{
										offTime=0;
										status = STATUS_OFF;
									}
								}
							}
						}	
						else
						{
							offTime=0;
						}	
						if(para_struct.pageOneInfo.batt>370)
						{
							stopLed(LED_LOPOWER);
							startLed(LED_HIPOWER,1000,300);
						}
						else{
								stopLed(LED_HIPOWER);
								startLed(LED_LOPOWER,5000,1500);
						}
					}
				  if(runTemperature()!=10000)
					{
 					 	para_struct.pageOneInfo.temperature = get_data(); 
					}
					if(ReadTimer(TIMER_LCD)==OK)
					{
						StartTimer(TIMER_LCD,100);
						if((keyHoldStatus&KEY1)==KEY1)
						{
							keyHoldStatus=0;
							status =STATUS_PAIR;
							LCD_CLS();
							radioPair();
							startLed(LED_BLE,500,200);
							LCD_Print(0,3,"     对码中....");
							break;
						}
						//音量设置
						if((keyHoldStatus&KEY2)==KEY2)
						{	
							while(key_check(3) != KEY_UP);
							para_struct.pageOneInfo.voiceLevel++;
							if(para_struct.pageOneInfo.voiceLevel>=4)
								para_struct.pageOneInfo.voiceLevel=1;
							setVoice(para_struct.pageOneInfo.voiceLevel);
							STMFLASH_Write( FLASH_BASE+60*1024, (uint16_t *) &para_struct, sizeof(para_struct)/2 ); 	
							keyPressStatus=0;
							keyHoldStatus=0;
						}						
						if((keyHoldStatus&KEY3)==KEY3)
						{
							keyHoldStatus=0;
							status =STATUS_SET_SEL;
							LCD_CLS();
							showPageSetSel(1);
							selectedNum=1;
							break;
						}								
						if((keyPressStatus&KEY2)==KEY2)
						{
							keyPressStatus=0;
							
							if(showPageNum==PAGE_RUN_THREE)
								showPageNum=PAGE_RUN_ONE;
							else
								showPageNum++;
							LCD_CLS();
						}
						//无按键则关闭屏幕省电
						if(lcdCloseCount >1)
						{
							lcdCloseCount--;
							if(showPageNum==PAGE_RUN_ONE)
							{
								if((keyHoldStatus&KEY5)==KEY5 && offTime >15)
								{
									keyHoldStatus=0;
									offTime = 900;
								}		
			
								showPageOne(&para_struct.pageOneInfo);
							}
							else if(showPageNum==PAGE_RUN_TWO)
							{
								showPageTwo(&para_struct.pageTwoInfo);
							}
							else if(showPageNum==PAGE_RUN_THREE)
							{
								showPageThree(&para_struct.pageThreeInfo);
							}
						}
						else
						{
							if(lcdCloseCount==1)
							{
								lcdCloseCount=0;
								LCD_CLS();
							}
						}
					}							
					break;
				case STATUS_PAIR:
							if(ReadTimer(TIMER_WIR433)==OK)
							{
								StartTimer(TIMER_WIR433,100);
								radioTask();
							}
							if(getPairStatus()==1)
							{
								LCD_Print(0,3,"     对码:      ");
								LCD_Print(24,6,para_struct.pageTwoInfo.HUD);
								DelayTimer(1000);
								status =STATUS_ON;
								LCD_CLS();
								keyHoldStatus=0;	
							}
					break;
				case STATUS_SET_SEL:
					
					if((keyHoldStatus&KEY3)==KEY3)
						{LCD_CLS();
							keyHoldStatus=0;
							LCD_Print(0,3,"     RESET?      ");
							while(1)
							{
									if((keyPressStatus&KEY4)==KEY4)
									{
										LCD_Print(0,3,"   RESETING...    ");
										keyPressStatus=0;
										paraInit();
										STMFLASH_Write( FLASH_BASE+60*1024, (uint16_t *) &para_struct, sizeof(para_struct)/2 ); 
										break;
									}
									if((keyPressStatus&KEY5)==KEY5)
									{
										keyPressStatus=0;
										break;
									}
							}
							LCD_CLS();
							showPageSetSel(selectedNum);
						}
					if((keyPressStatus&KEY2)==KEY2)
						{
							CLR_Arr();
							keyPressStatus=0;
							selectedNum++;
							if(selectedNum>4)
								selectedNum=1;
							showPageSetSel(selectedNum);
						}
						if((keyPressStatus&KEY5)==KEY5)
						{
							LCD_CLS();
							keyPressStatus=0;
							status =STATUS_ON;
						}
						if((keyPressStatus&KEY4)==KEY4)
						{
							LCD_CLS();
							keyPressStatus=0;
							switch(selectedNum)
							{
								case 1:status = STATUS_SET_HUD;
												selectedNum=1;
												showPageSetHUD(selectedNum);
									break;
								case 2:status = STATUS_SET_BLE;
												selectedNum=1;
												showPageSetBLE(selectedNum);
									break;
								case 3:status = STATUS_SET_DATA;
												selectedNum=1;
												showPageSetDATA(selectedNum);
									break;
								case 4:status = STATUS_SET_AIRPRESS_CAL;
												selectedNum=1;
												showPageSetCal(selectedNum);
									break;
								default:break;
							}
						}
					break;
				case STATUS_SET_HUD:
											if((keyPressStatus&KEY4)==KEY4)
												{
													
													keyPressStatus=0;
													para_struct.pageOneInfo.HUDflag=selectedNum;showPageSetHUD(selectedNum); 
													STMFLASH_Write( FLASH_BASE+60*1024, (uint16_t *) &para_struct, sizeof(para_struct)/2 );
												}
											if((keyPressStatus&KEY2)==KEY2)
												{
													CLR_Arr();
													keyPressStatus=0;
													selectedNum++;
													if(selectedNum>2)
														selectedNum=1;
												showPageSetHUD(selectedNum);
												}
												if((keyPressStatus&KEY5)==KEY5)
												{
													LCD_CLS();
													keyPressStatus=0;
													status =STATUS_SET_SEL;
													selectedNum=1;
													showPageSetSel(selectedNum);
												}
					break;
				case STATUS_SET_BLE:
											if((keyPressStatus&KEY4)==KEY4)
												{
													 
													keyPressStatus=0;
													para_struct.pageThreeInfo.BLE_Switch=selectedNum;	showPageSetBLE(selectedNum);
													STMFLASH_Write( FLASH_BASE+60*1024, (uint16_t *) &para_struct, sizeof(para_struct)/2 );
												}
											if((keyPressStatus&KEY2)==KEY2)
												{
													CLR_Arr();
													keyPressStatus=0;
													selectedNum++;
													if(selectedNum>2)
														selectedNum=1;
												showPageSetBLE(selectedNum);
												}
												if((keyPressStatus&KEY5)==KEY5)
												{
													LCD_CLS();
													keyPressStatus=0;
													status =STATUS_SET_SEL;
													selectedNum=2;
													showPageSetSel(selectedNum);
												}					
					break;
				case STATUS_SET_DATA:
										if((keyPressStatus&KEY4)==KEY4)
												{
													 
													keyPressStatus=0;
													para_struct.pageThreeInfo.DateSwitch=selectedNum;showPageSetDATA(selectedNum);
													STMFLASH_Write( FLASH_BASE+60*1024, (uint16_t *) &para_struct, sizeof(para_struct)/2 );
														if(para_struct.pageThreeInfo.DateSwitch==2)
															{
																if(para_struct.pageOneInfo.DATAflag!=DATA_NO){
																			para_struct.pageOneInfo.DATAflag=DATA_NO;
																			setMode(3);
																}
															}else{
																para_struct.pageOneInfo.DATAflag=DATA_UNCONN;
																init_WIR433(20);
																setMode(0);
															}
																									
													
												}
											if((keyPressStatus&KEY2)==KEY2)
												{
													CLR_Arr();
													keyPressStatus=0;
													selectedNum++;
													if(selectedNum>2)
														selectedNum=1;
												showPageSetDATA(selectedNum);
													
													
													
												}
												if((keyPressStatus&KEY5)==KEY5)
												{
													LCD_CLS();
													keyPressStatus=0;
													status =STATUS_SET_SEL;
													selectedNum=3;
													showPageSetSel(selectedNum);
												}					
												
					break;
				case STATUS_SET_AIRPRESS_CAL:
											if((keyPressStatus&KEY2)==KEY2)
												{
													CLR_Arr();
													keyPressStatus=0;
													selectedNum++;
													if(selectedNum>4)
														selectedNum=1;
												showPageSetCal(selectedNum);
												}
												if((keyPressStatus&KEY5)==KEY5)
												{
													LCD_CLS();
													keyPressStatus=0;
													status =STATUS_SET_SEL;
													selectedNum=4;
													showPageSetSel(selectedNum);
												}
											if((keyPressStatus&KEY4)==KEY4)
												{
													 
													keyPressStatus=0;
													switch(selectedNum)
													{			
														
														case 1:   para_struct.pageThreeInfo.AirF0 = getAdcAfterFilt(AIR_ADC_CH,0);
																			LCD_CLS();LCD_Print(0,3,"    标定中....");
																			for(temp_i=0;temp_i<10;temp_i++)
																			{
																				
																				Dis_Num(5,5,para_struct.pageThreeInfo.AirF0,4);DelayTimer(100);
																			}
																			
																			LCD_CLS();LCD_Print(0,3,"   F0 0Mpa标定");
																			DelayTimer(500);LCD_CLS();showPageSetCal(selectedNum);
															break;
														case 2:		para_struct.pageThreeInfo.AirF1 = getAdcAfterFilt(AIR_ADC_CH,0);
																			LCD_CLS();LCD_Print(0,3,"    标定中....");
																			for(temp_i=0;temp_i<10;temp_i++)
																			{
																				
																				Dis_Num(5,5,para_struct.pageThreeInfo.AirF1,4);DelayTimer(100);
																			}
																			LCD_CLS();LCD_Print(0,3,"   F1 10Mpa标定");
																			DelayTimer(500);LCD_CLS();showPageSetCal(selectedNum);
															break;
														case 3:		para_struct.pageThreeInfo.AirF2 = getAdcAfterFilt(AIR_ADC_CH,0);
																			LCD_CLS();LCD_Print(0,3,"     标定中....");
																			for(temp_i=0;temp_i<10;temp_i++)
																			{
																				
																				Dis_Num(5,5,para_struct.pageThreeInfo.AirF2,4);DelayTimer(100);
																			}
																			LCD_CLS();LCD_Print(0,3,"   F2 30Mpa标定");
																			DelayTimer(500);LCD_CLS();showPageSetCal(selectedNum);
															break;
														case 4:  index_cnt=1; ;status = STATUS_SET_SN;
																		 changeSN((char *)&para_struct.pageTwoInfo.SN,index_cnt);
															break;
														default:break;
													}
												STMFLASH_Write( FLASH_BASE+60*1024, (uint16_t *) &para_struct, sizeof(para_struct)/2 );	
												}												
					break;
					case STATUS_SET_SN:
											if((keyPressStatus&KEY1)==KEY1)
												{
													keyPressStatus=0;
													
													index_cnt+=1;
													if(index_cnt==9)
													{
														index_cnt=1;
													}
												changeSN((char *)&para_struct.pageTwoInfo.SN,index_cnt);
												}
												if((keyPressStatus&KEY2)==KEY2)
												{
													keyPressStatus=0;
													para_struct.pageTwoInfo.SN[8-index_cnt]+=1;
													if(para_struct.pageTwoInfo.SN[8-index_cnt]>'9')
													{
														para_struct.pageTwoInfo.SN[8-index_cnt]='0';
													}
												changeSN((char *)&para_struct.pageTwoInfo.SN,index_cnt);
												}
												if((keyPressStatus&KEY4)==KEY4)
												{
													LCD_CLS();
													keyPressStatus=0;
													status =STATUS_SET_SEL;	
													selectedNum=4;
													showPageSetSel(selectedNum);
													STMFLASH_Write( FLASH_BASE+60*1024, (uint16_t *) &para_struct, sizeof(para_struct)/2 );
												}
												if((keyPressStatus&KEY5)==KEY5)
												{
													LCD_CLS();
													keyPressStatus=0;
													status =STATUS_SET_SEL;
													selectedNum=4;
													showPageSetSel(selectedNum);
												}					
					break;												
				default:break;
				
			}
		}
  }
  /* USER CODE END 3 */

}

/** System Clock Configuration
*/
void SystemClock_Config(void)
{

  RCC_OscInitTypeDef RCC_OscInitStruct;
  RCC_ClkInitTypeDef RCC_ClkInitStruct;
  RCC_PeriphCLKInitTypeDef PeriphClkInit;

    /**Initializes the CPU, AHB and APB busses clocks 
    */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_LSI|RCC_OSCILLATORTYPE_HSE;
  RCC_OscInitStruct.HSEState = RCC_HSE_ON;
  RCC_OscInitStruct.HSEPredivValue = RCC_HSE_PREDIV_DIV1;
  RCC_OscInitStruct.HSIState = RCC_HSI_ON;
  RCC_OscInitStruct.LSIState = RCC_LSI_ON;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
  RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSE;
  RCC_OscInitStruct.PLL.PLLMUL = RCC_PLL_MUL2;
  if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
  {
    _Error_Handler(__FILE__, __LINE__);
  }

    /**Initializes the CPU, AHB and APB busses clocks 
    */
  RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK|RCC_CLOCKTYPE_SYSCLK
                              |RCC_CLOCKTYPE_PCLK1|RCC_CLOCKTYPE_PCLK2;
  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
  RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV2;
  RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV1;

  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_0) != HAL_OK)
  {
    _Error_Handler(__FILE__, __LINE__);
  }

  PeriphClkInit.PeriphClockSelection = RCC_PERIPHCLK_RTC|RCC_PERIPHCLK_ADC;
  PeriphClkInit.RTCClockSelection = RCC_RTCCLKSOURCE_LSI;
  PeriphClkInit.AdcClockSelection = RCC_ADCPCLK2_DIV6;
  if (HAL_RCCEx_PeriphCLKConfig(&PeriphClkInit) != HAL_OK)
  {
    _Error_Handler(__FILE__, __LINE__);
  }

    /**Configure the Systick interrupt time 
    */
  HAL_SYSTICK_Config(HAL_RCC_GetHCLKFreq()/8000);

    /**Configure the Systick 
    */
  HAL_SYSTICK_CLKSourceConfig(SYSTICK_CLKSOURCE_HCLK_DIV8);

  /* SysTick_IRQn interrupt configuration */
  HAL_NVIC_SetPriority(SysTick_IRQn, 0, 0);
}

/* USER CODE BEGIN 4 */

/* USER CODE END 4 */

/**
  * @brief  This function is executed in case of error occurrence.
  * @param  None
  * @retval None
  */
void _Error_Handler(char * file, int line)
{
  /* USER CODE BEGIN Error_Handler_Debug */
  /* User can add his own implementation to report the HAL error return state */
  while(1) 
  {
  }
  /* USER CODE END Error_Handler_Debug */ 
}

#ifdef USE_FULL_ASSERT

/**
   * @brief Reports the name of the source file and the source line number
   * where the assert_param error has occurred.
   * @param file: pointer to the source file name
   * @param line: assert_param error line source number
   * @retval None
   */
void assert_failed(uint8_t* file, uint32_t line)
{
  /* USER CODE BEGIN 6 */
  /* User can add his own implementation to report the file name and line number,
    ex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */
  /* USER CODE END 6 */

}

#endif

/**
  * @}
  */ 

/**
  * @}
*/ 

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
