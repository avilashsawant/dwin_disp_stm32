/*
 * DWIN_DISP.c
 *
 *  Created on: Feb 12, 2025
 *      Author: Avilash.Sawant
 */

/*********** SYSTEM INCLUDES FILES*****************/
#include "DWIN_DISP.h"

/********** USER INCLUDES FILES ********************/


/********** variable declarations*******************/

       DwinHandleTypeDef      my_dwin_var;
       DwinFlagHandleTypeDef  my_dwin_flags;
       enum ScreenHandleTypeDef my_screen_num;

//extern configuration         config;
extern DMA_HandleTypeDef     hdma_usart3_rx;
extern UART_HandleTypeDef    huart3;
//extern DateTimeConfiguration DateTime;
//extern list_selected         list_select;





/********** USER variable declarations ********************/

unsigned char pp_version[6]={'V','6','.','1'};
unsigned char ppe_version[6]={'V','6','.','1','.','0'};
unsigned char hmi_version[4]={'V','1','.','0'};

char num_key_return[20];
unsigned char passowrd_login=0;
extern uint8_t no_of_sel_meth;
char print_string[MAX_PRINT_SIZE]={};
char empty_string[31];
char temp_tch=0;
char *ptr;
float num_return;
unsigned short pic_id;
unsigned char read_brightness,rom_data_rcvd;
char recvd_dta[30];
unsigned int tempp,tempq,tempr;
const unsigned short text_vp_arr[MAX_TEXT_VP]={0x1000,
		0x1004,
		0x1008,
		0x1012,
		0x1016,
		0x1020,
		0x8010
};
/*
const unsigned short text_vp_arr[MAX_TEXT_VP]={0x1000,
		0x1018,
		0x1030,
		0x1048,
		0x1060,
		0x1078,
		0x1090,
		0x10A8,
		0x10C0,
		0x10D8,
		0x10F0,
		0x1108,
		0x1120,
		0x1138,
		0x1150,
		0x1168,
		0x1180,
		0x1198,
		0x11B0,
		0x11C8,
		0x11E0,
		0x11F8,
		0x1210,
		0x1228,
		0x1240,
		0x1258,
		0x1270,
		0x1288,
		0x12A0,
		0x12B8,
		0x12D0,
		0x12E8,
		0x1300,
		0x1318,
		0x1330,
		0x1348,
		0x1360,
		0x1378,
		0x1390,
		0x13A8,
		0x13C0,
		0x13D8,
		0x13F0,
		0x1408,
		0x1420,
		0x1438,
		0x1450,
		0x1468,
		0x1480,
		0x1498,
		0x14B0,
		0x14C8,
		0x14E0,
		0x14F8,
		0x1510,
		0x1528,
		0x1540,
		0x1558,
		0x1570,
		0x1588,
		0x15A0,
		0x15B8,
		0x15D0,
		0x15E8,
		0x1600,
		0x1618,
		0x1630,
		0x1648,
		0x1660,
		0x1678,
		0x1690,
		0x16A8,
		0x16C0,
		0x16D8,
		0x16F0,
		0x1708,
		0x1720,
		0x1738,
		0x1750,
		0x1768,
		0x1780,
		0x1798,
		0x17B0,
		0x17C8,
		0x17E0,
		0x17F8,
		0x1810,
		0x1828,
		0x1840,
		0x1858,
		0x1870,
		0x1888,
		0x18A0,
		0x18B8,
		0x18D0,
		0x18E8,
		0x1900,
		0x1918,
		0x1930,
		0x1948,
		0x1960,
		0x1978,
		0x1990,
		0x19A8,
		0x19C0,
		0x19D8,
		0x19F0,
		0x1A08,
		0x1A20
};

*/
const unsigned short icon_vp_arr[MAX_ICON_VP]={0x2000,
		0x2018,
		0x2030,
		0x2048,
		0x2060,
		0x2078,
		0x2090,
		0x20A8,
		0x20C0,
		0x20D8,
		0x20F0,
		0x2108,
		0x2120,
		0x2138,
		0x2150,
		0x2168,
		0x2180,
		0x2198,
		0x21B0,
		0x21C8,
		0x21E0,
		0x21F8,
		0x2210,
		0x2228,
		0x2240,
		0x2258,
		0x2270,
		0x2288,
		0x22A0,
		0x22B8,
		0x22D0,
		0x22E8,
		0x2300,
		0x2318,
		0x2330,
		0x2348,
		0x2360,
		0x2378,
		0x2390,
		0x23A8,
		0x23C0,
		0x23D8,
		0x23F0,
		0x2408,
		0x2420,
		0x2438,
		0x2450,
		0x2468,
		0x2480,
		0x2498,
		0x24B0,
		0x24C8,
		0x24E0,
		0x24F8,
		0x2510,
		0x2528,
		0x2540,
		0x2558,
		0x2570,
		0x2588,
		0x25A0,
		0x25B8,
		0x25D0,
		0x25E8,
		0x2600,
		0x2618,
		0x2630,
		0x2648,
		0x2660,
		0x2678,
		0x2690,
		0x26A8,
		0x26C0,
		0x26D8,
		0x26F0,
		0x2708,
		0x2720,
		0x2738,
		0x2750,
		0x2768,
		0x2780,
		0x2798,
		0x27B0,
		0x27C8,
		0x27E0,
		0x27F8,
		0x2810,
		0x2828,
		0x2840,
		0x2858,
		0x2870,
		0x2888,
		0x28A0,
		0x28B8,
		0x28D0,
		0x28E8,
		0x2900,
		0x2918,
		0x2930,
		0x2948

};




