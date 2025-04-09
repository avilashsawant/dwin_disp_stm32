/*
 * DWIN_DISP.h
 *
 *  Created on: Feb 12, 2025
 *      Author: Avilash.Sawant
 */

#ifndef  _DWIN_DISP_H
#define  _DWIN_DISP_H


/******** Include Files**************/
#include "main.h"
#include <stdint.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>

/******** User include files*********/



/******** PRIVATE MACROS *************/




#define DWIN_USE_RTOS 			0
#define DWIN_HMI			    huart3

#define TOUCH_VP			    0X5000
#define KEYPAD_RETURN_VP	    0x5050
#define FLASH_VAR_ADDRESS	    0x5100

#define DWIN_WRITE 				0x82
#define DWIN_READ 				0x83

#define KEYPAD_ENTER			0x06
#define KEYPAD_BACK				0x18
#define KEYPAD_DELETE_CHAR		0x7F

#define MAX_TEXT_VP 		    110
#define MAX_ICON_VP 		    110
#define MAX_PRINT_SIZE 		    30

#define DWIN_RX_BUFF_SIZE		30
#define DWIN_MAIN_BUFF_SIZE		40



#define PASSWORD_SIZE           6
#define PP_PASSWORD             102030

#define CORRECT_PASSWORD        1
#define INCORRECT_PASSWORD      0
/************ Text colour**************/

#define BLACK_COLOUR	        0x0000
#define GREY_COLOUR		        0xC618
#define WHITE_COLOUR	        0xFFFF
#define RED_COLOUR		        0xF800


/************ screen *******************/
#define IND_DELAY               1000


enum  ScreenHandleTypeDef
{
	main_logo = 0,
	main_logo1,
	main_logo2,
	main_logo3,
	main_logo4,
	main_logo5,
	main_logo6,
	main_logo7,
	main_logo8,
	main_logo9,
	reserved1,
	pp_settings,
	pp_prime,
	pp_pumpcheck,
	pp_Engineermode,
	pp_about
};

#define pg_close           (unsigned short int)0x1090
#define pg_next            (unsigned short int)0x1091
#define pg_prev            (unsigned short int)0x1092

#define pg_settings        (unsigned short int)0x1010
#define pg_prime           (unsigned short int)0x1011
#define pg_pcheck          (unsigned short int)0x1012
#define pg_about           (unsigned short int)0x1013
#define pg_Emode           (unsigned short int)0x1014

#define pg_login           (unsigned short int)0x1071

/******************variable declarations *********************/



typedef  struct
{
	         unsigned char  Rx_buff[DWIN_RX_BUFF_SIZE];
	         unsigned char  Main_buff[DWIN_MAIN_BUFF_SIZE];
	         unsigned char  num_key_return[20];
             unsigned char  tx_complete;
       const unsigned short text_vp_arr[MAX_TEXT_VP];
       const unsigned short icon_vp_arr[MAX_ICON_VP];
	         unsigned char  print_string[MAX_PRINT_SIZE];
	                  char  pass_print[9];
	         unsigned short touch_key;
	         unsigned short saved_vp;
	         unsigned char  empty_string[31];
	                  float num_return;


	         unsigned short rx_buffer_size;
	         unsigned char  uart_active;
             unsigned char  pump_num;
             unsigned char  pcheck_num;
	         unsigned char store_passwd[PASSWORD_SIZE];
	         unsigned char temp_passwd[PASSWORD_SIZE];
	         unsigned char chk_password_length;

	                 float temp_flowrate;
	                 float temp_dead_pre_valve;
	                 float temp_dead_post_valve;

	                 float temp_sample_volume;
	                 float temp_rinse_volume;
	                 float temp_replenish_volume;
	         unsigned char check_sel_pump;
	                 float chk_diff_sample;

}DwinHandleTypeDef;


typedef struct
{
	volatile unsigned char  settings_flag:1;
	volatile unsigned char  pcheck_flag:1;
	volatile unsigned char  prime_flag:1;
	volatile unsigned char  about_flag:1;
	volatile unsigned char  emode_flag:1;

	volatile unsigned char  next_flag:1;
	volatile unsigned char  prev_flag:1;
	volatile unsigned char  close_flag:1;

	volatile unsigned char  flowrate_flag:1;
	volatile unsigned char  Dead_Volume_pre_valve_flag:1;
	volatile unsigned char  Dead_Volume_post_valve_flag:1;

	         unsigned char  run_flag:1;
	volatile unsigned char  sample_path_flag:1;
	volatile unsigned char  replenish_path_flag:1;

             unsigned char  reset_8_flag:1;
             unsigned char  reset_12_flag:1;
	         unsigned char  dir_fwd_flag:1;
			 unsigned char  dir_rev_flag:1;

             unsigned char   rinse_flag: 1;
	         unsigned char  check_pw_flag:1;

             unsigned char  pg_switch_count;
}DwinFlagHandleTypeDef;

/***************** FUNC DECLARATIONS ********************/

void AL_DWIN_Start(void);
void AL_DWIN_End(void);
void AL_DWIN_Uart_Receive(void);
void AL_DWIN_Write_VP(uint16_t VP_add, uint16_t data);
void AL_DWIN_Send_Data(uint8_t);
void AL_DWIN_Picshow(uint16_t);
void AL_DWIN_Icon(uint8_t, uint16_t , uint16_t, uint16_t);
void AL_Dwin_Init(void);
void AL_DWIN_Keypad(char * str,uint8_t len);
void AL_DWIN_Beep(void);
void AL_DWIN_Text_Show(uint16_t vp,uint8_t data1,uint8_t data2,uint8_t data3);
void AL_DWIN_Text_Print(uint8_t VP_arr_pointer, uint8_t data_length, const char * print_str );
void AL_DWIN_Print_Float_Number(uint8_t VP_arr_pointer, float print_float );
void AL_DWIN_Print_Int_Number(uint8_t VP_arr_pointer, int num, char*format_specifer );
void AL_DWIN_Clear_Text(uint16_t VP_pointer, uint8_t data_length);
void AL_DWIN_Clear_Icon(uint8_t VP_arr_pointer);
void AL_DWIN_Write_RTC_Data(void);
void AL_DWIN_Change_Text_Colour(uint16_t sp_locator,uint16_t Font_colour);
void AL_DWIN_Bit_Icon_Display(uint16_t VP,uint16_t data);

void AL_DWIN_Text_Highlight(uint16_t sp_locator,uint8_t selection,uint8_t selection_bits);
void AL_DWIN_Disable_All_Program(void);
uint8_t AL_DWIN_Read_Vp(uint8_t VP_arr_pointer, uint8_t data_length);
void AL_DWIN_Get_Current_Pic_Id(void);
void AL_DWIN_Set_Brightness(uint8_t Brightness);
uint8_t AL_DWIN_Get_Brightness(void);
void AL_DWIN_Read_Flash(uint16_t ROM_add, uint8_t len);
void AL_DWIN_Write_Flash(uint16_t ROM_add, char * str, uint8_t len);
void AL_DWIN_Toggle(uint32_t delay_ms);
uint16_t AL_DWIN_Read_Value(uint8_t VP_arr_pointer);

void AL_DWIN_CONV_NUM_ARR(void);


#endif
/* DWIN_DISP_H */



