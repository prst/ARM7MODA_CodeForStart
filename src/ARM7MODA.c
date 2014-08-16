/******************************************************************************/
/* BLINKY.C: LED Flasher                                                      */
/******************************************************************************/
/* This file is part of the uVision/ARM development tools.                    */
/* Copyright (c) 2005-2006 Keil Software. All rights reserved.                */
/* This software may only be used under the terms of a valid, current,        */
/* end user licence from KEIL for a compatible version of KEIL software       */
/* development tools. Nothing else gives you the right to use this software.  */
/******************************************************************************/
                  
/* AT91SAMT7S64 definitions  */
//#include <AT91SAM7S64.H>  
#include "at91sam7s64.h"

#include "Board.h"

//#include	"variables.h"
#include	"init_base.h"
#include	"delays.h"
//#include	"pwm.h"
//#include	"adc.h"
#include	"lcd.h"


// Simple program that runs PWM channel 0
// connect the output (PA11) to an LED
 
//#define  PORT_PWM              11
#define  PORT_PWM0              0
#define  PORT_PWM1              1
#define  PORT_PWM2              2
 
#define  PORT_MASK(nBit) (1 << nBit)
 
#define  TRUE                   1
#define  FALSE                  0

#define  PORT_LED0  11
#define  PORT_LED1  27
#define  PORT_LED2  29
#define  PORT_LED3  31
#define  CHANNEL    7

//#define  ENABLE_PWM
//#define  ENABLE_ADC
#define  ENABLE_LCD

typedef  unsigned char       BOOL;
typedef  unsigned char       BYTE;
 

// ------------------------------------------------------------
BOOL GetPort(BYTE bPort)
{
	AT91PS_PIO pPio;
	 
	pPio = AT91C_BASE_PIOA;
	return( ( pPio->PIO_PDSR & PORT_MASK(bPort) )?TRUE:FALSE );  // return BOOL
}

// ------------------------------------------------------------
void SetPort(BYTE bPort, BOOL bState)
{
	AT91PS_PIO pPio;
	 
	pPio = AT91C_BASE_PIOA;
	if (bState) pPio->PIO_SODR = PORT_MASK(bPort);
	   else pPio->PIO_CODR = PORT_MASK(bPort);
}

// ------------------------------------------------------------
void ADCOutData( unsigned int uiPort )
{
	AT91PS_PIO pPio;

	pPio   = AT91C_BASE_PIOA;
	uiPort = uiPort & 0x3ff;

	pPio->PIO_ODSR &= 0xfffffc00;
	pPio->PIO_ODSR |= uiPort;
}

// ------------------------------------------------------------
int main(void)
{

    // perform initialization tasks
	//Init_MAIN();

	LCD_Init_sys();

	delay();
	LCD_Config();	// init of LCD

	while (1)
	{
	    delay();
        LCD_Main();
	}
}
 
// -------------------------------------------------------------
// end of module main
// -------------------------------------------------------------
