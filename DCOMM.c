/*
 * DISPLAY.c
 *
 *  Created on: Feb 19, 2025
 *      Author: Avilash.Sawant
 */

/*********** SYSTEM INCLUDES FILES*****************/
#include "COMMON.h"
#include "math.h"
/********** USER INCLUDES FILES ********************/


/********** variable declarations*******************/
            disso_com_var_HandleTypeDef  my_disso_var;
            disso_com_flag_HandleTypeDef my_disso_flags;


extern      eeprom_HandleTypeDef   my_eeprom_var;
extern      Display_HandleTypeDef  my_disp_var;
extern      Display_flags_HandleTypeDef   my_disp_flags;
extern      stepperHandletypeDef       my_stepper_var;
extern      stepperFlagsHandletypeDef  my_stepflags_var;
extern unsigned char pp_version[6];
extern unsigned char ppe_version[6];
extern unsigned char hmi_version[4];
extern      DwinFlagHandleTypeDef  my_dwin_flags;


extern enum ScreenHandleTypeDef    my_screen_num;
extern      DwinHandleTypeDef      my_dwin_var;

extern      TIM_HandleTypeDef htim1;
extern      UART_HandleTypeDef huart5;

/********** USER variable declarations ********************/











/********************USER FUNCTION BODY**********************************/

void AL_PP_Read_Uart_Data(void)
{

	/*
    my_disso_var.recv_com_buff[my_disso_var.recv_com_count] = UART5->RDR;

    // start to get data
    if(my_disso_var.recv_com_buff[0] ==  START_CHAR)
    {
    	my_disso_flags.com_flag = 1;
    }
    else
    {
    	my_disso_flags.com_flag = 0;
    	my_disso_var.recv_com_count = 0;
    }

    // chk end char of data
    if(my_disso_flags.com_flag == 1)
    {
    	 if(my_disso_var.recv_com_buff[my_disso_var.recv_com_count] != END_CHAR)
    	 {
    		 my_disso_var.recv_com_count += 1;
    	 }
    	 else
    	 {
    		 my_disso_var.recv_com_count = 0;
    		 my_disso_flags.com_flag = 0;
    	 }

    }
     */


	 // start to get data
	 if(my_disso_var.recv_com_buff[0] ==  START_CHAR)
	 {
		my_disso_flags.com_flag = 1;
	 }
	 else
	 {
		my_disso_flags.com_flag = 0;
	 }






}


void AL_PP_Chk_Comm_Data(void)
{
	// Master- Disso commands to piston pump

  if(my_disso_flags.com_flag  == 1)
  {
    // Pump reset
	if( (my_disso_var.recv_com_buff[1] == 'P'       ) &&
		(my_disso_var.recv_com_buff[2] == 'P'       ) &&
		(my_disso_var.recv_com_buff[3] == 'R'       ) &&
		(my_disso_var.recv_com_buff[4] == 'S'       ) &&
		(my_disso_var.recv_com_buff[5] == 'T'       ) 	)
	{

         my_disso_flags.pump_reset = 1;
         AL_PP_Clear_Com_Buffer();

	} // Pump start process
	else if( (my_disso_var.recv_com_buff[1] == 'P'       ) &&
			 (my_disso_var.recv_com_buff[2] == 'P'       ) &&
			 (my_disso_var.recv_com_buff[3] == 'S'       ) &&
			 (my_disso_var.recv_com_buff[4] == 'T'       ))
	{
		 my_disso_flags.pump_start = 1;
		 AL_PP_Clear_Com_Buffer();

	}// Pump forward direction
	else if( (my_disso_var.recv_com_buff[1] == 'P'       ) &&
			 (my_disso_var.recv_com_buff[2] == 'P'       ) &&
			 (my_disso_var.recv_com_buff[3] == 'F'       ) &&
			 (my_disso_var.recv_com_buff[4] == 'W'       ) &&
			 (my_disso_var.recv_com_buff[5] == 'D'       ) )
	{
		 my_disso_flags.pump_forward = 1;
		 AL_PP_Clear_Com_Buffer();

	}// Pump reversed direction
	else if( (my_disso_var.recv_com_buff[1] == 'P'       ) &&
			 (my_disso_var.recv_com_buff[2] == 'P'       ) &&
			 (my_disso_var.recv_com_buff[3] == 'R'       ) &&
			 (my_disso_var.recv_com_buff[4] == 'E'       ) &&
			 (my_disso_var.recv_com_buff[5] == 'V'       ) )
	{
		 my_disso_flags.pump_reversed = 1;
		 AL_PP_Clear_Com_Buffer();

	}
	// check buffer change
	else if( (my_disso_var.recv_com_buff[1] == 'P'       ) &&
			 (my_disso_var.recv_com_buff[2] == 'P'       ) &&
			 (my_disso_var.recv_com_buff[3] == 'B'       ) &&
			 (my_disso_var.recv_com_buff[4] == 'U'       ) &&
			 (my_disso_var.recv_com_buff[5] == 'F'       ) )
	{
		 my_disso_flags.pump_buff_change = 1;
		 AL_PP_Clear_Com_Buffer();

	}// Check pump state
	else if( (my_disso_var.recv_com_buff[1] == 'P'       ) &&
			 (my_disso_var.recv_com_buff[2] == 'P'       ) &&
			 (my_disso_var.recv_com_buff[3] == 'C'       ) &&
			 (my_disso_var.recv_com_buff[4] == 'H'       ) &&
			 (my_disso_var.recv_com_buff[5] == 'K'       ) )
	{
			 my_disso_flags.pump_chk = 1;
			 AL_PP_Clear_Com_Buffer();

	}// Ask pump ready or not
	else if( (my_disso_var.recv_com_buff[1] == 'P'       ) &&
			 (my_disso_var.recv_com_buff[2] == 'P'       ) &&
			 (my_disso_var.recv_com_buff[3] == 'R'       ) &&
			 (my_disso_var.recv_com_buff[4] == 'D'       ) &&
			 (my_disso_var.recv_com_buff[5] == 'Y'       ) )
	{
			 my_disso_flags.pump_ready = 1;
			 AL_PP_Clear_Com_Buffer();

	}//Escape from any process
	else if( (my_disso_var.recv_com_buff[1] == 'P'       ) &&
			 (my_disso_var.recv_com_buff[2] == 'P'       ) &&
			 (my_disso_var.recv_com_buff[3] == 'E'       ) &&
			 (my_disso_var.recv_com_buff[4] == 'S'       ) &&
			 (my_disso_var.recv_com_buff[5] == 'C'       ) )
	{
		  my_disso_flags.pump_escape = 1;
		  AL_PP_Clear_Com_Buffer();

	}// cleaning process
	else if( (my_disso_var.recv_com_buff[1] == 'P'       ) &&
			 (my_disso_var.recv_com_buff[2] == 'P'       ) &&
			 (my_disso_var.recv_com_buff[3] == 'C'       ) &&
			 (my_disso_var.recv_com_buff[4] == 'L'       ) &&
			 (my_disso_var.recv_com_buff[5] == 'N'       ) )
	{
		  my_disso_flags.pump_clean = 1;
		  AL_PP_Clear_Com_Buffer();

	}// sampling or replenishing validation
	else if( (my_disso_var.recv_com_buff[1] == 'P'       ) &&
			 (my_disso_var.recv_com_buff[2] == 'P'       ) &&
			 (my_disso_var.recv_com_buff[3] == 'V'       ) &&
			 (my_disso_var.recv_com_buff[4] == 'A'       ) &&
			((my_disso_var.recv_com_buff[5] == 'S'       )||(my_disso_var.recv_com_buff[5] == 'R'       )) )
	{
		  if(my_disso_var.recv_com_buff[5] == 'S')
		  {
		     my_disso_flags.pump_validate_sampling = 1;
		     my_disso_flags.pump_validate_replenishing = 0;
		  }
		  else if(my_disso_var.recv_com_buff[5] == 'R')
		  {
			 my_disso_flags.pump_validate_sampling = 0;
			 my_disso_flags.pump_validate_replenishing = 1;
		  }
		  else
		  {
			 my_disso_flags.pump_validate_sampling = 0;
			 my_disso_flags.pump_validate_replenishing = 0;

		  }
		  AL_PP_Clear_Com_Buffer();

	}// auto
	else if( (my_disso_var.recv_com_buff[1] == 'P'       ) &&
			 (my_disso_var.recv_com_buff[2] == 'P'       ) &&
			 (my_disso_var.recv_com_buff[3] == 'A'       ) &&
			 (my_disso_var.recv_com_buff[4] == 'U'       ) &&
			 (my_disso_var.recv_com_buff[5] == 'T'      ) )
	{
		  my_disso_flags.pump_auto_samling = 1;
		  AL_PP_Clear_Com_Buffer();

	}// buffer fill
	else if( (my_disso_var.recv_com_buff[1] == 'P'       ) &&
			 (my_disso_var.recv_com_buff[2] == 'P'       ) &&
			 (my_disso_var.recv_com_buff[3] == 'F'       ) &&
			 (my_disso_var.recv_com_buff[4] == 'I'       ) &&
			 (my_disso_var.recv_com_buff[5] == 'L'      ) )
	{
		  my_disso_flags.pump_fill = 1;
		  AL_PP_Clear_Com_Buffer();

	}// clean with filter
	else if( (my_disso_var.recv_com_buff[1] == 'P'       ) &&
			 (my_disso_var.recv_com_buff[2] == 'P'       ) &&
			 (my_disso_var.recv_com_buff[3] == 'C'       ) &&
			 (my_disso_var.recv_com_buff[4] == 'L'       ) &&
			 (my_disso_var.recv_com_buff[5] == 'F'      ) )
	{
		   my_disso_flags.pump_clean_wfilter = 1;
		   AL_PP_Clear_Com_Buffer();

	}
	// clean with filter
	else if( (my_disso_var.recv_com_buff[1] == 'P'       ) &&
			 (my_disso_var.recv_com_buff[2] == 'P'       ) &&
			 (my_disso_var.recv_com_buff[3] == 'N'       ) &&
			 (my_disso_var.recv_com_buff[4] == 'F'       ) &&
			 (my_disso_var.recv_com_buff[5] == 'C'      ) )
	{
		   my_disso_flags.pump_no_filter_change = 1;
		   AL_PP_Clear_Com_Buffer();

	}// get data
	else if(((my_disso_var.recv_com_buff[1] == 'P') &&
		   (my_disso_var.recv_com_buff[2] == 'P')) &&
		   ((my_disso_var.recv_com_buff[3] == '0') || (my_disso_var.recv_com_buff[3] == ' ') ||
		   (my_disso_var.recv_com_buff[3] == '1') || (my_disso_var.recv_com_buff[3] == '2')))
	{

           my_disso_flags.pump_param_status = 1;
           AL_PP_Receive_parameters();

	}
	else
	{
		/*nothing to do*/
	}

  }

}


