/**
 ******************************************************************************
 * @file    	  usart_soft.c
 * @brief   	  Functions to use the USART specific to the Jammerbox Project
 * 				  Project name	: STM32-Jammerbox
 * 				  Controller	: STM32F103RB
 * @date		  May 27, 2022
 ******************************************************************************
 * @attention
 *
 * All rights reserved.
 *
 * This software is licensed under terms that can be found in the LICENSE file
 * in the root directory of this software component.
 * If no LICENSE file comes with this software, it is provided AS-IS.
 *
 ******************************************************************************
 */

// ### Basic includes ###
#include "../../JammerBox_Source/header/usart_soft.h"

#include <stdbool.h>
#include <string.h>
#include <stdlib.h>

// ### Hardware specific includes ###

#include "usart.h"
#include "tim.h"

// ### Program includes ###
#include "../../JammerBox_Source/header/failures.h"
#include "../../JammerBox_Source/header/system_configuration.h"
#include "../../JammerBox_Source/header/timer.h"

/* External variables --------------------------------------------------------*/

//** Receive **
extern int char_counter;
extern int data_counter;
extern bool receiving;
extern int input_char_counter;
extern bool message_received;
extern bool com_error;
extern char in;
extern char start_char;
extern char end_char;
extern char input_chars[];
extern char temp_chars_1[];
extern char temp_chars_2[];
extern char temp_chars_3[];
extern char temp_chars_4[];
extern char temp_chars_5[];
extern char temp_chars_6[];
extern char temp_chars_7[];
extern char temp_chars_8[];
extern char temp_chars_9[];
extern char temp_chars_10[];
extern char message_identify;

//** CRK_CAM-configuration **
extern bool configuration_complete;

//** CRK-configuration **
extern int number_teeth_CRK;
extern int number_miss_teeth;
extern int number_gap;
extern double tdc_to_gap;
extern double factor_filter_time_CRK;
extern double first_seg_angle;
extern int number_cylinder;
extern double gap_ratio_CRK_DET;
extern double gap_ratio_CRK_VLD;
extern bool CRK_synch;
extern bool CRK_synch_temp;
extern bool CRK_config;
extern char sensortype_CRK;

//** CAM-configuration **
extern unsigned int number_active_edges_CAM[2];
extern double filter_time_CAM[2];
extern double CAM_edges[2][24];
extern bool CRK_CAM_synch;
extern bool CAM_config;
extern char active_CAM_edges[2];
extern char sensortype_CAM[2];
extern int number_of_CAM;
extern int cam_setup_counter;

//** Send **
extern char message[];

//** Failure treatment **
extern bool failure_configured;
extern bool failure_configured_CAM_blank_out;
extern bool failure_set;
extern bool failure_active;
extern char failure_identify;
extern int active_cam_failure;

//** CRK_RUN_OUT **
extern char sc_type_CRK_RUN_OUT;
extern unsigned int angle_to_start_failure_CRK_RUN_OUT;
extern unsigned int failure_period_CRK_RUN_OUT;

//** CAM_PER **	
extern unsigned int number_edges_CAM_PER;
extern double microsecond_per_timer_tick_CAM_PER__CRK_TOOTH_PER;
extern bool all_active_edges_CAM_PER;
extern char active_edges_CAM_PER;

//** CRK_TOOTH_PER **
extern unsigned int start_tooth_CRK_TOOTH_PER;
extern unsigned int number_pulses_CRK_TOOTH_PER;
extern unsigned int engine_revolutions_CRK_TOOTH_PER;
extern double calculation_factor_CRK_TOOTH_PER;

//** CAM_DELAY(CAM_TOOTH_OFF / CAM_REF_CRK / CAM_SYN / CAM_SYN_CRK) **
extern double delay_angle_CAM_delay;
extern double delay_factor_CAM_delay;

//** CAM_REF_CRK **											// t: time / c: �CRK	
extern long double angle_time_to_start_failure_CAM_REF_CRK; // Value Delay (ms or �CRK)			
extern char delay_type_CAM_REF_CRK;

//** CRK_TOOTH_OFF **
extern unsigned int number_tooth_off;

//** SEG_ADP_ER_LIM **
extern double crk_delay_SEG_ADP_ER_LIM;

//** CRK_PLS_ORNG **
extern double crk_pulse_duration_CRK_PLS_ORNG;

//** POSN_ENG_STST **
extern unsigned int crk_teeth_off_POSN_ENG_STST;

