/*
 * STEPPERPP.c
 *
 *  Created on: Feb 25, 2025
 *      Author: Avilash.Sawant
 */
/*********** SYSTEM INCLUDES FILES*****************/
#include "COMMON.h"

/********** USER INCLUDES FILES ********************/


/********** variable declarations*******************/
            stepperHandletypeDef       my_stepper_var;
            stepperFlagsHandletypeDef  my_stepflags_var;
extern      eeprom_HandleTypeDef       my_eeprom_var;

extern      TIM_HandleTypeDef          htim1;
extern      TIM_HandleTypeDef          htim2;
extern unsigned char pp_version[6];
extern unsigned char ppe_version[6];
extern unsigned char hmi_version[4];
extern      DwinFlagHandleTypeDef      my_disp_flags;


extern enum ScreenHandleTypeDef        my_screen_num;
extern      DwinHandleTypeDef          my_dwin_var;





/********** USER variable declarations ********************/


uint8_t sol_count = 0;

extern uint32_t revolution_counter;






/********************USER FUNCTION BODY**********************************/


// Function to rotate the motor in the specified direction
void AL_PP_StepperMotor_Setdirection(uint8_t direction)
{
    HAL_GPIO_WritePin(GPIOA, GPIO_PIN_12, direction); // Set the direction
}


void AL_PP_StepperMotor_Enable(uint16_t sel_motor, uint8_t sig)
{
	 HAL_GPIO_WritePin(GPIOA, sel_motor, sig); // Set the direction
}

// Set the PWM frequency to achieve desired RPM
void AL_PP_Set_RPM(uint32_t rpm)
{
   // float step_frequency = (float)(rpm * STEPS_PER_REV) / 60;  // Hz
    my_stepper_var.rps = (float)(((double)rpm / 60)* STEPS_PER_REV) ;
    uint32_t timer_period = (1000000 / my_stepper_var.rps) - 1; // Timer period for desired frequency
    my_stepper_var.step_count = 0;
    // Update PWM period to match the calculated frequency
    __HAL_TIM_SET_AUTORELOAD(&htim1, timer_period);

    TIM1->CCR1 = timer_period / 2;

}

void AL_PP_Stepper_run(void)
{

   HAL_TIM_PWM_Start(&htim1, TIM_CHANNEL_1);            // Start PWM on STEP pin

}

void AL_PP_Calculate_RPM(void)
{


   if(my_stepper_var.rpm_ticks >= 2000)    // 1 sec
   {
	   my_stepper_var.rpm_ticks = 0;
	   my_stepper_var.current_rpm =   ((float)my_stepper_var.step_count / STEPS_PER_REV)*30 ;

	   my_stepper_var.step_count = 0;
   }
}

/*
 *  Single motor controlled
 */
void AL_PP_Start_Motor(uint16_t mot_no , uint32_t rpm, uint8_t dir)
{

	if(rpm < MAX_RAMP_RPM)
	{
	  AL_PP_StepperMotor_Setdirection(dir);
	  AL_PP_Set_RPM(rpm);
	  AL_PP_Stepper_run();
	  AL_PP_StepperMotor_Enable(mot_no, PP_Enable);
	}
	else
	{
         /*
		  AL_PP_StepperMotor_Setdirection(dir);
		  AL_PP_Set_RPM(rpm);
		  AL_PP_Stepper_run();
		  AL_PP_StepperMotor_Enable(mot_no, PP_Enable);
         */

		// acceleration in while loop
		  my_stepper_var.jump_rpm = START_RPM;
		  while( my_stepper_var.jump_rpm <= rpm)
		  {
			   AL_PP_StepperMotor_Setdirection(dir);
			   AL_PP_Set_RPM(my_stepper_var.jump_rpm);
			   AL_PP_Stepper_run();
			   AL_PP_StepperMotor_Enable(mot_no, PP_Enable);
			   HAL_Delay(10);
			   my_stepper_var.jump_rpm += MAX_STEP_RUN;
		  }



         /*

          // motor acceleration in timer
		  if(my_stepflags_var.jump_flag == 0)
		  {


			my_stepper_var.rpm_wait_time += 1;

				if(my_stepper_var.rpm_wait_time == MAX_WAIT_TIME )
				{
					  my_stepper_var.rpm_wait_time = 0;
					  my_stepper_var.jump_rpm += MAX_STEP_RUN;
					  if(my_stepper_var.jump_rpm <= rpm )
					  {
						    AL_PP_StepperMotor_Setdirection(dir);
							   AL_PP_Set_RPM(my_stepper_var.jump_rpm);
							   AL_PP_Stepper_run();
							   AL_PP_StepperMotor_Enable(mot_no, PP_Enable);
					  }
					  else
					  {
						   my_stepper_var.jump_rpm = START_RPM;
						   my_stepflags_var.jump_flag = 1;
					  }
				 }
		   }
*/
	 }
}

void AL_PP_Stop_Motor(uint16_t mot_no)
{

	 HAL_TIM_PWM_Stop(&htim1, TIM_CHANNEL_1);            // Start PWM on STEP pin
	 AL_PP_StepperMotor_Enable(mot_no, PP_Disable);
}


/*
 *  All motor controlled
 */
