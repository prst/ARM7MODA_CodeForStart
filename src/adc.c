
#ifdef ENABLE_ADC

// ------------------------------------------------------------
// Init() initializes the Flash, the watchdog timer, the Power 
// Management Controller (PMC), and the PIO port.
// See the steps outlined in the section titled "Programming 
// Sequence" of the Atmel AT91SAM7S data sheet (Section 26.7 )
// to program the PMC.
// ------------------------------------------------------------
void ADC_Init_sys(void)
{
	AT91PS_PMC pPMC;
//	AT91PS_PIO pPIO;

    /*	 
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
	*/
	
	// 6. enable the peripheral clock used by PIOA and ADC
	pPMC->PMC_PCER|=(1<<AT91C_ID_PIOA)|(1<<AT91C_ID_ADC);
	
	// 7. Configure PIO
	Init_ADC_IO_PIO();
}

void ADC_Init_IO_PIO(void)
{
	AT91PS_SYS   regs = AT91C_BASE_SYS;
	unsigned int data;
	
		/*
		// initializ the port defined as PORT_LED as an output port.
		//configure the PIO register used by the LED
		unsigned int u;
		// pPIO=AT91C_BASE_PIOA;
		u = PORT_MASK(PORT_LED3) | 
		    PORT_MASK(PORT_LED2) | 
		    PORT_MASK(PORT_LED1) | 
		PORT_MASK(PORT_LED0);
		pPIO->PIO_PER  = u;   // enable LED ports
		pPIO->PIO_OER  = u;   // configure LED ports as an outputs
		pPIO->PIO_SODR = u;   // setting the output ports turn off the LEDs
		*/
	
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
}

void ADC_main(void)
{
	AT91PS_ADC   pADC;
	volatile int n, nADC;

    // perform initialization tasks
	 Init();
	 
	// initialize the ADC
	// track-an-hold : 600 nS   --> SHTIM   =  2
	// start up time :  20 uS   --> STARTUP = 12
	// 10-bit adc clk:   5 MHz  --> PRESCAL =  3
	 pADC=AT91C_BASE_ADC;
//	 pADC->ADC_MR   = 0x020C0300;             // SHTIM:STARTUP:PRESCAL:00
	 pADC->ADC_MR   = 0x020C0300;             // SHTIM:STARTUP:PRESCAL:00
	 pADC->ADC_CHER = (1<<CHANNEL);           // enable adc channel
	 pADC->ADC_CR   = AT91C_ADC_START /*2*/;  // start ADC
	// endless loop blinks the LED
	 nADC=0;
	 
	 while(TRUE)
	 {
	   for(n=0; n<0x10000; n++);        // delay

	   if((pADC->ADC_SR)&(1<<CHANNEL))  // check if adc channel is ready
	   {
	     nADC = pADC->ADC_LCDR;           // read conversion value
	     pADC->ADC_CR = AT91C_ADC_START /*2*/;                // start ADC
	   }
	   //LedMeter((nADC>>6)&0xFF);
	   ADCOutData (~nADC>>0);
	 }
}

#endif /*ENABLE_ADC*/