void AL_PP_Receive_parameters(void)
{

		if(my_disso_var.recv_com_buff[3]==' ')
		{
			my_disso_var.temp_recv_data =my_disso_var.recv_com_buff[4] - 48;
			my_disso_var.temp_recv_data =my_disso_var.temp_recv_data*10;
		}

		else
		{
			my_disso_var.temp_recv_data =my_disso_var.recv_com_buff[3] - 48;
			my_disso_var.temp_recv_data =my_disso_var.temp_recv_data*10;
			my_disso_var.temp_recv_data =my_disso_var.temp_recv_data + (my_disso_var.recv_com_buff[4] - 48);
			my_disso_var.temp_recv_data =my_disso_var.temp_recv_data*10;
		}
		my_disso_var.temp_recv_data =my_disso_var.temp_recv_data + (my_disso_var.recv_com_buff[6] - 48);
		my_disso_var.recv_flowrate =my_disso_var.temp_recv_data;


		if(my_disso_flags.VALS == 1)
		{
			AL_PP_Receive_Volume_parameters();
			my_disso_var.temp_recv_data =my_disso_var.recv_com_buff[32]-48;
			my_disso_var.recv_fc_status=my_disso_var.temp_recv_data;
			my_disso_var.recv_Replenish_status = 2;
		}
		else if(my_disso_flags.VALR == 1)
		{
			if(my_disso_var.recv_com_buff[10]==' ')
			{
				my_disso_var.temp_recv_data =my_disso_var.recv_com_buff[11]-48;
				my_disso_var.temp_recv_data =my_disso_var.temp_recv_data*10;
			}

			else
			{
				my_disso_var.temp_recv_data =my_disso_var.recv_com_buff[10]-48;
				my_disso_var.temp_recv_data =my_disso_var.temp_recv_data*10;
				my_disso_var.temp_recv_data =my_disso_var.temp_recv_data + (my_disso_var.recv_com_buff[11]-48);
				my_disso_var.temp_recv_data =my_disso_var.temp_recv_data*10;
			}
			my_disso_var.temp_recv_data =my_disso_var.temp_recv_data + (my_disso_var.recv_com_buff[13]-48);
			my_disso_var.recv_sample_volume =my_disso_var.temp_recv_data;

			my_disso_var.temp_recv_data =my_disso_var.recv_com_buff[16]-0x30;
			my_disso_var.temp_recv_data =my_disso_var.temp_recv_data*10;
			my_disso_var.temp_recv_data =my_disso_var.temp_recv_data + (my_disso_var.recv_com_buff[18]-48);
			my_disso_var.recv_Dead_prev =my_disso_var.temp_recv_data;

			my_disso_var.temp_recv_data =my_disso_var.recv_com_buff[22]-0x30;
			my_disso_var.temp_recv_data =my_disso_var.temp_recv_data*10;
			my_disso_var.temp_recv_data =my_disso_var.temp_recv_data + (my_disso_var.recv_com_buff[24]-48);
			my_disso_var.recv_Dead_repv =my_disso_var.temp_recv_data;
			my_disso_var.recv_Replenish_status = 2;
		}

		else if(my_disso_flags.CLEAN == 0)
		{
			AL_PP_Receive_Volume_parameters();
			my_disso_var.temp_recv_data =my_disso_var.recv_com_buff[33]-48;
			my_disso_var.temp_recv_data =my_disso_var.temp_recv_data*10;
			my_disso_var.temp_recv_data =my_disso_var.temp_recv_data + (my_disso_var.recv_com_buff[35]-48);
			my_disso_var.recv_Dead_repv =my_disso_var.temp_recv_data;

			my_disso_var.temp_recv_data =my_disso_var.recv_com_buff[38]-48;
			if(my_disso_var.recv_com_buff[39]!=',')
			{
				my_disso_var.temp_recv_data =my_disso_var.temp_recv_data*10;
				my_disso_var.temp_recv_data =my_disso_var.temp_recv_data + (my_disso_var.recv_com_buff[39]-48);
			}
	        my_disso_var.recv_filter_change =my_disso_var.temp_recv_data;

			if(my_disso_var.recv_com_buff[39]==',')
			{
				my_disso_var.temp_recv_data =my_disso_var.recv_com_buff[40]-48;
				if(my_disso_var.recv_com_buff[41]!=',')
				{
					my_disso_var.temp_recv_data =my_disso_var.temp_recv_data*10;
					my_disso_var.temp_recv_data =my_disso_var.temp_recv_data + (my_disso_var.recv_com_buff[41]-48);
				}
			}
			else
			{
				my_disso_var.temp_recv_data =my_disso_var.recv_com_buff[41]-0x30;
				if(my_disso_var.recv_com_buff[42]!=',')
				{
					my_disso_var.temp_recv_data =my_disso_var.temp_recv_data*10;
					my_disso_var.temp_recv_data =my_disso_var.temp_recv_data + (my_disso_var.recv_com_buff[42]-48);
				}
			}
			my_disso_var.recv_intervals =my_disso_var.temp_recv_data;

			if(my_disso_var.recv_com_buff[41]==',')
			{
				my_disso_var.temp_recv_data =my_disso_var.recv_com_buff[42]-48;
			}
			else
			{
				if(my_disso_var.recv_com_buff[42]==',')
					my_disso_var.temp_recv_data =my_disso_var.recv_com_buff[43]-48;
				else
					my_disso_var.temp_recv_data=my_disso_var.recv_com_buff[44]-48;
			}
			my_disso_var.recv_Replenish_status=my_disso_var.temp_recv_data;

			if(my_disso_var.recv_com_buff[41]==',')
			{
				my_disso_var.temp_recv_data=my_disso_var.recv_com_buff[44]-48;
				my_disso_var.recv_change_buff_status =my_disso_var.recv_com_buff[46]-48;
				if(my_disso_var.recv_com_buff[47]!='#')
					my_disso_var.recv_change_buff_status=(my_disso_var.recv_change_buff_status*10)+(my_disso_var.recv_com_buff[47]-48);
			}
			else
			{
				if(my_disso_var.recv_com_buff[42]==',')
				{
					my_disso_var.temp_recv_data =my_disso_var.recv_com_buff[45]-48;
					my_disso_var.recv_change_buff_status=my_disso_var.recv_com_buff[47]-48;
					if(my_disso_var.recv_com_buff[48]!='#')
						my_disso_var.recv_change_buff_status=(my_disso_var.recv_change_buff_status*10)+(my_disso_var.recv_com_buff[48]-48);
				}
				else
				{
					my_disso_var.temp_recv_data=my_disso_var.recv_com_buff[46]-48;
					my_disso_var.recv_change_buff_status=my_disso_var.recv_com_buff[48]-48;
					if(my_disso_var.recv_com_buff[49]!='#')
						my_disso_var.recv_change_buff_status=(my_disso_var.recv_change_buff_status*10)+(my_disso_var.recv_com_buff[49]-48);
				}
			}
			my_disso_var.recv_fc_status = my_disso_var.temp_recv_data;

		}

		else
		{
			my_disso_var.temp_recv_data =my_disso_var.recv_com_buff[11]-48;
			my_disso_var.temp_recv_data =my_disso_var.temp_recv_data*10;
			my_disso_var.temp_recv_data =my_disso_var.temp_recv_data + (my_disso_var.recv_com_buff[13]-48);
			my_disso_var.recv_Dead_prev =my_disso_var.temp_recv_data;

			my_disso_var.temp_recv_data =my_disso_var.recv_com_buff[17]-48;
			my_disso_var.temp_recv_data =my_disso_var.temp_recv_data*10;
			my_disso_var.temp_recv_data =my_disso_var.temp_recv_data + (my_disso_var.recv_com_buff[19]-48);
			my_disso_var.recv_Dead_post =my_disso_var.temp_recv_data;

			my_disso_var.temp_recv_data =my_disso_var.recv_com_buff[23]-48;
			my_disso_var.temp_recv_data =my_disso_var.temp_recv_data*10;
			my_disso_var.temp_recv_data =my_disso_var.temp_recv_data + (my_disso_var.recv_com_buff[25]-48);
			my_disso_var.recv_Dead_repv =my_disso_var.temp_recv_data;

		}




}



