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

// PP STH17
#define DPR    DISPENSE_PER_REV      // ml/rev = displacement per revolution
/********** variable declarations*******************/
            eeprom_HandleTypeDef   my_eeprom_var;
            Display_HandleTypeDef  my_disp_var;
            Display_flags_HandleTypeDef   my_disp_flags;
extern      stepperHandletypeDef       my_stepper_var;
extern      stepperFlagsHandletypeDef  my_stepflags_var;
extern unsigned char pp_version[6];
extern unsigned char ppe_version[6];
extern unsigned char hmi_version[4];
extern      DwinFlagHandleTypeDef  my_dwin_flags;


extern enum ScreenHandleTypeDef    my_screen_num;
extern      DwinHandleTypeDef      my_dwin_var;

extern      TIM_HandleTypeDef htim1;
uint32_t   saved_rpm= 0;

uint32_t   stamp_opto_count = 0;

/********** USER variable declarations ********************/











/********************USER FUNCTION BODY**********************************/

void AL_PP_Disp_logo(void)
{
	  for(unsigned int scr_no = 0; scr_no <= (my_screen_num = main_logo9); scr_no++ )
	  {
			AL_DWIN_Picshow(scr_no);
			HAL_Delay(100);
	  }
	  HAL_Delay(500);


}


void AL_PP_Disp_Home_check(void)
{


	 switch(my_dwin_var.touch_key)
	 {

	    case  pg_settings:
                           my_dwin_flags.settings_flag = 1;
                           my_dwin_flags.prime_flag = 0;
                           my_dwin_flags.pcheck_flag = 0;
                           my_dwin_flags.emode_flag = 0;
                           my_dwin_flags.about_flag = 0;
                           AL_DWIN_Toggle(10);
						   break;


	    case  pg_prime:
	    	               my_dwin_flags.settings_flag = 0;
						   my_dwin_flags.prime_flag = 1;
						   my_dwin_flags.pcheck_flag = 0;
						   my_dwin_flags.emode_flag = 0;
						   my_dwin_flags.about_flag = 0;
	    	    	       AL_DWIN_Toggle(10);
	    				   break;


	    case  pg_pcheck:
	    	               my_dwin_flags.settings_flag = 0;
						   my_dwin_flags.prime_flag = 0;
						   my_dwin_flags.pcheck_flag = 1;
						   my_dwin_flags.emode_flag = 0;
						   my_dwin_flags.about_flag = 0;
	    	    	       AL_DWIN_Toggle(10);
	    				   break;


	    case  pg_Emode:
	    	               my_dwin_flags.settings_flag = 0;
						   my_dwin_flags.prime_flag = 0;
						   my_dwin_flags.pcheck_flag = 0;
						   my_dwin_flags.emode_flag = 1;
						   my_dwin_flags.about_flag = 0;
	    	               AL_DWIN_Clear_Text(0x8010,6);
	    	               my_dwin_var.chk_password_length = 0;
						   AL_DWIN_Toggle(10);
						   break;


	    case  pg_about:
	    	               my_dwin_flags.settings_flag = 0;
						   my_dwin_flags.prime_flag = 0;
						   my_dwin_flags.pcheck_flag = 0;
						   my_dwin_flags.emode_flag = 0;
						   my_dwin_flags.about_flag = 1;
						   AL_DWIN_Toggle(10);
						   break;


	    default:


		                   break;

	 }
	 AL_PP_Disp_Home_Settings_check();
	 AL_PP_Disp_Home_Engmode_check();
	 AL_PP_Disp_Home_Prime_check();
	 AL_PP_Disp_Home_Pump_check();
	 AL_PP_Disp_Home_about_check();

}

void AL_PP_Disp_Home_Settings_check(void)
{
     if( my_dwin_flags.settings_flag == 1)
     {
           switch( my_dwin_var.saved_vp )
           {
                 // read flowrate data (ml/min)
                 case 0x1000:

                	          my_dwin_var.temp_flowrate = ((float)(my_dwin_var.Main_buff[8]))/10;
                	          AL_EEPROM_Write_NUM(1, 0, my_dwin_var.temp_flowrate );
                	          HAL_Delay(10);
                	          my_dwin_var.saved_vp = 0;
                	          break;


                 // read dead volume prev valve
                 case 0x1004:
                	          my_dwin_var.temp_dead_pre_valve = ((float)(my_dwin_var.Main_buff[8]))/10;
                	          AL_EEPROM_Write_NUM(1, 4, my_dwin_var.temp_dead_pre_valve );
                	          HAL_Delay(10);
                              my_dwin_var.saved_vp = 0;
                              break;


                 // read dead volume post valve
                 case 0x1008:
                	          my_dwin_var.temp_dead_post_valve = ((float)(my_dwin_var.Main_buff[8]))/10;
                	          AL_EEPROM_Write_NUM(1, 8 , my_dwin_var.temp_dead_post_valve );
                	          HAL_Delay(10);
                	          my_dwin_var.saved_vp = 0;
                	          break;


                 default:

                	        break;
           }





     }
}



void AL_PP_Disp_Home_Engmode_check(void)
{
     if( my_dwin_flags.emode_flag == 1)
     {
    	   switch( my_dwin_var.saved_vp )
    	   {


    	         case 0x8000:
    	        	          // storage data
    	        	          for(unsigned char j= 0;j < PASSWORD_SIZE;j++)
    	        	          {
    	        	        	  my_dwin_var.store_passwd[j] = my_dwin_var.Main_buff[7+j];

							  }
    	        	          // check length
    	        	          if((my_dwin_var.store_passwd[0] == 0xFF) &&
    	        	             (my_dwin_var.store_passwd[1] == 0xFF))
    	        	          {
    	        	        	  my_dwin_var.chk_password_length = 0;
    	        	        	  // clear buffer
    	        	        	  for(unsigned char j= 0;j < PASSWORD_SIZE;j++)
    	        	        	  {
    	        	        	    my_dwin_var.store_passwd[j] = 0x00;

    	        	              }
    	        	          }
    	        	          else
    	        	          {
    	        	        	  // otherwise find length of password
								  for(unsigned char i= 1; i <= PASSWORD_SIZE;i++)
								  {
									  if(my_dwin_var.store_passwd[i-1] == 0xFF)
									  {
										  break;
									  }
									  my_dwin_var.chk_password_length = i;
								  }
    	        	          }

                              AL_DWIN_Start();
                              AL_DWIN_Send_Data(5 + my_dwin_var.chk_password_length);
                              AL_DWIN_Send_Data(DWIN_WRITE);
                              AL_DWIN_Send_Data(0x80);
                              AL_DWIN_Send_Data(0x10);
                              switch( my_dwin_var.chk_password_length)
                              {
                                  case 1 :
                                	       AL_DWIN_Send_Data('*');
                                	       AL_DWIN_End();
                                	       break;

                                  case 2 :
										   AL_DWIN_Send_Data('*');
										   AL_DWIN_Send_Data('*');
										   AL_DWIN_End();
										   break;

                                  case 3 :
										   AL_DWIN_Send_Data('*');
										   AL_DWIN_Send_Data('*');
										   AL_DWIN_Send_Data('*');
										   AL_DWIN_End();
										   break;


                                  case 4 :
										   AL_DWIN_Send_Data('*');
										   AL_DWIN_Send_Data('*');
										   AL_DWIN_Send_Data('*');
										   AL_DWIN_Send_Data('*');

										   AL_DWIN_End();
										   break;

                                  case 5 :
										   AL_DWIN_Send_Data('*');
										   AL_DWIN_Send_Data('*');
										   AL_DWIN_Send_Data('*');
										   AL_DWIN_Send_Data('*');
										   AL_DWIN_Send_Data('*');
										   AL_DWIN_End();
										   break;

                                  case 6 :
										   AL_DWIN_Send_Data('*');
										   AL_DWIN_Send_Data('*');
										   AL_DWIN_Send_Data('*');
										   AL_DWIN_Send_Data('*');
										   AL_DWIN_Send_Data('*');
										   AL_DWIN_Send_Data('*');
										   AL_DWIN_End();
										   break;

                                  default :
                                	       AL_DWIN_Send_Data(' ');
										   AL_DWIN_Send_Data(' ');
										   AL_DWIN_Send_Data(' ');
										   AL_DWIN_Send_Data(' ');
										   AL_DWIN_Send_Data(' ');
										   AL_DWIN_Send_Data(' ');
										   AL_DWIN_End();
                                	       break;

                              }



                              my_dwin_var.saved_vp = 0;

				              break;


				 case  0x1024:

					          my_dwin_var.check_sel_pump = (uint8_t)my_dwin_var.Main_buff[8];

					          AL_EEPROM_Page_Write(3, 0, &my_dwin_var.check_sel_pump ,1);
					          HAL_Delay(10);
					          my_dwin_var.saved_vp = 0;
					          break;

    	         default:

    	                      break;
    	   }

           switch(my_dwin_var.touch_key)
           {
					   case  pg_login :
									   // password size checking
										 if(my_dwin_var.chk_password_length == 6)
										 {
										   for(unsigned int i= 0; i < my_dwin_var.chk_password_length;i++)
										   {
											   if( my_dwin_var.store_passwd[i] != my_dwin_var.temp_passwd[i])
											   {
												   my_dwin_flags.check_pw_flag = 0;
												   break;
											   }
											   else
											   {
												   my_dwin_flags.check_pw_flag = 1;
											   }
										   }

										   if(my_dwin_flags.check_pw_flag == CORRECT_PASSWORD)
										   {
											   // change engg. settings screen
											     if(my_dwin_var.check_sel_pump == 6)
											     {
											         AL_DWIN_Picshow(85);
											     }
											     else if(my_dwin_var.check_sel_pump == 8)
												 {
													 AL_DWIN_Picshow(86);
												 }
											     else if(my_dwin_var.check_sel_pump == 12)
												 {
													 AL_DWIN_Picshow(87);
												 }
											     else if(my_dwin_var.check_sel_pump == 14)
												 {
													 AL_DWIN_Picshow(88);
												 }
											     else
											     {
											    	 AL_DWIN_Picshow(85);
											     }

											     AL_DWIN_Start();
												 AL_DWIN_Send_Data(5+6);
												 AL_DWIN_Send_Data(DWIN_WRITE);
												 AL_DWIN_Send_Data((0x7520 & 0xFF00)>>8);
												 AL_DWIN_Send_Data(0x7520 & 0x00FF);
												 AL_DWIN_Send_Data(ppe_version[0]);
												 AL_DWIN_Send_Data(ppe_version[1]);
												 AL_DWIN_Send_Data(ppe_version[2]);
												 AL_DWIN_Send_Data(ppe_version[3]);
												 AL_DWIN_Send_Data(ppe_version[4]);
												 AL_DWIN_Send_Data(ppe_version[5]);
												 AL_DWIN_End();

												 HAL_Delay(2);


										   }
										   else
										   {
											   AL_DWIN_Picshow(150);    // incorrect passowrd screen
											   HAL_Delay(2000);         // display for 2 sec
											   AL_DWIN_Picshow(81);
										   }

										 }
										 else
										 {
										   AL_DWIN_Picshow(150);
										   HAL_Delay(2000);         // wait for 2 sec
										   AL_DWIN_Picshow(81);
										 }

										 AL_DWIN_Toggle(10);
									   break;


					   case 0x1081 :   my_dwin_var.check_sel_pump = 6;

				                       AL_EEPROM_Page_Write(3, 0, &my_dwin_var.check_sel_pump ,1);
				                       HAL_Delay(10);
				                       AL_DWIN_Toggle(10);
						               break;


					   case 0x1082 :   my_dwin_var.check_sel_pump = 8;

									   AL_EEPROM_Page_Write(3, 0, &my_dwin_var.check_sel_pump ,1);
									   HAL_Delay(10);
									   AL_DWIN_Toggle(10);
						               break;

					   case 0x1083 :   my_dwin_var.check_sel_pump = 12;

									   AL_EEPROM_Page_Write(3, 0, &my_dwin_var.check_sel_pump ,1);
									   HAL_Delay(10);
									   AL_DWIN_Toggle(10);
									   break;

					   case 0x1084 :   my_dwin_var.check_sel_pump = 14;

									   AL_EEPROM_Page_Write(3, 0, &my_dwin_var.check_sel_pump ,1);
									   HAL_Delay(10);
									   AL_DWIN_Toggle(10);
									   break;


					   default:
						               AL_DWIN_Toggle(10);
									   break;
		      }

     }

}


