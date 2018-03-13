/************************* (C) COPYRIGHT 2013 DZXH ************************
* ��  ��  ��      : KEY.C
* ��      ��      : DZXH@Dream Creater
* ��      ��      ��DZXH ������V2.1/V1.0��
* ��  ��  ��      : http://shop102062100.taobao.com/
* ��      ��      : V1.0
* ��      ��      : 2013/11
* ��  ��  MCU     : STM32F103VET6
* ��  ��  ��      : Keil ARM 4.54
* ��      ��      : ������ʼ��
**************************************************************************/


#include "key.h"
 
uint16_t ADCConvertedValue[2];
 uint16_t KEY_One[KEY_MAX] = {KEY_0,KEY_1,KEY_2,KEY_3,KEY_4,KEY_UPP, KEY_DOWNN, KEY_L, KEY_R, KEY_OK};
  uint16_t KEY_index[KEY_MAX] = {Key_0,Key_1,Key_2,Key_3,Key_4,Key_UP, Key_DOWN, Key_L, Key_R, Key_OK};
 
 /************************************************************************
* ��  ��  ��  : KEY_ALL_init
* ��      ��  : ��ʼ��AD  ����ת�� DMA ģʽ
								��ʼ����ʱ�� ��������ֵ
* ��      ��  : ��.
* ��      ��  : ��.
************************************************************************/
void    KEY_ALL_init(void)
{ 	
 


}

 /************************************************************************
* ��  ��  ��  : key_check
* ��      ��  : ���key״̬
* ��      ��  : ������ţ�1-KEY_MAX����ĿǰKEY_MAX=10
* ��      ��  : ��.
************************************************************************/
KEY_STATUS_e key_check(uint8_t key)
{

    uint16_t key_Value    = KEY_One[key];
	
	if(key<5)//��ADC ͨ��0��
		{
			if(absi(ADCConvertedValue[0]-key_Value)<=KEY_)   //KEY_Ϊ����������
				{
					return KEY_DOWN;             //����������
				}
			else
				{
					 return KEY_UP;
				}
		}
	else //��ͨ��1��
		{
			if(absi(ADCConvertedValue[1]-key_Value+KEY_OFFSET)<=KEY_)
				{
					return KEY_DOWN;
				}
			else
				{
					 return KEY_UP;
				}		
	
		}

}





//���尴��״̬�ṹ��
typedef enum
{
    KEY_MSG_EMPTY,      //û�а�����Ϣ
    KEY_MSG_NORMAL,     //�������а�����Ϣ��������
    KEY_MSG_FULL,       //������Ϣ��
} key_msg_e;

#define KEY_MSG_FIFO_SIZE   20      //��� 255��������Ҫ�޸�key_msg_front/key_msg_rear����
KEY_MSG_t       key_msg[KEY_MSG_FIFO_SIZE];
volatile uint8_t     key_msg_front = 0, key_msg_rear = 0; //����FIFO��ָ��
volatile uint8_t     key_msg_flag = KEY_MSG_EMPTY;

 /************************************************************************
* ��  ��  ��  : send_key_msg
* ��      ��  : 
* ��      ��  : 
* ��      ��  : ��.
************************************************************************/
void send_key_msg(KEY_MSG_t keymsg)
{
    //������FIFO��
    if(key_msg_flag == KEY_MSG_FULL)
    {
        //����ֱ�Ӳ�����
        return ;
    }
    key_msg[key_msg_rear].key = keymsg.key;
    key_msg[key_msg_rear].status = keymsg.status;

    key_msg_rear++;

    if(key_msg_rear >= KEY_MSG_FIFO_SIZE)
    {
        key_msg_rear = 0;                       //��ͷ��ʼ
    }

    if(key_msg_rear == key_msg_front)                   //׷��ƨ���ˣ�����
    {
        key_msg_flag = KEY_MSG_FULL;
    }
    else
    {
        key_msg_flag = KEY_MSG_NORMAL;
    }
}
 /************************************************************************
* ��  ��  ��  : get_key_msg
* ��      ��  : 
* ��      ��  : 
* ��      ��  : ��.
************************************************************************/
uint8_t get_key_msg(KEY_MSG_t *keymsg)
{
    if(key_msg_flag == KEY_MSG_EMPTY)   //�գ�ֱ�ӷ���0
    {
        return 0;
    }
    keymsg->key = key_msg[key_msg_front].key;
    keymsg->status = key_msg[key_msg_front].status;

    key_msg_front++;                //���ڷǿգ����Կ���ֱ�ӳ�����

    if(key_msg_front >= KEY_MSG_FIFO_SIZE)
    {
        key_msg_front = 0;          //��ͷ��ʼ
    }

    if(key_msg_front == key_msg_rear)       //׷��ƨ���ˣ����ն��п�
    {
        key_msg_flag = KEY_MSG_EMPTY;
    }

    return 1;
}

 static uint16_t keytime[KEY_MAX];
//�ж��Ƿ���
uint8_t checkIdle(uint8_t keyNum)
{
	if(keytime[keyNum]>0)
	{
		return 0;
	}
	else
	{
		return 1;
	}
}

 /************************************************************************
* ��  ��  ��  : key_IRQHandler
* ��      ��  : ��ʱ���key״̬
* ��      ��  : ��.
* ��      ��  : ��.
************************************************************************/
void key_IRQHandler(void)
{

    uint8_t    keynum;

    KEY_MSG_t keymsg;
		static uint8_t hold_trig=0;;
		ADCConvertedValue[0]= getAdcAfterFilt( KEY_ADC_CH ,1);
    for(keynum = 0 ; keynum < KEY_MAX; keynum ++)
    {

        if(key_check( keynum)  == KEY_DOWN)
        {
            keytime[keynum]++;

            if(keytime[keynum] <= KEY_DOWN_TIME)
            {
                continue;
            }
            else if(keytime[keynum] == KEY_DOWN_TIME + 1 )        //��Ч����
            {
                keymsg.key = KEY_index[keynum];
                keymsg.status = KEY_DOWN;
                send_key_msg(keymsg);
            }
            else if(keytime[keynum] <= KEY_HOLD_TIME)
            {
                continue;
            }
            else if(keytime[keynum]  == KEY_HOLD_TIME + 1)    //��Ч��ס
            {
                keymsg.key = KEY_index[keynum];;
                keymsg.status = KEY_HOLD;
                send_key_msg(keymsg);
								hold_trig = 1;
              //  keytime[keynum] = 0;
            }
            else
            {
                keytime[keynum] = KEY_DOWN_TIME + 1;
            }
        }
        else
        {
            if(keytime[keynum] > KEY_DOWN_TIME)
            {		
								if(hold_trig==0)
								{
                keymsg.key = KEY_index[keynum];;
                keymsg.status = KEY_UP;
                send_key_msg(keymsg);           //��������
								}
								else
								{
									hold_trig = 0;
								}
            }
            keytime[keynum] = 0;
        }
    }
}
/************************************************************************
* ��  ��  ��  : absi
* ��      ��  : �����ֵ����
* ��      ��  : ��.
* ��      ��  : ��.
************************************************************************/
uint16_t absi(int16_t data_s)
{
	return data_s>=0?data_s:-data_s;
}
//*****************************END**********************************
