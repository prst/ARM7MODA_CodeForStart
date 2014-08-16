#include	"at91sam7s64.h"
#include	"variables.h"
#include	"delays.h"
#include	"lcd.h"

// ------------------------------------------------------------
// Init() initializes the Flash, the watchdog timer, the Power 
// Management Controller (PMC), and the PIO port.
// See the steps outlined in the section titled "Programming 
// Sequence" of the Atmel AT91SAM7S data sheet (Section 26.7 )
// to program the PMC.
// ------------------------------------------------------------
void LCD_Init_sys (void)
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
	pPMC->PMC_PCER|=(1<<AT91C_ID_PIOA);

	// 7. Configure PIO
	Init_LCD_IO_PIO();

} /* LCD_Init_sys */


void Init_LCD_IO_PIO(void)
{
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
} /* Init_LCD_IO_PIO */


//-----------------------------------------------------------
void LCD_Config(void)
{

#ifdef LCD_4_BITS
  AT91PS_SYS  regs = AT91C_BASE_SYS;

	tick(600*LCD_TIME); regs->PIOA_ODSR &=~_RS;	//   RS=0;
	udelay();
	tick(60*LCD_TIME); regs->PIOA_ODSR &= 0xffffff0f;
    tick(1);          regs->PIOA_ODSR |= (0);
	tick(60*LCD_TIME); regs->PIOA_ODSR |=_E;	//   E=1;
	tick(60*LCD_TIME); regs->PIOA_ODSR &=~_E;	//   E=0;
	udelay();
	tick(60*LCD_TIME); regs->PIOA_ODSR &= 0xffffff0f;
    tick(1);          regs->PIOA_ODSR |= (0);
	tick(60*LCD_TIME); regs->PIOA_ODSR |=_E;	//   E=1;
	tick(60*LCD_TIME); regs->PIOA_ODSR &=~_E;	//   E=0;
	udelay();
	tick(60*LCD_TIME); regs->PIOA_ODSR &= 0xffffff0f;
    tick(1);          regs->PIOA_ODSR |= (0);
	tick(60*LCD_TIME); regs->PIOA_ODSR |=_E;	//   E=1;
	tick(60*LCD_TIME); regs->PIOA_ODSR &=~_E;	//   E=0;
	delay();
	tick(60*LCD_TIME); regs->PIOA_ODSR &= 0xffffff0f;
    tick(1);          regs->PIOA_ODSR |= (0);
	tick(60*LCD_TIME); regs->PIOA_ODSR |=_E;	//   E=1;
	tick(60*LCD_TIME); regs->PIOA_ODSR &=~_E;	//   E=0;

	delay();
	tick(60*LCD_TIME); regs->PIOA_ODSR &= 0xffffff0f;
    tick(1);          regs->PIOA_ODSR |= (0x30);
	tick(60*LCD_TIME); regs->PIOA_ODSR |=_E;	//   E=1;
	tick(60*LCD_TIME); regs->PIOA_ODSR &=~_E;	//   E=0;
	delay();
	tick(60*LCD_TIME); regs->PIOA_ODSR &= 0xffffff0f;
    tick(1);          regs->PIOA_ODSR |= (0x30);
	tick(60*LCD_TIME); regs->PIOA_ODSR |=_E;	//   E=1;
	tick(60*LCD_TIME); regs->PIOA_ODSR &=~_E;	//   E=0;
	udelay();
	tick(60*LCD_TIME); regs->PIOA_ODSR &= 0xffffff0f;
    tick(1);          regs->PIOA_ODSR |= (0x20);
	tick(60*LCD_TIME); regs->PIOA_ODSR |=_E;	//   E=1;
	tick(60*LCD_TIME); regs->PIOA_ODSR &=~_E;	//   E=0;
	
	delay();
	tick(60*LCD_TIME); regs->PIOA_ODSR &= 0xffffff0f;
    tick(1);          regs->PIOA_ODSR |= (0x20);
	tick(60*LCD_TIME); regs->PIOA_ODSR |=_E;	//   E=1;
	tick(60*LCD_TIME); regs->PIOA_ODSR &=~_E;	//   E=0;
	udelay();
	tick(60*LCD_TIME); regs->PIOA_ODSR &= 0xffffff0f;
    tick(1);          regs->PIOA_ODSR |= (0x80);
	tick(60*LCD_TIME); regs->PIOA_ODSR |=_E;	//   E=1;
	tick(60*LCD_TIME); regs->PIOA_ODSR &=~_E;	//   E=0;
	delay();
	tick(60*LCD_TIME); regs->PIOA_ODSR &= 0xffffff0f;
    tick(1);          regs->PIOA_ODSR |= (0x00);
	tick(60*LCD_TIME); regs->PIOA_ODSR |=_E;	//   E=1;
	tick(60*LCD_TIME); regs->PIOA_ODSR &=~_E;	//   E=0;
	udelay();
	tick(60*LCD_TIME); regs->PIOA_ODSR &= 0xffffff0f;
    tick(1);          regs->PIOA_ODSR |= (0x80);
	tick(60*LCD_TIME); regs->PIOA_ODSR |=_E;	//   E=1;
	tick(60*LCD_TIME); regs->PIOA_ODSR &=~_E;	//   E=0;
	delay();	
	tick(60*LCD_TIME); regs->PIOA_ODSR &= 0xffffff0f;
    tick(1);          regs->PIOA_ODSR |= (0x00);
	tick(60*LCD_TIME); regs->PIOA_ODSR |=_E;	//   E=1;
	tick(60*LCD_TIME); regs->PIOA_ODSR &=~_E;	//   E=0;
	udelay();
	tick(60*LCD_TIME); regs->PIOA_ODSR &= 0xffffff0f;
    tick(1);          regs->PIOA_ODSR |= (0x10);
	tick(60*LCD_TIME); regs->PIOA_ODSR |=_E;	//   E=1;
	tick(60*LCD_TIME); regs->PIOA_ODSR &=~_E;	//   E=0;
	delay();
	tick(60*LCD_TIME); regs->PIOA_ODSR &= 0xffffff0f;
    tick(1);          regs->PIOA_ODSR |= (0x00);
	tick(60*LCD_TIME); regs->PIOA_ODSR |=_E;	//   E=1;
	tick(60*LCD_TIME); regs->PIOA_ODSR &=~_E;	//   E=0;
	udelay();
	tick(60*LCD_TIME); regs->PIOA_ODSR &= 0xffffff0f;
    tick(1);          regs->PIOA_ODSR |= (0x30);
	tick(60*LCD_TIME); regs->PIOA_ODSR |=_E;	//   E=1;
	tick(60*LCD_TIME); regs->PIOA_ODSR &=~_E;	//   E=0;
	delay();
	tick(60*LCD_TIME); regs->PIOA_ODSR &=~_RS;	//   RS=0;
#endif /*LCD_4_BITS*/


#ifdef LCD_8_BITS
	unsigned char i=0, init_value[] = { 0x01, 0x02, 0x06, 0x0c, 0x14, 0x38 }; /* 16x2 */ 

    for (i=0; i<sizeof(init_value); i++) { 
	  lcd_cmd ( init_value[i] );
    }
#endif /*LCD_8_BITS*/

} /* LCD_Config() */