void AL_PP_ALL_Start_Motor(uint32_t rpm, uint8_t dir)
{

	if(rpm < MAX_RAMP_RPM)
	{
	  AL_PP_StepperMotor_Setdirection(dir);
	  AL_PP_Set_RPM(rpm);
	  AL_PP_Stepper_run();
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

		          my_stepper_var.jump_rpm = START_RPM;
				  while( my_stepper_var.jump_rpm <= rpm)
				  {
					   AL_PP_StepperMotor_Setdirection(dir);
					   AL_PP_Set_RPM(my_stepper_var.jump_rpm);
					   AL_PP_Stepper_run();
					   AL_PP_StepperMotor_Enable(PP_Step_01, PP_Enable);
					   AL_PP_StepperMotor_Enable(PP_Step_02, PP_Enable);
					   AL_PP_StepperMotor_Enable(PP_Step_03, PP_Enable);
					   AL_PP_StepperMotor_Enable(PP_Step_04, PP_Enable);
					   AL_PP_StepperMotor_Enable(PP_Step_05, PP_Enable);
					   AL_PP_StepperMotor_Enable(PP_Step_06, PP_Enable);
					   AL_PP_StepperMotor_Enable(PP_Step_07, PP_Enable);
					   HAL_Delay(MAX_WAIT_TIME);
					   my_stepper_var.jump_rpm += MAX_STEP_RUN;
				  }


/*
          // motor acceleration
		  if(my_stepflags_var.jump_flag == 0)
		  {

			my_stepper_var.rpm_wait_time += 1;

				if(my_stepper_var.rpm_wait_time == MAX_WAIT_TIME )   // every 10 ms cycle for ramping in step of 20 rpm
				{
					  my_stepper_var.rpm_wait_time = 0;
					  my_stepper_var.jump_rpm += MAX_STEP_RUN;
					  if(my_stepper_var.jump_rpm <= rpm )
					  {
						   AL_PP_StepperMotor_Setdirection(dir);
						   AL_PP_Set_RPM(my_stepper_var.jump_rpm);
						   AL_PP_Stepper_run();
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
						   my_stepper_var.jump_rpm = START_RPM;
						   my_stepflags_var.jump_flag = 1;
					  }
				 }
		   }
*/
	 }
}

void AL_PP_ALL_Stop_Motor(void)
{

	 HAL_TIM_PWM_Stop(&htim1, TIM_CHANNEL_1);            // Start PWM on STEP pin
	 AL_PP_StepperMotor_Enable(PP_Step_01, PP_Disable);
	 AL_PP_StepperMotor_Enable(PP_Step_02, PP_Disable);
	 AL_PP_StepperMotor_Enable(PP_Step_03, PP_Disable);
	 AL_PP_StepperMotor_Enable(PP_Step_04, PP_Disable);
	 AL_PP_StepperMotor_Enable(PP_Step_05, PP_Disable);
	 AL_PP_StepperMotor_Enable(PP_Step_06, PP_Disable);
	 AL_PP_StepperMotor_Enable(PP_Step_07, PP_Disable);
}


