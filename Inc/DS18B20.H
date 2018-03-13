#ifndef __18B20_H__
#define __18B20_H__


#include "stm32f1xx_hal.h"
#include "main.h"

 


typedef enum{FAILED = 0 ,PASSED = !FAILED} TestStatus ;
#define DQ_GPIO  DS_DQ_GPIO_Port
#define DQ_GPIO_Pin DS_DQ_Pin
//REGISTER COMMANDS
#define REGISTER_9_BITS 0x1F
#define REGISTER_10_BITS 0x3F
#define REGISTER_11_BITS 0x5F
#define REGISTER_12_BIT2 0x7F
//ROM COMMANDS
#define ROM_Search_Cmd  0xF0
#define ROM_Read_Cmd  0x33
#define ROM_Match_Cmd  0x55
#define ROM_Skip_Cmd  0xCC
#define ROM_AlarmSearch_Cmd 0xEC
//DS18b20 FUNCTION COMMANDS
#define Convert_T  0x44
#define Write_Scratchpad  0x4E
#define Read_Scratchpad  0xBE
#define Copy_Scratchpad  0x48
#define Recall_EEPROM  0x88
#define Read_PowerSupply 0x84
#define DQ_Write_1()   HAL_GPIO_WritePin(DQ_GPIO, DQ_GPIO_Pin,(GPIO_PinState)1)
#define DQ_Write_0()  HAL_GPIO_WritePin(DQ_GPIO, DQ_GPIO_Pin,(GPIO_PinState)0)
#define DQ_ReadBit()   HAL_GPIO_ReadPin(DQ_GPIO, DQ_GPIO_Pin)
extern void GPIO_DQ_Out_Mode(void) ; //DQ输出模式
extern void GPIO_DQ_Input_Mode(void) ;  //DQ输入模式
extern void Tx_ResetPulse(void) ;  //发送复位脉冲
extern void Rx_PresencePulse(void) ; //接受应答信号
extern void Write_OneByte_ToDS18b20(unsigned char data) ; //写一个字节到18b20
extern unsigned char Read_OneByte_FromDS18b20(void) ; //从18b20读一个字节
extern float runTemperature(void);
//写配置参数TH，TL和初始化配置寄存器
extern void Write_EEPROM(unsigned char Th,unsigned char Tl,unsigned char Register_Con );
extern float get_data(void);
void DS18B20_Init(void) ; //初始化
void Delay_Nus(uint16_t us);

void Delay_Nms(uint16_t ms);

/* Private macro -------------------------------------------------------------*/
#define SKIP_ROM_COMMAND  		0xCC                                                    /* 跳过ROM匹配操作 */
#define	CONVERT_TEMPERATURE		0x44													/* 启动温度转换 */
#define	READ_SCRATCHPAD			0xBE													/* 读温度中间暂存寄存器*/
/* Private variables ---------------------------------------------------------*/
/* Private function prototypes -----------------------------------------------*/
void _delay_us(uint16_t us);
void _delay_ms(uint16_t nCount);

void read(void);
//------------------------------------------------------------------------------------
void Init_DS18B20(void) ;
//--------------------------------------------------------------------------------------

uint8_t readTemp(void) ;
#endif