void AL_PP_Receive_Volume_parameters(void)
{
	if(my_disso_var.recv_com_buff[10]==' ')
		{
		    my_disso_var.temp_recv_data =my_disso_var.recv_com_buff[11]-48;
			my_disso_var.temp_recv_data =my_disso_var.temp_recv_data*10;
		}

		else
		{
			my_disso_var.temp_recv_data =my_disso_var.recv_com_buff[10]-48;
			my_disso_var.temp_recv_data =my_disso_var.temp_recv_data*10;
			my_disso_var.temp_recv_data =my_disso_var.temp_recv_data + (my_disso_var.recv_com_buff[11]-48);
			my_disso_var.temp_recv_data =my_disso_var.temp_recv_data*10;
		}
		my_disso_var.temp_recv_data =my_disso_var.temp_recv_data + (my_disso_var.recv_com_buff[13]-48);
		my_disso_var.recv_sample_volume =my_disso_var.temp_recv_data;

	    if(my_disso_var.recv_com_buff[15]==' ')
	    {
	    	my_disso_var.temp_recv_data =my_disso_var.recv_com_buff[16]-48;
	    	my_disso_var.temp_recv_data =my_disso_var.temp_recv_data*10;
	    }
	    else
	    {
	        my_disso_var.temp_recv_data =my_disso_var.recv_com_buff[15]-48;
			my_disso_var.temp_recv_data =my_disso_var.temp_recv_data*10;
			my_disso_var.temp_recv_data =my_disso_var.temp_recv_data + (my_disso_var.recv_com_buff[16]-48);
			my_disso_var.temp_recv_data =my_disso_var.temp_recv_data*10;
	    }
		my_disso_var.temp_recv_data =my_disso_var.temp_recv_data + (my_disso_var.recv_com_buff[18]-48);
		my_disso_var.recv_rinse_volume =my_disso_var.temp_recv_data;

		my_disso_var.temp_recv_data =my_disso_var.recv_com_buff[21]-48;
		my_disso_var.temp_recv_data =my_disso_var.temp_recv_data*10;
		my_disso_var.temp_recv_data =my_disso_var.temp_recv_data + (my_disso_var.recv_com_buff[23]-48);
		my_disso_var.recv_Dead_prev =my_disso_var.temp_recv_data;

		my_disso_var.temp_recv_data =my_disso_var.recv_com_buff[27]-48;
		my_disso_var.temp_recv_data =my_disso_var.temp_recv_data*10;
		my_disso_var.temp_recv_data =my_disso_var.temp_recv_data + (my_disso_var.recv_com_buff[29]-48);
		my_disso_var.recv_Dead_post =my_disso_var.temp_recv_data;
}

void AL_PP_Clear_Com_Buffer(void)
{

	for(uint8_t i= 0;i <= COMM_BUFF_LEN; i++)
	{
		my_disso_var.recv_com_buff[i] = 0;
	}
}



