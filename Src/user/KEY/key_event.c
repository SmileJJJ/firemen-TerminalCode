/************************* (C) COPYRIGHT 2013 DZXH ************************
* ��  ��  ��      : Key_EVENT.C
* ��      ��      : DZXH@Dream Creater
* ��      ��      ��DZXH ������V2.1/V1.0��
* ��  ��  ��      : http://shop102062100.taobao.com/
* ��      ��      : V1.0
* ��      ��      : 2013/11
* ��  ��  MCU     : STM32F103VET6
* ��  ��  ��      : Keil ARM 4.54
* ��      ��      : �����¼�����
**************************************************************************/
#include "key_event.h"
#include "oled.h"
#include "main.h"
extern uint8_t keyPressStatus;
extern uint8_t keyHoldStatus;
extern uint8_t keyDownStatus;
extern uint16_t lcdCloseCount;//50ms��λ
/************************************************************************
* ��  ��  ��  : deal_key_event
* ��      ��  : �����¼�����
* ��      ��  : ��.
* ��      ��  : ��.
************************************************************************/
void deal_key_event()
{

    KEY_MSG_t keymsg;

    while(get_key_msg(& keymsg))     //��ð����ͽ��д���
    {
			if(keymsg.status == KEY_UP)
        {
         lcdCloseCount = 1200;//50ms��λ
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
