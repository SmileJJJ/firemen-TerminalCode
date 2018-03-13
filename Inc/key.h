/************************* (C) COPYRIGHT 2013 DZXH ************************
* ��  ��  ��      : KEY.H
* ��      ��      : DZXH@Dream Creater
* ��      ��      ��DZXH ������V2.1/V1.0��
* ��  ��  ��      : http://shop102062100.taobao.com/
* ��      ��      : V1.0
* ��      ��      : 2013/11
* ��  ��  MCU     : STM32F103VET6
* ��  ��  ��      : Keil ARM 4.54
* ��      ��      : ������ʼ��ͷ�ļ�
**************************************************************************/
#ifndef __KEY_H__
#define __KEY_H__

#include "stm32f1xx_hal.h"
#include "main.h"
#define  Key_0    (1<<1) 
#define  Key_1    (1<<2) 
#define  Key_2    (1<<3) 
#define  Key_3    (1<<4) 
#define  Key_4    (1<<5) 
#define  Key_UP    (1<<6) 
#define  Key_DOWN   (1<<7) 
#define  Key_L    (1<<8) 
#define  Key_R    (1<<9) 
#define  Key_OK    (1<<10) 

//�����Ƕ��尴����ʱ�䣬��λΪ �� 1ms
#define KEY_DOWN_TIME   10
#define KEY_HOLD_TIME   500         //���253��������Ҫ�޸�keytime������

#define KEY_OFFSET 4096
#define KEY_ 200
#define KEY_GPIO GPIOB
//�����˿ڵ�ö��
typedef enum
{
    KEY_0=300, 
    KEY_1=1332,
    KEY_2=1995,
    KEY_3=2673,
    KEY_4=3650,
  
    KEY_UPP=372+KEY_OFFSET, 
   KEY_OK =1365+KEY_OFFSET,
    KEY_L=1985+KEY_OFFSET,
    KEY_R=2730+KEY_OFFSET,
   KEY_DOWNN =3723+KEY_OFFSET,
    KEY_MAX=10,
} KEY_e;



//key״̬�궨��
typedef enum
{
    KEY_DOWN  =   0,         //��������ʱ��Ӧ��ƽ
    KEY_UP    =   1,         //��������ʱ��Ӧ��ƽ
    KEY_HOLD,

} KEY_STATUS_e;

//��������״̬�ṹ��
typedef struct
{
    uint16_t           key;
    KEY_STATUS_e    status;
} KEY_MSG_t;

void    KEY_init(KEY_e key);                    // KEY��ʼ��    ����
void    KEY_ALL_init(void);                     //ȫ��Key��ʼ��
KEY_STATUS_e key_check(uint8_t key);              //���key״̬
 

//��ʱɨ�谴��
uint8_t get_key_msg(KEY_MSG_t *keymsg);                  //��ȡ������Ϣ������1��ʾ�а�����Ϣ��0Ϊ�ް�����Ϣ
void key_IRQHandler(void);                      //
uint16_t absi(int16_t data_s);

#endif 
//*****************************END**********************************