/*
 *
 * Initialize LCD and start uart reception.
 * @param None
 * @retval None
 */
void AL_Dwin_Init(void)
{
    __HAL_DMA_DISABLE_IT(&hdma_usart3_rx,DMA_IT_HT);
	HAL_UARTEx_ReceiveToIdle_DMA(&DWIN_HMI, my_dwin_var.Rx_buff, DWIN_RX_BUFF_SIZE);

}


/*
 *
 * send DISPLAY one character
 * @param uint8_t
 * @retval None
 */
void AL_DWIN_Send_Data(uint8_t data)
{
	HAL_UART_Transmit(&DWIN_HMI, &data, 1, 2);
}


/*
 *start of command to display
 * @param None
 * @retval None
 * */
void AL_DWIN_Start(void)
{
	// frame header
	AL_DWIN_Send_Data(0x5A);
	AL_DWIN_Send_Data(0xA5);
}


/*
 *end of command to display
 * @param None
 * @retval None
 * */
void AL_DWIN_End(void)
{
	//frame tailor
	AL_DWIN_Send_Data(0xFF);
	AL_DWIN_Send_Data(0xFF);
}


/*
 * function to show picture on display
 * @param pic_id
 * @retval None
 * */
void AL_DWIN_Picshow(uint16_t pic_id)
{
	AL_DWIN_Start();
	AL_DWIN_Send_Data(0x07);
	AL_DWIN_Send_Data(DWIN_WRITE);
	AL_DWIN_Send_Data(0x00);
	AL_DWIN_Send_Data(0x84);
	AL_DWIN_Send_Data(0x5A);
	AL_DWIN_Send_Data(0x01);
	AL_DWIN_Send_Data((uint8_t)((pic_id&0xff00)>>8));
	AL_DWIN_Send_Data((uint8_t)(pic_id&0x00ff));


}

void AL_DWIN_Get_Current_Pic_Id(void)
{
    //	5A A5 04 83 0014 01
	uint16_t cntr=0;
	pic_id=0;
	AL_DWIN_Start();
	AL_DWIN_Send_Data(0x04);
	AL_DWIN_Send_Data(DWIN_READ);
	AL_DWIN_Send_Data(0);
	AL_DWIN_Send_Data(0x14);
	AL_DWIN_Send_Data(0x01);
	while(pic_id==0)
	{
		cntr++;
		if(cntr > 65534)
		break;
	}
}
/*
 *
 * To display icon
 *
 */