void AL_PP_test_motors(void)
{
	 // check motor
		      if( my_stepflags_var.flag_1sec == 0 )
		      {

					  if( my_stepper_var.run_time < SET_TIME )
					  {

						  switch(CHECK_NUM)
						  {
						     case  0:

						    	 	   AL_PP_Start_Motor(PP_Step_01, CHECK_RPM , PP_Forward );
						    	       break;

						     case  1:
						    	       AL_PP_Start_Motor(PP_Step_02, CHECK_RPM , PP_Forward );
						     		   break;

						     case  2:
									   AL_PP_Start_Motor(PP_Step_03, CHECK_RPM , PP_Forward );
									   break;

						     case  3:
									   AL_PP_Start_Motor(PP_Step_04, CHECK_RPM, PP_Forward );
									   break;

						     case  4:
									   AL_PP_Start_Motor(PP_Step_05, CHECK_RPM , PP_Forward );
									   break;

						     case  5:
									   AL_PP_Start_Motor(PP_Step_06, CHECK_RPM , PP_Forward );
									   break;

						     case  6:
									   AL_PP_Start_Motor(PP_Step_07, CHECK_RPM , PP_Forward );
									   break;



						     case 80:
						    	       HAL_GPIO_WritePin(GPIOE, GPIO_PIN_8, GPIO_PIN_SET);
						    	       HAL_GPIO_WritePin(GPIOE, GPIO_PIN_9, GPIO_PIN_SET);
						    	       HAL_GPIO_WritePin(GPIOE, GPIO_PIN_10, GPIO_PIN_SET);
						    	       HAL_GPIO_WritePin(GPIOE, GPIO_PIN_11, GPIO_PIN_SET);
						    	       HAL_GPIO_WritePin(GPIOE, GPIO_PIN_12, GPIO_PIN_SET);
						    	       HAL_GPIO_WritePin(GPIOE, GPIO_PIN_13, GPIO_PIN_SET);
						    	       HAL_GPIO_WritePin(GPIOE, GPIO_PIN_14, GPIO_PIN_SET);
						    	       HAL_GPIO_WritePin(GPIOE, GPIO_PIN_15, GPIO_PIN_SET);
									   sol_count += 1;
									   if(sol_count == 10)
									   {
										 sol_count = 0;
										 HAL_GPIO_WritePin(GPIOE, GPIO_PIN_8, GPIO_PIN_RESET);
										 HAL_GPIO_WritePin(GPIOE, GPIO_PIN_8, GPIO_PIN_RESET);
										   HAL_GPIO_WritePin(GPIOE, GPIO_PIN_9, GPIO_PIN_RESET);
										   HAL_GPIO_WritePin(GPIOE, GPIO_PIN_10, GPIO_PIN_RESET);
										   HAL_GPIO_WritePin(GPIOE, GPIO_PIN_11, GPIO_PIN_RESET);
										   HAL_GPIO_WritePin(GPIOE, GPIO_PIN_12, GPIO_PIN_RESET);
										   HAL_GPIO_WritePin(GPIOE, GPIO_PIN_13, GPIO_PIN_RESET);
										   HAL_GPIO_WritePin(GPIOE, GPIO_PIN_14, GPIO_PIN_RESET);
										   HAL_GPIO_WritePin(GPIOE, GPIO_PIN_15, GPIO_PIN_RESET);
									   }
						    	       AL_PP_ALL_Start_Motor(CHECK_RPM , PP_Forward );



						    		   break;

						     default :

						    	       break;


						  }

						  my_stepper_var.run_time += 1;
					  }
					  else
					  {

						  switch(CHECK_NUM)
						  {
							 case  0:
								       AL_PP_Stop_Motor(PP_Step_01);;
									   break;

							 case  1:
								       AL_PP_Stop_Motor(PP_Step_02);;
									   break;

							 case  2:
								       AL_PP_Stop_Motor(PP_Step_03);;
									   break;

							 case  3:
								       AL_PP_Stop_Motor(PP_Step_04);;
									   break;

							 case  4:
								       AL_PP_Stop_Motor(PP_Step_05);;
									   break;

							 case  5:
								       AL_PP_Stop_Motor(PP_Step_06);;
									   break;

							 case  6:
								       AL_PP_Stop_Motor(PP_Step_07);;
									   break;



							 case 80:
								       AL_PP_ALL_Stop_Motor();


									   break;

							 default :

									   break;


						  }


						   my_stepper_var.run_time = 0;
						   my_stepflags_var.flag_1sec = 1;
						   my_stepflags_var.jump_flag = 0;
						   my_stepper_var.jump_rpm = START_RPM;
					  }

		      }

		      if(  my_stepflags_var.flag_1sec == 1)
		      {

		    	  my_stepper_var.run_time += 1;

		    	  if(my_stepper_var.run_time == 5000) //  milli sec
		    	  {
		    		  my_stepper_var.run_time = 0;
		    		  my_stepflags_var.flag_1sec = 0;
		    	  }

		      }

}



