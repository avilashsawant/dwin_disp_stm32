/*
 * STEPPERPP.h
 *
 *  Created on: Feb 25, 2025
 *      Author: Avilash.Sawant
 */

#ifndef _STEPPERPP_H_
#define _STEPPERPP_H_

// RPM according to process

#define HOME_RPM      100
#define CLEAN_RPM     200
#define FLUSH1_RPM    225
#define FLUSH2_RPM    150
#define FLUSH3_RPM    CLEAN_RPM


#define INIT_RPM      HOME_RPM
#define CHECK_RPM     100
#define MAX_RAMP_RPM  90



// test motor num
#define CHECK_NUM     80

// test motor time to run
#define SET_TIME      5000   // 2 msec/count
// Define the number of steps per revolution and the RPM
/*
 *   0 - full step mode
 *   2 - half step mode
 *   4 - quarter step mode
 *   8 - 4 half step mode
 *   16 - 8 half step mode
 */
#define SPR_SEL    8

#if    SPR_SEL == 0

      #define   STEPS_PER_REV   200         // angle 1.8 deg

#elif  SPR_SEL == 2

      #define   STEPS_PER_REV   400

#elif  SPR_SEL == 4

      #define   STEPS_PER_REV   800

#elif  SPR_SEL == 8

      #define   STEPS_PER_REV   1600

#elif  SPR_SEL ==  16

      #define   STEPS_PER_REV   3200

#endif

#define DISPENSE_PER_REV        (double)(0.100)        // ml/rev

#define DISPENSE_PER_STEP       (double)(DISPENSE_PER_REV/ (double)STEPS_PER_REV)      // ml/step



#define MAX_WAIT_TIME 5    // 1 msec/count
#define MAX_STEP_RUN  5

#define START_RPM     50

#define PP_Step_01    GPIO_PIN_0
#define PP_Step_02    GPIO_PIN_1
#define PP_Step_03    GPIO_PIN_2
#define PP_Step_04    GPIO_PIN_3
#define PP_Step_05    GPIO_PIN_4
#define PP_Step_06    GPIO_PIN_5
#define PP_Step_07    GPIO_PIN_6
#define PP_Step_08    GPIO_PIN_7

#define PP_Enable     GPIO_PIN_RESET
#define PP_Disable    GPIO_PIN_SET

#define PP_Forward    GPIO_PIN_RESET
#define PP_Reversed   GPIO_PIN_SET

#define SET_FLAG      1U
#define RESET_FLAG    0U

#define BIT_SET       1U
#define BIT_RESET     0U
// Time tracking variables

typedef struct
{
		    uint32_t last_step_count;
		    uint32_t last_time ; // Timer value when last RPM was calculated
   volatile uint32_t step_count ;
               float current_rpm ;

               float rps;
            uint32_t run_time;
               float read_rpm;

            uint32_t start_rpm;
            uint32_t jump_rpm;
            uint32_t rpm_wait_time;
            uint8_t  check_opto_state[10];
            uint32_t rpm_ticks;

}stepperHandletypeDef;



typedef struct
{
        	uint8_t flag_1sec:1;
        	uint8_t jump_flag:1;


        	uint8_t opto_1_flag:1;
        	uint8_t opto_2_flag:1;
        	uint8_t opto_3_flag:1;
        	uint8_t opto_4_flag:1;
        	uint8_t opto_5_flag:1;
        	uint8_t opto_6_flag:1;
        	uint8_t opto_7_flag:1;
        	uint8_t opto_8_flag:1;
        	uint8_t mot_home_flag:1;
            uint8_t park_home_flag:1;



}stepperFlagsHandletypeDef;



void AL_PP_StepperMotor_Setdirection(uint8_t direction);
void AL_PP_StepperMotor_Enable(uint16_t sel_motor, uint8_t sig);
void AL_PP_Set_RPM(uint32_t rpm) ;
void AL_PP_Calculate_RPM(void);

void AL_PP_Start_Motor(uint16_t mot_no , uint32_t rpm, uint8_t dir);
void AL_PP_Stop_Motor(uint16_t mot_no);

void AL_PP_ALL_Stop_Motor(void);
void AL_PP_ALL_Start_Motor(uint32_t rpm, uint8_t dir);

void AL_PP_test_motors(void);

void AL_PP_Home_forward(uint8_t position, uint32_t rpm);
void AL_PP_Home_reversed(uint8_t position, uint32_t rpm);


void AL_PP_Park_Home(uint8_t position, uint32_t rpm);
void AL_PP_Park(void);
void AL_PP_Solonoid_valve_reset(void);
void AL_PP_Stepper_run(void);
void AL_PP_COM_NV_Park(void);
void AL_PP_Solonoid_valve_on(void);


void AL_PP_ALL_Motor_switch(uint8_t sw);


#endif
/* INC_STEPPERPP_H_ */