void AL_PP_Run_process(void)
{



       /********************************************************************************************/
	      // run auto mode

	      // reset pump process
		  if( my_disso_flags.pump_reset  == 1 )
		  {
			  my_disso_flags.pump_auto_mode = 1;
			  AL_PP_ALL_Stop_Motor();
			  AL_PP_Solonoid_valve_reset();
			  my_disso_flags.CLEAN = 0;
			  my_disso_flags.VALS = 0;
			  my_disso_flags.VALR = 0;
			  my_disso_flags.PRIME = 0;
			  my_disso_flags.chk_pp_dir_flag = 0;
			  my_disso_var.start_cycle_counter = 0;
			  AL_DWIN_Picshow(100);   // manual mode


	          my_disso_var.tx_com_buff[0]= START_CHAR;
	          my_disso_var.tx_com_buff[1]= 'P';
	          my_disso_var.tx_com_buff[2]= 'P';
	          my_disso_var.tx_com_buff[3]= 'R';
	          my_disso_var.tx_com_buff[4]= 'S';
	          my_disso_var.tx_com_buff[5]= 'T';
	          my_disso_var.tx_com_buff[6]= END_CHAR;
	          HAL_UART_Transmit(&huart5, my_disso_var.tx_com_buff, 7, 10);

	          my_disso_flags.pump_reset = 0;
	          AL_PP_COM_NV_Park();


	          AL_DWIN_Picshow(11);     // settings

	          my_disso_flags.pump_auto_mode = 0;
		  }

       /*************************************************************************************************/
         // pump forward
		 else if(my_disso_flags.pump_forward == 1)
		 {


			  AL_DWIN_Picshow(102);   // ready in auto mode


			  // clear flags
			  my_disso_flags.CLEAN = 0;
			  my_disso_flags.VALS = 0;
			  my_disso_flags.VALR = 0;
			  my_disso_var.start_cycle_counter = 0;
			  my_disso_var.cycle_count = 0;

			  AL_PP_Solonoid_valve_reset();

			  my_disso_var.tx_com_buff[0]= START_CHAR;
			  my_disso_var.tx_com_buff[1]= 'P';
			  my_disso_var.tx_com_buff[2]= 'P';
			  my_disso_var.tx_com_buff[3]= 'F';
			  my_disso_var.tx_com_buff[4]= 'W';
			  my_disso_var.tx_com_buff[5]= 'D';
			  my_disso_var.tx_com_buff[6]= END_CHAR;
			  HAL_UART_Transmit(&huart5, my_disso_var.tx_com_buff, 7, 10);


			  my_disso_flags.pump_forward = 0;
			  my_disso_flags.chk_pp_dir_flag = 1;

			  my_disso_var.recv_Replenish_status = 2;

		 }

		 // pump reversed
		 else if(my_disso_flags.pump_reversed == 1)
		 {
			  //clear flags
			  my_disso_flags.CLEAN = 0;
			  my_disso_flags.VALR = 0;
			  my_disso_flags.VALR = 0;
			  my_disso_var.start_cycle_counter = 0;
			  my_disso_var.cycle_count = 0;

			  my_disso_var.recv_Replenish_status = 2;

			  AL_PP_Solonoid_valve_on();

			  my_disso_var.tx_com_buff[0]= START_CHAR;
			  my_disso_var.tx_com_buff[1]= 'P';
			  my_disso_var.tx_com_buff[2]= 'P';
			  my_disso_var.tx_com_buff[3]= 'R';
			  my_disso_var.tx_com_buff[4]= 'E';
			  my_disso_var.tx_com_buff[5]= 'V';
			  my_disso_var.tx_com_buff[6]= END_CHAR;
			  HAL_UART_Transmit(&huart5, my_disso_var.tx_com_buff, 7, 10);


			  AL_DWIN_Picshow(102);   //ready in auto mode

			  my_disso_flags.pump_reversed = 0;
			  my_disso_flags.chk_pp_dir_flag = 2;
		 }

        /*************************************************************************************************/
		  // start pump
		 else if(my_disso_flags.pump_start == 1 )
		 {



				my_disso_var.tx_com_buff[0]= START_CHAR;
				my_disso_var.tx_com_buff[1]= 'P';
				my_disso_var.tx_com_buff[2]= 'P';
				my_disso_var.tx_com_buff[3]= 'S';
				my_disso_var.tx_com_buff[4]= 'T';
				my_disso_var.tx_com_buff[5]= END_CHAR;
				HAL_UART_Transmit(&huart5, my_disso_var.tx_com_buff, 6, 10);


				my_disso_flags.pump_auto_mode = 1;

                // start pump in forward direction
				if(my_disso_flags.chk_pp_dir_flag == 1)
				{

					AL_DWIN_Picshow(130);     // sampling

					my_disso_var.recv_flowrate = AL_EEPROM_Read_NUM(1, 0);

					my_disso_var.load_recv_rpm = (uint32_t)((float)((double)my_disso_var.recv_flowrate / (double)DISPENSE_PER_REV));
					AL_PP_ALL_Start_Motor(my_disso_var.load_recv_rpm, PP_Forward);
					my_disso_flags.pump_start = 0;

					my_disso_var.load_recv_rpm = 0;

				}
				// start pump in reversed direction
				else if(my_disso_flags.chk_pp_dir_flag  == 2)
				{
					AL_DWIN_Picshow(135);    // replenishing

					my_disso_var.recv_flowrate = AL_EEPROM_Read_NUM(1, 0);

					my_disso_var.load_recv_rpm = (uint32_t)((float)((double)my_disso_var.recv_flowrate / (double)DISPENSE_PER_REV));
					AL_PP_ALL_Start_Motor(my_disso_var.load_recv_rpm, PP_Reversed);
					my_disso_flags.pump_start = 0;

					my_disso_var.load_recv_rpm = 0;
				}
				// start pump for cleaning
				else if(my_disso_flags.CLEAN == 1)
				{
					AL_DWIN_Picshow(113);    // start cleaning cycle

					my_disso_flags.PRIME = 1;
					// sample path cleaning

					my_disso_var.get_dispense_volume = (uint32_t)(((my_disso_var.recv_Dead_prev*10) +  (my_disso_var.recv_Dead_post*10))*3);
                    AL_PP_Dispense_Volume(PP_Forward,my_disso_var.get_dispense_volume);

                    // replenish path cleaning
                    AL_PP_Solonoid_valve_on();

                    my_disso_var.get_dispense_volume = (uint32_t)(((my_disso_var.recv_Dead_prev*10) +  (my_disso_var.recv_Dead_repv*10))*3);
                    my_disso_var.track_steps = 0;
                    AL_PP_ALL_Start_Motor(CLEAN_RPM, PP_Forward);
                    while(my_disso_var.track_steps < (my_disso_var.get_dispense_volume * STEPS_PER_REV))
                    {
                         /* nothing to do*/
                    }
                    AL_PP_ALL_Stop_Motor();
                    AL_PP_Solonoid_valve_reset();
                    my_disso_var.track_steps = 0;
                    my_disso_var.get_dispense_volume = 0;



                    // track 2 times
                    my_disso_var.start_cycle_counter ++;
                    if(my_disso_var.start_cycle_counter == 2 )
                    {
                    	// clear flags
                    	my_disso_var.start_cycle_counter = 0;
                    	my_disso_flags.CLEAN = 0;
                    	my_disso_flags.PRIME = 0;
                    	my_disso_flags.VALR = 0;
                    	my_disso_flags.VALS = 0;
                    	// bring to home position
                    	AL_PP_COM_NV_Park();

                    	my_disso_var.tx_com_buff[0]= START_CHAR;
					    my_disso_var.tx_com_buff[1]= 'P';
					    my_disso_var.tx_com_buff[2]= 'P';
					    my_disso_var.tx_com_buff[3]= 'E';
					    my_disso_var.tx_com_buff[4]= 'N';
					    my_disso_var.tx_com_buff[5]= 'D';
					    my_disso_var.tx_com_buff[6]= END_CHAR;
					    HAL_UART_Transmit(&huart5, my_disso_var.tx_com_buff, 7, 10);

                    	AL_DWIN_Picshow(114);    // completed cleaning cycle

                        AL_PP_Buzzer_check();
                        AL_PP_Buzzer_check();

                    }
                    else
                    {
                    	  my_disso_var.tx_com_buff[0]= START_CHAR;
						  my_disso_var.tx_com_buff[1]= 'P';
						  my_disso_var.tx_com_buff[2]= 'P';
						  my_disso_var.tx_com_buff[3]= 'E';
						  my_disso_var.tx_com_buff[4]= 'N';
						  my_disso_var.tx_com_buff[5]= 'D';
						  my_disso_var.tx_com_buff[6]= END_CHAR;
						  HAL_UART_Transmit(&huart5, my_disso_var.tx_com_buff, 7, 10);


                    }

                    my_disso_flags.pump_start = 0;

				  }

				  // validate sampling
				  else if(my_disso_flags.VALS == 1)
				  {

                      if(my_disso_var.start_cycle_counter == 0)
                      {

                    	 my_disso_flags.PRIME = 1;
                    	 my_disso_flags.FLUSH3 = 1;
                    	 // check rinse volume and sample volume

                    	 my_disso_var.get_dispense_volume = (uint32_t)(( (my_disso_var.recv_Dead_prev*10) +( my_disso_var.recv_Dead_post*10))+7) ;
                    	 AL_PP_Dispense_Volume(PP_Forward, my_disso_var.get_dispense_volume);
                    	 my_disso_flags.FLUSH3 = 0;
                    	 my_disso_var.get_dispense_volume = 0;


                    	 if(my_disso_var.recv_rinse_volume != 0)
                    	 {
                    		 my_disso_var.get_dispense_volume = (uint32_t) my_disso_var.recv_rinse_volume*10;
                    		 AL_DWIN_Picshow(117);    // validate rinsing

                    	 }
                    	 else
                    	 {
                    		 my_disso_var.get_dispense_volume = (uint32_t) my_disso_var.recv_sample_volume*10;
                    		 AL_DWIN_Picshow(116);    // validate sampling

                    	 }

                      }
                      else
                      {

						 my_disso_var.get_dispense_volume = (uint32_t) ((my_disso_var.recv_sample_volume*10) - (my_disso_var.recv_rinse_volume*10));
						 AL_DWIN_Picshow(116);    // validate sampling


                      }
                      my_disso_flags.PRIME = 1;

                      AL_PP_Dispense_Volume(PP_Forward, my_disso_var.get_dispense_volume);
                      AL_DWIN_Picshow(102);    // ready in auto mode

                      my_disso_var.get_dispense_volume = 0;

                      my_disso_var.tx_com_buff[0]= START_CHAR;
					  my_disso_var.tx_com_buff[1]= 'P';
					  my_disso_var.tx_com_buff[2]= 'P';
					  my_disso_var.tx_com_buff[3]= 'E';
					  my_disso_var.tx_com_buff[4]= 'N';
					  my_disso_var.tx_com_buff[5]= 'D';
					  my_disso_var.tx_com_buff[6]= END_CHAR;
					  HAL_UART_Transmit(&huart5, my_disso_var.tx_com_buff, 7, 10);


					  my_disso_var.start_cycle_counter ++;
					  if((my_disso_var.start_cycle_counter == 2)||
						 ((my_disso_var.start_cycle_counter == 1) && (my_disso_var.recv_rinse_volume == 0)))
					  {
						  my_disso_var.start_cycle_counter = 0;

						  AL_PP_Buzzer_check();
						  AL_PP_Buzzer_check();

					  }


				  }

				  // validate replenishing
				  else if(my_disso_flags.VALR == 1)
				  {


					  my_disso_var.get_dispense_volume = (uint32_t) (my_disso_var.recv_sample_volume*10);
					  AL_DWIN_Picshow(118);    // validate replenishing


					  my_disso_flags.PRIME = 1;
				      AL_PP_Dispense_Volume(PP_Reversed, my_disso_var.get_dispense_volume);

				      AL_DWIN_Picshow(102);    // ready in auto mode


				      my_disso_var.tx_com_buff[0]= START_CHAR;
					  my_disso_var.tx_com_buff[1]= 'P';
					  my_disso_var.tx_com_buff[2]= 'P';
					  my_disso_var.tx_com_buff[3]= 'E';
					  my_disso_var.tx_com_buff[4]= 'N';
					  my_disso_var.tx_com_buff[5]= 'D';
					  my_disso_var.tx_com_buff[6]= END_CHAR;
					  HAL_UART_Transmit(&huart5, my_disso_var.tx_com_buff, 7, 10);


					  my_disso_var.replenish_count++;
					  AL_PP_Buzzer_check();
					  AL_PP_Buzzer_check();

				  }

                  // start disso run with rinsing or sampling
				  else if( my_disso_var.start_cycle_counter == 0)
				  {


					  if( my_disso_var.recv_rinse_volume != 0)
					  {
						  AL_DWIN_Picshow(131);    // auto rinsing
						  my_disso_flags.PRIME = 1;
						  my_disso_flags.FLUSH3  = 1;
						  my_disso_var.get_dispense_volume =  (uint32_t)((my_disso_var.recv_Dead_prev*10) + (my_disso_var.recv_Dead_post*10)+7);
						  AL_PP_Dispense_Volume(PP_Forward, my_disso_var.get_dispense_volume);
						  my_disso_flags.FLUSH3  = 0;

						  my_disso_var.get_dispense_volume =  (uint32_t)(my_disso_var.recv_rinse_volume*10);
						  AL_PP_Dispense_Volume(PP_Forward, my_disso_var.get_dispense_volume);

					  }
					  else
					  {
						  AL_DWIN_Picshow(130);    // auto sampling
						  my_disso_flags.PRIME = 1;
						  my_disso_flags.FLUSH3  = 1;
						  my_disso_var.get_dispense_volume =  (uint32_t)((my_disso_var.recv_Dead_prev*10) + (my_disso_var.recv_Dead_post*10)+7);
						  AL_PP_Dispense_Volume(PP_Forward, my_disso_var.get_dispense_volume);
						  my_disso_flags.FLUSH3  = 0;

						  my_disso_var.get_dispense_volume =  (uint32_t)(my_disso_var.recv_sample_volume*10);
						  AL_PP_Dispense_Volume(PP_Forward, my_disso_var.get_dispense_volume);

						  my_disso_var.cycle_count ++;
						  // store the cycle count
					  }


					  my_disso_var.get_dispense_volume = 0;
                      // inc cycle count
					  my_disso_var.start_cycle_counter++;

					  my_disso_var.tx_com_buff[0]= START_CHAR;
					  my_disso_var.tx_com_buff[1]= 'P';
					  my_disso_var.tx_com_buff[2]= 'P';
					  my_disso_var.tx_com_buff[3]= 'E';
					  my_disso_var.tx_com_buff[4]= 'N';
					  my_disso_var.tx_com_buff[5]= 'D';
					  my_disso_var.tx_com_buff[6]= END_CHAR;
					  HAL_UART_Transmit(&huart5, my_disso_var.tx_com_buff, 7, 10);


					  AL_DWIN_Picshow(102);    // ready in auto mode


					  AL_PP_Buzzer_check();
					  AL_PP_Buzzer_check();
				  }
                  // sample or replenishing run
				  else if( my_disso_var.start_cycle_counter == 1)
				  {
					  // check rinse volume and sample volume



					  if(my_disso_var.recv_rinse_volume != 0)
					  {
						  AL_DWIN_Picshow(130);    // auto sampling
						  my_disso_flags.PRIME = 1;
                          my_disso_var.get_dispense_volume = (uint32_t)((my_disso_var.recv_sample_volume*10) - (my_disso_var.recv_rinse_volume*10));
                          AL_PP_Dispense_Volume(PP_Forward, my_disso_var.get_dispense_volume);
                          my_disso_var.get_dispense_volume = 0;
                          // inc cycle count
                          my_disso_var.start_cycle_counter++;
                          my_disso_var.cycle_count ++;
                          // store cycle count
                          my_disso_var.tx_com_buff[0]= START_CHAR;
						  my_disso_var.tx_com_buff[1]= 'P';
						  my_disso_var.tx_com_buff[2]= 'P';
						  my_disso_var.tx_com_buff[3]= 'E';
						  my_disso_var.tx_com_buff[4]= 'N';
						  my_disso_var.tx_com_buff[5]= 'D';
						  my_disso_var.tx_com_buff[6]= END_CHAR;
						  HAL_UART_Transmit(&huart5, my_disso_var.tx_com_buff, 7, 10);


						  // check run cycle completed count
						  AL_DWIN_Picshow(102);    //ready in auto mode


						  AL_PP_Buzzer_check();
						  AL_PP_Buzzer_check();

					  }
					  else
					  {
						  my_disso_var.start_cycle_counter = 0;
						  my_disso_var.recv_intervals --;
						  my_disso_var.interval_count ++;
						  // store the interval and count

						  if(my_disso_var.recv_intervals == 0)
						  {
							  my_disso_var.run_interrupted = 0;
							  my_disso_var.interval_count = 0;
							  // store the counts
							  my_disso_var.run_in_progress = 0;
						  }

						  if(my_disso_var.recv_Replenish_status == 1)
						  {
							   my_disso_flags.PRIME = 1;
							   my_disso_flags.FLUSH2 = 1;
							   AL_DWIN_Picshow(135);    // auto replenishing
							   my_disso_var.get_dispense_volume = (uint32_t)(my_disso_var.recv_sample_volume*10 );
							   AL_PP_Dispense_Volume(PP_Reversed, my_disso_var.get_dispense_volume);
							   my_disso_flags.FLUSH2 = 0;
							   my_disso_flags.FLUSH3 = 1;
							   my_disso_var.get_dispense_volume = (uint32_t)(((my_disso_var.recv_Dead_prev*10) + (my_disso_var.recv_Dead_post*10 ))*2);
							   AL_PP_Dead_Volume_Empty(PP_Reversed,my_disso_var.get_dispense_volume);
							   my_disso_flags.FLUSH3 = 0;

							   AL_DWIN_Picshow(102);    // ready in auto mode


							   my_disso_var.tx_com_buff[0]= START_CHAR;
							   my_disso_var.tx_com_buff[1]= 'P';
							   my_disso_var.tx_com_buff[2]= 'P';
							   my_disso_var.tx_com_buff[3]= 'E';
							   my_disso_var.tx_com_buff[4]= 'N';
							   my_disso_var.tx_com_buff[5]= 'D';
							   my_disso_var.tx_com_buff[6]= END_CHAR;
							   HAL_UART_Transmit(&huart5, my_disso_var.tx_com_buff, 7, 10);

							   AL_PP_Buzzer_check();
   	   						   AL_PP_Buzzer_check();

						  }
						  else
						  {
							    my_disso_flags.PRIME = 1;
							    my_disso_flags.FLUSH3 = 1;
							    my_disso_var.get_dispense_volume = (uint32_t)((my_disso_var.recv_Dead_prev + my_disso_var.recv_Dead_post )*2);
							  	AL_PP_Dead_Volume_Empty(PP_Reversed,my_disso_var.get_dispense_volume);
							    my_disso_flags.FLUSH3 = 0;

							    my_disso_var.tx_com_buff[0]= START_CHAR;
							    my_disso_var.tx_com_buff[1]= 'P';
							    my_disso_var.tx_com_buff[2]= 'P';
							    my_disso_var.tx_com_buff[3]= 'E';
							    my_disso_var.tx_com_buff[4]= 'N';
							    my_disso_var.tx_com_buff[5]= 'D';
							    my_disso_var.tx_com_buff[6]= END_CHAR;
							    HAL_UART_Transmit(&huart5, my_disso_var.tx_com_buff, 7, 10);

						  }

					  }


				   }
                  // replenishing run
				  else if( my_disso_var.start_cycle_counter == 2)
				  {


					  // clear variable count
					  my_disso_var.start_cycle_counter = 0;
                      my_disso_var.recv_intervals -- ;
                      my_disso_var.interval_count++;
                      // store the result

                      if(my_disso_var.recv_intervals == 0)
                      {
                    	  my_disso_var.run_interrupted = 0;
                    	  my_disso_var.interval_count = 0;
                    	  // store the result
                    	  my_disso_var.run_in_progress = 0;
                      }

                      if(my_disso_var.recv_Replenish_status == 1)
					  {
						   my_disso_flags.PRIME = 1;
						   my_disso_flags.FLUSH2 = 1;
						   AL_DWIN_Picshow(135);    // auto replenishing
						   my_disso_var.get_dispense_volume = (uint32_t)(my_disso_var.recv_sample_volume );
						   AL_PP_Dispense_Volume(PP_Reversed, my_disso_var.get_dispense_volume);
						   my_disso_flags.FLUSH2 = 0;
						   my_disso_flags.FLUSH3 = 1;

						   my_disso_var.get_dispense_volume = (uint32_t)((my_disso_var.recv_Dead_prev + my_disso_var.recv_Dead_post )*2);
						   AL_PP_Dead_Volume_Empty(PP_Reversed,my_disso_var.get_dispense_volume);
						   my_disso_flags.FLUSH3 = 0;

						   AL_DWIN_Picshow(102);    // ready in auto mode

						   my_disso_var.tx_com_buff[0]= START_CHAR;
						   my_disso_var.tx_com_buff[1]= 'P';
						   my_disso_var.tx_com_buff[2]= 'P';
						   my_disso_var.tx_com_buff[3]= 'E';
						   my_disso_var.tx_com_buff[4]= 'N';
						   my_disso_var.tx_com_buff[5]= 'D';
						   my_disso_var.tx_com_buff[6]= END_CHAR;
						   HAL_UART_Transmit(&huart5, my_disso_var.tx_com_buff, 7, 10);


						   AL_PP_Buzzer_check();
						   AL_PP_Buzzer_check();

					  }
					  else
					  {
							my_disso_flags.PRIME = 1;
							my_disso_flags.FLUSH3 = 1;
							my_disso_var.get_dispense_volume = (uint32_t)((my_disso_var.recv_Dead_prev + my_disso_var.recv_Dead_post )*2);
							AL_PP_Dead_Volume_Empty(PP_Reversed,my_disso_var.get_dispense_volume);
							my_disso_flags.FLUSH3 = 0;

							my_disso_var.tx_com_buff[0]= START_CHAR;
							my_disso_var.tx_com_buff[1]= 'P';
							my_disso_var.tx_com_buff[2]= 'P';
							my_disso_var.tx_com_buff[3]= 'E';
							my_disso_var.tx_com_buff[4]= 'N';
							my_disso_var.tx_com_buff[5]= 'D';
							my_disso_var.tx_com_buff[6]= END_CHAR;
							HAL_UART_Transmit(&huart5, my_disso_var.tx_com_buff, 7, 10);

					  }
					  AL_PP_Buzzer_check();
					  AL_PP_Buzzer_check();
				  }
				  else
				  {
                              // nothing to do
				  }

				my_disso_flags.pump_start = 0;
				my_disso_flags.pump_auto_mode = 0;

		 }
		 /*************************************************************************************************/
		  // cleaning cycle
		 else if(my_disso_flags.pump_clean == 1 )
		 {
			  my_disso_flags.pump_auto_mode = 1;

			  AL_DWIN_Picshow(102);    // ready in auto mode

			  my_disso_var.tx_com_buff[0]= START_CHAR;
			  my_disso_var.tx_com_buff[1]= 'P';
			  my_disso_var.tx_com_buff[2]= 'P';
			  my_disso_var.tx_com_buff[3]= 'C';
			  my_disso_var.tx_com_buff[4]= 'L';
			  my_disso_var.tx_com_buff[5]= 'N';
			  my_disso_var.tx_com_buff[6]= END_CHAR;
			  HAL_UART_Transmit(&huart5, my_disso_var.tx_com_buff, 7, 10);

			  my_disso_flags.pump_clean = 0;

			  my_disso_flags.CLEAN = 1;
			  my_disso_flags.VALR = 0;
			  my_disso_flags.VALS = 0;
			  my_disso_var.start_cycle_counter = 0;


			  my_disso_flags.pump_auto_mode = 0;

		 }
		 /*************************************************************************************************/
          // validation of sampling and replenishing
		 else if((my_disso_flags.pump_validate_replenishing == 1 ) ||
		     (my_disso_flags.pump_validate_sampling == 1 )	  )
		  {
			  my_disso_flags.pump_auto_mode = 1;

			  my_disso_var.tx_com_buff[0]= START_CHAR;
			  my_disso_var.tx_com_buff[1]= 'P';
			  my_disso_var.tx_com_buff[2]= 'P';
			  my_disso_var.tx_com_buff[3]= 'V';
			  my_disso_var.tx_com_buff[4]= 'A';

			  if(my_disso_flags.pump_validate_replenishing == 1)
			  {
			     my_disso_var.tx_com_buff[5]= 'R';
			     my_disso_flags.VALR = 1;
			     my_disso_flags.VALS = 0;
			  }

			  if(my_disso_flags.pump_validate_sampling == 1)
			  {
				 my_disso_var.tx_com_buff[5]= 'S';
				 my_disso_flags.VALR = 0;
				 my_disso_flags.VALS = 1;
			  }
			  my_disso_var.tx_com_buff[6]= END_CHAR;


			  AL_DWIN_Picshow(102);    // auto mode


			  if(my_disso_flags.VALS == 1)
			  {
				  AL_PP_Sampling_Empty();
				  my_disso_var.recv_Replenish_status = 2;
			  }

			  if(my_disso_flags.VALR == 1)
			  {
				  AL_PP_Replenish_Empty();
			  }

			  HAL_UART_Transmit(&huart5, my_disso_var.tx_com_buff, 7, 10);

			  my_disso_flags.CLEAN = 0;
			  my_disso_var.start_cycle_counter = 0;

			  my_disso_flags.pump_validate_replenishing = 0;
			  my_disso_flags.pump_validate_sampling = 0;
			  my_disso_flags.pump_auto_mode = 0;

		  }
		  /*************************************************************************************************/
		  // escape from the running process
		 else if(my_disso_flags.pump_escape == 1 )
		  {

			     // clear flags
				 my_disso_flags.CLEAN = 0;
			     my_disso_var.start_cycle_counter = 0;
			     AL_PP_ALL_Stop_Motor();
			     AL_PP_Solonoid_valve_reset();

                 if(my_disso_flags.CLEAN == 0)
                 {
				  if((my_disso_flags.chk_pp_dir_flag == 2) ||
					  (my_disso_flags.VALR == 1) || (my_disso_var.recv_Replenish_status == 1))
				  {
                      AL_PP_Replenish_Empty();
				  }

				  if(my_disso_flags.VALS == 1)
				  {
					  AL_PP_Sampling_Empty();
				  }
                 }

                 // clear flags
                  my_disso_flags.CLEAN = 0;
				  my_disso_flags.VALS = 0;
				  my_disso_flags.VALR = 0;
				  my_disso_flags.chk_pp_dir_flag = 0;
				  my_disso_var.start_cycle_counter = 0;
				  AL_PP_COM_NV_Park();
				  AL_DWIN_Picshow(100);    // manual mode


				  AL_DWIN_Picshow(11);     // settings

				  my_disso_var.tx_com_buff[0]= START_CHAR;
				  my_disso_var.tx_com_buff[1]= 'P';
				  my_disso_var.tx_com_buff[2]= 'P';
				  my_disso_var.tx_com_buff[3]= 'E';
				  my_disso_var.tx_com_buff[4]= 'N';
				  my_disso_var.tx_com_buff[5]= 'D';
				  my_disso_var.tx_com_buff[6]= END_CHAR;
				  HAL_UART_Transmit(&huart5, my_disso_var.tx_com_buff, 7, 10);



				  my_disso_flags.pump_auto_mode = 0;

				  my_disso_flags.pump_escape = 0;

				  if(my_disso_var.run_in_progress == 1)
				  {
                      my_disso_var.run_interrupted = 1;
                      // store the run
					  my_disso_var.run_in_progress = 0;
				  }

		   }
		  /*************************************************************************************************/
		   // chk pump
		   else if(my_disso_flags.pump_chk == 1 )
           {
        	      my_disso_flags.pump_auto_mode = 1;
        	      my_disso_var.tx_com_buff[0]= START_CHAR;
				  my_disso_var.tx_com_buff[1]= 'P';
				  my_disso_var.tx_com_buff[2]= 'P';
				  my_disso_var.tx_com_buff[3]= 'C';
				  my_disso_var.tx_com_buff[4]= 'H';
				  my_disso_var.tx_com_buff[5]= 'K';
				  my_disso_var.tx_com_buff[6]= END_CHAR;
				  HAL_UART_Transmit(&huart5, my_disso_var.tx_com_buff, 7, 10);

				  my_disso_flags.pump_chk = 0;
				  my_disso_flags.pump_auto_mode = 0;

           }

           /*************************************************************************************************/
		   // clean with filter
		   else  if(my_disso_flags.pump_clean_wfilter == 1 )
		   {
				  my_disso_flags.pump_auto_mode = 1;

				  AL_DWIN_Picshow(102);    // ready auto mode



				  my_disso_var.tx_com_buff[0]= START_CHAR;
				  my_disso_var.tx_com_buff[1]= 'P';
				  my_disso_var.tx_com_buff[2]= 'P';
				  my_disso_var.tx_com_buff[3]= 'C';
				  my_disso_var.tx_com_buff[4]= 'L';
				  my_disso_var.tx_com_buff[5]= 'N';
				  my_disso_var.tx_com_buff[6]= END_CHAR;
				  HAL_UART_Transmit(&huart5, my_disso_var.tx_com_buff, 7, 10);


				  // clear flags
				  my_disso_flags.CLEAN = 1;
				  my_disso_flags.VALR = 0;
				  my_disso_flags.VALS = 0;

				  my_disso_var.start_cycle_counter = 0;


				  my_disso_flags.pump_clean_wfilter = 0;
				  my_disso_flags.pump_auto_mode = 0;

			}


			/*************************************************************************************************/
		      // no filter change
		    else if(my_disso_flags.pump_no_filter_change == 1 )
			{
				  my_disso_flags.pump_auto_mode = 1;

				  AL_DWIN_Picshow(102);    // ready in auto mode
				  HAL_Delay(10);


				  my_disso_var.tx_com_buff[0]= START_CHAR;
				  my_disso_var.tx_com_buff[1]= 'P';
				  my_disso_var.tx_com_buff[2]= 'P';
				  my_disso_var.tx_com_buff[3]= 'C';
				  my_disso_var.tx_com_buff[4]= 'L';
				  my_disso_var.tx_com_buff[5]= 'N';
				  my_disso_var.tx_com_buff[6]= END_CHAR;
				  HAL_UART_Transmit(&huart5, my_disso_var.tx_com_buff, 7, 10);


				  // clear flags
				  my_disso_flags.CLEAN = 1;
				  my_disso_flags.VALR = 0;
				  my_disso_flags.VALS = 0;

				  my_disso_var.start_cycle_counter = 0;


				  my_disso_flags.pump_no_filter_change = 0;
				  my_disso_flags.pump_auto_mode = 0;

		   }
           /*************************************************************************************************/
           // chk pump ready or not
		   else if(my_disso_flags.pump_ready == 1 )
		   {

        	      my_disso_flags.pump_auto_mode = 1;
				  //clear flags
				  my_disso_flags.CLEAN = 0;
				  my_disso_flags.VALR = 0;
				  my_disso_flags.VALS = 0;
				  my_disso_flags.chk_pp_dir_flag = 0;
				  my_disso_var.start_cycle_counter = 0;
				  my_disso_var.cycle_count = 0;
				  my_disso_var.interval_count = 0;

                  my_disso_var.run_interrupted = 0;
                  // store the run

				  AL_DWIN_Picshow(102);   // ready in auto mode


				  AL_PP_Sampling_Empty();
				  AL_PP_Replenish_Empty();

				  my_disso_var.tx_com_buff[0]= START_CHAR;
				  my_disso_var.tx_com_buff[1]= 'P';
				  my_disso_var.tx_com_buff[2]= 'P';
				  my_disso_var.tx_com_buff[3]= 'R';
				  my_disso_var.tx_com_buff[4]= 'D';
				  my_disso_var.tx_com_buff[5]= 'Y';
				  my_disso_var.tx_com_buff[6]= END_CHAR;
				  HAL_UART_Transmit(&huart5, my_disso_var.tx_com_buff, 7, 10);

				  my_disso_flags.pump_ready = 0;
				  my_disso_flags.pump_auto_mode = 0;

		    }
           /*************************************************************************************************/
            // check buffer mode
		    else if(my_disso_flags.pump_buff_change == 1 )
            {
            	 my_disso_flags.pump_auto_mode = 1;

            	 AL_DWIN_Picshow(102);   // ready in auto mode
                 HAL_Delay(10);

                 my_disso_var.recv_Dead_repv =  (AL_EEPROM_Read_NUM(4, 0)*10);

                 if(my_disso_var.recv_Replenish_status == 1 )
                 {
                	 my_disso_var.get_dispense_volume = (my_disso_var.recv_Dead_repv) + 6;
                	 my_disso_flags.PRIME = 1;
                	 my_disso_flags.FLUSH1 = 1;
                	 AL_PP_Dispense_Volume(PP_Reversed,my_disso_var.get_dispense_volume );
                	 my_disso_flags.FLUSH1 = 0;
                 }


                  my_disso_var.tx_com_buff[0]= START_CHAR;
				  my_disso_var.tx_com_buff[1]= 'P';
				  my_disso_var.tx_com_buff[2]= 'P';
				  my_disso_var.tx_com_buff[3]= 'E';
				  my_disso_var.tx_com_buff[4]= 'N';
				  my_disso_var.tx_com_buff[5]= 'D';
				  my_disso_var.tx_com_buff[6]= END_CHAR;
				  HAL_UART_Transmit(&huart5, my_disso_var.tx_com_buff, 7, 10);

				  my_disso_flags.pump_buff_change = 0;
            	  my_disso_flags.pump_auto_mode = 0;

            }
            /*************************************************************************************************/
            // pump in auto for filter changer
		    else if(my_disso_flags.pump_auto_samling == 1)
            {
            	 my_disso_flags.pump_auto_mode = 1;

            	 AL_DWIN_Picshow(102);   // ready in auto mode
            	 HAL_Delay(10);

            	 if(my_disso_var.recv_Replenish_status == 1)
            	 {
            		 AL_PP_Replenish_Empty();
            	 }

            	  my_disso_var.tx_com_buff[0]= START_CHAR;
				  my_disso_var.tx_com_buff[1]= 'P';
				  my_disso_var.tx_com_buff[2]= 'P';
				  my_disso_var.tx_com_buff[3]= 'A';
				  my_disso_var.tx_com_buff[4]= 'U';
				  my_disso_var.tx_com_buff[5]= 'T';
				  my_disso_var.tx_com_buff[6]= END_CHAR;
				  HAL_UART_Transmit(&huart5, my_disso_var.tx_com_buff, 7, 10);



            	 my_disso_flags.pump_auto_samling = 0;
            	 my_disso_flags.pump_auto_mode = 0;
            }
            /*************************************************************************************************/
            // filling in buffer mode
		    else if(my_disso_flags.pump_fill == 1)
            {

            	 my_disso_flags.pump_auto_mode = 1;
            	 if(my_disso_var.recv_Replenish_status == 1)
            	 {

            		 my_disso_var.get_dispense_volume = (my_disso_var.recv_Dead_repv * 10)+6;
            		 my_disso_flags.PRIME = 1;
            		 my_disso_flags.FLUSH1 = 1;
            		 AL_PP_Dispense_Volume(PP_Reversed,my_disso_var.get_dispense_volume );
            		 my_disso_flags.FLUSH1 = 0;
            		 my_disso_var.get_dispense_volume = 0;
            	 }


           	      my_disso_var.tx_com_buff[0]= START_CHAR;
				  my_disso_var.tx_com_buff[1]= 'P';
				  my_disso_var.tx_com_buff[2]= 'P';
				  my_disso_var.tx_com_buff[3]= 'F';
				  my_disso_var.tx_com_buff[4]= 'I';
				  my_disso_var.tx_com_buff[5]= 'L';
				  my_disso_var.tx_com_buff[6]= END_CHAR;
				  HAL_UART_Transmit(&huart5, my_disso_var.tx_com_buff, 7, 10);


				 // check run in progress and set status
				  if(my_disso_var.run_interrupted == 1)
				  {
					  my_disso_var.run_interrupted = 0;
					  // store the run
				  }
				  my_disso_var.run_in_progress = 1;

            	 my_disso_flags.pump_fill = 0;
            	 my_disso_flags.pump_auto_mode = 0;
            }
            /*************************************************************************************************/
           // read parameters and handle uninterrupted commands
		    else if(my_disso_flags.pump_param_status == 1)
           {
        	      my_disso_flags.pump_auto_mode = 1;



				  AL_PP_Disso_Parameters_func();
				  // read data from disso
				  my_disso_var.recv_flowrate = AL_EEPROM_Read_NUM(1, 0);
				  HAL_Delay(10);
				  my_disso_var.recv_Dead_prev = AL_EEPROM_Read_NUM(1, 4);
				  HAL_Delay(10);
				  my_disso_var.recv_Dead_post = AL_EEPROM_Read_NUM(1, 8);
				  HAL_Delay(10);
				  my_disso_var.recv_sample_volume = AL_EEPROM_Read_NUM(2, 0);
				  HAL_Delay(10);
				  my_disso_var.recv_rinse_volume = AL_EEPROM_Read_NUM(2, 4);
				  HAL_Delay(10);

				  my_disso_var.recv_Dead_repv = AL_EEPROM_Read_NUM(4, 0);
				  HAL_Delay(10);
				  my_disso_var.recv_filter_change = AL_EEPROM_Read_NUM(4, 4);
				  HAL_Delay(10);
				  my_disso_var.recv_intervals = AL_EEPROM_Read_NUM(4, 8);
				  HAL_Delay(10);
				  my_disso_var.recv_Replenish_status = AL_EEPROM_Read_NUM(4, 12);
				  HAL_Delay(10);
				  my_disso_var.recv_fc_status = AL_EEPROM_Read_NUM(4, 16);
				  HAL_Delay(10);
				  my_disso_var.recv_change_buff_status = AL_EEPROM_Read_NUM(4, 20);
				  HAL_Delay(10);


				  AL_PP_Clear_Com_Buffer();



				  // write parameters to display address

				  AL_DWIN_Write_VP(0x1000 , (uint16_t) ( my_disso_var.recv_flowrate *10));
				  AL_DWIN_Write_VP(0x1004 , (uint16_t) ( my_disso_var.recv_Dead_prev *10));
				  AL_DWIN_Write_VP(0x1008 , (uint16_t) ( my_disso_var.recv_Dead_post *10));

				  AL_DWIN_Write_VP(0x1012 , (uint16_t) ( my_disso_var.recv_sample_volume *10));
				  AL_DWIN_Write_VP(0x1016 , (uint16_t) ( my_disso_var.recv_rinse_volume *10));

                  // check replenish status 1=yes 2=No
				  if((( my_disso_var.recv_Replenish_status == 2) || (my_disso_flags.VALS == 1))
					  && (my_disso_flags.CLEAN == 0) &&  (my_disso_flags.VALR == 0))
				  {
					    if(my_disso_flags.VALS == 1)
					    {
					    	my_disso_var.get_dispense_volume = 50;
					    }
					    else
					    {
					    	my_disso_var.get_dispense_volume = 30;
					    }
					    my_disso_flags.PRIME = 1;
                        my_disso_flags.FLUSH1 = 1;
                        AL_PP_Dispense_Volume(PP_Forward, my_disso_var.get_dispense_volume);
                        my_disso_flags.FLUSH1 =0;
                        my_disso_var.get_dispense_volume = 0;

                          my_disso_var.tx_com_buff[0]= START_CHAR;
						  my_disso_var.tx_com_buff[1]= 'P';
						  my_disso_var.tx_com_buff[2]= 'P';
						  my_disso_var.tx_com_buff[3]= 'E';
						  my_disso_var.tx_com_buff[4]= 'N';
						  my_disso_var.tx_com_buff[5]= 'D';
						  my_disso_var.tx_com_buff[6]= END_CHAR;
						  HAL_UART_Transmit(&huart5, my_disso_var.tx_com_buff, 7, 10);


						AL_PP_Buzzer_check();
						AL_PP_Buzzer_check();



				  }//Fill the replenish tube till tip of sampling tubing.
				  if((( my_disso_var.recv_Replenish_status == 1) || (my_disso_flags.VALR == 1))
				  					  && (my_disso_flags.CLEAN == 0) &&  (my_disso_flags.VALS == 0))
				  {

					  if(my_disso_var.recv_Replenish_status == 1)
					  {
					      my_disso_var.get_dispense_volume = (my_disso_var.recv_Dead_repv*10)+ 6;
					  }
					  else
					  {
						  my_disso_var.get_dispense_volume = 50;
					  }
					  my_disso_flags.PRIME = 1;
					  my_disso_flags.FLUSH1 = 1;
					  AL_PP_Dispense_Volume(PP_Reversed, my_disso_var.get_dispense_volume);
					  my_disso_flags.FLUSH1 =0;


					  // check run in progress
                      if(my_disso_flags.VALR == 0)
                    	  my_disso_var.run_in_progress = 1;

					  my_disso_var.tx_com_buff[0]= START_CHAR;
					  my_disso_var.tx_com_buff[1]= 'P';
					  my_disso_var.tx_com_buff[2]= 'P';
					  my_disso_var.tx_com_buff[3]= 'E';
					  my_disso_var.tx_com_buff[4]= 'N';
					  my_disso_var.tx_com_buff[5]= 'D';
					  my_disso_var.tx_com_buff[6]= END_CHAR;
					  HAL_UART_Transmit(&huart5, my_disso_var.tx_com_buff, 7, 10);


					AL_PP_Buzzer_check();
					AL_PP_Buzzer_check();

				  }
				  my_disso_flags.pump_param_status = 0;
				  my_disso_flags.pump_auto_mode = 0;

           }
		   else
		   {
			   // run manual mode
			   	      if(my_disso_flags.pump_auto_mode == 0)
			   		  {
			   			  AL_PP_Sampling_Process();

			   			  AL_PP_Disp_Home_check();
			   		  }
		   }






}



