#ifndef _TIMER_H_
#define _TIMER_H_
/* Includes ------------------------------------------------------------------*/
#include "main.h"
 #include "stdint.h"
//define the number of the Timer
enum  {
       TIMERCOMMON=0,	  		
	   TIMERDELAY,
	   TIMERSTATUS_Machine,
	    TIMER_BLE,
		 TIMER_WIR433,
		 TIMER_BATT_Test,
			TIMER_LCD,
		  TIMER_DS18B20,
	   TIMERAMOUNT			  
}; 
enum
{
    WAIT=0,
    OK,
    NOTSTART
};
/*define the on/off state*/
enum
{
   SYSOFF=0,
   SYSON

};
typedef struct TimerItem
{
   uint8_t runningstate;/*ON:running ;OFF:stop*/
   //uint8_t  timernumb;
   uint32_t interval;
   uint8_t  overflag;  /*timer over flag*/   
} TimerItemDef;


extern TimerItemDef TimerTable[TIMERAMOUNT];
extern void StartTimer(uint8_t timernum, uint32_t interval);
extern void StopTimer(uint8_t timernum);
extern uint8_t ReadTimer(uint8_t timernum);
extern uint32_t ReadTimerValue(uint8_t timernum);
extern void DelayTimer(uint16_t interval);
extern void timer_IRQ_Handle(void);

#endif
