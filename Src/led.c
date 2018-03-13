#include "led.h"

led_hw led_buff[MAX_LED]={0};
 
 void led_on(uint8_t led);
void led_off(uint8_t led);
void LED_IRQHandle(void)
{
	uint8_t i=0;
	for(i=0;i<MAX_LED;i++)
	{
		led_buff[i].counter++;
		if(led_buff[i].on_off==1)
		{
			if(led_buff[i].counter>led_buff[i].period)
			{
				led_buff[i].counter=0;
				led_on(i);
			}else if(led_buff[i].counter>led_buff[i].duty)
			{
				 led_off(i);
			}
		}
		else
		{   
				led_off(i);
				led_buff[i].on_off = 0;
				led_buff[i].period = 0;
				led_buff[i].duty = 0;
		}
		
	}
		
}
void register_led(uint8_t led ,GPIO_TypeDef* port,uint16_t pin,GPIO_PinState state)
{
		led_buff[led].port = port;
		led_buff[led].pin = pin;
		led_buff[led].state = state;
}
void startLed(uint8_t led ,uint16_t period,uint16_t duty)
{
	led_buff[led].on_off = 1;
	led_buff[led].period = period;
	led_buff[led].duty = duty;
}

void stopLed(uint8_t led)
{
	led_buff[led].on_off = 0;
}
void led_on(uint8_t led)
{
	HAL_GPIO_WritePin(led_buff[led].port,led_buff[led].pin ,led_buff[led].state);
}
void led_off(uint8_t led)
{
	if(led_buff[led].state==GPIO_PIN_RESET)
	{
		HAL_GPIO_WritePin(led_buff[led].port,led_buff[led].pin ,GPIO_PIN_SET);
	}
	else
	{
			HAL_GPIO_WritePin(led_buff[led].port,led_buff[led].pin ,GPIO_PIN_RESET);
	}
}

