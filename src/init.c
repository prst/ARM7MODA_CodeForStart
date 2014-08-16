
//#include <AT91SAM7S64.H>  
#include <at91sam7s64.h>  

// ------------------------------------------------------------
// Init() initializes the Flash, the watchdog timer, the Power 
// Management Controller (PMC), and the PIO port.
// See the steps outlined in the section titled "Programming 
// Sequence" of the Atmel AT91SAM7S data sheet (Section 26.7 )
// to program the PMC.
// ------------------------------------------------------------
void Init_MAIN(void)
{
	AT91PS_PMC pPMC;
//	AT91PS_PIO pPIO;
	 
	// --- set flash memory parameters ---
	// MCK will be set to 47.92 MHz (approximately 48MHz) below
	// so set FMCN=48.  Add minimal wait states since we are above 40 MHz.
	 AT91C_BASE_MC->MC_FMR = ( (AT91C_MC_FMCN) & (48 << 16) ) | 
	                            AT91C_MC_FWS_1FWS ;
	 
	// --- disable the watchdog timer ---
	 AT91C_BASE_WDTC->WDTC_WDMR = AT91C_WDTC_WDDIS;
	 
	// --- program the Power Management Controller (PMC) ---
	// steps are as given in Section 26.7 "Programming Sequence"
	 
	 pPMC = AT91C_BASE_PMC;
	 
	// 1. start the "main oscillator" (Section 25.3.3)
	// slow clock frequency = 32.768 KHz
	// slow clock period = 30.5176 microseconds
	// Start up time = 8*7*30.5176 = 1708.984375 microseconds or 1.709 milliseconds
	 pPMC->PMC_MOR = ( AT91C_CKGR_OSCOUNT & ( 0x07 << 8 ) ) | 
	                   AT91C_CKGR_MOSCEN;
	 
	// 2. check the main oscillator frequency (optional)
	// skipped
	 
	// wait the master clock to settle
	 while( !(pPMC->PMC_SR & AT91C_PMC_MOSCS) );
	 
	// 3. set the PLL and the divider
	// main clock or (crystal) frequency = 18.432MHz)
	// use: USBDIV=1, DIV=5, MUL=25, and OUT=0, which yields,
	// (18.432/5)*(25+1)=95,8464MHz
	// for a LOCK time of 1000 microseconds (1 milliseconds),
	// set PLLCOUNT to 1000/30.5176=33
	 pPMC->PMC_PLLR = ( (AT91C_CKGR_DIV & 5) | 
	                    (AT91C_CKGR_PLLCOUNT & (33 << 8)) | 
						(AT91C_CKGR_MUL & (25 << 16)) | 
						(AT91C_CKGR_USBDIV & (1<< 28)) );
	
	// wait for the PLL to stabalize
	 while ( !(pPMC->PMC_SR & AT91C_PMC_LOCK) );
	// wait for the "master clock ready" flag
	 while ( !(pPMC->PMC_SR & AT91C_PMC_MCKRDY) );
	 
	// 4. select the master clock source and the prescalar
	// source    = the PLL clock
	// prescalar = 2
	 pPMC->PMC_MCKR = AT91C_PMC_CSS_PLL_CLK | 
	//                  AT91C_PMC_PRES_CLK_2;
	                  AT91C_PMC_PRES_CLK;
	 while( !(pPMC->PMC_SR & AT91C_PMC_MCKRDY) );
	 
	// 5. select programmable clocks
	// (programmable clocks are not used in this application)

	// 6. enable the peripheral clock used by PIOA and ADC
	//pPMC->PMC_PCER|=(1<<AT91C_ID_PIOA);

}