void AL_PP_Disso_Parameters_func(void)
{
	        HAL_Delay(10);
	        AL_EEPROM_Write_NUM(1, 0, (float)(my_disso_var.recv_flowrate/10.0));
	        HAL_Delay(10);
			AL_EEPROM_Write_NUM(1, 4, (float)(my_disso_var.recv_Dead_prev/10.0));
			HAL_Delay(10);
			AL_EEPROM_Write_NUM(1, 8, (float)(my_disso_var.recv_Dead_post/10.0));
			HAL_Delay(10);
			AL_EEPROM_Write_NUM(2, 0, (float)(my_disso_var.recv_sample_volume/10.0));
			HAL_Delay(10);
			AL_EEPROM_Write_NUM(2, 4, (float)(my_disso_var.recv_rinse_volume/10.0));
			HAL_Delay(10);
			AL_EEPROM_Write_NUM(4, 0, (float)(my_disso_var.recv_Dead_repv/10.0) );
			HAL_Delay(10);
			AL_EEPROM_Write_NUM(4, 4, my_disso_var.recv_filter_change );
			HAL_Delay(10);
			AL_EEPROM_Write_NUM(4, 8, my_disso_var.recv_intervals );
			HAL_Delay(10);
			AL_EEPROM_Write_NUM(4, 12, my_disso_var.recv_Replenish_status );
			HAL_Delay(10);
			AL_EEPROM_Write_NUM(4, 16, my_disso_var.recv_fc_status );
			HAL_Delay(10);
			AL_EEPROM_Write_NUM(4, 20, my_disso_var.recv_change_buff_status);
			HAL_Delay(10);

}


