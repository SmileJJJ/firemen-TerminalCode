#include "main.h"
#include "timer.h"
#include "stdint.h"
/*start a timer with the set timer number and time interval*/
TimerItemDef TimerTable[TIMERAMOUNT];
void timer_IRQ_Handle(void)
{uint8_t i=0;
	for(i=0;i<TIMERAMOUNT;i++)
	 {
		 
		 if(TimerTable[i].runningstate==SYSON)
		 {
			 if(TimerTable[i].overflag==SYSOFF)
			 {
				 if(TimerTable[i].interval>=1)
				  TimerTable[i].interval--;
				 else
				 {
					 TimerTable[i].overflag=SYSON;
					 TimerTable[i].interval=0;
				 }
			 }
		 }
	 }

}

/*the maximum timers should be less than TIMERAMOUNT*/
void StartTimer(uint8_t timernum, uint32_t interval)
{
  //  NVIC_SETPRIMASK();
    TimerTable[timernum].runningstate=SYSOFF;
    TimerTable[timernum].interval=interval;
    TimerTable[timernum].overflag=SYSOFF;
    TimerTable[timernum].runningstate=SYSON;
  //  NVIC_RESETPRIMASK();
}

/*stop a timer with the set timer number */
void StopTimer(uint8_t timernum)
{
  //  NVIC_SETPRIMASK();
    TimerTable[timernum].runningstate=SYSOFF;
    TimerTable[timernum].overflag=SYSOFF;
  //  NVIC_RESETPRIMASK();
}

/*read a timer with the set timer number.it will return 1 if it is over,
otherwise it will return 0 
also stop the timer*/
uint8_t ReadTimer(uint8_t timernum)
{
  //  NVIC_SETPRIMASK();
    if(TimerTable[timernum].runningstate==SYSON)
    {
        if(TimerTable[timernum].overflag==SYSON)
        {  //after overflag,operate ReadTimer function,timer will stop itself
            TimerTable[timernum].runningstate=SYSOFF;
            TimerTable[timernum].overflag=SYSOFF;
	//      	NVIC_RESETPRIMASK();
	      	return(OK);
			
        }
	    else
	    {
	 //       NVIC_RESETPRIMASK();
	        return(WAIT);
	    }
	 
    }
    else
    {
    //     NVIC_RESETPRIMASK();
         return(NOTSTART);
    }
}
/*read the time remainded*/
uint32_t ReadTimerValue(uint8_t timernum)
{
 return(TimerTable[timernum].interval);

}

/*to realize the delay time rated in the parameter interval*/
void DelayTimer(uint16_t interval)
{
    StartTimer(TIMERDELAY, interval);
    while(ReadTimer(TIMERDELAY)!=OK);
    
}	


