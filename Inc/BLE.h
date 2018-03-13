#ifndef	_BLE_H_
#define _BLE_H_
#include "stm32f1xx_hal.h"
#include "main.h"

extern uint8_t bleSendBuff[40];
extern uint8_t bleRecBuff[40];
extern uint8_t recFlag;
uint8_t initBLE(void);
uint8_t checkConnInData(void);
uint8_t bleEnterTrainsmit(void);
uint8_t bleCheckConn(void);
uint8_t exit_trans(void);
void sendInfo(void);
void bleGetDate(void);
uint8_t getCheck(uint8_t * str,uint8_t len);
#endif