void AL_DWIN_Icon(uint8_t VP_arr_pointer, uint16_t x_coord, uint16_t y_coord, uint16_t icon_no)
{
	AL_DWIN_Start();
	AL_DWIN_Send_Data(0x0f);
	AL_DWIN_Send_Data(DWIN_WRITE);
	AL_DWIN_Send_Data((uint8_t)((icon_vp_arr[VP_arr_pointer]&0xff00)>>8));
	AL_DWIN_Send_Data((uint8_t)(icon_vp_arr[VP_arr_pointer]&0x00ff));
	AL_DWIN_Send_Data(0x30);
	AL_DWIN_Send_Data(0x07);
	AL_DWIN_Send_Data(0x00);
	AL_DWIN_Send_Data(0x01);

	AL_DWIN_Send_Data((uint8_t)((x_coord & 0xff00)>>8));
	AL_DWIN_Send_Data((uint8_t)(x_coord & 0x00ff));


	AL_DWIN_Send_Data((uint8_t)((y_coord & 0xff00)>>8));
	AL_DWIN_Send_Data((uint8_t)(y_coord & 0x00ff));

	AL_DWIN_Send_Data((uint8_t)((icon_no & 0xff00)>>8));
	AL_DWIN_Send_Data((uint8_t)(icon_no & 0x00ff));

	AL_DWIN_End();
}


uint8_t AL_DWIN_Read_Vp(uint8_t VP_arr_pointer, uint8_t data_length)
{
    //	5a a5 04 83 40 00 01

	my_dwin_var.Main_buff[7]= 0;
	AL_DWIN_Start();
	AL_DWIN_Send_Data(data_length+3);
	AL_DWIN_Send_Data(DWIN_READ);
	AL_DWIN_Send_Data((uint8_t)((text_vp_arr[VP_arr_pointer]&0xff00)>>8));
	AL_DWIN_Send_Data((uint8_t)(text_vp_arr[VP_arr_pointer]&0x00ff));
	AL_DWIN_Send_Data(data_length);

	return my_dwin_var.Main_buff[7];
}


void AL_DWIN_Write_VP(uint16_t VP_add, uint16_t data)
{
    //	5a a5 05 82 10 00 00 97
	AL_DWIN_Start();
	AL_DWIN_Send_Data(0x05);
	AL_DWIN_Send_Data(DWIN_WRITE);
	AL_DWIN_Send_Data((uint8_t)((VP_add & 0xff00)>>8));
	AL_DWIN_Send_Data((uint8_t)(VP_add & 0x00ff));
	AL_DWIN_Send_Data((uint8_t)((data & 0xff00)>>8));
    AL_DWIN_Send_Data((uint8_t)(data & 0x00ff));
    HAL_Delay(5);
}

/*
 *Function to print any string on DWIN LCD
 *
 * */
void AL_DWIN_Text_Print(uint8_t VP_arr_pointer, uint8_t data_length, const char * print_str )
{
	uint16_t text_roll_VP[]={71,74,76,77};		//add VP pointers with roll text in this arrey
	uint8_t rolltext=0;
	for(uint16_t i=0;i<sizeof(text_roll_VP)/sizeof(uint16_t);i++)
	{
		if(text_roll_VP[i]==VP_arr_pointer)
		{
			rolltext=1;
		}
	}
	if(rolltext)
	{
		AL_DWIN_Clear_Text(VP_arr_pointer, 36);
	}
	AL_DWIN_Start();
	if(data_length%2==0)
		AL_DWIN_Send_Data(data_length+3);
	else
		AL_DWIN_Send_Data(data_length+4);
	    AL_DWIN_Send_Data(DWIN_WRITE);
	    AL_DWIN_Send_Data((uint8_t)((text_vp_arr[VP_arr_pointer]&0xff00)>>8));
	if(rolltext)
		AL_DWIN_Send_Data((uint8_t)(text_vp_arr[VP_arr_pointer]&0x00ff)+3);
	else
		AL_DWIN_Send_Data((uint8_t)(text_vp_arr[VP_arr_pointer]&0x00ff));
	for(uint8_t i=0;data_length!=i;i++)
	{
		AL_DWIN_Send_Data(print_str[i]);
	}
	if(data_length%2==1)
	{
		AL_DWIN_Send_Data(0x20);
	}
	AL_DWIN_End();
}


