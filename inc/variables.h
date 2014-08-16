#ifndef	_TYPES_H
#define	_TYPES_H

// PINS of CHIP
#define	_B0		0x00000001
#define	_B1		0x00000002
#define	_B2		0x00000004
#define	_B3		0x00000008
#define	_B4		0x00000010
#define	_B5		0x00000020
#define	_B6		0x00000040
#define	_B7		0x00000080
#define	_B8		0x00000100
#define	_B9		0x00000200
#define	_B10	0x00000400
#define	_B11	0x00000800
#define	_B12	0x00001000
#define	_B13	0x00002000
#define	_B14	0x00004000
#define	_B15	0x00008000
#define	_B16	0x00010000
#define	_B17	0x00020000
#define	_B18	0x00040000
#define	_B19	0x00080000
#define	_B20	0x00100000
#define	_B21	0x00200000
#define	_B22	0x00400000
#define	_B23	0x00800000
#define	_B24	0x01000000
#define	_B25	0x02000000
#define	_B26	0x04000000
#define	_B27	0x08000000
#define	_B28	0x10000000
#define	_B29	0x20000000
#define	_B30	0x40000000
#define	_B31	0x80000000

// колличество архивов  а каждые 64MByte MMC - MMC__MAX_ARCHIVES=(64*N)-1
//#define MMC__MAX_ARCHIVES	7 // 64M 
//#define MMC__MAX_ARCHIVES	15 // 128M 
//#define MMC__MAX_ARCHIVES	31 // 256M 
//#define MMC__MAX_ARCHIVES	63 // 512M 
#define MMC__MAX_ARCHIVES	127 // 1024M 

// USART buffer
#define __MAX_CH_USART	512 // размер буфера дл€ USART DBGU
#define __MAX_CH_USART1	512 // размер буфера дл€ USART1

// Receiver State
#define HEAD_SYMBOL_REQUIRE	'%' // 
#define HEAD_SYMBOL_WRITE	'+' // 
#define HEAD_SYMBOL_READ	'!' // 
#define HEAD_SYMBOL_BAD_CRC	'?' // 

// фазы протокола
#define	RX_IDLE		0
#define	RX_HEAD		1
#define	RX_ADR		2
#define	RX_CMD		3
#define	RX_DLINA	4
#define	RX_DATA		5
#define	RX_CONTR_SUMM	6
#define	RX_DONE		7
#define	RX_FAIL		8
#define	RX_OFFSET	4
#define	RX_ADR_REQUIRE	12
#define	RX_CMD_REQUIRE	13
#define	RX_DLINA_REQUIRE 14
#define	RX_DATA_REQUIRE	15
#define	RX_CONTR_SUMM_REQUIRE	16
#define	RX_BAD		54
#define	TX_IDLE		20
#define	TX_SEND		21
#define	TX_START	22
#define	TX_STOP		23

// точки меню
#define	OPERATION_FINISH	0
#define	SELECT_OF_WORK		1
#define	NUMBER_OF_ARCH		2
#define	ADDRES_OF_NETWORK	3
#define	SET_USART1_SPEED	4
#define	WORK_WRITE_READ		5
#define	TIME_READ			6
#define	TIME_EDIT			7
#define	TIME_WRITE			8
#define	TIME_CHECK			9
#define	VIEW_ARCH			10
#define	VIEW_JURN			11
#define	END_MENU			12     // конец меню
#define	FROM_MDSA_TO_PC		15
#define	FROM_ARCH_TO_MDSA	16

// режим
#define	REZHIM_FROM_BAR		0
#define	REZHIM_TO_PC		1
#define	REZHIM_SET_TIME		2
#define	REZHIM_VIEW_ARCH	3
#define	REZHIM_VIEW_JURN	4
#define	REZHIM_END			5