//** CAM_PAT_ERR **
extern unsigned int active_CAM_edges_counter;

//** SC_CAM_CRK **
extern unsigned int sc_type_SC_CAM_CRK;

//** label indicating engine start **
extern bool engine_start;

//***************** Communication validation ************
extern bool communication_active;
extern bool communication_ready;

//**Signal recording of CRK and CAM**
extern volatile bool should_record;

/* Public functions ----------------------------------------------------------*/

void USART_ProcessMessage(void)
{

	data_counter = 0; //Set data counter to 0

	input_char_counter = 0; //Set input char counter to 0

	message_identify = input_chars[1]; //Read message identifier

	int i; //Set counter variable to read received message in for-loop

	if (message_identify == '2' && CAM_config == false)
	{
		for (i = 1; i < strlen(input_chars); i++)
		{
			char temp = input_chars[i];

			if (temp == '/')
			{
				if (data_counter > 4)
				{
					CAM_edges[cam_setup_counter][data_counter - 5] = atof(
							temp_chars_5);
					temp_chars_5[0] = '\0';
				}

				data_counter++;
				input_char_counter = 0;
			}
			else if (data_counter > 0 && data_counter < 5)
			{
				switch (data_counter)
				{

				case 1:
				{
					temp_chars_1[input_char_counter] = temp;

					temp_chars_1[input_char_counter + 1] = '\0';

					break;
				}
				case 2:
				{
					temp_chars_2[input_char_counter] = temp;

					temp_chars_2[input_char_counter + 1] = '\0';

					break;
				}
				case 3:
				{
					temp_chars_3[input_char_counter] = temp;

					temp_chars_3[input_char_counter + 1] = '\0';

					break;
				}
				case 4:
				{
					temp_chars_4[input_char_counter] = temp;

					temp_chars_4[input_char_counter + 1] = '\0';

					break;
				}

				}

				input_char_counter++;
			}
			else
			{
				temp_chars_5[input_char_counter] = temp;

				temp_chars_5[input_char_counter + 1] = '\0';

				input_char_counter++;

			}

			if (i == strlen(input_chars) - 1)
			{
				CAM_edges[cam_setup_counter][data_counter - 5] = atof(
						temp_chars_5);
				temp_chars_5[0] = '\0';

				input_char_counter = 0;
			}

		}

		number_active_edges_CAM[cam_setup_counter] = atoi(temp_chars_1);

	}
	else
	{
		for (i = 1; i < strlen(input_chars); i++)
		{
			char temp = input_chars[i];

			if (temp == '/')
			{
				data_counter++;
				input_char_counter = 0;
			}
			else if (data_counter > 0)
			{
				switch (data_counter)
				{

				case 1:
				{
					temp_chars_1[input_char_counter] = temp;

					temp_chars_1[input_char_counter + 1] = '\0';

					break;
				}
				case 2:
				{
					temp_chars_2[input_char_counter] = temp;

					temp_chars_2[input_char_counter + 1] = '\0';

					break;
				}
				case 3:
				{
					temp_chars_3[input_char_counter] = temp;

					temp_chars_3[input_char_counter + 1] = '\0';

					break;
				}
				case 4:
				{
					temp_chars_4[input_char_counter] = temp;

					temp_chars_4[input_char_counter + 1] = '\0';

					break;
				}
				case 5:
				{
					temp_chars_5[input_char_counter] = temp;

					temp_chars_5[input_char_counter + 1] = '\0';

					break;
				}
				case 6:
				{
					temp_chars_6[input_char_counter] = temp;

					temp_chars_6[input_char_counter + 1] = '\0';
					break;
				}
				case 7:
				{
					temp_chars_7[input_char_counter] = temp;

					temp_chars_7[input_char_counter + 1] = '\0';

					break;
				}
				case 8:
				{
					temp_chars_8[input_char_counter] = temp;

					temp_chars_8[input_char_counter + 1] = '\0';

					break;
				}
				case 9:
				{
					temp_chars_9[input_char_counter] = temp;

					temp_chars_9[input_char_counter + 1] = '\0';

					break;
				}
				case 10:
				{
					temp_chars_10[input_char_counter] = temp;

					temp_chars_10[input_char_counter + 1] = '\0';

					break;
				}

				}

				input_char_counter++;

			}
		}
	}

	//set specific variables of actual message
	switch (message_identify)
	{
	case ('1'): // CRK configuration
	{
		if (CRK_config == false)
		{
			if (data_counter == 7)
			{
				number_teeth_CRK = atoi(temp_chars_1);
				number_miss_teeth = atoi(temp_chars_2);
				number_gap = atoi(temp_chars_3);
				tdc_to_gap = atof(temp_chars_4);
				first_seg_angle = atof(temp_chars_5);
				number_cylinder = atoi(temp_chars_6);
				sensortype_CRK = temp_chars_7[0];

				if (number_teeth_CRK > 255 || number_miss_teeth > 2
						|| number_gap > 3 || tdc_to_gap > 360.0
						|| number_teeth_CRK <= 0 || number_miss_teeth <= 0
						|| number_gap <= 0 || tdc_to_gap <= 0
						|| first_seg_angle <= 0 || number_cylinder <= 0
						|| (sensortype_CRK != 'c' && sensortype_CRK != 'h'))
				{
					USART_COM_error();
				}

				//configure CRK if no communication error is present
				if (com_error == false)
				{
					CRK_configuration();
				}

			}
			else
			{
				USART_COM_error();
			}
		}
		// Delete temporary character arrays
		Reset_temp_arrays();

		break;

	}

	case ('2'): // CAM configuration
	{
		if (CAM_config == false)
		{
			if (data_counter
					== (4 + number_active_edges_CAM[cam_setup_counter]))
			{
				number_of_CAM = atoi(temp_chars_2);
				active_CAM_edges[cam_setup_counter] = temp_chars_3[0];
				filter_time_CAM[cam_setup_counter] = atof(temp_chars_4);

				if (filter_time_CAM[cam_setup_counter] <= 0
						|| number_active_edges_CAM[cam_setup_counter] <= 0
						|| CAM_edges[cam_setup_counter][0] <= 0)
				{
					USART_COM_error();
				}

				cam_setup_counter++;
				//configure CAM if no communication error is present
				if (com_error == false && (cam_setup_counter == number_of_CAM))
				{
					cam_setup_counter = 0;
					CAM_configuration();
				}
			}
			else
			{
				USART_COM_error();
			}
		}

		// Delete temporary character arrays
		Reset_temp_arrays();

		break;
	}

	case ('3'): // CRK configuration delete
	{
		if (data_counter == 1 && temp_chars_1[0] == 'S')
		{
			CRK_config = false;

			configuration_complete = false;
			CRK_synch = false;
			CRK_CAM_synch = false;
			//CRK_CAM_synch[1] = false;
			CRK_synch_temp = false;
		}
		else
		{
			USART_COM_error();
		}

		// Delete temporary character arrays
		Reset_temp_arrays();

		break;
	}

	case ('4'): // CAM configuration delete
	{
		if (data_counter == 1 && temp_chars_1[0] == 'S')
		{
			CAM_config = false;

			cam_setup_counter = 0;

			configuration_complete = false;
			CRK_synch = false;
			CRK_CAM_synch = false;
			//CRK_CAM_synch[1] = false;
			CRK_synch_temp = false;
		}
		else
		{
			USART_COM_error();
		}

		// Delete temporary character arrays
		Reset_temp_arrays();

		break;
	}

	case ('5'): // active_cam_failure init
	{
		if (data_counter == 1 && (atoi(temp_chars_1) <= 2))
		{
			active_cam_failure = atoi(temp_chars_1);

		}
		else
		{
			USART_COM_error();
		}
		break;
	}

	case ('6'): //
	{
		break;
	}

	case ('7'): // CRK_RUN_OUT(START or STOP)
	{
		if (data_counter == 4 && temp_chars_1[0] == 'B')
		{
			angle_to_start_failure_CRK_RUN_OUT = atoi(temp_chars_2);
			sc_type_CRK_RUN_OUT = temp_chars_3[0];
			failure_period_CRK_RUN_OUT = atoi(temp_chars_4);

			failure_identify = '4';
		}
		else if (data_counter == 1 && temp_chars_1[0] == 'S')
		{
			failure_identify = '0';

			CRK_RUN_OUT_reset();
		}
		else
		{
			USART_COM_error();
		}

		// Delete temporary character arrays
		Reset_temp_arrays();

		break;
	}

	case ('8'): // CAM_PER(START or STOP)
	{
		if (data_counter == 3 && temp_chars_1[0] == 'B')
		{
			active_edges_CAM_PER = temp_chars_2[0];
			number_edges_CAM_PER = atoi(temp_chars_3);

			failure_identify = '5';

		}
		else if (data_counter == 1 && temp_chars_1[0] == 'S')
		{
			failure_identify = '0';
			CAM_PER_reset();

		}
		else
		{
			USART_COM_error();
		}

		// Delete temporary character arrays
		Reset_temp_arrays();

		break;
	}

	case ('9'): // CRK_TOOTH_PER(START or STOP)
	{
		if (data_counter == 1 && temp_chars_1[0] == 'B')
		{
			__HAL_TIM_SET_AUTORELOAD(&htim3, 17);
			failure_identify = '6';

		}
		else if (data_counter == 1 && temp_chars_1[0] == 'S')
		{
			failure_identify = '0';

			CRK_TOOTH_PER_reset();
		}
		else
		{
			USART_COM_error();
		}

		// Delete temporary character arrays
		Reset_temp_arrays();

		break;
	}

	case ('a'): //
	{
		break;
	}

	case ('b'): // CAM_Pat_Err(START or STOP)
	{
		if (data_counter == 1 && temp_chars_1[0] == 'B')
		{
			failure_identify = '8';

		}
		else if (data_counter == 1 && temp_chars_1[0] == 'S')
		{
			failure_identify = '0';
			CAM_PAT_ERR_reset();
		}
		else
		{
			USART_COM_error();
		}

		// Delete temporary character arrays
		Reset_temp_arrays();

		break;
	}

	case ('c'): // CAM_SYN_CRK(START or STOP)
	{
		if (data_counter == 2 && temp_chars_1[0] == 'B')
		{
			delay_angle_CAM_delay = atof(temp_chars_2);

			delay_factor_CAM_delay = 1;

			failure_identify = '9';
		}
		else if (data_counter == 1 && temp_chars_1[0] == 'S')
		{
			failure_identify = '0';
			CAM_delay_reset();
		}
		else
		{
			USART_COM_error();
		}

		// Delete temporary character arrays
		Reset_temp_arrays();

		break;
	}

	case ('d'): //
	{
		break;
	}

	case ('f'): //
	{
		break;
	}

	case ('g'): //
	{
		break;
	}

	case ('h'): // CRK_TOOTH_OFF
	{
		if (data_counter == 2 && temp_chars_1[0] == 'B')
		{

			number_tooth_off = atoi(temp_chars_2);
			failure_identify = 'h';

		}
		else if (data_counter == 1 && temp_chars_1[0] == 'S')
		{
			failure_identify = '0';
			CRK_TOOTH_OFF_reset();
		}
		else
		{
			USART_COM_error();
		}

		// Delete temporary character arrays
		Reset_temp_arrays();

		break;
	}

	case ('i'): // CRK_GAP_NOT_DET
	{
		if (data_counter == 1 && temp_chars_1[0] == 'B')
		{

			failure_identify = 'i';

		}
		else if (data_counter == 1 && temp_chars_1[0] == 'S')
		{
			failure_identify = '0';
			CRK_GAP_NOT_DET_reset();
		}
		else
		{
			USART_COM_error();
		}

		// Delete temporary character arrays
		Reset_temp_arrays();

		break;
	}

	case ('j'): // SEG_ADP_ER_LIM
	{
		if (data_counter == 2 && temp_chars_1[0] == 'B')
		{
			crk_delay_SEG_ADP_ER_LIM = atof(temp_chars_2);
			failure_identify = 'j';

		}
		else if (data_counter == 1 && temp_chars_1[0] == 'S')
		{
			failure_identify = '0';
			SEG_ADP_ER_LIM_reset();
		}
		else
		{
			USART_COM_error();
		}

		// Delete temporary character arrays
		Reset_temp_arrays();

		break;
	}

	case ('k'): // CrkPlsOrng wrong pulse duration
	{
		if (data_counter == 2 && temp_chars_1[0] == 'B')
		{
			crk_pulse_duration_CRK_PLS_ORNG = atof(temp_chars_2);
			__HAL_TIM_SET_AUTORELOAD(&htim4,
					18 * ((crk_pulse_duration_CRK_PLS_ORNG / 0.217) + 1) - 1);
			failure_identify = 'k';
			failure_active = true;

		}
		else if (data_counter == 1 && temp_chars_1[0] == 'S')
		{
			failure_identify = '0';
			CRK_pulse_duration_reset();
		}
		else
		{
			USART_COM_error();
		}

		// Delete temporary character arrays
		Reset_temp_arrays();

		break;
	}

	case ('l'): // PosnEngStst Wrong engine position at synchronized start
	{
		if (data_counter == 2 && temp_chars_1[0] == 'B')
		{
			crk_teeth_off_POSN_ENG_STST = atoi(temp_chars_2);
			failure_identify = 'l';
			failure_active = true;

		}
		else if (data_counter == 1 && temp_chars_1[0] == 'S')
		{
			failure_identify = '0';
			POSN_ENG_STST_reset();
		}
		else
		{
			USART_COM_error();
		}

		// Delete temporary character arrays
		Reset_temp_arrays();

		break;
	}
	case ('m'): // SC_CAM_CRK set a short circuit to ground or battery, to CAM or CRK
	{
		if (data_counter == 2 && temp_chars_1[0] == 'B')
		{
			sc_type_SC_CAM_CRK = atoi(temp_chars_2);
			failure_identify = 'm';
			failure_active = true;

		}
		else if (data_counter == 1 && temp_chars_1[0] == 'S')
		{
			failure_identify = '0';
			SC_CAM_CRK_reset();
		}
		else
		{
			USART_COM_error();
		}

		// Delete temporary character arrays
		Reset_temp_arrays();

		break;
	}
	case ('n'):
	{
		should_record = true;
		break;
	}
	case ('z'): // communication validation
	{
		if (data_counter == 0)
		{
			if (communication_active == false)
			{
				communication_active = true;
				communication_ready = true;

				SysTick->CTRL &= ~(1); //disable systick
				uint8_t msg11 = message[11];
				HAL_UART_Transmit_IT(&huart1, &msg11, 1);
			}
			else
			{
				communication_ready = true;
			}
		}
		else
		{
			USART_COM_error();
		}

		Reset_temp_arrays();

		break;
	}

	default:
	{
		if (com_error == false)
		{
			USART_COM_error();
		}

		break;
	}

	}

	input_chars[0] = '\0';

	message_received = false;

	//communication receive status
	uint8_t msg_communication_receive_ready = message[12];
	HAL_UART_Transmit_IT(&huart1, &msg_communication_receive_ready, 1);
}