void AL_DWIN_Text_Show(uint16_t vp,uint8_t data1,uint8_t data2,uint8_t data3)
{
	 AL_DWIN_Start();
	 AL_DWIN_Send_Data(5+3);
	 AL_DWIN_Send_Data(DWIN_WRITE);
	 AL_DWIN_Send_Data((vp & 0xFF00)>>8);
	 AL_DWIN_Send_Data(vp & 0x00FF);
	 AL_DWIN_Send_Data(data1);
	 AL_DWIN_Send_Data(data2);
	 AL_DWIN_Send_Data(data3);
	 AL_DWIN_End();
}

void AL_DWIN_Bit_Icon_Display(uint16_t VP,uint16_t data)
{
	AL_DWIN_Start();
	AL_DWIN_Send_Data(5);
	AL_DWIN_Send_Data(DWIN_WRITE);
	AL_DWIN_Send_Data((uint8_t)((VP&0xff00)>>8));
	AL_DWIN_Send_Data((uint8_t)(VP&0x00ff));
	AL_DWIN_Send_Data((uint8_t)((data&0xff00)>>8));
	AL_DWIN_Send_Data((uint8_t)(data&0x00ff));
}

/*
 *Function to print floating number on DWIN LCD
 *
 * */
void AL_DWIN_Print_Float_Number(uint8_t VP_arr_pointer,  float num )
{
	char print_string[30]={0};
	uint8_t len = sprintf(print_string,"%2.2f  ",num); // @suppress("Float formatting support")
	AL_DWIN_Start();
	AL_DWIN_Send_Data(len+3);
	AL_DWIN_Send_Data(DWIN_WRITE);
	AL_DWIN_Send_Data((uint8_t)((text_vp_arr[VP_arr_pointer]&0xff00)>>8));
	AL_DWIN_Send_Data((uint8_t)(text_vp_arr[VP_arr_pointer]&0x00ff));
	for(uint8_t i=0;6!=i;i++)
	{
		AL_DWIN_Send_Data(print_string[i]);
	}
	AL_DWIN_End();

	#if(DWIN_USE_RTOS)
		osDelay(100);
	#else
	HAL_Delay(100);
	#endif
}
/*
 *Function to print integer number on DWIN LCD
 *
 * */
void AL_DWIN_Print_Int_Number(uint8_t VP_arr_pointer, int num, char	*format_specifer )
{
	char print_string[30]={0};
	//clear_text(VP_arr_pointer, 2);
	uint8_t len = sprintf(print_string,format_specifer,num);
	AL_DWIN_Start();
	AL_DWIN_Send_Data(len+3);
	AL_DWIN_Send_Data(DWIN_WRITE);
	AL_DWIN_Send_Data((uint8_t)((text_vp_arr[VP_arr_pointer]&0xff00)>>8));
	AL_DWIN_Send_Data((uint8_t)(text_vp_arr[VP_arr_pointer]&0x00ff));
	for(uint8_t i=0;len!=i;i++)
	{
		AL_DWIN_Send_Data(print_string[i]);
	}
	AL_DWIN_End();
}
/*
 * clear text box
 *
 */