//-----------------------------------------------------------
void lcd_cmd (unsigned char ch)
{
  AT91PS_SYS  regs = AT91C_BASE_SYS;

#ifdef LCD_4_BITS
	udelay();
	tick(600*LCD_TIME); regs->PIOA_ODSR &=~_RS;//   RS=0;
	tick(60*LCD_TIME); regs->PIOA_ODSR &= 0xffffff0f;
    tick(1);           regs->PIOA_ODSR |= (ch&0xf0);
	tick(60*LCD_TIME); regs->PIOA_ODSR |=_E;	//   E=1;
	tick(60*LCD_TIME); regs->PIOA_ODSR &=~_E;	//   E=0;
	udelay();
	tick(60*LCD_TIME); regs->PIOA_ODSR &= 0xffffff0f;
    tick(1);           regs->PIOA_ODSR |= (ch&0x0f)<<4;
	tick(60*LCD_TIME); regs->PIOA_ODSR |=_E;	//   E=1;
	tick(60*LCD_TIME); regs->PIOA_ODSR &=~_E;	//   E=0;
	tick(60*LCD_TIME); regs->PIOA_ODSR &=~_RS;	//   RS=0;
#endif /*LCD_4_BITS*/

#ifdef LCD_8_BITS
	tick(600*LCD_TIME); regs->PIOA_ODSR &=~_RS;//   RS=0;
	tick(6*LCD_TIME); regs->PIOA_ODSR |=_E;	//   E=1;
	tick(6*LCD_TIME); regs->PIOA_ODSR &= 0xffffff00;
	tick(6*LCD_TIME); regs->PIOA_ODSR |= ch;
	tick(6*LCD_TIME); regs->PIOA_ODSR &=~_E;	//   E=0;
	tick(6*LCD_TIME); regs->PIOA_ODSR &=~_RS;	//   RS=0;
#endif /*LCD_8_BITS*/
} /* lcd_cmd() */