// значени€ пол€ типа данных в протоколе
#define	_OPISANIE			0x41
#define	_NET_ADDRESS		0x42
#define	_SPEED				0x43
#define	_STRING_INIT		0x44
#define	_DATA_TIME			0x45
#define	_STRING_MEMORY		0x46
#define	_STAT_ARCH_MINUTES	0x48
#define	_ARCH_MINUTES		0x47
#define	_RESET_ARCH_MIN		0x4a
#define	_STAT_ARCH_HOUR		0x4c
#define	_ARCH_HOUR			0x4b
#define	_RESET_ARCH_HOUR	0x4d
#define	_STAT_ARCH_MESJAC	0x4f
#define	_ARCH_MESJAC		0x4e
#define	_RESET_ARCH_MESJAC	0x50
#define	_STAT_JOURNAL_SOBIT	0x52
#define	_JOURNAL_SOBIT		0x51
#define	_RESET_JOURNAL_SOBIT	0x53
#define	_PARAM_IRKA			0x54
#define	_TEK_RASHOD			0x55
#define	_OBIEM				0x56
#define	_VERSION_SOFT		0x57

// адреса дл€ записи в протоколе - также лежат и в MMC в свободном месте
#define	_ADR_FIRST_OPERATION	0x10
#define	_ADR_OPISANIE		0x11
#define	_ADR_NET_ADDRESS	0x12
#define	_ADR_SPEED		0x13
#define	_ADR_STRING_INIT	0x14
#define	_ADR_DATA_TIME		0x15
#define	_ADR_STRING_MEMORY	0x16
#define	_ADR_STAT_ARCH_MINUTES	0x18
#define	_ADR_RESET_ARCH_MIN	0x1a
#define	_ADR_STAT_ARCH_HOUR	0x1c
#define	_ADR_RESET_ARCH_HOUR	0x1d
#define	_ADR_STAT_ARCH_MESJAC	0x1f
#define	_ADR_RESET_ARCH_MESJAC	0x20
#define	_ADR_STAT_JOURNAL_SOBIT	0x22
#define	_ADR_RESET_JOURNAL_SOBIT 0x23
#define	_ADR_PARAM_IRKA		0x24
#define	_ADR_TEK_RASHOD		0x25
#define	_ADR_OBIEM		0x26
#define	_ADR_VERSION_SOFT	0x27

// адреса архивов в смещенной €чейке
#define	_ADR_ARCH_MINUTES	( 0x100 )
#define	_ADR_ARCH_HOUR		( 0x6a0 )
#define	_ADR_ARCH_MESJAC	( 0x6dc )
#define	_ADR_JOURNAL_SOBIT	( 0x6e6 )

// колличество записей в каждом архиве
#define _NUM_IN_MINUTES_ARCH_	0x5A0 /* 1440 */
#define _NUM_IN_HOURS_ARCH_	0x3C  /* 60 */
#define _NUM_IN_MONTH_ARCH_	0x0A  /* 10 */
#define _NUM_IN_SOBIT_ARCH_	0x168 /* 360 */

// размер архива
#define SIZE_PER_ARCHIVE	0x900
#define STATISTICA_ARHIVA 	0x05

// готовности
#define _NONE 			0x00
#define _DONE 			0x00
#define _DONE_ERROR		0xff

#define _START_COPY_FROM_ARCH_TO_MDSA 0x88
//#define _SIZEOF_DATA_		(4+sizeof_data)

// коды клавиш и соответстви€ их в меню
#define DO_BACK		1
#define DO_MIN		2
#define DO_MAX		4
#define DO_NEXT		8
#define DO_SETUP	9

// признаки зан€тости и параметры загрузочных сектов
#define TABLE_CLEAR		0x30
#define TABLE_BUSY		0x31
#define BOOT_SECTOR		0	/* хранитс€ конфигураци€ зан€тых €чеек */
#define CONF_SECTOR		1	/* хранитс€ скорость USART */
#define START_SECTOR	2	/* начало общего пространства */

// параметры таймера
#define TIMER_LIMIT		8/*9*/ 	/* ќжидание посылки */
#define TIMER_SET_TIME	100 /* ƒетект разрыва линии */

#define TIME_SHOW_SCREEN	150 /* 50 = 1Sec */

