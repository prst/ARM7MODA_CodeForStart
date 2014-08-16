
//#include <AT91SAM7S64.H>  
#include <at91sam7s64.h>  


// ------------------------------------------------------------
// Init() initializes the Flash, the watchdog timer, the Power 
// Management Controller (PMC), and the PIO port.
// See the steps outlined in the section titled "Programming 
// Sequence" of the Atmel AT91SAM7S data sheet (Section 26.7 )
// to program the PMC.
// ------------------------------------------------------------
void PWM_Init_sys(void)
{
#ifdef ENABLE_PWM
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
	
	// 6. enable the peripheral clock used by PIOA and PWMC
	 pPMC->PMC_PCER = ( 1 << AT91C_ID_PIOA) | 
	                  ( 1 << AT91C_ID_PWMC );
	 
	// initializ the port defined as PORT_LED as an output port.
	 
	 pPIO = AT91C_BASE_PIOA;
	//configure the PIO register used by the PWM channel 0
	/*
	 pPIO->PIO_PDR = PORT_MASK(PORT_PWM0); // PWM port is controlled by the peripheral
	 //pPIO->PIO_BSR = PORT_MASK(PORT_PWM); // PWM port is peripheral B (multiplexed with PA11)
	 pPIO->PIO_ASR = PORT_MASK(PORT_PWM0); // PWM port is peripheral A (multiplexed with PA2) 
	*/
	// PWM port is controlled by the peripheral
	 pPIO->PIO_PDR = PORT_MASK(PORT_PWM0)|PORT_MASK(PORT_PWM1)|PORT_MASK(PORT_PWM2); 
	// PWM port is peripheral A (multiplexed with PA0..2)
	 pPIO->PIO_ASR = PORT_MASK(PORT_PWM0)|PORT_MASK(PORT_PWM1)|PORT_MASK(PORT_PWM2); 

	// 7. Configure PIO
	 Init_PWM_IO_PIO();

#endif /*ENABLE_PWM*/
}

void PWM_Init_IO_PIO(void)
{
#ifdef ENABLE_PWM
	AT91PS_SYS   regs = AT91C_BASE_SYS;
	unsigned int data;
	/* Init I/O Lines 
	----------------------------------------------
	    D	N	Discription of connect to pins
	----------------------------------------------
	    0	48	led0	lcd_d0
	    1	47	led1	lcd_d1
	    2	44	led2	lcd_d2
	    3	43			lcd_d3
	    4	36			lcd_d4
	    5	35			lcd_d5
	    6	34			lcd_d6
	    7	32			lcd_d7
	    8	31	
	    9	30	DRXD
	    10	29	DTXD
	    11	28
	    12	27
	    13	22
	    14	21
	    15	20			lcd_E
	    16	19	k0
	    17	9	k1
	    18	10	k2
	    19	13	k3
	    20	16			lcd_RS
	    21	11			
	    22	14
	    23	15
	    24	23
	    25	25
	    26	26
	    27	37
	    28	38
	    29	41
	    30	42
	    31	52
	----------------------------------------------
	0x15ABCDEF   
	    0001 0101 1010 1011 1100 1101 1110 1111
	HEX    1    5    A    B    C    D    E    F 
	----------------------------------------------*/
	
	/* PIO enable */
	// PIO Enable/Disable Register
	data=0x0a00003ff;
	regs->PIOA_PER  = data;	regs->PIOA_PDR  =~data; // pck
	
	/* Output Enable */
	// Output Enable/Disable Register
	data=0x000003ff;
	regs->PIOA_OER  = data;	regs->PIOA_ODR  =~data;
	
	/* Glitch */
	// Input Filter Enable/Disable Register
	data=0x000F0000;
	regs->PIOA_IFER = data;	regs->PIOA_IFDR =~data;
	
	// Set/Clear Output Data Register
	data=0x00000000;
	regs->PIOA_SODR = data;	regs->PIOA_CODR = data;
	
	// Interrupt Enable/Disable Register
	data=0x00000000;
	regs->PIOA_IER  = data;	regs->PIOA_IDR  =~data;
	
	// Multi-driver Enable/Disable Register
	data=0x00000000;
	regs->PIOA_MDER = data;	regs->PIOA_MDDR =~data;
	
	/* Pull-up  */
	// Pull-up Enable/Disable Register
	data=0x000003ff;    
	regs->PIOA_PPUER= data;	regs->PIOA_PPUDR=~data; // pck
	//data=0x00000000;	
	//regs->PIOA_ASR  =data;	regs->PIOA_BSR  =~data;
	
	/* Out stat wr en*/ 
	// Output Write Enable/Disable Register
	data=0x000003ff;	
	regs->PIOA_OWER = data;	regs->PIOA_OWDR =~data; // pck
	/*---------------------------------------------------------------*/

#endif /*ENABLE_PWM*/
}

