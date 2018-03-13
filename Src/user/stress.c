#include "../src/user/stress.h"
extern para_struct_type para_struct;
	static uint16_t filter=0;
void set_Init(uint16_t adcValue)
{
	filter=adcValue;
}
uint16_t getStress(uint16_t adcValue)
{
	uint16_t stress=0;

	static uint8_t count = 0;
	filter =filter*0.8+adcValue*0.2;

	//filter=adcValue;
	if(filter<para_struct.pageThreeInfo.AirF0)
	{
		stress=0;
	}
	else if(filter<para_struct.pageThreeInfo.AirF1)
	{
		stress=(filter-para_struct.pageThreeInfo.AirF0)*1000/(para_struct.pageThreeInfo.AirF1-para_struct.pageThreeInfo.AirF0); 
	}	else if(adcValue<para_struct.pageThreeInfo.AirF2)
	{
		stress=(filter-para_struct.pageThreeInfo.AirF1)*2000/(para_struct.pageThreeInfo.AirF2-para_struct.pageThreeInfo.AirF1)+1000; 
	}
		else //ÏŞÖÆµ½40   
	{
		stress=(filter-para_struct.pageThreeInfo.AirF2)*2000/(para_struct.pageThreeInfo.AirF2-para_struct.pageThreeInfo.AirF1)+3000; 
	}
	
	if(stress>4000)
		stress=4000;
	return stress ;
}