#ifndef	_LCD_H
#define	_LCD_H

//#define	_lcd_RS	_B4
//#define	_lcd_E	_B5

// LCD
#define	_RS	_B8
#define	_E	_B9

#define LCD_TIME_FAST  (10)
#define LCD_TIME_SLOW  (50)
#define LCD_TIME  LCD_TIME_SLOW

#define LCD_4_BITS
//#define LCD_8_BITS


void  LCD_Init_sys   (void);
void  Init_LCD_IO_PIO(void);
void  LCD_Config     (void);
void  LCD_Clear      (void);
void  LCD_print_long (unsigned long XX, unsigned char x, unsigned char y);

unsigned char lcd_recode (char kod);

void  lcd_cmd      (unsigned char ch);
void  lcd_char     (unsigned char value);
void  lcd_str      (unsigned char *string, unsigned char x, unsigned char y);
void  lcd_xy       (unsigned char x, unsigned char y);
void  lcd_sniffit  (unsigned long dt, unsigned char x, unsigned char y, unsigned char attr);

void  LCD_Main (void);

#endif /* _LCD_H */
