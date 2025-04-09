/*
 * DISPLAY.h
 *
 *  Created on: Feb 19, 2025
 *      Author: Avilash.Sawant
 */

#ifndef _DISPLAY_H_
#define _DISPLAY_H_


/******** Include Files**************/


/******** User include files*********/



/******** PRIVATE MACROS *************/
#define EEPROM_BUFF_SIZE   128

#define CHANNEL_06     60
#define CHANNEL_08     80
#define CHANNEL_12     120
#define CHANNEL_14     140

// solenoid pins
#define SOL_1     GPIO_PIN_12
#define SOL_2     GPIO_PIN_8
#define SOL_3     GPIO_PIN_13
#define SOL_4     GPIO_PIN_9
#define SOL_5     GPIO_PIN_14
#define SOL_6     GPIO_PIN_10


/***********Variable declarations******************/

typedef struct
{
	    unsigned char save_buff[EEPROM_BUFF_SIZE];
	            float save_flowrate;
	            float save_dead_vol_pre_valve;
	            float save_dead_vol_post_valve;
                float save_sample_volume;
                float save_rinse_volume;
                float save_replenish_volume;

        unsigned char save_pump_channel_selection;


}eeprom_HandleTypeDef;


typedef struct
{
				unsigned int  sampling_time;
				unsigned int  sampling_rpm;
				unsigned int  rinsing_time;
				unsigned int  rinsing_rpm;
				unsigned int  replenish_time;
				unsigned int  replenish_rpm;
				unsigned int  prime_count;
                unsigned int  get_opto_rcount;
                unsigned int  get_opto_scount;
                unsigned int  get_opto_rpcount;
				unsigned int  sampling_channel;
 volatile       unsigned int  opto_rev_count[15];

				unsigned int  ramp_count;
 volatile	    unsigned int  ramp_ticks;
 volatile       unsigned int  U_ticks;

}Display_HandleTypeDef;


typedef struct
{
	   unsigned char  sampling_run_flag:1;
	   unsigned char  rinsing_run_flag:1;
	   unsigned char  ramp_up_flag:1;
	   unsigned char  replenish_run_flag:1;
	   unsigned char  ramp_completed_flag:1;

}Display_flags_HandleTypeDef;


/*****************function declarations**************/
void AL_PP_Power_On_Data(void);
void AL_PP_Disp_logo(void);
void AL_PP_Disp_check(void);
void AL_PP_Disp_Home_Settings_check(void);
void AL_PP_Disp_Home_Engmode_check(void);
void AL_PP_Disp_Home_Prime_check(void);
void AL_PP_Disp_Home_Pump_check(void);
void AL_PP_Disp_Home_about_check(void);
void AL_PP_Disp_Startup(void);
void AL_PP_Disp_Home_check(void);
void AL_PP_Disp_Default_Val(void);
void AL_PP_Buzzer_check(void);
uint16_t AL_PP_roundoff(float data);
void AL_PP_Sampling_Process(void);

#endif
/* _DISPLAY_H_ */