void AL_DWIN_Clear_Text(uint16_t VP_pointer, uint8_t data_length)
{
	AL_DWIN_Start();
	AL_DWIN_Send_Data(data_length+3);
	AL_DWIN_Send_Data(DWIN_WRITE);
	AL_DWIN_Send_Data((uint8_t)((VP_pointer & 0xff00)>>8));
	AL_DWIN_Send_Data((uint8_t)(VP_pointer & 0x00ff));
	for(uint8_t i=0;i < data_length;i++)
	{
		AL_DWIN_Send_Data(0);
	}
}

/*
 * clear icon
 *
 */
void AL_DWIN_Clear_Icon(uint8_t VP_arr_pointer)
{
	AL_DWIN_Start();
	AL_DWIN_Send_Data(0x1B);
	AL_DWIN_Send_Data(DWIN_WRITE);
	AL_DWIN_Send_Data((uint8_t)((icon_vp_arr[VP_arr_pointer]&0xff00)>>8));
	AL_DWIN_Send_Data((uint8_t)(icon_vp_arr[VP_arr_pointer]&0x00ff));
	for(uint8_t i=0;0x18!=i;i++)
	{
		AL_DWIN_Send_Data(0x20);
	}
}
void AL_DWIN_Change_Text_Colour(uint16_t sp_locator,uint16_t Font_colour)
{
	AL_DWIN_Start();
	AL_DWIN_Send_Data(0x05);
	AL_DWIN_Send_Data(DWIN_WRITE);
	AL_DWIN_Send_Data((uint8_t)((sp_locator&0xff00)>>8));
	AL_DWIN_Send_Data((uint8_t)((sp_locator&0x00ff)+3));
	AL_DWIN_Send_Data((uint8_t)((Font_colour&0xff00)>>8));		//font color
	AL_DWIN_Send_Data((uint8_t)((Font_colour&0x00ff)));				//font color
	#if(DWIN_USE_RTOS)
		osDelay(100);
	#else
	    HAL_Delay(100);
	#endif

}

/*
void Change_text_size(uint16_t sp_locator,uint8_t Font_size)
{
	start();
	sendlcd(0x05);
	sendlcd(WRITE_LCD);
	sendlcd((uint8_t)((sp_locator&0xff00)>>8));
	sendlcd((uint8_t)((sp_locator&0x00ff)+0x0A));
	sendlcd(Font_size);	//font size
	sendlcd(Font_size);	//font size
#if(DWIN_USE_RTOS)
	osDelay(100);
#else
HAL_Delay(100);
#endif
}
void Text_highlight(uint16_t sp_locator,uint8_t selection,uint8_t selection_bits)
{

	if(selection==0)
	{
		Change_text_colour(sp_locator,WHITE_COLOUR);
		//Change_text_size(sp_locator,0x28);
	}
	else
	{
		Change_text_colour(sp_locator,BLACK_COLOUR);
		//Change_text_size(sp_locator,0x30);
	}
	bit_icon_display(LIST_HIGHLIGHT_BIT_VP, selection_bits);

}

void Disable_all_program()
{
	for(uint8_t i=0;i<5;i++)
	{
		//if(arr[i]==0)
			//continue;
		Text_highlight(((i)*0x18)+0x4000, 0,0);

	}
	no_of_sel_meth=0;
	list_select.list_pos_sel=0;
}*/



/*
 * buzzer beeps for 1 second
 */
void AL_DWIN_Beep(void)
{

	AL_DWIN_Start();
	AL_DWIN_Send_Data(0x05);
	AL_DWIN_Send_Data(DWIN_WRITE);
	AL_DWIN_Send_Data(0);
	AL_DWIN_Send_Data(0xA0);
	AL_DWIN_Send_Data(0);
	AL_DWIN_Send_Data(0x7D);
}

/*
 * write RTC data to Display
 */