void PWM_main(void)
{
#ifdef ENABLE_PWM
	AT91PS_PWMC     pPwmc;
	AT91PS_PWMC_CH  pPwmCh0;
	AT91PS_PWMC_CH  pPwmCh1;
	AT91PS_PWMC_CH  pPwmCh2;
	
	volatile int     n = 0;
	volatile unsigned int  nDC = 0;
	volatile unsigned int  nDC0 = 0;
	volatile unsigned int  nDC1 = 0;
	volatile unsigned int  nDC2 = 0;
	 
	// perform initialization tasks
	 Init();
	 
	// enable pwm channel 0 -- no interrupts
	 pPwmc = AT91C_BASE_PWMC;
	 //pPwmc->PWMC_MR = 0x00000fff; // preB-divB-preA-divA
	 pPwmc->PWMC_ENA = 0x07;  // enable pwm channel 0-2
	  
	 pPwmCh0             = AT91C_BASE_PWMC_CH0;
	 pPwmCh0->PWMC_CMR   = 0x030A; // polarity 1 (start high), PWM_CLK = MCK/1024
	 pPwmCh0->PWMC_CPRDR = 0x100;  // period
	 pPwmCh0->PWMC_CDTYR = 0x0;   // duty cycle
	
	 pPwmCh1             = AT91C_BASE_PWMC_CH1;
	 pPwmCh1->PWMC_CMR   = 0x030A; // polarity 1 (start high), PWM_CLK = MCK/1024
	 pPwmCh1->PWMC_CPRDR = 0x100;  // period
	 pPwmCh1->PWMC_CDTYR = 0x0;   // duty cycle
	
	 pPwmCh2             = AT91C_BASE_PWMC_CH2;
	 pPwmCh2->PWMC_CMR   = 0x030A; // polarity 1 (start high), PWM_CLK = MCK/1024
	 pPwmCh2->PWMC_CPRDR = 0x100;  // period
	 pPwmCh2->PWMC_CDTYR = 0x30;   // duty cycle
	
	 nDC0 = (100/3)*3;
	 nDC1 = (100/3)*2;
	 nDC2 = (100/3)*1;
	
	// endless loop blinks the LED
	 while(TRUE)
	  {
	   for(n=0; n<0x40000; n++);	// delay between iterations
	
	   nDC0++;
	   if ( 0x40 > (nDC0 >> 1) )
	   {}
	   else nDC0=0;
	     pPwmCh0->PWMC_CUPDR = nDC0;
	
	   for(n=0; n<0x40000; n++);	// delay between iterations
	
	   nDC1++;
	   if ( 0x40 > (nDC1 >> 1) )
	   {}
	   else nDC1=0;
	     pPwmCh1->PWMC_CUPDR = nDC1;
	
	   for(n=0; n<0x40000; n++);	// delay between iterations
	
	   nDC2++;
	   if ( 0x40 > (nDC2 >> 1) )
	   {}
	   else nDC2=0;
	     pPwmCh2->PWMC_CUPDR = nDC2;
	
	  }

#endif /*ENABLE_PWM*/
}