//-----------------------------------------------------------
 struct {
 	unsigned char	DD[_NUM_IN_HOURS_ARCH_],
					MM[_NUM_IN_HOURS_ARCH_],
					YY[_NUM_IN_HOURS_ARCH_],
					hh[_NUM_IN_HOURS_ARCH_];
	unsigned long	obiem[_NUM_IN_HOURS_ARCH_][24], // запись, час
					rashod[_NUM_IN_HOURS_ARCH_][24];
 	unsigned char	stat[_NUM_IN_HOURS_ARCH_][24];
 					} hour_zapis;
					//value[_NUM_IN_HOURS_ARCH_,24];
 unsigned int	day_in_table;
 unsigned char	cnt_days,qqq,perepare_phase;

//-----------------------------------------------------------
 unsigned char	usart_buffer[__MAX_CH_USART], 	// приемный буффер DBGU
		usart1_buffer[__MAX_CH_USART1], 		// приемный буффер USART1
		table_buffer[512],			// буфер дл€ чтнеи€ из MMC 
		conf_buffer[512],			// буфер конфигурации дл€ MMC
		//urart_rx,
		usart_ptr,
		usart1_ptr,
		usart1_complite,
		usart1_complite_full,
		prev_operations_complite,
		ContrSumm,
		rx_cnt,
		rx_state,
		dlina_data,
		cmd_data,
		global_temp,
		global_TX,
		global_debug_TX_priznak,
		start_tmr,
		stop_tmp,
		X_1,
		X_2, 
		a0, 
		a1, 
		a2, 
		a3,
		selected_rezhim,
		starting_get_data,
		CRC,
		lcd_str_en,
		dejstvie,
		TX_done,
		LOCK,
		process_busy,
		DAY,
		MESJAC,
		YEAR,
		HOUR,
		MINUTA,
		SECUNDA,
//		TX_TEST_ERRORS,
		TX_FLAG_ERRORS,
		TX_TRY,
		TX_TIMER,
		TMP_TIMER,
		RX_packet_was_recive,
		RX_wait_next,
		next_zapis,
		addres_of_net,
		blocked_keys,
      	beeper_enable,
      	beeper_pulse,
      	beeper_pik,
		in,
		lcd_temp,
		usart_speed_ptr,
		usart_speed_conf,
		usart_speed_auto,
		usart_speed_auto_ptr,
		usart_speed_auto_complet,
		key,
		key_pressed,
		key_two, 
		two_k,
		was_pressed,
		restore_boot,
		enter_a_sel_speed,
		enter_a_sel_run,
		go_back__key,
		ss_time,
		phase_set_time,
		time_value,
		time_check_wait,
		N_zapisey,
		tmp_key_loop,
		tmp_key_loop_enable,
		perepare_table,
		failer_time_request,
		enable_system_work;

 char			mmc_buffer[512],	// Buffer for mmc i/o for data and registers
				buffer[512],
				hour_num_zapisi,
				hour_num_of_day,
				hour_num_status;
 
 unsigned int	jurn_num_zapisi,
 				jurn_num_limit;
 
 unsigned int	hour_index_tbl;

 unsigned long 	tmr_global_cnt,
				menu_state,
				menu_sub_state,
				tmr_dly,
				ADR_LO,
				ADR_HI,
				ADR,
				temp,
				x0,
				x1,
				x2,
				num_zapisi,
				number_of_archiv,
				persent,
				sizeof_data,
				mmc_addr,
				boot_screen;

 long			menu_value;
 unsigned long 	Obiem,
 				Obiem_next,
				Obiem_new_day;
 unsigned long	//long_data,
 				//long_data_last, 
				rashod;

 unsigned int	rx_num_bytes,
				kbd_x,
				kbd_x_last,
				TX_state,
				//YYY,
				//ZZZ,
				III,
				tmr_show_info;

 unsigned int	u, c;

 unsigned int 	usart_reg_speed[10];

//-----------------------------------------------------------
// unsigned char lcdkod[64];
//-----------------------------------------------------------
 /*union F {
  unsigned long l;
  unsigned char b[4];
 }; 
 union F f;*/
//-----------------------------------------------------------

#endif	/* _TYPES_H */