void AL_PP_Replenish_Empty(void)
{
	AL_PP_Solonoid_valve_on();

	my_disso_var.get_dispense_volume = (DV_THRESHOLD*10)* STEPS_PER_REV;  // volume into steps

	AL_PP_ALL_Start_Motor(FLUSH1_RPM, PP_Forward);
	my_disso_var.track_steps = 0;
	//  random dispensing volume
	while(my_disso_var.track_steps < my_disso_var.get_dispense_volume)
	{
          /* nothing to do*/
	}
	my_disso_var.track_steps = 0;
	AL_PP_ALL_Stop_Motor();
	my_disso_var.get_dispense_volume = 0;

	AL_PP_Solonoid_valve_reset();


}

void AL_PP_Sampling_Empty(void)
{

	AL_PP_Solonoid_valve_reset();

	my_disso_var.get_dispense_volume =  (DV_THRESHOLD*10)* STEPS_PER_REV;  // volume into steps

	AL_PP_ALL_Start_Motor(FLUSH1_RPM, PP_Forward);
	my_disso_var.track_steps = 0;
	//  random dispensing volume
	while(my_disso_var.track_steps < my_disso_var.get_dispense_volume)
	{
		  /* nothing to do*/
	}
	my_disso_var.track_steps = 0;
	AL_PP_ALL_Stop_Motor();
	my_disso_var.get_dispense_volume = 0;

	AL_PP_Solonoid_valve_reset();



}