/*
void AL_DWIN_Write_RTC_Data()
{
	AL_DWIN_Start();
	AL_DWIN_Send_Data(0x0B);
	AL_DWIN_Send_Data(DWIN_WRITE);
	AL_DWIN_Send_Data(0);
	AL_DWIN_Send_Data(0x10);
	AL_DWIN_Send_Data(DateTime.Year);
	AL_DWIN_Send_Data(DateTime.Month);
	AL_DWIN_Send_Data(DateTime.Date);
	AL_DWIN_Send_Data(0);
	AL_DWIN_Send_Data(DateTime.Hours);
	AL_DWIN_Send_Data(DateTime.Minutes);
	AL_DWIN_Send_Data(DateTime.Sec);
	AL_DWIN_Send_Data(0);
}

*/


void  AL_DWIN_Set_Brightness(uint8_t Brightness)
{
	AL_DWIN_Start();
	AL_DWIN_Send_Data(0x04);
	AL_DWIN_Send_Data(DWIN_WRITE);
	AL_DWIN_Send_Data(0);
	AL_DWIN_Send_Data(0x82);
	AL_DWIN_Send_Data(Brightness);

}
/*
void Write_flash_LCD(uint16_t ROM_add, char * str, uint8_t len)
{
	uint8_t temp_str[30]={0};
	memcpy(temp_str,str,len);
	start();
	sendlcd(len+3);
	sendlcd(WRITE_LCD);
	sendlcd((uint8_t)((FLASH_VAR_ADDRESS&0xff00)>>8));
	sendlcd((uint8_t)(FLASH_VAR_ADDRESS&0x00ff));
	for(uint8_t i=0;len!=i;i++)
	{
		sendlcd(temp_str[i]);
	}
	osDelay(200);
	start();
	sendlcd(0x0b);
	sendlcd(WRITE_LCD);
	sendlcd(0);
	sendlcd(0x08);
	sendlcd(0xA5);
	sendlcd(0);
	sendlcd((uint8_t)((ROM_add&0xff00)>>8));
	sendlcd((uint8_t)(ROM_add&0x00ff));
	sendlcd((uint8_t)((FLASH_VAR_ADDRESS&0xff00)>>8));
	sendlcd((uint8_t)(FLASH_VAR_ADDRESS&0x00ff));
	sendlcd(0);
	sendlcd(len/2);
}

void Read_flash_LCD(uint16_t ROM_add, uint8_t len)
{
	uint8_t cntr=0;
	start();
	sendlcd(0x0b);
	sendlcd(WRITE_LCD);
	sendlcd(0);
	sendlcd(0x08);
	sendlcd(0x5A);
	sendlcd(0);
	sendlcd((uint8_t)((FLASH_VAR_ADDRESS&0xff00)>>8));
	sendlcd((uint8_t)(FLASH_VAR_ADDRESS&0x00ff));
	sendlcd((uint8_t)((ROM_add&0xff00)>>8));
	sendlcd((uint8_t)(ROM_add&0x00ff));
	sendlcd(0);
	sendlcd(len);
	osDelay(1000);
	start();
	sendlcd(0x04);
	sendlcd(READ_LCD);
	sendlcd((uint8_t)((FLASH_VAR_ADDRESS&0xff00)>>8));
	sendlcd((uint8_t)(FLASH_VAR_ADDRESS&0x00ff));
	sendlcd(len);
	while(rom_data_rcvd==0)
	{
		cntr++;
		osDelay(30);
		if(cntr==500)
			break;
	}
	if(rom_data_rcvd==1)
	{
		memcpy(recvd_dta,&Main_buff[7],len);
	}
}
uint8_t Get_brightness(void)
{
	uint8_t cntr=0;
	read_brightness=0;
	start();
	sendlcd(0x04);
	sendlcd(READ_LCD);
	sendlcd(0);
	sendlcd(0x31);
	sendlcd(0x01);
	while(!read_brightness)
	{
		osDelay(100);
		cntr++;
		if(cntr>10)
			break;
	}
	if(read_brightness<10||read_brightness>100)
	{
		read_brightness=50;
	}
	return read_brightness;
}
*/