void AL_PP_Disp_Home_Prime_check(void)
{
     if ( my_dwin_flags.prime_flag == 1)
     {
    	           switch(my_dwin_var.touch_key)
    	           {



    						   case  0x1095 :

											   switch( my_dwin_var.check_sel_pump)
											   {

												  case   6:
												             AL_DWIN_Picshow(31);
															 break;

												  case   8:
													         AL_DWIN_Picshow(38);
															 break;

												  case   12:
													         AL_DWIN_Picshow(36);
															 break;

												  case   14:
													         AL_DWIN_Picshow(36);
												 	         break;
												  default :


															 break;

											   }
											   AL_DWIN_Toggle(10);
											   break;

                               case  0x1101 :

											   switch( my_dwin_var.check_sel_pump)
											   {

											      case   12:
															 AL_DWIN_Picshow(37);
															 break;

												  case   14:
															 AL_DWIN_Picshow(39);
															 break;
												  default :


															 break;

											   }
											   AL_DWIN_Toggle(10);
											   break;

                               // sel pumps
							   case  0x1020 :
                                               my_dwin_var.pump_num = 1;
								               AL_DWIN_Text_Show(0x7500,'0','0' ,'1');
								               AL_DWIN_Toggle(10);
								               break;


							   case  0x1021 :
											   my_dwin_var.pump_num = 2;
											   AL_DWIN_Text_Show(0x7500,'0','0' ,'2');
											   AL_DWIN_Toggle(10);
											   break;


							   case  0x1022 :
											   my_dwin_var.pump_num = 3;
											   AL_DWIN_Text_Show(0x7500,'0','0' ,'3');
											   AL_DWIN_Toggle(10);
											   break;

							   case  0x1023 :
											   my_dwin_var.pump_num = 4;
											   AL_DWIN_Text_Show(0x7500,'0','0' ,'4');
											   AL_DWIN_Toggle(10);
											   break;

							   case  0x1024 :
											   my_dwin_var.pump_num = 5;
											   AL_DWIN_Text_Show(0x7500,'0','0' ,'5');
											   AL_DWIN_Toggle(10);
											   break;

							   case  0x1025 :
											   my_dwin_var.pump_num = 6;
											   AL_DWIN_Text_Show(0x7500,'0','0' ,'6');
											   AL_DWIN_Toggle(10);
											   break;

							   case  0x1026 :
											   my_dwin_var.pump_num = 7;
											   AL_DWIN_Text_Show(0x7500,'0','0' ,'7');
											   AL_DWIN_Toggle(10);
											   break;

							   case  0x1027 :
											   my_dwin_var.pump_num = 8;
											   AL_DWIN_Text_Show(0x7500,'0','0' ,'8');
											   AL_DWIN_Toggle(10);
											   break;


							   case  0x1028 :
											   my_dwin_var.pump_num = 9;
											   AL_DWIN_Text_Show(0x7500,'0','0' ,'9');
											   AL_DWIN_Toggle(10);
											   break;

							   case  0x1029 :
											   my_dwin_var.pump_num = 10;
											   AL_DWIN_Text_Show(0x7500,'0','1' ,'0');
											   AL_DWIN_Toggle(10);
											   break;

							   case  0x1030 :
											   my_dwin_var.pump_num = 11;
											   AL_DWIN_Text_Show(0x7500,'0','1' ,'1');
											   AL_DWIN_Toggle(10);
											   break;


							   case  0x1031 :
											   my_dwin_var.pump_num = 12;
											   AL_DWIN_Text_Show(0x7500,'0','1' ,'2');
											   AL_DWIN_Toggle(10);
											   break;

							   case  0x1032 :
											   my_dwin_var.pump_num = 13;
											   AL_DWIN_Text_Show(0x7500,'0','1' ,'3');
											   AL_DWIN_Toggle(10);
											   break;

							   case  0x1033 :
											   my_dwin_var.pump_num = 14;
											   AL_DWIN_Text_Show(0x7500,'0','1' ,'4');
											   AL_DWIN_Toggle(10);
											   break;

							   case  0x1034 :
											   my_dwin_var.pump_num = 15;
											   AL_DWIN_Text_Show(0x7500,'S','T' ,'A');
											   AL_DWIN_Toggle(10);
											   break;

							   case  0x1035 :
											   my_dwin_var.pump_num = 16;
											   AL_DWIN_Text_Show(0x7500,'S','T' ,'B');
											   AL_DWIN_Toggle(10);
											   break;


							   case  0x1036 :

								               if(my_dwin_var.check_sel_pump == 6)
								               {
											     my_dwin_var.pump_num = CHANNEL_06;
											     AL_DWIN_Text_Show(0x7500,'A','L' ,'L');
								               }
								               else if(my_dwin_var.check_sel_pump == 8)
											   {
												 my_dwin_var.pump_num = CHANNEL_08;
												 AL_DWIN_Text_Show(0x7500,'A','L' ,'L');
											   }
								               else if(my_dwin_var.check_sel_pump == 12)
											   {
												 my_dwin_var.pump_num = CHANNEL_12;
												 AL_DWIN_Text_Show(0x7500,'A','L' ,'L');
											   }
								               else if(my_dwin_var.check_sel_pump == 14)
								               {
								            	 my_dwin_var.pump_num = CHANNEL_14;
								            	 AL_DWIN_Text_Show(0x7500,'A','L' ,'L');
								               }

											   AL_DWIN_Toggle(10);
											   break;




					           case  0x1040 :

					        	              // my_dwin_flags.sample_path_flag = 1;
					        	               //my_dwin_flags.replenish_path_flag = 0;

					        	               AL_DWIN_Toggle(10);
					        	               break;


					           case  0x1041 :

											   //my_dwin_flags.replenish_path_flag = 1;
											   //my_dwin_flags.sample_path_flag = 0;
											   AL_DWIN_Toggle(10);
											   break;


					           case  0x2000 :

											  // check rinse volume to calculate sample volume actual
											   if(((uint32_t) (my_dwin_var.temp_rinse_volume * 10)) != 0 )
											   {

												  my_dwin_flags.rinse_flag = 1;
												  my_dwin_var.chk_diff_sample = my_dwin_var.temp_sample_volume - my_dwin_var.temp_rinse_volume;

												  my_disp_var.rinsing_time = (uint32_t)((my_dwin_var.temp_rinse_volume / my_dwin_var.temp_flowrate)*60);
												  my_disp_var.rinsing_rpm = (uint32_t) (AL_PP_roundoff(((double)my_dwin_var.temp_flowrate/ (double)DPR)))/100;
												  my_disp_var.sampling_rpm = my_disp_var.rinsing_rpm;
												  my_disp_var.sampling_time =  (uint32_t)((my_dwin_var.chk_diff_sample / my_dwin_var.temp_flowrate)*60);
                                                  my_disp_var.get_opto_scount = (my_disp_var.sampling_rpm * my_disp_var.sampling_time)/60;
                                                  my_disp_var.get_opto_rcount = (my_disp_var.rinsing_rpm * my_disp_var.rinsing_time)/60;
											   }
											   else
											   {

												  my_dwin_flags.rinse_flag = 0;
												  my_dwin_var.chk_diff_sample = my_dwin_var.temp_sample_volume;
												  my_disp_var.rinsing_time = 0;
												  my_disp_var.rinsing_rpm = 0;
												  my_disp_var.sampling_rpm = (uint32_t) (AL_PP_roundoff(((double)my_dwin_var.temp_flowrate/ (double)DPR)))/100;
												  my_disp_var.sampling_time =  (uint32_t)((my_dwin_var.chk_diff_sample / my_dwin_var.temp_flowrate)*60);
												  my_disp_var.get_opto_scount = (my_disp_var.sampling_rpm * my_disp_var.sampling_time)/60;
												  my_disp_var.get_opto_rcount = 0;

											   }

											   switch(my_dwin_var.pump_num)
											   {

												 case  1:

														 if(my_dwin_flags.rinse_flag  == 1)
														 {

															AL_DWIN_Picshow(111);  // rinsing
															my_disp_var.sampling_channel = PP_Step_01;
                                                            my_disp_flags.rinsing_run_flag = 1;
                                                            my_disp_var.prime_count = 0;
                                                            my_disp_var.ramp_count = 0;
														 }
														 else
														 {
															 AL_DWIN_Picshow(111);  // rinsing
															 HAL_Delay(1);
															 AL_DWIN_Picshow(112);  // sampling cmd
														 }
														 break;



												  case  3:

														 if(my_dwin_flags.rinse_flag  == 1)
														 {

																AL_DWIN_Picshow(111);  // rinsing
																my_disp_var.sampling_channel = PP_Step_02;
	                                                            my_disp_flags.rinsing_run_flag = 1;
	                                                            my_disp_var.prime_count = 0;
	                                                            my_disp_var.ramp_count = 0;

														 }
														 else
														 {
															 AL_DWIN_Picshow(111);  // rinsing
															 HAL_Delay(1);
															 AL_DWIN_Picshow(112);  // sampling cmd
														 }
														 break;

												  case  5:

														 if(my_dwin_flags.rinse_flag  == 1)
														 {

																AL_DWIN_Picshow(111);  // rinsing
																my_disp_var.sampling_channel = PP_Step_03;
	                                                            my_disp_flags.rinsing_run_flag = 1;
	                                                            my_disp_var.prime_count = 0;
	                                                            my_disp_var.ramp_count = 0;

														 }
														 else
														 {
															 AL_DWIN_Picshow(111);  // rinsing
															 HAL_Delay(1);
															 AL_DWIN_Picshow(112);  // sampling cmd
														 }
														 break;




												   case  2:

														 if(my_dwin_flags.rinse_flag  == 1)
														 {

																AL_DWIN_Picshow(111);  // rinsing
																my_disp_var.sampling_channel = PP_Step_05;
	                                                            my_disp_flags.rinsing_run_flag = 1;
	                                                            my_disp_var.prime_count = 0;
	                                                            my_disp_var.ramp_count = 0;

														 }
														 else
														 {
															 AL_DWIN_Picshow(111);  // rinsing
															 HAL_Delay(1);
															 AL_DWIN_Picshow(112);  // sampling cmd
														 }
														 break;




												   case  4:

														 if(my_dwin_flags.rinse_flag  == 1)
														 {

																AL_DWIN_Picshow(111);  // rinsing
																my_disp_var.sampling_channel = PP_Step_06;
	                                                            my_disp_flags.rinsing_run_flag = 1;
	                                                            my_disp_var.prime_count = 0;
	                                                            my_disp_var.ramp_count = 0;

														 }
														 else
														 {
															 AL_DWIN_Picshow(111);  // rinsing
															 HAL_Delay(1);
															 AL_DWIN_Picshow(112);  // sampling cmd
														 }
														 break;


												   case  6:

														 if(my_dwin_flags.rinse_flag  == 1)
														 {

																AL_DWIN_Picshow(111);  // rinsing
																my_disp_var.sampling_channel = PP_Step_07;
	                                                            my_disp_flags.rinsing_run_flag = 1;
	                                                            my_disp_var.prime_count = 0;
	                                                            my_disp_var.ramp_count = 0;

														 }
														 else
														 {
															 AL_DWIN_Picshow(111);  // rinsing
															 HAL_Delay(1);
															 AL_DWIN_Picshow(112);  // sampling cmd
														 }
														 break;

												  case CHANNEL_06:

														 if(my_dwin_flags.rinse_flag  == 1)
														 {

																AL_DWIN_Picshow(111);  // rinsing
																my_disp_var.sampling_channel = CHANNEL_06;
	                                                            my_disp_flags.rinsing_run_flag = 1;
	                                                            my_disp_var.prime_count = 0;
	                                                            my_disp_var.ramp_count = 0;

														 }
														 else
														 {
															 AL_DWIN_Picshow(111);  // rinsing
															 HAL_Delay(1);
															 AL_DWIN_Picshow(112);  // sampling cmd
														 }
														 break;


												   case CHANNEL_12:

													     if(my_dwin_flags.rinse_flag  == 1)
														 {

																AL_DWIN_Picshow(111);  // rinsing
																my_disp_var.sampling_channel = CHANNEL_12;
																my_disp_flags.rinsing_run_flag = 1;
																my_disp_var.prime_count = 0;
																my_disp_var.ramp_count = 0;

														 }
														 else
														 {
															 AL_DWIN_Picshow(111);  // rinsing
															 HAL_Delay(1);
															 AL_DWIN_Picshow(112);  // sampling cmd
														 }

														 break;

												   default:

												         break;


											   }


                                               AL_DWIN_Toggle(10);
					        	               break;

					           case  0x1042 :

					        	               switch(my_dwin_var.pump_num)
											   {

												 case  1:

														 if(my_dwin_flags.rinse_flag  == 1)
														 {

															my_disp_var.sampling_channel = PP_Step_01;
															my_disp_flags.sampling_run_flag = 1;
															my_disp_var.prime_count = 0;
															my_disp_var.ramp_count = 0;


														 }
														 else
														 {

															my_disp_var.sampling_channel = PP_Step_01;
															my_disp_flags.sampling_run_flag = 1;
															my_disp_var.prime_count = 0;
															my_disp_var.ramp_count = 0;
														 }
														 break;

												 case  3:
													    if(my_dwin_flags.rinse_flag  == 1)
														 {

															my_disp_var.sampling_channel = PP_Step_02;
															my_disp_flags.sampling_run_flag = 1;
															my_disp_var.prime_count = 0;
															my_disp_var.ramp_count = 0;


														 }
														 else
														 {

															my_disp_var.sampling_channel = PP_Step_02;
															my_disp_flags.sampling_run_flag = 1;
															my_disp_var.prime_count = 0;
															my_disp_var.ramp_count = 0;
														 }


														 break;

												 case  5:
													     if(my_dwin_flags.rinse_flag  == 1)
														 {

															my_disp_var.sampling_channel = PP_Step_03;
															my_disp_flags.sampling_run_flag = 1;
															my_disp_var.prime_count = 0;
															my_disp_var.ramp_count = 0;


														 }
														 else
														 {

															my_disp_var.sampling_channel = PP_Step_03;
															my_disp_flags.sampling_run_flag = 1;
															my_disp_var.prime_count = 0;
															my_disp_var.ramp_count = 0;
														 }


														 break;


												 case  2:
													    if(my_dwin_flags.rinse_flag  == 1)
														 {

															my_disp_var.sampling_channel = PP_Step_05;
															my_disp_flags.sampling_run_flag = 1;
															my_disp_var.prime_count = 0;
															my_disp_var.ramp_count = 0;


														 }
														 else
														 {

															my_disp_var.sampling_channel = PP_Step_05;
															my_disp_flags.sampling_run_flag = 1;
															my_disp_var.prime_count = 0;
															my_disp_var.ramp_count = 0;
														 }


														 break;

												 case  4:
													    if(my_dwin_flags.rinse_flag  == 1)
														 {

															my_disp_var.sampling_channel = PP_Step_06;
															my_disp_flags.sampling_run_flag = 1;
															my_disp_var.prime_count = 0;
															my_disp_var.ramp_count = 0;


														 }
														 else
														 {

															my_disp_var.sampling_channel = PP_Step_06;
															my_disp_flags.sampling_run_flag = 1;
															my_disp_var.prime_count = 0;
															my_disp_var.ramp_count = 0;
														 }


														 break;

												 case  6:
													    if(my_dwin_flags.rinse_flag  == 1)
														 {

															my_disp_var.sampling_channel = PP_Step_07;
															my_disp_flags.sampling_run_flag = 1;
															my_disp_var.prime_count = 0;
															my_disp_var.ramp_count = 0;


														 }
														 else
														 {

															my_disp_var.sampling_channel = PP_Step_07;
															my_disp_flags.sampling_run_flag = 1;
															my_disp_var.prime_count = 0;
															my_disp_var.ramp_count = 0;
														 }


														 break;


												 case CHANNEL_06:
													    if(my_dwin_flags.rinse_flag  == 1)
														 {

															my_disp_var.sampling_channel = CHANNEL_06;
															my_disp_flags.sampling_run_flag = 1;
															my_disp_var.prime_count = 0;
															my_disp_var.ramp_count = 0;


														 }
														 else
														 {

															my_disp_var.sampling_channel = CHANNEL_06;
															my_disp_flags.sampling_run_flag = 1;
															my_disp_var.prime_count = 0;
															my_disp_var.ramp_count = 0;
														 }


													     break;


												 case CHANNEL_12:
													     if(my_dwin_flags.rinse_flag  == 1)
														 {

															my_disp_var.sampling_channel = CHANNEL_12;
															my_disp_flags.sampling_run_flag = 1;
															my_disp_var.prime_count = 0;
															my_disp_var.ramp_count = 0;


														 }
														 else
														 {

															my_disp_var.sampling_channel = CHANNEL_12;
															my_disp_flags.sampling_run_flag = 1;
															my_disp_var.prime_count = 0;
															my_disp_var.ramp_count = 0;
														 }

													      break;



											   }
					        	               AL_DWIN_Toggle(10);
					        	               break;



					           case  0x2001 :

											  my_disp_var.replenish_rpm = (uint32_t) AL_PP_roundoff(((double)my_dwin_var.temp_flowrate/ (double)DPR))/100;
											  my_disp_var.replenish_time =  (uint32_t)((my_dwin_var.temp_replenish_volume  / my_dwin_var.temp_flowrate)*60);  // seconds

											  my_disp_var.get_opto_rpcount = (my_disp_var.replenish_rpm * my_disp_var.replenish_time)/60;


											   switch(my_dwin_var.pump_num)
											   {

												 case  1:
														   AL_DWIN_Picshow(115); // replenishing
														   my_disp_var.sampling_channel = PP_Step_01;
														   my_disp_flags.replenish_run_flag = 1;
														   my_disp_var.prime_count = 0;
														   my_disp_var.ramp_count = 0;
														    HAL_GPIO_WritePin(GPIOE, SOL_1, GPIO_PIN_SET);


														 break;



												  case  3:

													       AL_DWIN_Picshow(115); // replenishing
														   my_disp_var.sampling_channel = PP_Step_02;
														   my_disp_flags.replenish_run_flag = 1;
														   my_disp_var.prime_count = 0;
														   my_disp_var.ramp_count = 0;
														   HAL_GPIO_WritePin(GPIOE, SOL_3, GPIO_PIN_SET);

														 break;

												  case  5:

													       AL_DWIN_Picshow(115); // replenishing
														   my_disp_var.sampling_channel = PP_Step_03;
														   my_disp_flags.replenish_run_flag = 1;
														   my_disp_var.prime_count = 0;
														   my_disp_var.ramp_count = 0;
														   HAL_GPIO_WritePin(GPIOE, SOL_5, GPIO_PIN_SET);

														 break;




												   case  2:

													       AL_DWIN_Picshow(115); // replenishing
														   my_disp_var.sampling_channel = PP_Step_05;
														   my_disp_flags.replenish_run_flag = 1;
														   my_disp_var.prime_count = 0;
														   my_disp_var.ramp_count = 0;
														   HAL_GPIO_WritePin(GPIOE, SOL_2, GPIO_PIN_SET);

														 break;




												   case  4:

													       AL_DWIN_Picshow(115); // replenishing
														   my_disp_var.sampling_channel = PP_Step_06;
														   my_disp_flags.replenish_run_flag = 1;
														   my_disp_var.prime_count = 0;
														   my_disp_var.ramp_count = 0;
														   HAL_GPIO_WritePin(GPIOE, SOL_4, GPIO_PIN_SET);

														 break;


												   case  6:

													       AL_DWIN_Picshow(115); // replenishing
														   my_disp_var.sampling_channel = PP_Step_07;
														   my_disp_flags.replenish_run_flag = 1;
														   my_disp_var.prime_count = 0;
														   my_disp_var.ramp_count = 0;
														   HAL_GPIO_WritePin(GPIOE, SOL_6, GPIO_PIN_SET);

														 break;

													case CHANNEL_06:

														   AL_DWIN_Picshow(115); // replenishing
														   my_disp_var.sampling_channel = CHANNEL_06;
														   my_disp_flags.replenish_run_flag = 1;
														   my_disp_var.prime_count = 0;
														   my_disp_var.ramp_count = 0;
														   AL_PP_Solonoid_valve_on();
														 break;


												   case CHANNEL_12:

													       AL_DWIN_Picshow(115); // replenishing
														   my_disp_var.sampling_channel = CHANNEL_12;
														   my_disp_flags.replenish_run_flag = 1;
														   my_disp_var.prime_count = 0;
														   my_disp_var.ramp_count = 0;
														   AL_PP_Solonoid_valve_on();

														 break;

												   default:

														 break;


											   }

											   AL_DWIN_Toggle(10);
					        	               break;


					           case   0x1090 :
					        	                my_disp_flags.rinsing_run_flag = 0;
					        	                my_disp_flags.sampling_run_flag = 0;
					        	                my_disp_flags.replenish_run_flag = 0;
					        	                AL_PP_ALL_Stop_Motor();
					        	                AL_PP_Solonoid_valve_reset();
                                                 /*
					        	                my_stepflags_var.park_home_flag = 0;
					        	                AL_PP_Park();
					        	                my_stepflags_var.park_home_flag = 1;
                                                */
					        	                AL_DWIN_Toggle(10);

					        	                break;

    						   default:

    										   break;
    			      }

    	              switch( my_dwin_var.saved_vp )
    	              {
    	                           // read sample volume (ml)
    	                           case 0x1012:

    	                          	          my_dwin_var.temp_sample_volume = ((float)(my_dwin_var.Main_buff[8]))/10;
    	                          	          AL_EEPROM_Write_NUM(2, 0, my_dwin_var.temp_sample_volume );
    	                          	          HAL_Delay(10);

    	                          	          my_dwin_var.saved_vp = 0;
    	                          	          break;


    	                           // read rinse volume (ml)
    	                           case 0x1016:
    	                          	          my_dwin_var.temp_rinse_volume = ((float)(my_dwin_var.Main_buff[8]))/10;
    	                          	          AL_EEPROM_Write_NUM(2, 4, my_dwin_var.temp_rinse_volume );
    	                          	          HAL_Delay(10);

    	                          	          my_dwin_var.saved_vp = 0;
    	                                      break;


    	                           // read replenish volume (ml)
    	                           case 0x1020:
    	                          	          my_dwin_var.temp_replenish_volume = ((float)(my_dwin_var.Main_buff[8]))/10;
    	                          	          AL_EEPROM_Write_NUM(2, 8,  my_dwin_var.temp_replenish_volume );
    	                          	          HAL_Delay(10);

    	                          	          my_dwin_var.saved_vp = 0;
    	                          	          break;


    	                           default:

    	                          	        break;
    	               }
     }
}