void AL_PP_Dispense_Volume(unsigned char direction,unsigned int volume)
{
    if(direction == 0)   //  forward
    {
    	AL_PP_Solonoid_valve_reset();
    	AL_PP_StepperMotor_Setdirection(PP_Forward);

    }
    else                   // reversed
    {
    	if( my_disso_flags.PRIME == 1)
    	{
    	  AL_PP_Solonoid_valve_on();
    	}
    	else
    	{
    		// one solonoid on
    	}
    	AL_PP_StepperMotor_Setdirection(PP_Reversed);
    }


    if( my_disso_flags.PRIME == 1)
    {
    	 AL_PP_Home_forward(1, HOME_RPM);
    	 AL_PP_Home_forward(3, HOME_RPM);
    	 AL_PP_Home_forward(5, HOME_RPM);
    	 AL_PP_Home_forward(2, HOME_RPM);
    	 AL_PP_Home_forward(4, HOME_RPM);
    	 AL_PP_Home_forward(6, HOME_RPM);

    }
    else
    {
    	// one home position
    }
    AL_PP_ALL_Motor_switch(PP_Disable);


    // check flowrate
    my_disso_var.load_recv_rpm = 0;
    my_disso_var.track_steps = 0;
    if(my_disso_flags.PRIME == 1)
	{
	   if(my_disso_flags.CLEAN == 1)
	   {
		 my_disso_var.load_recv_rpm = CLEAN_RPM;
		 AL_PP_ALL_Start_Motor(my_disso_var.load_recv_rpm, direction);
	   }
	   else if(my_disso_flags.FLUSH1 == 1)
	   {
		 my_disso_var.load_recv_rpm = FLUSH1_RPM;
		 AL_PP_ALL_Start_Motor(my_disso_var.load_recv_rpm, direction);
	   }
	   else if(my_disso_flags.FLUSH2 == 1)
	   {
	     my_disso_var.load_recv_rpm = FLUSH2_RPM;
	  	 AL_PP_ALL_Start_Motor(my_disso_var.load_recv_rpm, direction);
	   }
	   else if(my_disso_flags.FLUSH3 == 1)
	   {
		 my_disso_var.load_recv_rpm = FLUSH3_RPM;
		 AL_PP_ALL_Start_Motor(my_disso_var.load_recv_rpm, direction);
	   }
	   else
	   {
		 my_disso_var.load_recv_rpm = (uint32_t)(my_disso_var.recv_flowrate*10);
		 AL_PP_ALL_Start_Motor(my_disso_var.load_recv_rpm, direction);
	   }
	}
    while(my_disso_var.track_steps <= (volume*STEPS_PER_REV))
    {

    }
    my_disso_var.track_steps = 0;
    my_disso_var.load_recv_rpm = 0;

    AL_PP_ALL_Motor_switch(PP_Disable);
    AL_PP_Solonoid_valve_reset();

}

