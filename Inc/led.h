
#ifndef	_LED_H_
#define _LED_H_

#include "stm32f1xx_hal.h"
#include "main.h"

 
typedef enum
{
   LED_BLE,
	 LED_HIPOWER,
	 LED_LOPOWER,
	 MAX_LED
}led;
typedef struct 
{
	GPIO_TypeDef* port;
	uint16_t pin;
	GPIO_PinState state;
	uint16_t period;
	uint16_t duty;
	uint16_t counter;
	uint8_t on_off;
}led_hw;
void register_led(uint8_t led ,GPIO_TypeDef* port,uint16_t pin,GPIO_PinState state);
void startLed(uint8_t led ,uint16_t period,uint16_t duty);
void stopLed(uint8_t led);
void LED_IRQHandle(void);
#endif