void AL_PP_Home_forward(uint8_t position, uint32_t rpm)
{
	// home forward positioning
                // polling of all motor
		        /************************************************************************************************/
		  		if( position   ==  1U  )
		  		{
		  			AL_PP_Set_RPM(rpm);
					AL_PP_StepperMotor_Setdirection(PP_Forward);
					HAL_TIM_PWM_Start(&htim1, TIM_CHANNEL_1);

		  	           my_stepper_var.check_opto_state[1] = HAL_GPIO_ReadPin(GPIOE,GPIO_PIN_1);  // read pin status
		  	           if(my_stepper_var.check_opto_state[1] == BIT_RESET)      // falling edge detection
		  	           {
		  	        	 AL_PP_StepperMotor_Enable(PP_Step_01, PP_Enable);
		  			     while((HAL_GPIO_ReadPin(GPIOE,GPIO_PIN_1))  == BIT_RESET){};
		  			    // while((HAL_GPIO_ReadPin(GPIOE,GPIO_PIN_1))  == BIT_SET){};
		  	        	 // disable motor
		  				 AL_PP_StepperMotor_Enable(PP_Step_01, PP_Disable);

		  	           }
		  		   HAL_Delay(2);
		  		   HAL_TIM_PWM_Stop(&htim1, TIM_CHANNEL_1);
		  		}
		  	   /***********************************************************************************************************/

		  		if( position   ==  3U  )
		  		{
		  			AL_PP_Set_RPM(rpm);
					AL_PP_StepperMotor_Setdirection(PP_Forward);
					HAL_TIM_PWM_Start(&htim1, TIM_CHANNEL_1);

		  		          my_stepper_var.check_opto_state[2] = HAL_GPIO_ReadPin(GPIOE,GPIO_PIN_2);  // read pin status

		  		          if(my_stepper_var.check_opto_state[2] == BIT_RESET)      // falling edge detection
		  				  {
		  						 AL_PP_StepperMotor_Enable(PP_Step_02, PP_Enable);
		  						 while((HAL_GPIO_ReadPin(GPIOE,GPIO_PIN_2))  == BIT_RESET){};
		  						// while((HAL_GPIO_ReadPin(GPIOE,GPIO_PIN_2))  == BIT_SET){};
		  						 // disable motor
		  						 AL_PP_StepperMotor_Enable(PP_Step_02, PP_Disable);


		  				  }
		  	   	       HAL_Delay(2);
		  	   	       HAL_TIM_PWM_Stop(&htim1, TIM_CHANNEL_1);
		  		 }
		  		/***********************************************************************************************************/
		  		if( position   ==  5U  )
		  		{
		  			AL_PP_Set_RPM(rpm);
					AL_PP_StepperMotor_Setdirection(PP_Forward);
					HAL_TIM_PWM_Start(&htim1, TIM_CHANNEL_1);
		  		   my_stepper_var.check_opto_state[3] = HAL_GPIO_ReadPin(GPIOE,GPIO_PIN_3);  // read pin status

		  			      if(my_stepper_var.check_opto_state[3] == BIT_RESET)      // falling edge detection
		  				  {
		  						 AL_PP_StepperMotor_Enable(PP_Step_03, PP_Enable);
		  						 while((HAL_GPIO_ReadPin(GPIOE,GPIO_PIN_3))  == BIT_RESET){};
		  						// while((HAL_GPIO_ReadPin(GPIOE,GPIO_PIN_3))  == BIT_SET){};
		  						 // disable motor
		  						 AL_PP_StepperMotor_Enable(PP_Step_03, PP_Disable);

		  				  }
		  		   HAL_Delay(2);
		  		   HAL_TIM_PWM_Stop(&htim1, TIM_CHANNEL_1);
		  		}
		  	    /***********************************************************************************************************/
		  		if(position   ==  2U )
		  		{
		  			AL_PP_Set_RPM(rpm);
					AL_PP_StepperMotor_Setdirection(PP_Forward);
					HAL_TIM_PWM_Start(&htim1, TIM_CHANNEL_1);

		  				  my_stepper_var.check_opto_state[5] = HAL_GPIO_ReadPin(GPIOE,GPIO_PIN_5);  // read pin status

		  				  if(my_stepper_var.check_opto_state[5] == BIT_RESET)      // falling edge detection
		  				  {
		  						 AL_PP_StepperMotor_Enable(PP_Step_05, PP_Enable);
		  						 while((HAL_GPIO_ReadPin(GPIOE,GPIO_PIN_5))  == BIT_RESET){};
		  						// while((HAL_GPIO_ReadPin(GPIOE,GPIO_PIN_5))  == BIT_SET){};
		  						 // disable motor
		  						 AL_PP_StepperMotor_Enable(PP_Step_05, PP_Disable);


		  				  }
		  		   HAL_Delay(2);
		  		   HAL_TIM_PWM_Stop(&htim1, TIM_CHANNEL_1);
		  		 }
		  		 /***********************************************************************************************************/
		  		 if(position   ==  4U )
		  		 {
		  			AL_PP_Set_RPM(rpm);
					AL_PP_StepperMotor_Setdirection(PP_Forward);
					HAL_TIM_PWM_Start(&htim1, TIM_CHANNEL_1);

		  				  my_stepper_var.check_opto_state[6] = HAL_GPIO_ReadPin(GPIOE,GPIO_PIN_6);  // read pin status

		  				  if(my_stepper_var.check_opto_state[6] == BIT_RESET)      // falling edge detection
		  				  {
		  						 AL_PP_StepperMotor_Enable(PP_Step_06, PP_Enable);
		  						 while((HAL_GPIO_ReadPin(GPIOE,GPIO_PIN_6))  == BIT_RESET){};
		  						// while((HAL_GPIO_ReadPin(GPIOE,GPIO_PIN_6))  == BIT_SET){};
		  						 // disable motor
		  						 AL_PP_StepperMotor_Enable(PP_Step_06, PP_Disable);

		  				  }
		  		   HAL_Delay(2);
		  		   HAL_TIM_PWM_Stop(&htim1, TIM_CHANNEL_1);
		  		 }
		  		 /***********************************************************************************************************/
		  		 if(position   ==  6U )
		  	     {
		  			AL_PP_Set_RPM(rpm);
					AL_PP_StepperMotor_Setdirection(PP_Forward);
					HAL_TIM_PWM_Start(&htim1, TIM_CHANNEL_1);

		  				  my_stepper_var.check_opto_state[7] = HAL_GPIO_ReadPin(GPIOB,GPIO_PIN_6);  // read pin status

		  				  if(my_stepper_var.check_opto_state[7] == BIT_RESET)      // falling edge detection
		  				  {
		  						 AL_PP_StepperMotor_Enable(PP_Step_07, PP_Enable);
		  						 while((HAL_GPIO_ReadPin(GPIOB,GPIO_PIN_6))  == BIT_RESET){};
		  						// while((HAL_GPIO_ReadPin(GPIOB,GPIO_PIN_6))  == BIT_SET){};
		  						 // disable motor
		  						 AL_PP_StepperMotor_Enable(PP_Step_07, PP_Disable);


		  				  }
		  		   HAL_Delay(2);
		  		   HAL_TIM_PWM_Stop(&htim1, TIM_CHANNEL_1);
		  	     }





}