void AL_PP_Disp_Home_Pump_check(void)
{
     if( my_dwin_flags.pcheck_flag == 1)
     {
    	               switch(my_dwin_var.touch_key)
    	    	       {



						   case  0x1099 :

										   switch( my_dwin_var.check_sel_pump)
										   {

											  case   6:

														 AL_DWIN_Picshow(62);
														 break;

											  case   8:

												         AL_DWIN_Picshow(66);
														 break;

											  case   12:

														 AL_DWIN_Picshow(63);
														 break;

											  case   14:

												         AL_DWIN_Picshow(63);
														 break;
											  default :

														 // by default sel channels = 8
														 break;

										   }
										   AL_DWIN_Toggle(10);
										   break;


						   case  0x1103 :

										   switch( my_dwin_var.check_sel_pump)
										   {

											  case   12:
														 AL_DWIN_Picshow(64);
														 break;

											  case   14:
														 AL_DWIN_Picshow(67);
														 break;
											  default :


														 break;

										   }
										   AL_DWIN_Toggle(10);
										   break;

						   // sel pumps
						   case  0x1051 :
										   my_dwin_var.pcheck_num = 1;
										   AL_DWIN_Text_Show(0x7503,'0','0' ,'1');
										   AL_DWIN_Toggle(10);
										   break;


						   case  0x1052 :
							               my_dwin_var.pcheck_num = 2;
										   AL_DWIN_Text_Show(0x7503,'0','0' ,'2');
										   AL_DWIN_Toggle(10);
										   break;


						   case  0x1053 :
							               my_dwin_var.pcheck_num = 3;
										   AL_DWIN_Text_Show(0x7503,'0','0' ,'3');
										   AL_DWIN_Toggle(10);
										   break;

						   case  0x1054 :
							               my_dwin_var.pcheck_num = 4;
										   AL_DWIN_Text_Show(0x7503,'0','0' ,'4');
										   AL_DWIN_Toggle(10);
										   break;

						   case  0x1055 :
							               my_dwin_var.pcheck_num = 5;
										   AL_DWIN_Text_Show(0x7503,'0','0' ,'5');
										   AL_DWIN_Toggle(10);
										   break;

						   case  0x1056 :
							               my_dwin_var.pcheck_num = 6;
										   AL_DWIN_Text_Show(0x7503,'0','0' ,'6');
										   AL_DWIN_Toggle(10);
										   break;

						   case  0x1057 :
							               my_dwin_var.pcheck_num = 7;
										   AL_DWIN_Text_Show(0x7503,'0','0' ,'7');
										   AL_DWIN_Toggle(10);
										   break;

						   case  0x1058 :
							               my_dwin_var.pcheck_num = 8;
										   AL_DWIN_Text_Show(0x7503,'0','0' ,'8');
										   AL_DWIN_Toggle(10);
										   break;


						   case  0x1059 :
							               my_dwin_var.pcheck_num = 9;
										   AL_DWIN_Text_Show(0x7503,'0','0' ,'9');
										   AL_DWIN_Toggle(10);
										   break;

						   case  0x1060 :
							               my_dwin_var.pcheck_num = 10;
										   AL_DWIN_Text_Show(0x7503,'0','1' ,'0');
										   AL_DWIN_Toggle(10);
										   break;

						   case  0x1061 :
							               my_dwin_var.pcheck_num = 11;
										   AL_DWIN_Text_Show(0x7503,'0','1' ,'1');
										   AL_DWIN_Toggle(10);
										   break;


						   case  0x1062 :
							               my_dwin_var.pcheck_num = 12;
										   AL_DWIN_Text_Show(0x7503,'0','1' ,'2');
										   AL_DWIN_Toggle(10);
										   break;

						   case  0x1063 :
										   my_dwin_var.pcheck_num = 13;
										   AL_DWIN_Text_Show(0x7503,'0','1' ,'3');
										   AL_DWIN_Toggle(10);
										   break;


						   case  0x1064 :
										   my_dwin_var.pcheck_num = 14;
										   AL_DWIN_Text_Show(0x7503,'0','1' ,'4');
										   AL_DWIN_Toggle(10);
										   break;



						   case  0x1065 :
										   my_dwin_var.pcheck_num = 15;
										   AL_DWIN_Text_Show(0x7503,'S','T' ,'A');
										   AL_DWIN_Toggle(10);
										   break;


						   case  0x1066 :
										   my_dwin_var.pcheck_num = 16;
										   AL_DWIN_Text_Show(0x7503,'S','T' ,'B');
										   AL_DWIN_Toggle(10);
										   break;

						   case  0x1067 :

										   if(my_dwin_var.check_sel_pump == 6)
										   {
											 my_dwin_var.pcheck_num = CHANNEL_06;
											 AL_DWIN_Text_Show(0x7503,'A','L' ,'L');
										   }
										   else if(my_dwin_var.check_sel_pump == 8)
										   {
											 my_dwin_var.pcheck_num = CHANNEL_08;
											 AL_DWIN_Text_Show(0x7503,'A','L' ,'L');
										   }
										   else if(my_dwin_var.check_sel_pump == 12)
										   {
											 my_dwin_var.pcheck_num = CHANNEL_12;
											 AL_DWIN_Text_Show(0x7503,'A','L' ,'L');
										   }
										   else if(my_dwin_var.check_sel_pump == 14)
										   {
											 my_dwin_var.pcheck_num = CHANNEL_14;
											 AL_DWIN_Text_Show(0x7503,'A','L' ,'L');
										   }
										   else
										   {
											 my_dwin_var.pcheck_num = CHANNEL_06;
											 AL_DWIN_Text_Show(0x7503,'A','L' ,'L');
										   }

										   AL_DWIN_Toggle(10);
										   break;


						   case  0x1068 :

							               if(my_dwin_var.check_sel_pump == 6)
										   {

							            	   AL_DWIN_Picshow(123);
							            	   AL_PP_Solonoid_valve_reset();
												  // park pumps
											   AL_PP_Park_Home(1,INIT_RPM);
											   AL_PP_Park_Home(2,INIT_RPM);
											   AL_PP_Park_Home(3,INIT_RPM);
											   AL_PP_Park_Home(4,INIT_RPM);
											   AL_PP_Park_Home(5,INIT_RPM);
											   AL_PP_Park_Home(6,INIT_RPM);


											   HAL_Delay(100);
												// sol on
											   AL_PP_Solonoid_valve_on();

											   HAL_Delay(10);
											   // reversed pump quarter degree on selected steps per revolution
											   AL_PP_Set_RPM(INIT_RPM);
											   AL_PP_StepperMotor_Setdirection(PP_Reversed);
											   HAL_TIM_PWM_Start(&htim1, TIM_CHANNEL_1);
											   AL_PP_StepperMotor_Enable(PP_Step_01, PP_Enable);
											   AL_PP_StepperMotor_Enable(PP_Step_02, PP_Enable);
											   AL_PP_StepperMotor_Enable(PP_Step_03, PP_Enable);
											   AL_PP_StepperMotor_Enable(PP_Step_04, PP_Enable);
											   AL_PP_StepperMotor_Enable(PP_Step_05, PP_Enable);
											   AL_PP_StepperMotor_Enable(PP_Step_06, PP_Enable);
											   AL_PP_StepperMotor_Enable(PP_Step_07, PP_Enable);

											   while(  my_stepper_var.step_count <  (STEPS_PER_REV/4)){};
											   AL_PP_ALL_Stop_Motor();

											   HAL_Delay(10);

												   // sol off
												AL_PP_Solonoid_valve_reset();

												AL_DWIN_Picshow(62);

										   }
										   else
										   {
											  my_dwin_flags.reset_12_flag = 1;
											  my_dwin_flags.reset_8_flag = 0;

										   }

										   AL_DWIN_Toggle(10);
							               break;

						   case  0x1045 :

										   //my_dwin_flags.dir_fwd_flag = 1;
										   //my_dwin_flags.dir_rev_flag = 0;
										   AL_DWIN_Picshow(120);

										   // Read settings variables
										   my_eeprom_var.save_flowrate = AL_EEPROM_Read_NUM(1, 0);
										   saved_rpm = (uint32_t)((float)((double)(my_eeprom_var.save_flowrate)/DPR));

										   switch(my_dwin_var.pcheck_num)
										   {

												case 1:

													    AL_PP_Start_Motor(PP_Step_01,saved_rpm , PP_Forward);
														break;

												case 3:

														AL_PP_Start_Motor(PP_Step_02,saved_rpm , PP_Forward);
														break;

												case 5:
														AL_PP_Start_Motor(PP_Step_03,saved_rpm , PP_Forward);
														break;

												case 2:
														AL_PP_Start_Motor(PP_Step_05,saved_rpm , PP_Forward);
														break;

												case 4:
														AL_PP_Start_Motor(PP_Step_06,saved_rpm , PP_Forward);
														break;

												case 6:
														AL_PP_Start_Motor(PP_Step_07,saved_rpm , PP_Forward);
														break;
                                                /*
												case 7:
														AL_PP_Start_Motor(PP_Step_08,saved_rpm , PP_Forward);
														break;

												case 8:
														AL_PP_Start_Motor(PP_Step_09,saved_rpm , PP_Forward);
														break;

												case 9:
														AL_PP_Start_Motor(PP_Step_09,saved_rpm , PP_Forward);
														break;

												case 10:
														AL_PP_Start_Motor(PP_Step_10,saved_rpm , PP_Forward);
														break;

												case 11:
														AL_PP_Start_Motor(PP_Step_11,saved_rpm , PP_Forward);
														break;

												case 12:
														AL_PP_Start_Motor(PP_Step_12,saved_rpm , PP_Forward);
														break;
													*/
												case CHANNEL_06:
													    AL_PP_ALL_Start_Motor(saved_rpm, PP_Forward);
													    break;

												case CHANNEL_12:
														AL_PP_ALL_Start_Motor(saved_rpm, PP_Forward);
														break;


										   }
										   AL_DWIN_Toggle(10);
										   break;


						   case  0x1046 :

							              // my_dwin_flags.dir_fwd_flag = 0;
							              // my_dwin_flags.dir_rev_flag = 1;
							               AL_DWIN_Picshow(121);
							               switch(my_dwin_var.pcheck_num)
										   {

												case 1:
													    HAL_GPIO_WritePin(GPIOE, SOL_1, GPIO_PIN_SET);
														AL_PP_Start_Motor(PP_Step_01,saved_rpm , PP_Reversed);
														break;

												case 3:
													    HAL_GPIO_WritePin(GPIOE, SOL_3, GPIO_PIN_SET);
														AL_PP_Start_Motor(PP_Step_02,saved_rpm , PP_Reversed);
														break;

												case 5:
													    HAL_GPIO_WritePin(GPIOE, SOL_5, GPIO_PIN_SET);
														AL_PP_Start_Motor(PP_Step_03,saved_rpm , PP_Reversed);
														break;

												case 2:
													    HAL_GPIO_WritePin(GPIOE, SOL_2, GPIO_PIN_SET);
														AL_PP_Start_Motor(PP_Step_05,saved_rpm , PP_Reversed);
														break;

												case 4:
													    HAL_GPIO_WritePin(GPIOE, SOL_4, GPIO_PIN_SET);
														AL_PP_Start_Motor(PP_Step_06,saved_rpm , PP_Reversed);
														break;

												case 6:
													    HAL_GPIO_WritePin(GPIOE, SOL_6, GPIO_PIN_SET);
														AL_PP_Start_Motor(PP_Step_07,saved_rpm , PP_Reversed);
														break;
                                               /*
												case 7:
														AL_PP_Start_Motor(PP_Step_07,saved_rpm , PP_Reversed);
														break;

												case 8:
														AL_PP_Start_Motor(PP_Step_08,saved_rpm , PP_Reversed);
														break;

												case 9:
														AL_PP_Start_Motor(PP_Step_09,saved_rpm , PP_Reversed);
														break;

												case 10:
														AL_PP_Start_Motor(PP_Step_10,saved_rpm , PP_Reversed);
														break;

												case 11:
														AL_PP_Start_Motor(PP_Step_11,saved_rpm , PP_Reversed);
														break;

												case 12:
														AL_PP_Start_Motor(PP_Step_12,saved_rpm , PP_Reversed);
														break;
														*/
											 case CHANNEL_06:
													    AL_PP_Solonoid_valve_on();
														AL_PP_ALL_Start_Motor(saved_rpm, PP_Reversed);
														break;

											 case CHANNEL_12:
												        AL_PP_Solonoid_valve_on();
														AL_PP_ALL_Start_Motor(saved_rpm, PP_Reversed);
														break;


										   }

										   AL_DWIN_Toggle(10);
										   break;


						   case  0x1090 :


							               AL_PP_ALL_Stop_Motor();
							               AL_PP_Solonoid_valve_reset();

							               AL_DWIN_Toggle(10);
							               break;

						   default:

										   break;
    	    	      }
     }

}