/*
 * keypad to get user input
 * @param  string to save data, length of data
 * @retval  None
 */
/*
void keypad(char * str,uint8_t len)
{
	uint8_t cntr=0;
	char temp_str[30]={"\0"};
	strcpy(temp_str,str);
	clear_text(KEYPAD_TEXT_PRINT_VP, 30);
	text_print(KEYPAD_TEXT_PRINT_VP,strlen(temp_str),temp_str);
	picshow(KEYPAD_IMAGE);
	touch_key=0;

	while(1)
	{


		if(touch_key!=0)
		{
			 temp_tch=(char)(touch_key&0x00ff);
			 touch_key=0;
		}
		if(temp_tch==KEYPAD_ENTER)		// save string and exit
		{

			memcpy(str,temp_str,len);
//			strncpy(str,temp_str,len);
			touch_key=0;
			temp_tch=0;
			break;
		}
		else if(temp_tch==KEYPAD_BACK)	//exit without saving
		{
			touch_key=0;
			temp_tch=0;
			break;
		}
		else if(temp_tch==KEYPAD_DELETE_CHAR&&strlen(temp_str)>0)		//Delete character
		{
			temp_str[strlen(temp_str)-1] = '\0';
			if(passowrd_login==0)
				text_print(KEYPAD_TEXT_PRINT_VP,sizeof(temp_str),temp_str);
			else if (passowrd_login==1)
			{
				clear_text(KEYPAD_TEXT_PRINT_VP, sizeof(temp_str));
				text_print(KEYPAD_TEXT_PRINT_VP,strlen(temp_str),pass_print);
			}
			touch_key=0;
			temp_tch=0;
		}
		else if(temp_tch>=0x20 && temp_tch<=0x7E &&	len>strlen(temp_str))	//Add character
		{
			strncat(temp_str, &temp_tch,1);
			if(passowrd_login==0)
				text_print(KEYPAD_TEXT_PRINT_VP,strlen(temp_str),temp_str);
			else if (passowrd_login==1)
			{
				text_print(KEYPAD_TEXT_PRINT_VP,strlen(temp_str),pass_print);
			}
			touch_key=0;
			temp_tch=0;
		}

		cntr++;
		if(cntr==50)
		{
			uart_active=0;
			print_float_number(BATH_TEMP_TEXT, ds18b20[config.Bath_sensor].Temperature);
			cntr=0;
		}
		if(uart_active==0)
		{
			Dwin_LCD_init();		//FOR DISPLAY STUCK PROBLEM
		}

		osDelay(50);
	}
}*/
/*
 * To receive data from display
 */