//-----------------------------------------------------------
// Set address of first byte on desired row
void lcd_xy( unsigned char y, unsigned char x )
{	
  unsigned char  LCD_ROW_ADDRESS[4] = { 0x00, 0x40, 0x14, 0x54 };
  unsigned char  AddrXYData;
  AT91PS_SYS     regs = AT91C_BASE_SYS;
 
    AddrXYData = LCD_ROW_ADDRESS[y]+x;

#ifdef LCD_4_BITS
    tick(60*LCD_TIME); regs->PIOA_ODSR   &=~_RS; //   RS=0;

     tick(40*LCD_TIME); regs->PIOA_ODSR  &= 0xffffff0f;
     tick(1);          regs->PIOA_ODSR  |=0x80|(AddrXYData&0xF0);
     tick(40*LCD_TIME); regs->PIOA_ODSR  |=_E;	//   E=1;
     tick(40*LCD_TIME); regs->PIOA_ODSR  &=~_E;	//   E=0;

      tick(40*LCD_TIME); regs->PIOA_ODSR &= 0xffffff0f;
      tick(1);          regs->PIOA_ODSR |=(AddrXYData&0x0F)<<4;
      tick(40*LCD_TIME); regs->PIOA_ODSR |=_E;	//   E=1;
      tick(40*LCD_TIME); regs->PIOA_ODSR &=~_E;	//   E=0;
#endif /*LCD_4_BITS*/

#ifdef LCD_8_BITS
    tick(6*LCD_TIME); regs->PIOA_ODSR &=~_RS;//   RS=0;
	tick(4*LCD_TIME); regs->PIOA_ODSR |=_E;	//   E=1;
	tick(4*LCD_TIME); regs->PIOA_ODSR &= 0xffffff00;
    tick(1);          regs->PIOA_ODSR |=(0x80|AddrXYData);
	tick(4*LCD_TIME); regs->PIOA_ODSR &=~_E;	//   E=0;
#endif /*LCD_8_BITS*/
} /* lcd_xy() */

//-----------------------------------------------------------
/* Show message on LCD*/
void lcd_char( unsigned char value )
{ 
  AT91PS_SYS     regs = AT91C_BASE_SYS;
  unsigned char  kod;

    kod = value;
    kod = lcd_recode( value );

#ifdef LCD_4_BITS
    tick(30*LCD_TIME); regs->PIOA_ODSR   |=_RS; //   RS=1;

	 tick(20*LCD_TIME); regs->PIOA_ODSR  &= 0xffffff0f;
     //tick(1);          regs->PIOA_ODSR  |=(kod&0xF0);
     tick(1);          regs->PIOA_ODSR  |=0x30;
	 tick(30*LCD_TIME); regs->PIOA_ODSR  |=_E;  //   E=1;
	 tick(20*LCD_TIME); regs->PIOA_ODSR  &=~_E; //   E=0;
      tick(1);          regs->PIOA_ODSR |=0xFF;

	  tick(20*LCD_TIME); regs->PIOA_ODSR &= 0xffffff0f;
      //tick(1);          regs->PIOA_ODSR |=(kod<<4)&0xF0;
      tick(1);          regs->PIOA_ODSR |=0x30;
      tick(30*LCD_TIME); regs->PIOA_ODSR |=_E;  //   E=1;
	  tick(20*LCD_TIME); regs->PIOA_ODSR &=~_E; //   E=0;
      tick(1);          regs->PIOA_ODSR |=0xFF;
#endif /*LCD_4_BITS*/

#ifdef LCD_8_BITS
    tick(3*LCD_TIME); regs->PIOA_ODSR |=_RS;	 //   RS=1;
	tick(3*LCD_TIME); regs->PIOA_ODSR |=_E;	 //   E=1;
	tick(2*LCD_TIME); regs->PIOA_ODSR &= 0xffffff00;
	tick(1);          regs->PIOA_ODSR |= kod;
	tick(2*LCD_TIME); regs->PIOA_ODSR &= ~_E; //   E=0;
#endif /*LCD_8_BITS*/
} /* lcd_char() */