void AL_PP_Disp_Home_about_check(void)
{

     if( my_dwin_flags.about_flag == 1)
     {


			 AL_DWIN_Start();
			 AL_DWIN_Send_Data(5+6);
			 AL_DWIN_Send_Data(DWIN_WRITE);
			 AL_DWIN_Send_Data((0x7540 & 0xFF00)>>8);
			 AL_DWIN_Send_Data(0x7540 & 0x00FF);
			 AL_DWIN_Send_Data('P');
			 AL_DWIN_Send_Data('P');
			 AL_DWIN_Send_Data('-');

			 if(my_dwin_var.check_sel_pump == 6)
			 {
				 AL_DWIN_Send_Data('0');
				 AL_DWIN_Send_Data('8');
				 AL_DWIN_Send_Data(' ');

			 }
			 else if(my_dwin_var.check_sel_pump == 8)
			 {
				 AL_DWIN_Send_Data('0');
				 AL_DWIN_Send_Data('8');
				 AL_DWIN_Send_Data('i');

			 }
			 else if(my_dwin_var.check_sel_pump == 12)
			 {
				 AL_DWIN_Send_Data('1');
				 AL_DWIN_Send_Data('4');
				 AL_DWIN_Send_Data(' ');

			 }
			 else if(my_dwin_var.check_sel_pump == 14)
			 {
				 AL_DWIN_Send_Data('1');
				 AL_DWIN_Send_Data('4');
				 AL_DWIN_Send_Data('i');

			 }
			 else
			 {
				 AL_DWIN_Send_Data('0');
				 AL_DWIN_Send_Data('8');
				 AL_DWIN_Send_Data(' ');
			 }
			 AL_DWIN_End();

			 HAL_Delay(2);


    	     AL_DWIN_Start();
    		 AL_DWIN_Send_Data(5+6);
    		 AL_DWIN_Send_Data(DWIN_WRITE);
    		 AL_DWIN_Send_Data((0x7510 & 0xFF00)>>8);
    		 AL_DWIN_Send_Data(0x7510 & 0x00FF);
    		 AL_DWIN_Send_Data(pp_version[0]);
    		 AL_DWIN_Send_Data(pp_version[1]);
    		 AL_DWIN_Send_Data(pp_version[2]);
    		 AL_DWIN_Send_Data(pp_version[3]);
    		 AL_DWIN_Send_Data(pp_version[4]);
    		 AL_DWIN_Send_Data(pp_version[5]);
    		 AL_DWIN_End();

    		 HAL_Delay(2);




			 AL_DWIN_Start();
			 AL_DWIN_Send_Data(5+4);
			 AL_DWIN_Send_Data(DWIN_WRITE);
			 AL_DWIN_Send_Data((0x7530 & 0xFF00)>>8);
			 AL_DWIN_Send_Data(0x7530 & 0x00FF);
			 AL_DWIN_Send_Data(hmi_version[0]);
			 AL_DWIN_Send_Data(hmi_version[1]);
			 AL_DWIN_Send_Data(hmi_version[2]);
			 AL_DWIN_Send_Data(hmi_version[3]);
			 AL_DWIN_End();

			 HAL_Delay(2);
     }

}

