#ifndef SYS_H
#define SYS_H
#include "stm32f1xx_hal.h"
#include "main.h"

int8_t menFind(uint8_t *src,uint8_t *des,uint8_t len1,uint8_t len2);
void menclr(uint8_t *str1,uint8_t len);
void mencpy(uint8_t *src,uint8_t *des,uint8_t len);
uint8_t mencpr(uint8_t *str1,uint8_t *str2,uint8_t len);
#endif