/*!
 * File:
 *  radio_hal.c
 *
 * Description:
 *  This file contains RADIO HAL.
 *
 * Silicon Laboratories Confidential
 * Copyright 2011 Silicon Laboratories, Inc.
 */

                /* ======================================= *
                 *              I N C L U D E              *
                 * ======================================= */

#include "bsp.h"
#include "spi.h"

                /* ======================================= *
                 *          D E F I N I T I O N S          *
                 * ======================================= */

                /* ======================================= *
                 *     G L O B A L   V A R I A B L E S     *
                 * ======================================= */

                /* ======================================= *
                 *      L O C A L   F U N C T I O N S      *
                 * ======================================= */

                /* ======================================= *
                 *     P U B L I C   F U N C T I O N S     *
                 * ======================================= */

void radio_hal_AssertShutdown(void)
{
HAL_GPIO_WritePin(SI_SDN_GPIO_Port,SI_SDN_Pin,GPIO_PIN_SET);
}

void radio_hal_DeassertShutdown(void)
{
HAL_GPIO_WritePin(SI_SDN_GPIO_Port,SI_SDN_Pin,GPIO_PIN_RESET);
}

void radio_hal_ClearNsel(void)
{
HAL_GPIO_WritePin(SI_SEL_GPIO_Port,SI_SEL_Pin,GPIO_PIN_RESET);
  //  RF_NSEL = 0;
}

void radio_hal_SetNsel(void)
{
  HAL_GPIO_WritePin(SI_SEL_GPIO_Port,SI_SEL_Pin,GPIO_PIN_SET);
  //  RF_NSEL = 1;
}

GPIO_PinState radio_hal_NirqLevel(void)
{
	
    return HAL_GPIO_ReadPin(SI_IRQ_GPIO_Port,SI_IRQ_Pin);
}
void radio_hal_SpiWriteByte(uint8_t byteToWrite)
{
	HAL_SPI_Transmit(&hspi1,&byteToWrite,1,100);
  //SpiReadWrite(byteToWrite);
}

uint8_t radio_hal_SpiReadByte(void)
{
	uint8_t byteToRead;
	HAL_SPI_Receive(&hspi1,&byteToRead,1,100);
  return byteToRead;
}

void radio_hal_SpiWriteData(uint8_t byteCount, uint8_t* pData)
{
HAL_SPI_Transmit(&hspi1,pData,byteCount,100);
 // SpiWriteData(byteCount, pData);
}

void radio_hal_SpiReadData(uint8_t byteCount, uint8_t* pData)
{
 	HAL_SPI_Receive(&hspi1,pData,byteCount,100);
}

#ifdef RADIO_DRIVER_EXTENDED_SUPPORT
GPIO_PinState radio_hal_Gpio0Level(void)
{
  GPIO_PinState retVal = GPIO_PIN_RESET;

#ifdef SILABS_PLATFORM_DKMB
  retVal = GPIO_PIN_RESET;
#endif
#ifdef SILABS_PLATFORM_UDP
  retVal = EZRP_RX_DOUT;
#endif
#if (defined SILABS_PLATFORM_RFSTICK) || (defined SILABS_PLATFORM_LCDBB)
  retVal = RF_GPIO0;
#endif
#if (defined SILABS_PLATFORM_WMB930)
  retVal = GPIO_PIN_RESET;
#endif
#if defined (SILABS_PLATFORM_WMB912)
  #ifdef SILABS_IO_WITH_EXTENDER
    //TODO
    retVal = GPIO_PIN_RESET;
  #endif
#endif

  return retVal;
}

GPIO_PinState radio_hal_Gpio1Level(void)
{
  GPIO_PinState retVal = GPIO_PIN_RESET;

#ifdef SILABS_PLATFORM_DKMB
  retVal = FSK_CLK_OUT;
#endif
#ifdef SILABS_PLATFORM_UDP
  retVal = GPIO_PIN_RESET; //No Pop
#endif
#if (defined SILABS_PLATFORM_RFSTICK) || (defined SILABS_PLATFORM_LCDBB) || (defined SILABS_PLATFORM_WMB930)
  retVal = RF_GPIO1;
#endif
#if defined (SILABS_PLATFORM_WMB912)
  #ifdef SILABS_IO_WITH_EXTENDER
    //TODO
    retVal = GPIO_PIN_RESET;
  #endif
#endif

  return retVal;
}

GPIO_PinState radio_hal_Gpio2Level(void)
{
  GPIO_PinState retVal = GPIO_PIN_RESET;

#ifdef SILABS_PLATFORM_DKMB
  retVal = DATA_NFFS;
#endif
#ifdef SILABS_PLATFORM_UDP
  retVal = GPIO_PIN_RESET; //No Pop
#endif
#if (defined SILABS_PLATFORM_RFSTICK) || (defined SILABS_PLATFORM_LCDBB) || (defined SILABS_PLATFORM_WMB930)
  retVal = RF_GPIO2;
#endif
#if defined (SILABS_PLATFORM_WMB912)
  #ifdef SILABS_IO_WITH_EXTENDER
    //TODO
    retVal = GPIO_PIN_RESET;
  #endif
#endif

  return retVal;
}

GPIO_PinState radio_hal_Gpio3Level(void)
{
  GPIO_PinState retVal = GPIO_PIN_RESET;

#if (defined SILABS_PLATFORM_RFSTICK) || (defined SILABS_PLATFORM_LCDBB) || (defined SILABS_PLATFORM_WMB930)
  retVal = RF_GPIO3;
#elif defined (SILABS_PLATFORM_WMB912)
  #ifdef SILABS_IO_WITH_EXTENDER
    //TODO
    retVal = GPIO_PIN_RESET;
  #endif
#endif

  return retVal;
}

#endif
