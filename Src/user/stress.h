#ifndef	_STRESS_H_
#define _STRESS_H_

#include "stm32f1xx_hal.h"
#include "main.h"
#include "../src/user/type.h"
uint16_t getStress(uint16_t adcValue);
void set_Init(uint16_t adcValue);
#endif