void AL_PP_Disp_Startup(void)
{
	 AL_Dwin_Init();
	 HAL_Delay(10);
	 AL_PP_Disp_logo();
	 AL_DWIN_CONV_NUM_ARR();
}


void AL_PP_Power_On_Data(void)
{
      HAL_Delay(10);

      // Read settings variables
      my_eeprom_var.save_flowrate = AL_EEPROM_Read_NUM(1, 0);
      my_eeprom_var.save_dead_vol_pre_valve = AL_EEPROM_Read_NUM(1, 4);
      my_eeprom_var.save_dead_vol_post_valve = AL_EEPROM_Read_NUM(1, 8);

      // read prime volumes
      my_eeprom_var.save_sample_volume    =  AL_EEPROM_Read_NUM(2,0) ;
      my_eeprom_var.save_rinse_volume     =  AL_EEPROM_Read_NUM(2,4) ;
      my_eeprom_var.save_replenish_volume =	 AL_EEPROM_Read_NUM(2,8) ;

      // read selected pump channels
      AL_EEPROM_Page_Read(3, 0, &my_eeprom_var.save_buff[0] , 1);
      my_eeprom_var.save_pump_channel_selection = my_eeprom_var.save_buff[0];


      my_dwin_var.check_sel_pump = my_eeprom_var.save_pump_channel_selection;
      my_dwin_var.temp_flowrate =  (float)(AL_PP_roundoff( my_eeprom_var.save_flowrate))/100;
      my_dwin_var.temp_dead_pre_valve = (float)(AL_PP_roundoff(my_eeprom_var.save_dead_vol_pre_valve))/100 ;
      my_dwin_var.temp_dead_post_valve =(float)(AL_PP_roundoff( my_eeprom_var.save_dead_vol_post_valve))/100;

      my_dwin_var.temp_sample_volume = (float)(AL_PP_roundoff(my_eeprom_var.save_sample_volume))/100;
      my_dwin_var.temp_rinse_volume =(float) (AL_PP_roundoff(my_eeprom_var.save_rinse_volume))/100 ;
      my_dwin_var.temp_replenish_volume =(float)( AL_PP_roundoff(my_eeprom_var.save_replenish_volume))/100;


      HAL_Delay(10);


}