//-----------------------------------------------------------
void lcd_str(unsigned char *string, unsigned char x, unsigned char y)
{
  lcd_xy(x,y);
  while ( *string )
  {
    lcd_char ( *string );
    string++;
  }
} /* lcd_str() */


//-----------------------------------------------------------
void lcd_sniffit (unsigned long XX, unsigned char x, unsigned char y, unsigned char attr)
{
  unsigned char  d[21], Sotn, Des, Eden;

    d[0]=0x30;
    d[1]=0x78;
    d[2]=(XX&0xF0000000)>>28;	if (d[2]>0x09) d[2]=(d[2]-0x0A)+0x61; else  d[2]+=0x30;
    d[3]=(XX&0x0F000000)>>24;	if (d[3]>0x09) d[3]=(d[3]-0x0A)+0x61; else  d[3]+=0x30;
    d[4]=(XX&0x00F00000)>>20;	if (d[4]>0x09) d[4]=(d[4]-0x0A)+0x61; else  d[4]+=0x30;
    d[5]=(XX&0x000F0000)>>16;	if (d[5]>0x09) d[5]=(d[5]-0x0A)+0x61; else  d[5]+=0x30;
    d[6]=(XX&0x0000F000)>>12;	if (d[6]>0x09) d[6]=(d[6]-0x0A)+0x61; else  d[6]+=0x30;
    d[7]=(XX&0x00000F00)>>8;	if (d[7]>0x09) d[7]=(d[7]-0x0A)+0x61; else  d[7]+=0x30;
    d[8]=(XX&0x000000F0)>>4;	if (d[8]>0x09) d[8]=(d[8]-0x0A)+0x61; else  d[8]+=0x30;
    d[9]=(XX&0x0000000F)>>0;	if (d[9]>0x09) d[9]=(d[9]-0x0A)+0x61; else  d[9]+=0x30;
    d[10]=0x20;	d[11]=0x20;	d[12]=0x20;    
    d[13]=0x20;	d[14]=0x20;	d[15]=0x20;
    d[16]=0x20;	d[17]=0x20;	d[18]=0x20;	
	d[19]=0x20;	d[20]=0x00;  

 switch(attr){
  case '%':
    Sotn=XX;
    Eden=XX/19;
    Des=Eden/10;
    lcd_xy(1,0);

    d[0]=Des;	if (d[0]>0x09) d[0]=(d[0]-0x0A)+0x61; else  d[0]+=0x30;
    d[1]=Eden-(Des*10);	if (d[1]>0x09) d[1]=(d[1]-0x0A)+0x61; else  d[1]+=0x30;
    d[2]='%';
    d[3]=0;
    lcd_str_en=0;    lcd_str(d,x,y);
  break;

  case 'M':
    Sotn=XX/100;
    Des=(XX-(Sotn*100))/10;
    Eden=(XX-(Sotn*100))-(Des*10);
    lcd_xy(1,0);
     d[0]=Sotn;	if (d[0]>0x09) d[0]=(d[0]-0x0A)+0x61; else  d[0]+=0x30;
     d[1]=Des;	if (d[1]>0x09) d[1]=(d[1]-0x0A)+0x61; else  d[1]+=0x30;
     d[2]=Eden;	if (d[2]>0x09) d[2]=(d[2]-0x0A)+0x61; else  d[2]+=0x30;
     d[3]=0;
     if (/*XX>=0 &&*/ XX<10)  {d[0]=' '; d[1]=' '; }
     if (XX>=10 && XX<100){d[0]=' '; }
     lcd_str_en=0;    lcd_str(d,x,y);
  break;
  
  case 'L':
	 LCD_print_long(XX,x,y);
  break;
  
  case 'S':
     if (XX&0x10000000){d[0]='7'; d[1]='*';}
     if (XX&0x01000000){d[2]='6'; d[3]='*';}
     if (XX&0x00100000){d[4]='5'; d[5]='*';}
     if (XX&0x00010000){d[6]='4'; d[7]='*';}
     if (XX&0x00001000){d[8]='3'; d[9]='*';}
     if (XX&0x00000100){d[10]='2'; d[11]='*';}
     if (XX&0x00000010){d[12]='1'; d[13]='*';}
     if (XX&0x00000001){d[14]='0'; d[15]='*';}
     lcd_str_en=0;    lcd_str(d,x,y);
  break;
 
  default:
    lcd_str_en=0;    lcd_str(d,x,y);
  break;
 }
}