void AL_DWIN_Uart_Receive(void)
{
	uint8_t point=0;
//	memset(Rx_buff,0,25);
	//if(HMIpntr!=0)
	//{
	  // 5a a5 06 83 80 00 01 10 00    	// key data return
	  // 5a a5 03 82 4f 4b				// feedback data
	//uart_active=1;
	  if(my_dwin_var.Main_buff[0]==0x5a)
	  {
		  if(my_dwin_var.Main_buff[1]==0xa5)
		  {
			  if(my_dwin_var.Main_buff[2]==0x06 && my_dwin_var.Main_buff[3]==0x83 &&
					  my_dwin_var.Main_buff[4]==((TOUCH_VP&0xFF00)>>8) && my_dwin_var.Main_buff[5]==(TOUCH_VP&0x00FF))
			  {
				    my_dwin_var.touch_key=(uint16_t)((my_dwin_var.Main_buff[7]<<8)|my_dwin_var.Main_buff[8]);

			  }
			  else if(my_dwin_var.Main_buff[3]==0x83 && my_dwin_var.Main_buff[4]==((FLASH_VAR_ADDRESS&0xFF00)>>8) && my_dwin_var.Main_buff[5]==(FLASH_VAR_ADDRESS&0x00FF))
			  {
					rom_data_rcvd=1;
			  }
//			  5a a5 06 83 00 31 01 5a 0a
			  else if(my_dwin_var.Main_buff[2]==0x06 && my_dwin_var.Main_buff[3]==0x83 && my_dwin_var.Main_buff[5]==0x31 && my_dwin_var.Main_buff[7]==0x5A)
			  {
				  read_brightness=my_dwin_var.Main_buff[8];
			  }
			  else if(my_dwin_var.Main_buff[3]==0x83 && my_dwin_var.Main_buff[2]>=0x06)// && Main_buff[4]==((KEYPAD_RETURN_VP&0xFF00)>>8) && Main_buff[5]==(KEYPAD_RETURN_VP&0x00FF))
			  {

				  if(my_dwin_var.Main_buff[4]==0 && my_dwin_var.Main_buff[5]==0x14)
				  {
//					  5a a5 06 83 00 14 01 00 21
					  pic_id=(uint16_t)((my_dwin_var.Main_buff[7]<<8)|my_dwin_var.Main_buff[8]);
				  }
				  else
				  {//5a a5 0a 83 50 50 03 36 33 39 32 ff ff
					  my_dwin_var.saved_vp=(uint16_t)((my_dwin_var.Main_buff[4]<<8)|my_dwin_var.Main_buff[5]);
					memset(num_key_return,0,20);
					for(int i=0;i<(my_dwin_var.Main_buff[6]*2);i++)
					{
						if(my_dwin_var.Main_buff[i+7]==0xff && my_dwin_var.Main_buff[i+8]==0xff)
							break;
						if(my_dwin_var.Main_buff[i+7]==46)
							point++;
						if(point>1)
						{
							//point--;
							//continue;
						//	memset(num_key_return,0,20);
							//clear_text(((saved_vp-0x1000)/0x18), 20);
						//	invalid_entry_error();
							my_dwin_var.num_key_return[i-point+1]=(char)(my_dwin_var.Main_buff[i+7]);
						}
						//else if(point==1)
						else
							my_dwin_var.num_key_return[i]=(char)(my_dwin_var.Main_buff[i+7]);
					}
					num_return=atof(num_key_return);
					if(point>1)
					{
						AL_DWIN_Clear_Text(((my_dwin_var.saved_vp-0x1000)/0x18), 20);
						AL_DWIN_Print_Float_Number(((my_dwin_var.saved_vp-0x1000)/0x18), num_return);
					}
				  }
			  }
			  if(my_dwin_var.Main_buff[2]==0x03 && my_dwin_var.Main_buff[3]==0x82 &&
					  my_dwin_var.Main_buff[4]==0x4F&& my_dwin_var.Main_buff[5]==0x4B)
			  {
				  my_dwin_var.uart_active=1;
			  }

		  }

	  }
	  else
	  {

	  }
}


void AL_DWIN_Toggle(uint32_t delay_ms)
{
	    my_dwin_var.touch_key = 0;              // clear key value
	    // toggle led at pin 13 for given delay_ms
		HAL_GPIO_WritePin(GPIOC, GPIO_PIN_13, GPIO_PIN_SET);
		HAL_Delay(delay_ms);
		HAL_GPIO_WritePin(GPIOC, GPIO_PIN_13, GPIO_PIN_RESET);
		HAL_Delay(delay_ms);
}


void AL_DWIN_CONV_NUM_ARR(void)
{
	tempp = PP_PASSWORD;
	for(unsigned int i= 0; i < PASSWORD_SIZE; i++)
	{

		  tempr = tempp % 10;                 // num % 10 = remainder
		  tempp = tempp / 10;                 // num /10 = quotient
		  my_dwin_var.temp_passwd[5-i] = tempr + 0x30;
	}

}
/**************** EOF **************************************/