void AL_PP_Home_reversed(uint8_t position, uint32_t rpm)
{



	 	  	// polling of all motor
	 	  	/************************************************************************************************/
	 	  		if( position   ==  1U  )
	 	  	    {
	 	  		  AL_PP_Set_RPM(rpm);
				  AL_PP_StepperMotor_Setdirection(PP_Reversed);
				  HAL_TIM_PWM_Start(&htim1, TIM_CHANNEL_1);

	 	  	           my_stepper_var.check_opto_state[1] = HAL_GPIO_ReadPin(GPIOE,GPIO_PIN_1);  // read pin status
	 	  	           if(my_stepper_var.check_opto_state[1] == BIT_SET)      // falling edge detection
	 	  	           {
	 	  	        	 AL_PP_StepperMotor_Enable(PP_Step_01, PP_Enable);
	 	  			     while((HAL_GPIO_ReadPin(GPIOE,GPIO_PIN_1))  == BIT_SET){};
	 	  			     while((HAL_GPIO_ReadPin(GPIOE,GPIO_PIN_1))  == BIT_RESET){};
	 	  	        	 // disable motor
	 	  				 AL_PP_StepperMotor_Enable(PP_Step_01, PP_Disable);

	 	  	           }

	 	  	       HAL_Delay(2);
	 	  	       HAL_TIM_PWM_Stop(&htim1, TIM_CHANNEL_1);
	 	  	    }
	 	  	  /***********************************************************************************************************/
	 	  		if( position   ==  3U  )
	 	  	    {
	 	  			AL_PP_Set_RPM(rpm);
					AL_PP_StepperMotor_Setdirection(PP_Reversed);
					HAL_TIM_PWM_Start(&htim1, TIM_CHANNEL_1);
	 	  		          my_stepper_var.check_opto_state[2] = HAL_GPIO_ReadPin(GPIOE,GPIO_PIN_2);  // read pin status

	 	  		          if(my_stepper_var.check_opto_state[2] == BIT_SET)      // falling edge detection
	 	  				  {
	 	  						 AL_PP_StepperMotor_Enable(PP_Step_02, PP_Enable);
	 	  						 while((HAL_GPIO_ReadPin(GPIOE,GPIO_PIN_2))  == BIT_SET){};
	 	  						 while((HAL_GPIO_ReadPin(GPIOE,GPIO_PIN_2))  == BIT_RESET){};
	 	  						 // disable motor
	 	  						 AL_PP_StepperMotor_Enable(PP_Step_02, PP_Disable);


	 	  				  }

	 	  		   	 HAL_Delay(2);
	 	  		     HAL_TIM_PWM_Stop(&htim1, TIM_CHANNEL_1);
	 	  	     }
	 	  	  /***********************************************************************************************************/
	 	  		 if( position   ==  5U  )
	 	  	     {
	 	  			AL_PP_Set_RPM(rpm);
					AL_PP_StepperMotor_Setdirection(PP_Reversed);
					HAL_TIM_PWM_Start(&htim1, TIM_CHANNEL_1);

	 	  			    my_stepper_var.check_opto_state[3] = HAL_GPIO_ReadPin(GPIOE,GPIO_PIN_3);  // read pin status

	 	  			      if(my_stepper_var.check_opto_state[3] == BIT_SET)      // falling edge detection
	 	  				  {
	 	  						 AL_PP_StepperMotor_Enable(PP_Step_03, PP_Enable);
	 	  						 while((HAL_GPIO_ReadPin(GPIOE,GPIO_PIN_3))  == BIT_SET){};
	 	  						 while((HAL_GPIO_ReadPin(GPIOE,GPIO_PIN_3))  == BIT_RESET){};
	 	  						 // disable motor
	 	  						 AL_PP_StepperMotor_Enable(PP_Step_03, PP_Disable);


	 	  				  }
                   HAL_Delay(2);
                   HAL_TIM_PWM_Stop(&htim1, TIM_CHANNEL_1);
	 	  	     }
	 	  	  /***********************************************************************************************************/
	 	  		if( position   ==  2U  )
	 	  	    {
	 	  			AL_PP_Set_RPM(rpm);
					AL_PP_StepperMotor_Setdirection(PP_Reversed);
					HAL_TIM_PWM_Start(&htim1, TIM_CHANNEL_1);

	 	  				  my_stepper_var.check_opto_state[5] = HAL_GPIO_ReadPin(GPIOE,GPIO_PIN_5);  // read pin status

	 	  				  if(my_stepper_var.check_opto_state[5] == BIT_SET)      // falling edge detection
	 	  				  {
	 	  						 AL_PP_StepperMotor_Enable(PP_Step_05, PP_Enable);
	 	  						 while((HAL_GPIO_ReadPin(GPIOE,GPIO_PIN_5))  == BIT_SET){};
	 	  						 while((HAL_GPIO_ReadPin(GPIOE,GPIO_PIN_5))  == BIT_RESET){};
	 	  						 // disable motor
	 	  						 AL_PP_StepperMotor_Enable(PP_Step_05, PP_Disable);

	 	  				  }
	 	  		   HAL_Delay(2);
	 	  		   HAL_TIM_PWM_Stop(&htim1, TIM_CHANNEL_1);
	 	  	    }
	 	  	  /***********************************************************************************************************/
	 	  		if( position   ==  4U  )
	 	  	    {
	 	  			AL_PP_Set_RPM(rpm);
					AL_PP_StepperMotor_Setdirection(PP_Reversed);
					HAL_TIM_PWM_Start(&htim1, TIM_CHANNEL_1);

	 	  				  my_stepper_var.check_opto_state[6] = HAL_GPIO_ReadPin(GPIOE,GPIO_PIN_6);  // read pin status

	 	  				  if(my_stepper_var.check_opto_state[6] == BIT_SET)      // falling edge detection
	 	  				  {
	 	  						 AL_PP_StepperMotor_Enable(PP_Step_06, PP_Enable);
	 	  						 while((HAL_GPIO_ReadPin(GPIOE,GPIO_PIN_6))  == BIT_SET){};
	 	  						 while((HAL_GPIO_ReadPin(GPIOE,GPIO_PIN_6))  == BIT_RESET){};
	 	  						 // disable motor
	 	  						 AL_PP_StepperMotor_Enable(PP_Step_06, PP_Disable);


	 	  				  }
	 	  		   HAL_Delay(2);
	 	  		   HAL_TIM_PWM_Stop(&htim1, TIM_CHANNEL_1);
	 	  	    }
	 	  	   /***********************************************************************************************************/
	 	  		if( position   ==  6U  )
	 	  	    {
	 	  			AL_PP_Set_RPM(rpm);
					AL_PP_StepperMotor_Setdirection(PP_Reversed);
					HAL_TIM_PWM_Start(&htim1, TIM_CHANNEL_1);

	 	  				  my_stepper_var.check_opto_state[7] = HAL_GPIO_ReadPin(GPIOB,GPIO_PIN_6);  // read pin status

	 	  				  if(my_stepper_var.check_opto_state[7] == BIT_SET)      // falling edge detection
	 	  				  {
	 	  						 AL_PP_StepperMotor_Enable(PP_Step_07, PP_Enable);
	 	  						 while((HAL_GPIO_ReadPin(GPIOB,GPIO_PIN_6))  == BIT_SET){};
	 	  						 while((HAL_GPIO_ReadPin(GPIOB,GPIO_PIN_6))  == BIT_RESET){};
	 	  						 // disable motor
	 	  						 AL_PP_StepperMotor_Enable(PP_Step_07, PP_Disable);


	 	  				  }
	 	  		   HAL_Delay(2);
	 	  		   HAL_TIM_PWM_Stop(&htim1, TIM_CHANNEL_1);
	 	  	    }


}

