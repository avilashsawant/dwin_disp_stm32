/*
 * DCOMM.h
 *
 *  Created on: Mar 17, 2025
 *      Author: Avilash.Sawant
 */

#ifndef _DCOMM_H_
#define _DCOMM_H_



/********** COMMUNICATION STREAM *****************/
/*   @all 1 byte data
 *
 *   start char |  data              |  stop char
 *   ----------------------------------------------
 *       $      |   letters,numbers  |    #
 *
 *   @ if Request = $PPCHK#
 *   @ then Response = $PPCHK#
 *
*/
/************************************************/

#define DV_THRESHOLD   10       //  ml


#define START_CHAR   '$'
#define END_CHAR     '#'

#define COMM_BUFF_LEN  50

typedef struct
{

               uint8_t   recv_com_buff[COMM_BUFF_LEN];
               uint8_t   recv_com_count;
               uint8_t   diss_main_buff[COMM_BUFF_LEN];

               uint8_t   tx_com_buff[COMM_BUFF_LEN];
               uint8_t   tx_com_count;

               float      recv_flowrate;
               float      recv_sample_volume;
               float      recv_rinse_volume;
               float      recv_Dead_prev;
               float      recv_Dead_post;
               float      recv_Dead_repv;
               float      recv_filter_change;
               float      recv_intervals;
               float      recv_Replenish_status;
               float      recv_fc_status;
               float      recv_change_buff_status;
               uint8_t    temp_recv_data;

               uint32_t   load_recv_rpm;
               uint32_t   track_steps;
               uint32_t   get_dispense_volume;

               uint8_t    start_cycle_counter;
               uint8_t    run_in_progress;
               uint8_t    cycle_count;
               uint8_t    run_interrupted;
               uint8_t    replenish_count;
               uint8_t    interval_count;

}disso_com_var_HandleTypeDef;


typedef struct
{

   volatile    uint8_t com_flag:1;
   volatile    uint8_t pump_reset:1;
               uint8_t pump_auto_mode:1;
               uint8_t chk_pp_dir_flag;


   volatile    uint8_t pump_reversed:1;
   volatile    uint8_t pump_forward:1;


   volatile    uint8_t pump_start:1;
   volatile    uint8_t pump_escape:1;
   volatile    uint8_t pump_chk:1;
   volatile    uint8_t pump_ready:1;
   volatile    uint8_t pump_busy:1;

   volatile    uint8_t pump_fill:1;
   volatile    uint8_t pump_validate_sampling:1;
   volatile    uint8_t pump_validate_replenishing:1;

   volatile    uint8_t pump_buff_change:1;
   volatile    uint8_t pump_clean:1;
   volatile    uint8_t pump_auto_samling:1;
   volatile    uint8_t pump_param_status:1;
   volatile    uint8_t pump_clean_wfilter:1;
   volatile    uint8_t pump_no_filter_change:1;


               uint8_t PRIME:1;
               uint8_t VALS:1;
               uint8_t VALR:1;
               uint8_t CLEAN:1;
               uint8_t FLUSH1:1;
               uint8_t FLUSH2:1;
               uint8_t FLUSH3:1;
               uint8_t HOME:1;


}disso_com_flag_HandleTypeDef;



void AL_PP_Read_Uart_Data(void);
void AL_PP_Chk_Comm_Data(void);
void AL_PP_Clear_Com_Buffer(void);
void AL_PP_Run_process(void);
void AL_PP_Receive_parameters(void);
void AL_PP_Disso_Parameters_func(void);
void AL_PP_Receive_Volume_parameters(void);
void AL_PP_Replenish_Empty(void);
void AL_PP_Sampling_Empty(void);
void AL_PP_Dispense_Volume(unsigned char direction,unsigned int volume);
void AL_PP_Dead_Volume_Empty(unsigned char direction,unsigned int volume);
#endif /* INC_DCOMM_H_ */





