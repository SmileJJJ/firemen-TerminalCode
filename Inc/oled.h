#ifndef	_OLED_H_
#define _OLED_H_
#include "stm32f1xx_hal.h"
#include "main.h"
 #include "timer.h"
 
//#define DCLOW() HAL_GPIO_WritePin(OLED_DC_GPIO_Port, OLED_DC_Pin,(GPIO_PinState)0)
//#define DCHIGH() HAL_GPIO_WritePin(OLED_DC_GPIO_Port, OLED_DC_Pin,(GPIO_PinState)1)
//#define CLKLOW() HAL_GPIO_WritePin(OLED_D0_GPIO_Port, OLED_D0_Pin,(GPIO_PinState)0)
//#define CLKHIGH() HAL_GPIO_WritePin(OLED_D0_GPIO_Port, OLED_D0_Pin,(GPIO_PinState)1)
//#define MISOLOW() HAL_GPIO_WritePin(OLED_D1_GPIO_Port, OLED_D1_Pin,(GPIO_PinState)0)
//#define MISOHIGH() HAL_GPIO_WritePin(OLED_D1_GPIO_Port, OLED_D1_Pin,(GPIO_PinState)1)	
//#define RESLOW() HAL_GPIO_WritePin(OLED_RST_GPIO_Port, OLED_RST_Pin,(GPIO_PinState)0)
//#define RESHIGH()  HAL_GPIO_WritePin(OLED_RST_GPIO_Port, OLED_RST_Pin,(GPIO_PinState)1)	
#define DCLOW() OLED_DC_GPIO_Port->ODR&=~OLED_DC_Pin 
#define DCHIGH() OLED_DC_GPIO_Port->ODR|=OLED_DC_Pin 
#define CLKLOW() OLED_D0_GPIO_Port->ODR&=~OLED_D0_Pin    
#define CLKHIGH() OLED_D0_GPIO_Port->ODR|=OLED_D0_Pin  
#define MISOLOW() OLED_D1_GPIO_Port->ODR&=~OLED_D1_Pin
#define MISOHIGH() OLED_D1_GPIO_Port->ODR|=OLED_D1_Pin  
#define RESLOW() OLED_RST_GPIO_Port->ODR&=~OLED_RST_Pin
#define RESHIGH()   OLED_RST_GPIO_Port->ODR|=OLED_RST_Pin


#define uint16 uint16_t 
#define uint8 uint8_t 
#define int16 int16_t 
#define int8 int8_t 

#define byte uint8_t
#define word uint16_t
 
#define GPIO_PIN(x)        (((1)<<(x & GPIO_PIN_MASK)))  //把当前位置1
// extern byte beyond96x64[512];
extern byte xinghao[64];
extern byte BLE[16];
extern byte NO_BLE[16];
// extern byte beyond64x64[512];
 void OLED_Init(void);
 void LCD_CLS(void);
 void LCD_P6x8Str(byte x,byte y,byte ch[]);
 void LCD_P8x16Str(byte x,byte y,byte ch[]);
 void LCD_P14x16Str(byte x,byte y,byte ch[]);
 void LCD_Print(byte x, byte y, byte ch[]);
 void LCD_PutPixel(byte x,byte y);
 void LCD_Rectangle(byte x1,byte y1,byte x2,byte y2,byte gif);
 void Draw_LQLogo(void);
 void Draw_LibLogo(void);
 void Draw_Image(void);
 void Draw_BMP(byte x0,byte y0,byte x1,byte y1,byte bmp[]);
 void OLCD_Fill(byte dat);
 void Dis_Num(byte y, byte x, unsigned int num,byte N) ;
void Draw_Power(uint16 TSC,uint8 qie);
 void Draw_XH(void);
 void Draw_WZ(int16 WEI2);
 void Dis_err(byte y, byte x, int16 num);
 void DZXH_LOGO(void);
 void TZXH_LOGO(void); 
 void NAME_LOGO(void);
 void LCD_WrCmd(byte cmd);
 void LCD_Set_Pos(byte x, byte y);
 void LCD_DLY_ms(word ms);
 void LED_PrintImage(byte *pucTable, word usRowNum, word usColumnNum);
#endif
