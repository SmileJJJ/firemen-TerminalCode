#ifndef	_RADIOAPP_H_
#define _RADIOAPP_H_
#include "stm32f1xx_hal.h"
#include "main.h"
#include "sys.h"

typedef struct{
			uint8_t header;
			uint8_t addrLocal[3];
			uint8_t stress[2];
			uint8_t temperature;
			uint8_t voltage;
			uint8_t checkSum;
}Hud1Typede;
typedef struct{
			uint8_t header;
			uint8_t addrLocal[3];
			uint8_t addrPromote[3];
			uint8_t stress[2];
			uint8_t temperature;
			uint8_t voltage;
			uint8_t checkSum;
}Hud2Typede;

#define RADIO_IDLE 0
#define RADIO_RUN 1
#define RADIO_PAIR 2
#define RADIO_ERR 3
uint8_t getPairStatus(void);
void radioPair(void);
uint8_t radioTask(void);
uint8_t radioAppInit(void);
#endif
