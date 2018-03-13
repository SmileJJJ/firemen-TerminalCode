#ifndef	_TYPR_H_
#define _TYPR_H_

#include "stm32f1xx_hal.h"
#include "main.h"



typedef struct{
	uint8_t HUDflag;
	uint8_t BLEflag;
	uint8_t DATAflag;
	short	temperature;
	uint16_t airPress;
	uint16_t batt;
	uint8_t voiceLevel;
	//系统参数
	uint16_t BLE_SendInterval;
	uint8_t BLE_SendTotal;

}pageOneInfoType;
typedef struct{
	uint8_t SN[9];
	uint8_t HUD[9];
	uint8_t	BHTMAC[13];
	uint8_t GP[12];
}pageTwoInfoType;
typedef struct{
	uint8_t Version[9];
	uint8_t DATE[9];
	//设置参数
	uint8_t BLE_Switch;
 
	uint8_t DateSwitch;
	uint16_t AirF0;
	uint16_t AirF1;
	uint16_t AirF2;
	uint16_t runTime;
	uint8_t channal;
	uint8_t serialRate;
	uint8_t airRate;
	uint8_t FFC;
	uint8_t poweSet;
}pageThreeInfoType;
typedef struct{
 
	pageOneInfoType pageOneInfo;
  pageTwoInfoType pageTwoInfo;
  pageThreeInfoType pageThreeInfo;
}para_struct_type;
 
typedef struct{
	uint8_t deviceType;
	uint8_t serialNum[4];
	uint8_t saved[11];
}downLoadInGroupType;
typedef struct{
	uint8_t deviceType;
	uint8_t serialNum[4];
	uint8_t airStress[2];
	uint8_t voltage[2];
	uint8_t temperature;
	uint8_t runTime[2];
	uint8_t saved[3];
	uint8_t status;
}upLoadInGroupType;
typedef struct{
	uint8_t deviceType;
	uint8_t serialNumDes[4];
	uint8_t serialNumCtr[4];
	uint8_t desChannal;
	uint8_t saved[5];
	uint8_t status;
}downLoadOutGroup_ExcType;
typedef struct{
	uint8_t deviceType;
	uint8_t serialNumDs[4];
	uint8_t serialNumNew[4];
	uint8_t saved[7];
}downLoadInGroup_Set1Type;
typedef struct{
	uint8_t deviceType;
	uint8_t serialNumDs[4];
	uint8_t serialRate;
	uint8_t airRate;
	uint8_t FFC;
	uint8_t poweSet;
	uint8_t serialRateNew;
	uint8_t airRateNew;
	uint8_t FFCNew;
	uint8_t poweSetNew;
	uint8_t saved[3];
}downLoadInGroup_Set2Type;
typedef struct{
	uint8_t deviceType;
	uint8_t serialNumDes[4];
	uint8_t serialNumCtr[4];
	uint8_t serialNumNew[4];
	uint8_t saved[3];
}downLoadOutGroup_GroupType;

typedef union {
	downLoadInGroupType downLoadInGroup;
	uint8_t parameter[16];
	downLoadInGroup_Set2Type downLoadInGroup_Set2;
	upLoadInGroupType upLoadInGroup;
	downLoadOutGroup_ExcType downLoadOutGroup_Exc;
	downLoadOutGroup_GroupType downLoadOutGroup_Group;
	downLoadInGroup_Set1Type downLoadInGroup_Set1;
}paraDataType ;

typedef struct
{
	uint8_t head[2];
	uint8_t direct;
	uint8_t path;
	uint8_t cmd;
	paraDataType paraData;
	uint8_t verify;
}framType;
typedef struct
{
  uint8_t starrByte;
	uint8_t version;
	uint8_t len;
	uint8_t cmd;
	uint8_t cmdBack;
	uint8_t para[2];
	uint8_t checkSum[2];
	uint8_t endByte;
	
}spkFramType;
 
#endif