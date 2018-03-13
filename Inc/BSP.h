
#ifndef _SI4455_bsp_H
#define _SI4455_bsp_H
//#error Keil C51 detected.
#define SILABS_RADIO_SI446X
#define RADIO_DRIVER_EXTENDED_SUPPORT

 
// NOP () macro support
extern void _nop_ (void);
#define NOP() _nop_()

#include "stm32f1xx_hal.h"
#include "main.h"
#include "..\src\user\radio\radio_hal.h"
#include "..\src\user\radio\radio_comm.h"
#include "..\src\user\radio\Si446x\si446x_api_lib.h"
#include "..\src\user\radio\Si446x\si446x_defs.h"
#include "..\src\user\radio\Si446x\si446x_nirq.h"
#include "..\src\radio_config.h"
#include "..\src\radio.h"




#endif