void AL_PP_Dead_Volume_Empty(unsigned char direction,unsigned int volume)
{

	AL_PP_Solonoid_valve_reset();

	AL_PP_Home_forward(1, HOME_RPM);
	AL_PP_Home_forward(3, HOME_RPM);
	AL_PP_Home_forward(5, HOME_RPM);
	AL_PP_Home_forward(2, HOME_RPM);
	AL_PP_Home_forward(4, HOME_RPM);
	AL_PP_Home_forward(6, HOME_RPM);



	if(my_disso_flags.CLEAN == 1)
	{
		 my_disso_var.load_recv_rpm = CLEAN_RPM;

	}
	else if(my_disso_flags.FLUSH1 == 1)
	{
		 my_disso_var.load_recv_rpm = FLUSH1_RPM;

	}
	else if(my_disso_flags.FLUSH2 == 1)
	{
		 my_disso_var.load_recv_rpm = FLUSH2_RPM;

	}
	else if(my_disso_flags.FLUSH3 == 1)
	{
		 my_disso_var.load_recv_rpm = FLUSH3_RPM;

	}
	else
	{
	  my_disso_var.load_recv_rpm =  (uint32_t)(my_disso_var.recv_flowrate*10);
	  HAL_Delay(10);

	}
	my_disso_var.get_dispense_volume = volume * STEPS_PER_REV;
	AL_PP_ALL_Stop_Motor();

	AL_PP_ALL_Start_Motor(my_disso_var.load_recv_rpm, direction);
	my_disso_var.track_steps = 0;

	while(my_disso_var.track_steps <= my_disso_var.get_dispense_volume)
	{
		// nothing to do
	}

	my_disso_var.track_steps = 0;
	my_disso_var.load_recv_rpm = 0;
	my_disso_var.get_dispense_volume = 0;

	AL_PP_Solonoid_valve_reset();
	AL_PP_ALL_Stop_Motor();

}

/**************** EOF **************************************/