uint16_t AL_PP_roundoff(float data)
{
	uint16_t m_data = 0;

	uint16_t store_data = (uint16_t)(data * 100);
	m_data = (uint16_t)(data * 100);

	if((m_data / 10) == 9)
	{
	 	 store_data = store_data + 1;

	}
	else
	{
		  store_data =  store_data;

	}

	return store_data;
}

void AL_PP_Disp_Default_Val(void)
{
     AL_DWIN_Write_VP(0x1024, my_eeprom_var.save_pump_channel_selection);

	 AL_DWIN_Write_VP(0x1000 , (uint16_t) ( my_dwin_var.temp_flowrate *10));
	 AL_DWIN_Write_VP(0x1004 , (uint16_t) ( my_dwin_var.temp_dead_pre_valve *10));
	 AL_DWIN_Write_VP(0x1008 , (uint16_t) ( my_dwin_var.temp_dead_post_valve *10));

	 AL_DWIN_Write_VP(0x1012 , (uint16_t) ( my_dwin_var.temp_sample_volume *10));
	 AL_DWIN_Write_VP(0x1016 , (uint16_t) ( my_dwin_var.temp_rinse_volume *10));
	 AL_DWIN_Write_VP(0x1020 , (uint16_t) ( my_dwin_var.temp_replenish_volume *10));

}

void AL_PP_Buzzer_check(void)
{
	HAL_GPIO_WritePin(GPIOC, GPIO_PIN_5, GPIO_PIN_SET);
	HAL_Delay(100);
	HAL_GPIO_WritePin(GPIOC, GPIO_PIN_5, GPIO_PIN_RESET);
}


