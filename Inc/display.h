


#ifndef	_DISPLAY_H_
#define _DISPLAY_H_
#include "stm32f1xx_hal.h"
#include "main.h"
#include "../src/user/type.h"
#define HUD1 1
#define HUD2 2	
#define BLE_CONN 3
#define BLE_UNCONN 2
#define BLE_NO 1
#define DATA_CONN 3
#define DATA_UNCONN 2
#define DATA_NO 1


 
void showPageOne(pageOneInfoType *pageOneInfo);
void showPageTwo(pageTwoInfoType *pageTwoInfo);
void showPageThree(pageThreeInfoType *pageThreeInfo);
void showPageSetSel(uint8_t selNum);

void showPageSetDATA(uint8_t selNum);
void showPageSetHUD(uint8_t selNum);
void showPageSetBLE(uint8_t selNum);
void showPageSetCal(uint8_t selNum);
void changeSN(char *SN,uint8_t lightNum);
void CLR_Arr(void);
#endif  