void AL_PP_Park_Home(uint8_t position, uint32_t rpm)
{

		  // polling of all motor
		/************************************************************************************************/
		if( position   ==  1U  )
		{
		   AL_PP_Set_RPM(rpm);
		   AL_PP_StepperMotor_Setdirection(PP_Forward);
		   HAL_TIM_PWM_Start(&htim1, TIM_CHANNEL_1);

	           my_stepper_var.check_opto_state[1] = HAL_GPIO_ReadPin(GPIOE,GPIO_PIN_1);  // read pin status

	           if(my_stepper_var.check_opto_state[1] == BIT_RESET)      // falling edge detection
	           {
	        	 AL_PP_StepperMotor_Enable(PP_Step_01, PP_Enable);
			     while((HAL_GPIO_ReadPin(GPIOE,GPIO_PIN_1))  == BIT_RESET){};
	        	 // disable motor
				 AL_PP_StepperMotor_Enable(PP_Step_01, PP_Disable);


	           }
	           else
	           {
	        	 AL_PP_StepperMotor_Enable(PP_Step_01, PP_Enable);       // rising edge detection
	        	 while((HAL_GPIO_ReadPin(GPIOE,GPIO_PIN_1))  == BIT_SET){};
	        	 while((HAL_GPIO_ReadPin(GPIOE,GPIO_PIN_1))  == BIT_RESET){};
				 // disable motor
				 AL_PP_StepperMotor_Enable(PP_Step_01, PP_Disable);

	           }


	       HAL_Delay(2);
	       HAL_TIM_PWM_Stop(&htim1, TIM_CHANNEL_1);
		}
	    /***********************************************************************************************************/
		if( position   ==  3U  )
	    {
			AL_PP_Set_RPM(rpm);
		    AL_PP_StepperMotor_Setdirection(PP_Forward);
		    HAL_TIM_PWM_Start(&htim1, TIM_CHANNEL_1);

		          my_stepper_var.check_opto_state[2] = HAL_GPIO_ReadPin(GPIOE,GPIO_PIN_2);  // read pin status

		          if(my_stepper_var.check_opto_state[2] == BIT_RESET)      // falling edge detection
				  {
						 AL_PP_StepperMotor_Enable(PP_Step_02, PP_Enable);
						 while((HAL_GPIO_ReadPin(GPIOE,GPIO_PIN_2))  == BIT_RESET){};
						 // disable motor
						 AL_PP_StepperMotor_Enable(PP_Step_02, PP_Disable);


				  }
				  else
				  {
						 AL_PP_StepperMotor_Enable(PP_Step_02, PP_Enable);       // rising edge detection
						 while((HAL_GPIO_ReadPin(GPIOE,GPIO_PIN_2))  == BIT_SET){};
						 while((HAL_GPIO_ReadPin(GPIOE,GPIO_PIN_2))  == BIT_RESET){};
						 // disable motor
						 AL_PP_StepperMotor_Enable(PP_Step_02, PP_Disable);


				  }


		   	 HAL_Delay(2);
		   	 HAL_TIM_PWM_Stop(&htim1, TIM_CHANNEL_1);
	     }
		 /***********************************************************************************************************/
		 if( position   ==  5U  )
	     {
		   AL_PP_Set_RPM(rpm);
		   AL_PP_StepperMotor_Setdirection(PP_Forward);
		   HAL_TIM_PWM_Start(&htim1, TIM_CHANNEL_1);

			    my_stepper_var.check_opto_state[3] = HAL_GPIO_ReadPin(GPIOE,GPIO_PIN_3);  // read pin status

			      if(my_stepper_var.check_opto_state[3] == BIT_RESET)      // falling edge detection
				  {
						 AL_PP_StepperMotor_Enable(PP_Step_03, PP_Enable);
						 while((HAL_GPIO_ReadPin(GPIOE,GPIO_PIN_3))  == BIT_RESET){};
						 // disable motor
						 AL_PP_StepperMotor_Enable(PP_Step_03, PP_Disable);


				  }
				  else
				  {
						 AL_PP_StepperMotor_Enable(PP_Step_03, PP_Enable);       // rising edge detection
						 while((HAL_GPIO_ReadPin(GPIOE,GPIO_PIN_3))  == BIT_SET){};
						 while((HAL_GPIO_ReadPin(GPIOE,GPIO_PIN_3))  == BIT_RESET){};
						 // disable motor
						 AL_PP_StepperMotor_Enable(PP_Step_03, PP_Disable);


				  }

		   HAL_Delay(2);
		   HAL_TIM_PWM_Stop(&htim1, TIM_CHANNEL_1);
	     }
		 /***********************************************************************************************************/
		 if( position   ==  2U  )
		 {
		   AL_PP_Set_RPM(rpm);
		   AL_PP_StepperMotor_Setdirection(PP_Forward);
		   HAL_TIM_PWM_Start(&htim1, TIM_CHANNEL_1);

				  my_stepper_var.check_opto_state[5] = HAL_GPIO_ReadPin(GPIOE,GPIO_PIN_5);  // read pin status

				  if(my_stepper_var.check_opto_state[5] == BIT_RESET)      // falling edge detection
				  {
						 AL_PP_StepperMotor_Enable(PP_Step_05, PP_Enable);
						 while((HAL_GPIO_ReadPin(GPIOE,GPIO_PIN_5))  == BIT_RESET){};
						 // disable motor
						 AL_PP_StepperMotor_Enable(PP_Step_05, PP_Disable);


				  }
				  else
				  {
						 AL_PP_StepperMotor_Enable(PP_Step_05, PP_Enable);       // rising edge detection
						 while((HAL_GPIO_ReadPin(GPIOE,GPIO_PIN_5))  == BIT_SET){};
						 while((HAL_GPIO_ReadPin(GPIOE,GPIO_PIN_5))  == BIT_RESET){};
						 // disable motor
						 AL_PP_StepperMotor_Enable(PP_Step_05, PP_Disable);


				  }


		   HAL_Delay(2);
		   HAL_TIM_PWM_Stop(&htim1, TIM_CHANNEL_1);
		 }
		 /***********************************************************************************************************/
		 if( position   ==  4U  )
		 {
		   AL_PP_Set_RPM(rpm);
		   AL_PP_StepperMotor_Setdirection(PP_Forward);
		   HAL_TIM_PWM_Start(&htim1, TIM_CHANNEL_1);

				  my_stepper_var.check_opto_state[6] = HAL_GPIO_ReadPin(GPIOE,GPIO_PIN_6);  // read pin status

				  if(my_stepper_var.check_opto_state[6] == BIT_RESET)      // falling edge detection
				  {
						 AL_PP_StepperMotor_Enable(PP_Step_06, PP_Enable);
						 while((HAL_GPIO_ReadPin(GPIOE,GPIO_PIN_6))  == BIT_RESET){};
						 // disable motor
						 AL_PP_StepperMotor_Enable(PP_Step_06, PP_Disable);


				  }
				  else
				  {
						 AL_PP_StepperMotor_Enable(PP_Step_06, PP_Enable);       // rising edge detection
						 while((HAL_GPIO_ReadPin(GPIOE,GPIO_PIN_6))  == BIT_SET){};
						 while((HAL_GPIO_ReadPin(GPIOE,GPIO_PIN_6))  == BIT_RESET){};
						 // disable motor
						 AL_PP_StepperMotor_Enable(PP_Step_06, PP_Disable);


				  }
				 HAL_Delay(2);
				 HAL_TIM_PWM_Stop(&htim1, TIM_CHANNEL_1);
		 }
		  /***********************************************************************************************************/
		 if( position   ==  6U  )
		 {
			AL_PP_Set_RPM(rpm);
			AL_PP_StepperMotor_Setdirection(PP_Forward);
			HAL_TIM_PWM_Start(&htim1, TIM_CHANNEL_1);

				  my_stepper_var.check_opto_state[7] = HAL_GPIO_ReadPin(GPIOB,GPIO_PIN_6);  // read pin status

				  if(my_stepper_var.check_opto_state[7] == BIT_RESET)      // falling edge detection
				  {
						 AL_PP_StepperMotor_Enable(PP_Step_07, PP_Enable);
						 while((HAL_GPIO_ReadPin(GPIOB,GPIO_PIN_6))  == BIT_RESET){};
						 // disable motor
						 AL_PP_StepperMotor_Enable(PP_Step_07, PP_Disable);

				  }
				  else
				  {
						 AL_PP_StepperMotor_Enable(PP_Step_07, PP_Enable);       // rising edge detection
						 while((HAL_GPIO_ReadPin(GPIOB,GPIO_PIN_6))  == BIT_SET){};
						 while((HAL_GPIO_ReadPin(GPIOB,GPIO_PIN_6))  == BIT_RESET){};
						 // disable motor
						 AL_PP_StepperMotor_Enable(PP_Step_07, PP_Disable);


				  }
		   HAL_Delay(2);
		   HAL_TIM_PWM_Stop(&htim1, TIM_CHANNEL_1);
		 }




}

