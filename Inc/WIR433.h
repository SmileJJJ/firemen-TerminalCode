#ifndef	_WIR433_H_
#define _WIR433_H_
#include "stm32f1xx_hal.h"
#include "main.h"
#include "sys.h"
#define CMD_PLAY1 0X01
#define CMD_PLAY1_RE 0X02
#define CMD_PLAY2 0X03
#define CMD_PLAY2_RE 0X04
#define CMD_PLAY3 0X05
#define CMD_PLAY3_RE 0X06
#define CMD_PLAY4 0X07
#define CMD_PLAY4_RE 0X08
#define CMD_PLAY5 0X09
#define CMD_PLAY5_RE 0X10
#define CMD_UPLOAD 0X13
#define CMD_UPLOAD_RE 0X14
#define CMD_CHECK 0X15
#define CMD_CHECK_RE 0X16
#define CMD_REGISIT 0X21
#define CMD_REGISIT_RE 0X22
#define CMD_TURNOFF 0X23
#define CMD_TURNOFF_RE 0X24
 

#define CMD_CHECK12 0X31
#define CMD_CHECK12_RE 0X32
#define CMD_MARST_EXCHANGE 0X33
#define CMD_MARST_EXCHANGE_RE 0X34
#define CMD_SLAVE_EXCHANGE 0X35
#define CMD_SLAVE_EXCHANGE_RE 0X36
#define CMD_GROUP 0X37
#define CMD_GROUP_RE 0X38
#define CMD_SET1 0X41
#define CMD_SET1_RE 0X42
#define CMD_SET2 0X43
#define CMD_SET2_RE 0X44

#define STATUS_WIR_OFF 				6
#define STATUS_WIR_RUN				1	
#define STATUS_WIR_ERR				2
#define STATUS_WIR_IDLE				3	
#define STATUS_WIR_REG		    4
#define STATUS_WIR_SET_ADD		    5 
#define STATUS_WIR_SET_PARA		    7
#define MAX_CMD 5

void init_WIR433(uint8_t channal);
 
uint8_t wir433Task(void);
void turnOnWir433(void);
void turnOffWir433(void);
 uint8_t getWirStatus();
#endif