//## USART COM Error Function

void USART_COM_error(void)
{
	if (com_error == false)
	{
		com_error = true;

		input_chars[0] = '\0';

		char_counter = 0;

		receiving = false;

		message_received = false;

		//communication error treatment
		uint8_t msg_COM_error = message[0];
		HAL_UART_Transmit_IT(&huart1, &msg_COM_error, 1);
	}
}

//## USART Send Failure Configuration Status Function

void USART_send_failure_configuration_status(char failure_ident,
		bool failure_conf, bool failure_conf_CAM_blank_out)
{
	if ((failure_ident == '0' || failure_ident == '2') && failure_conf == true)
	{
		uint8_t msg_failure_passive = message[8];
		HAL_UART_Transmit_IT(&huart1, &msg_failure_passive, 1);
		failure_configured = false;
	}
	else if ((failure_ident != '0' && failure_ident != '2')
			&& failure_conf == false)
	{
		uint8_t msg_failure_active = message[7];
		HAL_UART_Transmit_IT(&huart1, &msg_failure_active, 1);
		failure_configured = true;
	}

	if ((failure_ident != '2' && failure_ident != '3')
			&& failure_conf_CAM_blank_out == true)
	{
		uint8_t msg_failure_cam_blank_out_passive = message[10];
		HAL_UART_Transmit_IT(&huart1, &msg_failure_cam_blank_out_passive, 1);
		failure_configured_CAM_blank_out = false;
	}
	else if ((failure_ident == '2' || failure_ident == '3')
			&& failure_conf_CAM_blank_out == false)
	{
		uint8_t msg_failure_cam_blank_out_active = message[9];
		HAL_UART_Transmit_IT(&huart1, &msg_failure_cam_blank_out_active, 1);
		failure_configured_CAM_blank_out = true;
	}
}

//## USART Reset temporary char-arrays

void Reset_temp_arrays(void)
{
	temp_chars_1[0] = '\0';
	temp_chars_2[0] = '\0';
	temp_chars_3[0] = '\0';
	temp_chars_4[0] = '\0';
	temp_chars_5[0] = '\0';
	temp_chars_6[0] = '\0';
	temp_chars_7[0] = '\0';
	temp_chars_8[0] = '\0';
	temp_chars_9[0] = '\0';
	temp_chars_10[0] = '\0';
}