void AL_PP_Park(void)
{

    if( my_stepflags_var.park_home_flag == RESET_FLAG )
    {

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
		    HAL_GPIO_WritePin(GPIOE, GPIO_PIN_8, GPIO_PIN_SET);
			HAL_GPIO_WritePin(GPIOE, GPIO_PIN_9, GPIO_PIN_SET);
			HAL_GPIO_WritePin(GPIOE, GPIO_PIN_10, GPIO_PIN_SET);
			HAL_GPIO_WritePin(GPIOE, GPIO_PIN_11, GPIO_PIN_SET);
			HAL_GPIO_WritePin(GPIOE, GPIO_PIN_12, GPIO_PIN_SET);
			HAL_GPIO_WritePin(GPIOE, GPIO_PIN_13, GPIO_PIN_SET);
			HAL_GPIO_WritePin(GPIOE, GPIO_PIN_14, GPIO_PIN_SET);
			HAL_GPIO_WritePin(GPIOE, GPIO_PIN_15, GPIO_PIN_SET);

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

		    my_stepflags_var.park_home_flag = SET_FLAG;
    }

}

void AL_PP_COM_NV_Park(void)
{

	           // park pumps
			   AL_PP_Park_Home(1,INIT_RPM);
			   AL_PP_Park_Home(2,INIT_RPM);
			   AL_PP_Park_Home(3,INIT_RPM);
			   AL_PP_Park_Home(4,INIT_RPM);
			   AL_PP_Park_Home(5,INIT_RPM);
			   AL_PP_Park_Home(6,INIT_RPM);


			 //   HAL_Delay(100);
			    // sol on
			/*    HAL_GPIO_WritePin(GPIOE, GPIO_PIN_8, GPIO_PIN_SET);
				HAL_GPIO_WritePin(GPIOE, GPIO_PIN_9, GPIO_PIN_SET);
				HAL_GPIO_WritePin(GPIOE, GPIO_PIN_10, GPIO_PIN_SET);
				HAL_GPIO_WritePin(GPIOE, GPIO_PIN_11, GPIO_PIN_SET);
				HAL_GPIO_WritePin(GPIOE, GPIO_PIN_12, GPIO_PIN_SET);
				HAL_GPIO_WritePin(GPIOE, GPIO_PIN_13, GPIO_PIN_SET);
				HAL_GPIO_WritePin(GPIOE, GPIO_PIN_14, GPIO_PIN_SET);
				HAL_GPIO_WritePin(GPIOE, GPIO_PIN_15, GPIO_PIN_SET);
              */
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


}


