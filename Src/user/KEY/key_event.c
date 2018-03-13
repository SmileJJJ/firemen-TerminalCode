/************************* (C) COPYRIGHT 2013 DZXH ************************
* 文  件  名      : Key_EVENT.C
* 作      者      : DZXH@Dream Creater
* 适      用      ：DZXH 开发板V2.1/V1.0。
* 淘  宝  店      : http://shop102062100.taobao.com/
* 版      本      : V1.0
* 日      期      : 2013/11
* 主  控  MCU     : STM32F103VET6
* 编  译  器      : Keil ARM 4.54
* 描      述      : 按键事件处理
**************************************************************************/
#include "key_event.h"
#include "oled.h"
#include "main.h"
extern uint8_t keyPressStatus;
extern uint8_t keyHoldStatus;
extern uint8_t keyDownStatus;
extern uint16_t lcdCloseCount;//50ms单位
/************************************************************************
* 函  数  名  : deal_key_event
* 描      述  : 按键事件处理
* 输      入  : 无.
* 返      回  : 无.
************************************************************************/
void deal_key_event()
{

    KEY_MSG_t keymsg;

    while(get_key_msg(& keymsg))     //获得按键就进行处理
    {
			if(keymsg.status == KEY_UP)
        {
         lcdCloseCount = 1200;//50ms单位
            switch(keymsg.key)
            {
            case Key_0:  
										 keyPressStatus|= KEY5;
                             break;

            case Key_1: 
										 keyPressStatus|= KEY4;
                             break;

            case Key_2:	
							 keyPressStatus|= KEY3;
                             break;

            case Key_3: 
			           keyPressStatus|= KEY2;
                             break;

            case Key_4:				
								keyPressStatus|= KEY1;

            default:         break;
            }
       
        }
        else if(keymsg.status == KEY_DOWN)
        {
       
            switch(keymsg.key)
            {
            case Key_0:  
										 	keyDownStatus|= KEY5;
                             break;

            case Key_1: 
										 
                             break;

            case Key_2:	
							  
                             break;

            case Key_3: 
			           
                             break;

            case Key_4:				
					 
                             break;            
 

            default:         break;
            }
       
        }
        else if(keymsg.status == KEY_HOLD)
        {
         
            switch(keymsg.key)
            {
            case Key_0:  
														keyHoldStatus|= KEY5;
                             break;

            case Key_1:		 keyHoldStatus|= KEY4;
                             break;

            case Key_2:			 keyHoldStatus|= KEY3;
                             break;

            case Key_3:	 			keyHoldStatus|= KEY2;
			           
                             break;

            case Key_4:					 keyHoldStatus|= KEY1;
					
                             break;            

            default:         break;
		
            }
		
						
					}
    }
}
//*****************************END**********************************