//-----------------------------------------------------------
/*void u2a(char *buf, unsigned long x)
{
    char b[11], *s=&b[10];

    *s = 0;
    do {
    *--s = x % 10 | 0x30;
    } 
	while (x /= 10);
    while ((*buf++ = *s++) != 0);
}  */


//-----------------------------------------------------------
void LCD_print_long(unsigned long XDATA, unsigned char x, unsigned char y)
{
	unsigned char b[11], *s=&b[10];

	*s = 0;
	do {
	 *--s = XDATA % 10 | 0x30;
	} while (XDATA /= 10);

	lcd_str_en=0; 
	lcd_str(s,x,y);

/*	char b[11];
	int i;
	i = 9;
	b[10] = 0;
	do {
	b[i] = XDATA % 10 | 0x30;
	XDATA /= 10;
	} while (i--);
	lcd_str_en=0; lcd_str(b,x,y);  */    
}


//-----------------------------------------------------------
unsigned char lcd_recode(char kod)
{
  static unsigned char lcdkod[64]={ // CP-1251
   0x41, 0xa0, 0x42, 0xa1, 0xe0, //   á   â   ÷   ç   ä
   0x45, 0xa3, 0xa4, 0xa5,	     //   E   ö   ú   é
   0xa6, 0x4b, 0xa7, 0x4d, 0x48, //   ê   ë   ì   í   î
   0x4f, 0xa8, 0x50, 0x43, 0x54, //   ï   ð   ò   ó   ô
   0xa9, 0xaa, 0x58, 0xe1, 0xab, //   õ   æ   è   ã   þ
   0xac, 0xe2, 0xad, 0xae, 0x62, //   û   ý   ø   ù   ÿ
   0xaf, 0xb0, 0xb1, 0x61, 0xb2, //   ü   à   ñ   Á   Â
   0xb3, 0xb4, 0xe3, 0x65,       //   ×   Ç   Ä   Å
   0xb6, 0xb7, 0xb8, 0xb9, 0xba, //   Ö   Ú   É   Ê   Ë
   0xbb, 0xbc, 0xbd, 0x6f, 0xbe, //   Ì   Í   Î   Ï   Ð
   0x70, 0x63, 0xbf, 0x79, 0xe4, //   Ò   Ó   Ô   Õ   Æ
   0x78, 0xe5, 0xc0, 0xc1, 0xe6, //   È   Ã   Þ   Û   Ý
   0xc2, 0xc3, 0xc4, 0xc5, 0xc6, //   Ø   Ù   ß   Ü   À
   0xc7				             //   Ñ
  };

  if(kod < 'À')
  {
    return kod;
  }
  else
  {
    return lcdkod[kod-'À'];
  }
}


//-----------------------------------------------------------
void LCD_Clear(void)
{
  lcd_str_en=0; lcd_str("                    ",0,0);
  lcd_str_en=0; lcd_str("                    ",1,0);
  lcd_str_en=0; lcd_str("                    ",2,0);
  lcd_str_en=0; lcd_str("                    ",3,0);
}


//-----------------------------------------------------------
void LCD_Main(void)
{
	//lcd_char ( '1' );
	lcd_str("01234567",0,0);
	lcd_str("abcdefgh",1,0);
}