void AL_PP_Solonoid_valve_reset(void)
{

	HAL_GPIO_WritePin(GPIOE, GPIO_PIN_8, GPIO_PIN_RESET);
	HAL_GPIO_WritePin(GPIOE, GPIO_PIN_9, GPIO_PIN_RESET);
	HAL_GPIO_WritePin(GPIOE, GPIO_PIN_10, GPIO_PIN_RESET);
	HAL_GPIO_WritePin(GPIOE, GPIO_PIN_11, GPIO_PIN_RESET);
	HAL_GPIO_WritePin(GPIOE, GPIO_PIN_12, GPIO_PIN_RESET);
	HAL_GPIO_WritePin(GPIOE, GPIO_PIN_13, GPIO_PIN_RESET);
	HAL_GPIO_WritePin(GPIOE, GPIO_PIN_14, GPIO_PIN_RESET);
	HAL_GPIO_WritePin(GPIOE, GPIO_PIN_15, GPIO_PIN_RESET);
}

void AL_PP_Solonoid_valve_on(void)
{
	HAL_GPIO_WritePin(GPIOE, GPIO_PIN_8, GPIO_PIN_SET);
	HAL_GPIO_WritePin(GPIOE, GPIO_PIN_9, GPIO_PIN_SET);
	HAL_GPIO_WritePin(GPIOE, GPIO_PIN_10, GPIO_PIN_SET);
	HAL_GPIO_WritePin(GPIOE, GPIO_PIN_11, GPIO_PIN_SET);
	HAL_GPIO_WritePin(GPIOE, GPIO_PIN_12, GPIO_PIN_SET);
	HAL_GPIO_WritePin(GPIOE, GPIO_PIN_13, GPIO_PIN_SET);
	HAL_GPIO_WritePin(GPIOE, GPIO_PIN_14, GPIO_PIN_SET);
	HAL_GPIO_WritePin(GPIOE, GPIO_PIN_15, GPIO_PIN_SET);
}

void AL_PP_ALL_Motor_switch(uint8_t sw)
{
	  AL_PP_StepperMotor_Enable(PP_Step_01, sw);
	  AL_PP_StepperMotor_Enable(PP_Step_02, sw);
	  AL_PP_StepperMotor_Enable(PP_Step_03, sw);
	  AL_PP_StepperMotor_Enable(PP_Step_04, sw);
	  AL_PP_StepperMotor_Enable(PP_Step_05, sw);
	  AL_PP_StepperMotor_Enable(PP_Step_06, sw);
	  AL_PP_StepperMotor_Enable(PP_Step_07, sw);
}












/**************************************************************************************************************/