void AL_PP_Sampling_Process(void)
{

	if(my_disp_flags.rinsing_run_flag == SET_FLAG )
	{

       // start sampling set parameters
	   if(my_disp_var.prime_count == 0 )
	   {
		   my_disp_var.prime_count = 1;
		   my_disp_flags.ramp_up_flag = 0;
		   // check ramping required or not;
		   if(my_disp_var.rinsing_rpm < MAX_RAMP_RPM)
		   {
			   // direct run
		      my_disp_flags.ramp_up_flag = 0;
		      AL_PP_StepperMotor_Setdirection(PP_Forward);
			  AL_PP_Set_RPM(my_disp_var.rinsing_rpm);
			  AL_PP_Stepper_run();
			  // Enabling specific pumps
			  if((my_disp_var.sampling_channel != CHANNEL_06) && (my_disp_var.sampling_channel != CHANNEL_12))
			  {
			    AL_PP_StepperMotor_Enable(my_disp_var.sampling_channel , PP_Enable);
			  }
			  else
			  {

					if(my_disp_var.sampling_channel == CHANNEL_06)
					{
						AL_PP_StepperMotor_Enable(PP_Step_01, PP_Enable);
						AL_PP_StepperMotor_Enable(PP_Step_02, PP_Enable);
						AL_PP_StepperMotor_Enable(PP_Step_03, PP_Enable);
						AL_PP_StepperMotor_Enable(PP_Step_04, PP_Enable);
						AL_PP_StepperMotor_Enable(PP_Step_05, PP_Enable);
						AL_PP_StepperMotor_Enable(PP_Step_06, PP_Enable);
						AL_PP_StepperMotor_Enable(PP_Step_07, PP_Enable);
					}
					else
					{
						 // should be added
					}
			  }
		   }
		   else
		   {
			   // ramp with run
			   my_disp_flags.ramp_up_flag = 1;
			   AL_PP_StepperMotor_Setdirection(PP_Forward);
			   my_stepper_var.jump_rpm = START_RPM;
			   AL_PP_Set_RPM(my_stepper_var.jump_rpm);
			   AL_PP_Stepper_run();
			   // Enabling specific pumps
			      if((my_disp_var.sampling_channel != CHANNEL_06) && (my_disp_var.sampling_channel != CHANNEL_12))
				  {
					AL_PP_StepperMotor_Enable(my_disp_var.sampling_channel , PP_Enable);
				  }
				  else
				  {

						if(my_disp_var.sampling_channel == CHANNEL_06)
						{
							AL_PP_StepperMotor_Enable(PP_Step_01, PP_Enable);
							AL_PP_StepperMotor_Enable(PP_Step_02, PP_Enable);
							AL_PP_StepperMotor_Enable(PP_Step_03, PP_Enable);
							AL_PP_StepperMotor_Enable(PP_Step_04, PP_Enable);
							AL_PP_StepperMotor_Enable(PP_Step_05, PP_Enable);
							AL_PP_StepperMotor_Enable(PP_Step_06, PP_Enable);
							AL_PP_StepperMotor_Enable(PP_Step_07, PP_Enable);
						}
						else
						{
							 // should be added
						}
				  }

		   }
		   // clear ticks
		   my_disp_var.U_ticks = 0;
		   my_disp_var.opto_rev_count[1] = 1;
		   my_disp_var.opto_rev_count[2] = 1;
		   my_disp_var.opto_rev_count[3] = 1;
		   my_disp_var.opto_rev_count[4] = 1;
		   my_disp_var.opto_rev_count[5] = 1;
		   my_disp_var.opto_rev_count[6] = 1;

	   }
       // start sampling process
	   if(my_disp_var.prime_count == 1)
	   {
		   // check ramp flag set
			 if(my_disp_flags.ramp_up_flag == 1)
			 {
						//  ramping process
						 if(my_disp_var.ramp_count == 0)
						 {
							 AL_PP_Set_RPM(my_stepper_var.jump_rpm);
							 my_disp_var.ramp_count = 1;
							 my_disp_var.ramp_ticks = 0;
						 }
						 if(my_disp_var.ramp_count == 1)
						 {
							 if(my_disp_var.ramp_ticks  >=  MAX_WAIT_TIME)         // wait time for ramping in ms
							 {
								 my_stepper_var.jump_rpm += MAX_STEP_RUN;

								 if( my_stepper_var.jump_rpm >= my_disp_var.rinsing_rpm)
								 {
									 my_disp_var.ramp_count = 2;
								 }
								 else
								 {
									 my_disp_var.ramp_ticks = 0;
							         my_disp_var.ramp_count = 0;
								 }
							 }

						 }
						 if( my_disp_var.ramp_count == 2)
						 {
							 AL_PP_Set_RPM(my_disp_var.rinsing_rpm);
							 my_disp_var.ramp_count = 3;
						 }

						if(my_disp_var.U_ticks >= ((my_disp_var.rinsing_time * 1000)))
						{
							 if(my_disp_var.opto_rev_count[1] >= my_disp_var.get_opto_rcount)
			                 {
							   my_disp_var.U_ticks = 0;
							   my_disp_var.prime_count = 0;
							   my_disp_var.ramp_count = 0;
							   my_disp_var.ramp_ticks = 0;
							   my_disp_flags.ramp_up_flag = 0;
							   AL_PP_ALL_Stop_Motor();
							   my_disp_flags.rinsing_run_flag  = RESET_FLAG;
							   AL_DWIN_Picshow(112);  // sampling cmd
			                 }
						}

			  }
			 else
			 {

				   if(my_disp_var.U_ticks >= (my_disp_var.rinsing_time*1000))
				   {


					   my_disp_var.U_ticks = 0;
					   my_disp_var.prime_count = 0;
					   my_disp_var.ramp_count = 0;
					   my_disp_var.ramp_ticks = 0;
					   my_disp_flags.ramp_up_flag = 0;
					   AL_PP_ALL_Stop_Motor();
					   my_disp_flags.rinsing_run_flag  = RESET_FLAG;
					   AL_DWIN_Picshow(112);  // sampling cmd


				   }
			  }
	    }
	}

  /******************************* SAMPLING PROCESS******************************************/

	if(my_disp_flags.sampling_run_flag == SET_FLAG )
	{

		// start sampling set parameters
			   if(my_disp_var.prime_count == 0 )
			   {
				   my_disp_var.prime_count = 1;
				   my_disp_flags.ramp_up_flag = 0;
				   // check ramping required or not;
				   if(my_disp_var.sampling_rpm < MAX_RAMP_RPM)
				   {
					   // direct run
				      my_disp_flags.ramp_up_flag = 0;
				      AL_PP_StepperMotor_Setdirection(PP_Forward);
					  AL_PP_Set_RPM(my_disp_var.sampling_rpm);
					  AL_PP_Stepper_run();
					  // Enabling specific pumps
					  if((my_disp_var.sampling_channel != CHANNEL_06) && (my_disp_var.sampling_channel != CHANNEL_12))
					  {
					    AL_PP_StepperMotor_Enable(my_disp_var.sampling_channel , PP_Enable);
					  }
					  else
					  {

							if(my_disp_var.sampling_channel == CHANNEL_06)
							{
								AL_PP_StepperMotor_Enable(PP_Step_01, PP_Enable);
								AL_PP_StepperMotor_Enable(PP_Step_02, PP_Enable);
								AL_PP_StepperMotor_Enable(PP_Step_03, PP_Enable);
								AL_PP_StepperMotor_Enable(PP_Step_04, PP_Enable);
								AL_PP_StepperMotor_Enable(PP_Step_05, PP_Enable);
								AL_PP_StepperMotor_Enable(PP_Step_06, PP_Enable);
								AL_PP_StepperMotor_Enable(PP_Step_07, PP_Enable);
							}
							else
							{
								 // should be added
							}
					  }
				   }
				   else
				   {
					   // ramp with run
					   my_disp_flags.ramp_up_flag = 1;
					   my_disp_flags.ramp_completed_flag = 0;
					   AL_PP_StepperMotor_Setdirection(PP_Forward);
					   my_stepper_var.jump_rpm =START_RPM;
					   AL_PP_Set_RPM(my_stepper_var.jump_rpm);
					   AL_PP_Stepper_run();
						 // Enabling specific pumps
						  if((my_disp_var.sampling_channel != CHANNEL_06) && (my_disp_var.sampling_channel != CHANNEL_12))
						  {
							AL_PP_StepperMotor_Enable(my_disp_var.sampling_channel , PP_Enable);
						  }
						  else
						  {

								if(my_disp_var.sampling_channel == CHANNEL_06)
								{
									AL_PP_StepperMotor_Enable(PP_Step_01, PP_Enable);
									AL_PP_StepperMotor_Enable(PP_Step_02, PP_Enable);
									AL_PP_StepperMotor_Enable(PP_Step_03, PP_Enable);
									AL_PP_StepperMotor_Enable(PP_Step_04, PP_Enable);
									AL_PP_StepperMotor_Enable(PP_Step_05, PP_Enable);
									AL_PP_StepperMotor_Enable(PP_Step_06, PP_Enable);
									AL_PP_StepperMotor_Enable(PP_Step_07, PP_Enable);
								}
								else
								{
									 // should be added
								}
						  }


				   }
				   // clear ticks
				  my_disp_var.U_ticks = 0;
				  my_disp_var.opto_rev_count[1] = 0;
				  my_disp_var.opto_rev_count[2] = 0;
				  my_disp_var.opto_rev_count[3] = 0;
				  my_disp_var.opto_rev_count[4] = 0;
				  my_disp_var.opto_rev_count[5] = 0;
				  my_disp_var.opto_rev_count[6] = 0;

			   }
			   /************************************************************************/
		       // start sampling process
			   if(my_disp_var.prime_count == 1)
			   {
						  // check ramp flag set
						 if(my_disp_flags.ramp_up_flag == 1)
						 {

							     if(my_disp_var.ramp_count == 0)
								 {
									 AL_PP_Set_RPM(my_stepper_var.jump_rpm);

									 my_disp_var.ramp_count = 1;
									 my_disp_var.ramp_ticks = 0;
								 }
								 if(my_disp_var.ramp_count == 1)
								 {
									 if(my_disp_var.ramp_ticks  >=  MAX_WAIT_TIME)         // wait time for ramping in ms
									 {
										 my_stepper_var.jump_rpm += MAX_STEP_RUN;
										 if( my_stepper_var.jump_rpm >= (my_disp_var.sampling_rpm ))
										 {
											 stamp_opto_count = my_disp_var.opto_rev_count[1];
											 my_stepper_var.jump_rpm = my_disp_var.sampling_rpm;
											 my_disp_var.ramp_count = 2;
										 }
										 else
										 {
											 my_disp_var.ramp_ticks = 0;
											 my_disp_var.ramp_count = 0;
										 }

									 }

								 }
								 if( my_disp_var.ramp_count == 2)
								 {
									 AL_PP_Set_RPM(my_disp_var.sampling_rpm);
									 my_disp_var.ramp_count = 3;
								 }



									if(my_disp_var.U_ticks >= ((my_disp_var.sampling_time * 1000)))
									{
										 if(my_disp_var.opto_rev_count[1] >= my_disp_var.get_opto_scount)
									     {

												//  ramping process
													AL_PP_ALL_Stop_Motor();
													my_disp_var.U_ticks = 0;
													my_disp_var.prime_count = 0;
													my_disp_var.ramp_count = 0;
													my_disp_var.ramp_ticks = 0;
													my_disp_flags.ramp_up_flag = 0;

													my_disp_flags.sampling_run_flag  = RESET_FLAG;
													AL_DWIN_Picshow(32);  // sampling cmd
									     }
								     }


						 }
						 else
						 {

								   if(my_disp_var.U_ticks >= (my_disp_var.sampling_time * 1000))
								   {
									   my_disp_var.U_ticks = 0;
									   my_disp_var.prime_count = 0;
									   my_disp_var.ramp_count = 0;
									   my_disp_var.ramp_ticks = 0;
									   my_disp_flags.ramp_up_flag = 0;
									   AL_PP_ALL_Stop_Motor();
									   my_disp_flags.sampling_run_flag  = RESET_FLAG;
									   AL_DWIN_Picshow(32);  // sampling cmd
								   }
						 }

			    }

	}


	if( my_disp_flags.replenish_run_flag == SET_FLAG)
	{
		// start sampling set parameters
	   if(my_disp_var.prime_count == 0 )
	   {
		   my_disp_var.prime_count = 1;
		   my_disp_flags.ramp_up_flag = 0;
		   // check ramping required or not;
		   if(my_disp_var.replenish_rpm < MAX_RAMP_RPM)
		   {
			  // direct run

			  my_disp_flags.ramp_up_flag = 0;
			  AL_PP_StepperMotor_Setdirection(PP_Reversed);
			  AL_PP_Set_RPM(my_disp_var.replenish_rpm);
			  AL_PP_Stepper_run();
			  // Enabling specific pumps
			  if((my_disp_var.sampling_channel != CHANNEL_06) && (my_disp_var.sampling_channel != CHANNEL_12))
			  {
				AL_PP_StepperMotor_Enable(my_disp_var.sampling_channel , PP_Enable);
			  }
			  else
			  {

					if(my_disp_var.sampling_channel == CHANNEL_06)
					{
						AL_PP_StepperMotor_Enable(PP_Step_01, PP_Enable);
						AL_PP_StepperMotor_Enable(PP_Step_02, PP_Enable);
						AL_PP_StepperMotor_Enable(PP_Step_03, PP_Enable);
						AL_PP_StepperMotor_Enable(PP_Step_04, PP_Enable);
						AL_PP_StepperMotor_Enable(PP_Step_05, PP_Enable);
						AL_PP_StepperMotor_Enable(PP_Step_06, PP_Enable);
						AL_PP_StepperMotor_Enable(PP_Step_07, PP_Enable);
					}
					else
					{
						 // should be added
					}
			  }
		   }
		   else
		   {
			   // ramp with run
			   my_disp_flags.ramp_up_flag = 1;
			   AL_PP_StepperMotor_Setdirection(PP_Reversed);
			   my_stepper_var.jump_rpm = 50;
			   AL_PP_Set_RPM(my_stepper_var.jump_rpm);
			   AL_PP_Stepper_run();
			   // Enabling specific pumps
				  if((my_disp_var.sampling_channel != CHANNEL_06) && (my_disp_var.sampling_channel != CHANNEL_12))
				  {
					AL_PP_StepperMotor_Enable(my_disp_var.sampling_channel , PP_Enable);
				  }
				  else
				  {

						if(my_disp_var.sampling_channel == CHANNEL_06)
						{
							AL_PP_StepperMotor_Enable(PP_Step_01, PP_Enable);
							AL_PP_StepperMotor_Enable(PP_Step_02, PP_Enable);
							AL_PP_StepperMotor_Enable(PP_Step_03, PP_Enable);
							AL_PP_StepperMotor_Enable(PP_Step_04, PP_Enable);
							AL_PP_StepperMotor_Enable(PP_Step_05, PP_Enable);
							AL_PP_StepperMotor_Enable(PP_Step_06, PP_Enable);
							AL_PP_StepperMotor_Enable(PP_Step_07, PP_Enable);
						}
						else
						{
							 // should be added
						}
				  }

		   }
		   // clear ticks
		   my_disp_var.U_ticks = 0;
		   my_disp_var.opto_rev_count[1] = 1;
		   my_disp_var.opto_rev_count[2] = 1;
           my_disp_var.opto_rev_count[3] = 1;
           my_disp_var.opto_rev_count[4] = 1;
		   my_disp_var.opto_rev_count[5] = 1;
		   my_disp_var.opto_rev_count[6] = 1;

	   }
	   // start sampling process
	   if(my_disp_var.prime_count == 1)
	   {
		   // check ramp flag set
			 if(my_disp_flags.ramp_up_flag == 1)
			 {
						//  ramping process
						 if(my_disp_var.ramp_count == 0)
						 {
							 AL_PP_Set_RPM(my_stepper_var.jump_rpm);
							 my_disp_var.ramp_count = 1;
							 my_disp_var.ramp_ticks = 0;
						 }
						 if(my_disp_var.ramp_count == 1)
						 {
							 if(my_disp_var.ramp_ticks  >  MAX_WAIT_TIME)         // wait time for ramping in ms
							 {
								 my_stepper_var.jump_rpm += MAX_STEP_RUN;

								 if( my_stepper_var.jump_rpm >= (my_disp_var.replenish_rpm ))
								 {
									 my_stepper_var.jump_rpm = my_disp_var.replenish_rpm;
									 my_disp_var.ramp_count = 2;
								 }
								 else
								 {
									 my_disp_var.ramp_ticks = 0;
									 my_disp_var.ramp_count = 0;
								 }
							 }

						 }
						 if(my_disp_var.ramp_count == 2)
						 {
							 AL_PP_Set_RPM(my_disp_var.replenish_rpm);
							 my_disp_var.ramp_count = 3;
						 }

						 if(my_disp_var.U_ticks >= ((my_disp_var.replenish_time*1000)))
						 {
							 if(my_disp_var.opto_rev_count[1] >= my_disp_var.get_opto_rpcount)
					         {

							   my_disp_var.U_ticks = 0;
							   my_disp_var.prime_count = 0;
							   my_disp_var.ramp_count = 0;
							   my_disp_var.ramp_ticks = 0;
							   my_disp_flags.ramp_up_flag = 0;
							   AL_PP_ALL_Stop_Motor();
							   AL_PP_Solonoid_valve_reset();
							   my_disp_flags.replenish_run_flag  = RESET_FLAG;
							   AL_DWIN_Picshow(32);  // sampling cmd
					         }

						  }



			  }
			  else
			  {
					   if(my_disp_var.U_ticks >= ((my_disp_var.replenish_time*1000)))
					   {


						   my_disp_var.U_ticks = 0;
						   my_disp_var.prime_count = 0;
						   my_disp_var.ramp_count = 0;
						   my_disp_var.ramp_ticks = 0;
						   my_disp_flags.ramp_up_flag = 0;
						   AL_PP_ALL_Stop_Motor();
						   AL_PP_Solonoid_valve_reset();
						   my_disp_flags.replenish_run_flag  = RESET_FLAG;
						   AL_DWIN_Picshow(32);  // sampling cmd


					   }
			  }
		}
	}
}



/**************** EOF **************************************/




