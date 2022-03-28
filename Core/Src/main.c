/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.c
  * @brief          : Main program body
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2022 STMicroelectronics.
  * All rights reserved.
  *
  * This software is licensed under terms that can be found in the LICENSE file
  * in the root directory of this software component.
  * If no LICENSE file comes with this software, it is provided AS-IS.
  *
  ******************************************************************************
  */
/* USER CODE END Header */
/* Includes ------------------------------------------------------------------*/
#include "main.h"
#include "usb_device.h"
#include "usbd_cdc_if.h"
#include "string.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include "packets_float.h"
/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */

/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */
/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */

/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/
UART_HandleTypeDef huart2;

/* USER CODE BEGIN PV */

/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
static void MX_GPIO_Init(void);
static void MX_USART2_UART_Init(void);

/* USER CODE BEGIN PFP */
/* USER CODE END PFP */

/* Private user code ---------------------------------------------------------*/
/* USER CODE BEGIN 0 */

PACKET_STORAGE_s STORAGE;
PACKET_TELEMETRY_s TELEMETRY;
PACKET_FC_CONFIG_s FC_CONFIG;
PACKET_SENS_CALBRATION_s SENS_CALIBRATION;
PACKET_SYS_TESTING_s SYS_TESTING;
MESSAGE_PACKET_s MESSAGE_PACKET;

// This is our send and receive data buffer
//1000 bytes Ow0 this is a global buffer so we dont have to send the same buffer around
float data_buffer[4];
float data_buffer_s[4];
float tester_buffer[9];

int data_received_length = 0;

// DataProcessor is going to be the function that is first contact with new data either send or receive
void DataPacketProcessor(){
    //First we will check to see if we're sending or receiving data
    if(((int)MESSAGE_PACKET.validityState == 1) && ((int)MESSAGE_PACKET.transmissionMode == 1)){
        // If first and second bytes == 1 we are receiving data from GS app.
    	ReceivePacketProcessor();
    } else if (((int)MESSAGE_PACKET.validityState == 1) && ((int)MESSAGE_PACKET.transmissionMode == 0)) {
        // If first byte in buffer is 1 and second is 0; GS app is receiving data from us...
        //(TODO: do we need to always send data unless requested to send???
    	SendPacketProcessor();
    } else{
    	return 1; //error out
    }
}// end of DataProcessor()

void SendPacketProcessor(){
	switch((int)MESSAGE_PACKET.systemSelector){
		case 0: //packet_id_storage = 0,
			switch((int)MESSAGE_PACKET.variableSelector){
				case 0:
					data_buffer_s[0] = STORAGE.time;
					data_received_length = vcp_send(data_buffer_s, sizeof(data_buffer_s));
					break;
				case 1:
					data_buffer_s[0] = STORAGE.event_flags;
					data_received_length = vcp_send(data_buffer_s, sizeof(data_buffer_s));
					break;
				case 2:
					data_buffer_s[0] = STORAGE.system_flags;
					data_received_length = vcp_send(data_buffer_s, sizeof(data_buffer_s));
					break;
				case 3:
					data_buffer_s[0] = STORAGE.temperature;
					data_received_length = vcp_send(data_buffer_s, sizeof(data_buffer_s));
					break;
				case 4:
					data_buffer_s[0] = STORAGE.pressure;
					data_received_length = vcp_send(data_buffer_s, sizeof(data_buffer_s));
					break;
				case 5:
					data_buffer_s[0] = STORAGE.altitude;
					data_received_length = vcp_send(data_buffer_s, sizeof(data_buffer_s));
					break;
				case 6:
					data_buffer_s[0] = STORAGE.speed;
					data_received_length = vcp_send(data_buffer_s, sizeof(data_buffer_s));
					break;
				case 7:
					data_buffer_s[0] = STORAGE.accel_x;
					data_received_length = vcp_send(data_buffer_s, sizeof(data_buffer_s));
					break;
				case 8:
					data_buffer_s[0] = STORAGE.accel_y;
					data_received_length = vcp_send(data_buffer_s, sizeof(data_buffer_s));
					break;
				case 9:
					data_buffer_s[0] = STORAGE.accel_z;
					data_received_length = vcp_send(data_buffer_s, sizeof(data_buffer_s));
					break;
				case 10:
					data_buffer_s[0] = STORAGE.gyro_x;
					data_received_length = vcp_send(data_buffer_s, sizeof(data_buffer_s));
					break;
				case 11:
					data_buffer_s[0] = STORAGE.gyro_y;
					data_received_length = vcp_send(data_buffer_s, sizeof(data_buffer_s));
					break;
				case 12:
					data_buffer_s[0] = STORAGE.gyro_z;
					data_received_length = vcp_send(data_buffer_s, sizeof(data_buffer_s));
					break;
				case 13:
					data_buffer_s[0] = STORAGE.mag_x;
					data_received_length = vcp_send(data_buffer_s, sizeof(data_buffer_s));
					break;
				case 14:
					data_buffer_s[0] = STORAGE.mag_y;
					data_received_length = vcp_send(data_buffer_s, sizeof(data_buffer_s));
					break;
				case 15:
					data_buffer_s[0] = STORAGE.mag_z;
					data_received_length = vcp_send(data_buffer_s, sizeof(data_buffer_s));
					break;
				case 16:
					data_buffer_s[0] = STORAGE.latitude;
					data_received_length = vcp_send(data_buffer_s, sizeof(data_buffer_s));
					break;
				case 17:
					data_buffer_s[0] = STORAGE.longitude;
					data_received_length = vcp_send(data_buffer_s, sizeof(data_buffer_s));
					break;
				case 18:
					data_buffer_s[0] = STORAGE.gps_altitude;
					data_received_length = vcp_send(data_buffer_s, sizeof(data_buffer_s));
					break;
				case 19:
					data_buffer_s[0] = STORAGE.satellites;
					data_received_length = vcp_send(data_buffer_s, sizeof(data_buffer_s));
					break;
				default:
					break;
			}
		case 1:
			switch((int)MESSAGE_PACKET.variableSelector){
				case 0:
					data_buffer_s[0] = TELEMETRY.time;
					data_received_length = vcp_send(data_buffer_s, sizeof(data_buffer_s));
					break;
				case 1:
					data_buffer_s[0] = TELEMETRY.event_flags;
					data_received_length = vcp_send(data_buffer_s, sizeof(data_buffer_s));
					break;
				case 2:
					data_buffer_s[0] = TELEMETRY.system_flags;
					data_received_length = vcp_send(data_buffer_s, sizeof(data_buffer_s));
					break;
				case 3:
					data_buffer_s[0] = TELEMETRY.system_voltage;
					data_received_length = vcp_send(data_buffer_s, sizeof(data_buffer_s));
					break;
				case 4:
					data_buffer_s[0] = TELEMETRY.pressure;
					data_received_length = vcp_send(data_buffer_s, sizeof(data_buffer_s));
					break;
				case 5:
					data_buffer_s[0] = TELEMETRY.altitude;
					data_received_length = vcp_send(data_buffer_s, sizeof(data_buffer_s));
					break;
				case 6:
					data_buffer_s[0] = TELEMETRY.speed;
					data_received_length = vcp_send(data_buffer_s, sizeof(data_buffer_s));
					break;
				case 7:
					data_buffer_s[0] = TELEMETRY.latitude;
					data_received_length = vcp_send(data_buffer_s, sizeof(data_buffer_s));
					break;
				case 8:
					data_buffer_s[0] = TELEMETRY.longitude;
					data_received_length = vcp_send(data_buffer_s, sizeof(data_buffer_s));
					break;
				case 9:
					data_buffer_s[0] = TELEMETRY.gps_altitude;
					data_received_length = vcp_send(data_buffer_s, sizeof(data_buffer_s));
					break;
				case 10:
					data_buffer_s[0] = TELEMETRY.satellites;
					data_received_length = vcp_send(data_buffer_s, sizeof(data_buffer_s));
					break;
				default:
					break;
			}
		case 2:
			switch((int)MESSAGE_PACKET.variableSelector){
				case 0:
					data_buffer_s[0] = FC_CONFIG.recovery_drogue_delay;
					data_received_length = vcp_send(data_buffer_s, sizeof(data_buffer_s));
					break;
				case 1:
					data_buffer_s[0] = FC_CONFIG.recovery_main_deploy_alt;
					data_received_length = vcp_send(data_buffer_s, sizeof(data_buffer_s));
					break;
				case 2:
					data_buffer_s[0] = FC_CONFIG.recovery_pyro_igintion_time;
					data_received_length = vcp_send(data_buffer_s, sizeof(data_buffer_s));
					break;
				case 3:
					data_buffer_s[0] = FC_CONFIG.aux_pyro_c_enable;
					data_received_length = vcp_send(data_buffer_s, sizeof(data_buffer_s));
					break;
				case 4:
					data_buffer_s[0] = FC_CONFIG.aux_pyro_c_trigger;
					data_received_length = vcp_send(data_buffer_s, sizeof(data_buffer_s));
					break;
				case 5:
					data_buffer_s[0] = FC_CONFIG.aux_pyro_c_delay;
					data_received_length = vcp_send(data_buffer_s, sizeof(data_buffer_s));
					break;
				case 6:
					data_buffer_s[0] = FC_CONFIG.aux_pyro_d_enable;
					data_received_length = vcp_send(data_buffer_s, sizeof(data_buffer_s));
					break;
				case 7:
					data_buffer_s[0] = FC_CONFIG.aux_pyro_d_trigger;
					data_received_length = vcp_send(data_buffer_s, sizeof(data_buffer_s));
					break;
				case 8:
					data_buffer_s[0] = FC_CONFIG.aux_pyro_d_delay;
					data_received_length = vcp_send(data_buffer_s, sizeof(data_buffer_s));
					break;
				case 9:
					data_buffer_s[0] = FC_CONFIG.aux_pyro_e_enable;
					data_received_length = vcp_send(data_buffer_s, sizeof(data_buffer_s));
					break;
				case 10:
					data_buffer_s[0] = FC_CONFIG.aux_pyro_e_trigger;
					data_received_length = vcp_send(data_buffer_s, sizeof(data_buffer_s));
					break;
				case 11:
					data_buffer_s[0] = FC_CONFIG.aux_pyro_e_delay;
					data_received_length = vcp_send(data_buffer_s, sizeof(data_buffer_s));
					break;
				case 12:
					data_buffer_s[0] = FC_CONFIG.aux_pyro_f_enable;
					data_received_length = vcp_send(data_buffer_s, sizeof(data_buffer_s));
					break;
				case 13:
					data_buffer_s[0] = FC_CONFIG.aux_pyro_f_trigger;
					data_received_length = vcp_send(data_buffer_s, sizeof(data_buffer_s));
					break;
				case 14:
					data_buffer_s[0] = FC_CONFIG.aux_pyro_f_delay;
					data_received_length = vcp_send(data_buffer_s, sizeof(data_buffer_s));
					break;
				case 15:
					data_buffer_s[0] = FC_CONFIG.telemetry_transmit_power;
					data_received_length = vcp_send(data_buffer_s, sizeof(data_buffer_s));
					break;
				case 16:
					data_buffer_s[0] = FC_CONFIG.telemetry_baud_rate;
					data_received_length = vcp_send(data_buffer_s, sizeof(data_buffer_s));
					break;
				case 17:
					data_buffer_s[0] = FC_CONFIG.telemetry_channel;
					data_received_length = vcp_send(data_buffer_s, sizeof(data_buffer_s));
					break;
				case 18:
					data_buffer_s[0] = FC_CONFIG.telemetry_transmit_rate;
					data_received_length = vcp_send(data_buffer_s, sizeof(data_buffer_s));
					break;
				case 19:
					data_buffer_s[0] = FC_CONFIG.telemtery_landed_rate;
					data_received_length = vcp_send(data_buffer_s, sizeof(data_buffer_s));
					break;
				case 20:
					data_buffer_s[0] = FC_CONFIG.telemetry_callsign;
					data_received_length = vcp_send(data_buffer_s, sizeof(data_buffer_s));
					break;
				case 21:
					data_buffer_s[0] = FC_CONFIG.aux_sys_enable_pwm;
					data_received_length = vcp_send(data_buffer_s, sizeof(data_buffer_s));
					break;
				case 22:
					data_buffer_s[0] = FC_CONFIG.aux_sys_enable_buzzer;
					data_received_length = vcp_send(data_buffer_s, sizeof(data_buffer_s));
					break;
				case 23:
					data_buffer_s[0] = FC_CONFIG.aux_sys_buzzer_freq;
					data_received_length = vcp_send(data_buffer_s, sizeof(data_buffer_s));
					break;
				case 24:
					data_buffer_s[0] = FC_CONFIG.aux_sys_buzzer_pattern;
					data_received_length = vcp_send(data_buffer_s, sizeof(data_buffer_s));
					break;
				default:
					break;
			}
		case 3:
			switch((int)MESSAGE_PACKET.variableSelector){
				case 0:
					data_buffer_s[0] = SENS_CALIBRATION.cal_requesting;
					data_received_length = vcp_send(data_buffer_s, sizeof(data_buffer_s));
					break;
				case 1:
					data_buffer_s[0] = SENS_CALIBRATION.cal_accel_select;
					data_received_length = vcp_send(data_buffer_s, sizeof(data_buffer_s));
					break;
				case 2:
					data_buffer_s[0] = SENS_CALIBRATION.cal_accel_status;
					data_received_length = vcp_send(data_buffer_s, sizeof(data_buffer_s));
					break;
				case 3:
					data_buffer_s[0] = SENS_CALIBRATION.cal_mag_select;
					data_received_length = vcp_send(data_buffer_s, sizeof(data_buffer_s));
					break;
				case 4:
					data_buffer_s[0] = SENS_CALIBRATION.cal_mag_status;
					data_received_length = vcp_send(data_buffer_s, sizeof(data_buffer_s));
					break;
				default:
					break;
			}
		case 4:
			switch((int)MESSAGE_PACKET.variableSelector){
				case 0:
					data_buffer_s[0] = SYS_TESTING.test_requesting;
					data_received_length = vcp_send(data_buffer_s, sizeof(data_buffer_s));
					break;
				case 1:
					data_buffer_s[0] = SYS_TESTING.test_pyro_A;
					data_received_length = vcp_send(data_buffer_s, sizeof(data_buffer_s));
					break;
				case 2:
					data_buffer_s[0] = SYS_TESTING.test_pyro_A_status;
					data_received_length = vcp_send(data_buffer_s, sizeof(data_buffer_s));
					break;
				case 3:
					data_buffer_s[0] = SYS_TESTING.test_pyro_B;
					data_received_length = vcp_send(data_buffer_s, sizeof(data_buffer_s));
					break;
				case 4:
					data_buffer_s[0] = SYS_TESTING.test_pyro_B_status;
					data_received_length = vcp_send(data_buffer_s, sizeof(data_buffer_s));
					break;
				case 5:
					data_buffer_s[0] = SYS_TESTING.test_pyro_C;
					data_received_length = vcp_send(data_buffer_s, sizeof(data_buffer_s));
					break;
				case 6:
					data_buffer_s[0] = SYS_TESTING.test_pyro_C_status;
					data_received_length = vcp_send(data_buffer_s, sizeof(data_buffer_s));
					break;
				case 7:
					data_buffer_s[0] = SYS_TESTING.test_pyro_D;
					data_received_length = vcp_send(data_buffer_s, sizeof(data_buffer_s));
					break;
				case 8:
					data_buffer_s[0] = SYS_TESTING.test_pyro_D_status;
					data_received_length = vcp_send(data_buffer_s, sizeof(data_buffer_s));
					break;
				case 9:
					data_buffer_s[0] = SYS_TESTING.test_pyro_E;
					data_received_length = vcp_send(data_buffer_s, sizeof(data_buffer_s));
					break;
				case 10:
					data_buffer_s[0] = SYS_TESTING.test_pyro_E_status;
					data_received_length = vcp_send(data_buffer_s, sizeof(data_buffer_s));
					break;
				case 11:
					data_buffer_s[0] = SYS_TESTING.test_pyro_F;
					data_received_length = vcp_send(data_buffer_s, sizeof(data_buffer_s));
					break;
				case 12:
					data_buffer_s[0] = SYS_TESTING.test_pyro_F_status;
					data_received_length = vcp_send(data_buffer_s, sizeof(data_buffer_s));
					break;
				case 13:
					data_buffer_s[0] = SYS_TESTING.test_telemetry;
					data_received_length = vcp_send(data_buffer_s, sizeof(data_buffer_s));
					break;
				case 14:
					data_buffer_s[0] = SYS_TESTING.test_telemetry_status;
					data_received_length = vcp_send(data_buffer_s, sizeof(data_buffer_s));
					break;
				case 15:
					data_buffer_s[0] = SYS_TESTING.test_gps;
					data_received_length = vcp_send(data_buffer_s, sizeof(data_buffer_s));

					break;
				case 16:
					data_buffer_s[0] = SYS_TESTING.test_gps_status;
					data_received_length = vcp_send(data_buffer_s, sizeof(data_buffer_s));
					break;
				case 17:
					data_buffer_s[0] = SYS_TESTING.test_accelerometer_1;
					data_received_length = vcp_send(data_buffer_s, sizeof(data_buffer_s));
					break;
				case 18:
					data_buffer_s[0] = SYS_TESTING.test_accelerometer_1_status;
					data_received_length = vcp_send(data_buffer_s, sizeof(data_buffer_s));
					break;
				case 19:
					data_buffer_s[0] = SYS_TESTING.test_accelerometer_2;
					data_received_length = vcp_send(data_buffer_s, sizeof(data_buffer_s));
					break;
				case 20:
					data_buffer_s[0] = SYS_TESTING.test_accelerometer_2_status;
					data_received_length = vcp_send(data_buffer_s, sizeof(data_buffer_s));
					break;
				default:
					break;
			}
		default:
			break;
	}

}// end of SendProcessor()



void ReceivePacketProcessor(){
	  	// Now that we've been called and know we're getting data from GS app, we will next need to find out
	    // what type of data/ struct we need to update with the new values, we determine this with two things
		// [2] system picker, [3] variable picker, and finally [4] the data we set...
		// we will have two switch statements in each and determine with two enums.
		// the enums will be in packets_floats.h, variable picker and system pickers

	    // We're going to use an easy switch statement to set the structs to the send data

		switch((int)MESSAGE_PACKET.systemSelector){
			case 0: //packet_id_storage = 0,
				switch((int)MESSAGE_PACKET.variableSelector){
					case 0:
						STORAGE.time = MESSAGE_PACKET.data; // Milliseconds (note: Overflow in ~50 days)
						break;
					case 1:
						STORAGE.event_flags = MESSAGE_PACKET.data; // launch, coast_1, sustainer_iginition, coast_2, apogee, drogue_deploy, main_deploy, land
						break;
					case 2:
						STORAGE.system_flags = MESSAGE_PACKET.data;
						break;
					case 3:
						STORAGE.temperature = MESSAGE_PACKET.data; // Celsius (specially formatted)
						break;
					case 4:
						STORAGE.pressure = MESSAGE_PACKET.data; // Pascal
						break;
					case 5:
						STORAGE.altitude = MESSAGE_PACKET.data; // Meters
						break;
					case 6:
						STORAGE.speed = MESSAGE_PACKET.data; // meters per second
						break;
					case 7:
						STORAGE.accel_x = MESSAGE_PACKET.data; // Acceleration in direction of flight (m/s^2) (will swap accelerometers when sensor is saturated)
						break;
					case 8:
						STORAGE.accel_y = MESSAGE_PACKET.data;
						break;
					case 9:
						STORAGE.accel_z = MESSAGE_PACKET.data;
						break;
					case 10:
						STORAGE.gyro_x = MESSAGE_PACKET.data; // degrees per second
						break;
					case 11:
						STORAGE.gyro_y = MESSAGE_PACKET.data;
						break;
					case 12:
						STORAGE.gyro_z = MESSAGE_PACKET.data;
						break;
					case 13:
						STORAGE.mag_x = MESSAGE_PACKET.data; // Gauss
						break;
					case 14:
						STORAGE.mag_y = MESSAGE_PACKET.data;
						break;
					case 15:
						STORAGE.mag_z = MESSAGE_PACKET.data;
						break;
					case 16:
						STORAGE.latitude = MESSAGE_PACKET.data; // latitude in degrees
						break;
					case 17:
						STORAGE.longitude = MESSAGE_PACKET.data; // longitude in degrees
						break;
					case 18:
						STORAGE.gps_altitude = MESSAGE_PACKET.data; // meters (from GPS)
						break;
					case 19:
						STORAGE.satellites = MESSAGE_PACKET.data; // number of satellites for GPS lock
						break;
					default:
						break;
				}
			case 1:
				switch((int)MESSAGE_PACKET.variableSelector){
					case 0:
						TELEMETRY.time = MESSAGE_PACKET.data; // Milliseconds (note: Overflow in ~50 days)
						break;
					case 1:
						TELEMETRY.event_flags = MESSAGE_PACKET.data; // launch, coast_1, sustainer_iginition, coast_2, apogee, drogue_deploy, main_deploy, land
						break;
					case 2:
						TELEMETRY.system_flags = MESSAGE_PACKET.data;
						break;
					case 3:
						TELEMETRY.system_voltage = MESSAGE_PACKET.data; // Celsius (specially formatted)
						break;
					case 4:
						TELEMETRY.pressure = MESSAGE_PACKET.data; // Pascal
						break;
					case 5:
						TELEMETRY.altitude = MESSAGE_PACKET.data; // Meters
						break;
					case 6:
						TELEMETRY.speed = MESSAGE_PACKET.data; // meters per second
						break;
					case 7:
						TELEMETRY.latitude = MESSAGE_PACKET.data; // latitude in degrees
						break;
					case 8:
						TELEMETRY.longitude = MESSAGE_PACKET.data; // longitude in degrees
						break;
					case 9:
						TELEMETRY.gps_altitude = MESSAGE_PACKET.data; // meters (from GPS)
						break;
					case 10:
						TELEMETRY.satellites = MESSAGE_PACKET.data; // number of satellites for GPS lock
						break;
					default:
						break;
				}
			case 2:
				switch((int)MESSAGE_PACKET.variableSelector){
					case 0:
						FC_CONFIG.recovery_drogue_delay = MESSAGE_PACKET.data; // Milliseconds (note: Overflow in ~50 days)
						break;
					case 1:
						FC_CONFIG.recovery_main_deploy_alt = MESSAGE_PACKET.data; // launch, coast_1, sustainer_iginition, coast_2, apogee, drogue_deploy, main_deploy, land
						break;
					case 2:
						FC_CONFIG.recovery_pyro_igintion_time = MESSAGE_PACKET.data;
						break;
					case 3:
						FC_CONFIG.aux_pyro_c_enable = MESSAGE_PACKET.data; // Celsius (specially formatted)
						break;
					case 4:
						FC_CONFIG.aux_pyro_c_trigger = MESSAGE_PACKET.data; // Pascal
						break;
					case 5:
						FC_CONFIG.aux_pyro_c_delay = MESSAGE_PACKET.data; // Meters
						break;
					case 6:
						FC_CONFIG.aux_pyro_d_enable = MESSAGE_PACKET.data; // meters per second
						break;
					case 7:
						FC_CONFIG.aux_pyro_d_trigger = MESSAGE_PACKET.data; // Acceleration in direction of flight (m/s^2) (will swap accelerometers when sensor is saturated)
						break;
					case 8:
						FC_CONFIG.aux_pyro_d_delay = MESSAGE_PACKET.data;
						break;
					case 9:
						FC_CONFIG.aux_pyro_e_enable = MESSAGE_PACKET.data;
						break;
					case 10:
						FC_CONFIG.aux_pyro_e_trigger = MESSAGE_PACKET.data; // degrees per second
						break;
					case 11:
						FC_CONFIG.aux_pyro_e_delay = MESSAGE_PACKET.data;
						break;
					case 12:
						FC_CONFIG.aux_pyro_f_enable = MESSAGE_PACKET.data;
						break;
					case 13:
						FC_CONFIG.aux_pyro_f_trigger = MESSAGE_PACKET.data; // Gauss
						break;
					case 14:
						FC_CONFIG.aux_pyro_f_delay = MESSAGE_PACKET.data;
						break;
					case 15:
						FC_CONFIG.telemetry_transmit_power = MESSAGE_PACKET.data;
						break;
					case 16:
						FC_CONFIG.telemetry_baud_rate = MESSAGE_PACKET.data; // latitude in degrees
						break;
					case 17:
						FC_CONFIG.telemetry_channel = MESSAGE_PACKET.data; // longitude in degrees
						break;
					case 18:
						FC_CONFIG.telemetry_transmit_rate = MESSAGE_PACKET.data; // meters (from GPS)
						break;
					case 19:
						FC_CONFIG.telemtery_landed_rate = MESSAGE_PACKET.data; // number of satellites for GPS lock
						break;
					case 20:
						FC_CONFIG.telemetry_callsign = MESSAGE_PACKET.data;
						break;
					case 21:
						FC_CONFIG.aux_sys_enable_pwm = MESSAGE_PACKET.data; // latitude in degrees
						break;
					case 22:
						FC_CONFIG.aux_sys_enable_buzzer = MESSAGE_PACKET.data; // longitude in degrees
						break;
					case 23:
						FC_CONFIG.aux_sys_buzzer_freq = MESSAGE_PACKET.data; // meters (from GPS)
						break;
					case 24:
						FC_CONFIG.aux_sys_buzzer_pattern = MESSAGE_PACKET.data; // number of satellites for GPS lock
						break;
					default:
						break;
				}
			case 3:
				switch((int)data_buffer[2]){
					case 0:
						SENS_CALIBRATION.cal_requesting = MESSAGE_PACKET.data; // Milliseconds (note: Overflow in ~50 days)
						break;
					case 1:
						SENS_CALIBRATION.cal_accel_select = MESSAGE_PACKET.data; // launch, coast_1, sustainer_iginition, coast_2, apogee, drogue_deploy, main_deploy, land
						break;
					case 2:
						SENS_CALIBRATION.cal_accel_status = MESSAGE_PACKET.data;
						break;
					case 3:
						SENS_CALIBRATION.cal_mag_select = MESSAGE_PACKET.data; // Celsius (specially formatted)
						break;
					case 4:
						SENS_CALIBRATION.cal_mag_status = MESSAGE_PACKET.data; // Pascal
						break;
					default:
						break;
				}
			case 4:
				switch((int)MESSAGE_PACKET.variableSelector){
					case 0:
						SYS_TESTING.test_requesting = MESSAGE_PACKET.data; // Milliseconds (note: Overflow in ~50 days)
						break;
					case 1:
						SYS_TESTING.test_pyro_A = MESSAGE_PACKET.data; // launch, coast_1, sustainer_iginition, coast_2, apogee, drogue_deploy, main_deploy, land
						break;
					case 2:
						SYS_TESTING.test_pyro_A_status = MESSAGE_PACKET.data;
						break;
					case 3:
						SYS_TESTING.test_pyro_B = MESSAGE_PACKET.data; // Celsius (specially formatted)
						break;
					case 4:
						SYS_TESTING.test_pyro_B_status = MESSAGE_PACKET.data; // Pascal
						break;
					case 5:
						SYS_TESTING.test_pyro_C = MESSAGE_PACKET.data; // Meters
						break;
					case 6:
						SYS_TESTING.test_pyro_C_status = MESSAGE_PACKET.data; // meters per second
						break;
					case 7:
						SYS_TESTING.test_pyro_D = MESSAGE_PACKET.data; // latitude in degrees
						break;
					case 8:
						SYS_TESTING.test_pyro_D_status = MESSAGE_PACKET.data; // longitude in degrees
						break;
					case 9:
						SYS_TESTING.test_pyro_E = MESSAGE_PACKET.data; // meters (from GPS)
						break;
					case 10:
						SYS_TESTING.test_pyro_E_status = MESSAGE_PACKET.data; // number of satellites for GPS lock
						break;
					case 11:
						SYS_TESTING.test_pyro_F = MESSAGE_PACKET.data; // Milliseconds (note: Overflow in ~50 days)
						break;
					case 12:
						SYS_TESTING.test_pyro_F_status = MESSAGE_PACKET.data; // launch, coast_1, sustainer_iginition, coast_2, apogee, drogue_deploy, main_deploy, land
						break;
					case 13:
						SYS_TESTING.test_telemetry = MESSAGE_PACKET.data;
						break;
					case 14:
						SYS_TESTING.test_telemetry_status = MESSAGE_PACKET.data; // Celsius (specially formatted)
						break;
					case 15:
						SYS_TESTING.test_gps = MESSAGE_PACKET.data; // Pascal
						break;
					case 16:
						SYS_TESTING.test_gps_status = MESSAGE_PACKET.data; // Meters
						break;
					case 17:
						SYS_TESTING.test_accelerometer_1 = MESSAGE_PACKET.data; // meters per second
						break;
					case 18:
						SYS_TESTING.test_accelerometer_1_status = MESSAGE_PACKET.data; // latitude in degrees
						break;
					case 19:
						SYS_TESTING.test_accelerometer_2 = MESSAGE_PACKET.data; // longitude in degrees
						break;
					case 20:
						SYS_TESTING.test_accelerometer_2_status = MESSAGE_PACKET.data; // meters (from GPS)
						break;
					default:
						break;
				}
			default:
				break;
		}
}// end of ReceiveProcessor()

/* USER CODE END 0 */

/**
  * @brief  The application entry point.
  * @retval int
  */
int main(void)
{
  /* USER CODE BEGIN 1 */

  /* USER CODE END 1 */

  /* MCU Configuration--------------------------------------------------------*/

  /* Reset of all peripherals, Initializes the Flash interface and the Systick. */
  HAL_Init();

  /* USER CODE BEGIN Init */
  /* USER CODE END Init */

  /* Configure the system clock */
  SystemClock_Config();

  /* USER CODE BEGIN SysInit */
  /* USER CODE END SysInit */

  /* Initialize all configured peripherals */
	  MX_GPIO_Init();
	  MX_USART2_UART_Init();
	  MX_USB_DEVICE_Init();

	  /* USER CODE BEGIN 2 */

  /* USER CODE END 2 */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */


  while (1)
  {
    /* USER CODE END WHILE */

    /* USER CODE BEGIN 3 */
      // poll to see if we've gotten data
	  data_received_length = vcp_recv(data_buffer, 5);
      if(data_received_length  > 0){

    	  MESSAGE_PACKET.validityState = data_buffer[0];
    	  MESSAGE_PACKET.transmissionMode = data_buffer[1];
    	  MESSAGE_PACKET.systemSelector = data_buffer[2];
    	  MESSAGE_PACKET.variableSelector = data_buffer[3];
    	  MESSAGE_PACKET.data = data_buffer[4];


    	  data_buffer_s[0] = MESSAGE_PACKET.data;
    	  data_buffer_s[1] = MESSAGE_PACKET.variableSelector;
		  data_buffer_s[2] = MESSAGE_PACKET.systemSelector ;
	      data_buffer_s[3] = MESSAGE_PACKET.transmissionMode;
		  data_buffer_s[4] = MESSAGE_PACKET.validityState;

    	  data_received_length = vcp_send(data_buffer_s, 5);

    	  //DataPacketProcessor();
      }
  /* USER CODE END 3 */
  }
}

/**
  * @brief System Clock Configuration
  * @retval None
  */
void SystemClock_Config(void)
{
  RCC_OscInitTypeDef RCC_OscInitStruct = {0};
  RCC_ClkInitTypeDef RCC_ClkInitStruct = {0};

  /** Configure the main internal regulator output voltage
  */
  __HAL_RCC_PWR_CLK_ENABLE();
  __HAL_PWR_VOLTAGESCALING_CONFIG(PWR_REGULATOR_VOLTAGE_SCALE3);
  /** Initializes the RCC Oscillators according to the specified parameters
  * in the RCC_OscInitTypeDef structure.
  */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSE;
  RCC_OscInitStruct.HSEState = RCC_HSE_ON;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
  RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSE;
  RCC_OscInitStruct.PLL.PLLM = 4;
  RCC_OscInitStruct.PLL.PLLN = 72;
  RCC_OscInitStruct.PLL.PLLP = RCC_PLLP_DIV2;
  RCC_OscInitStruct.PLL.PLLQ = 3;
  RCC_OscInitStruct.PLL.PLLR = 2;
  if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
  {
    Error_Handler();
  }
  /** Initializes the CPU, AHB and APB buses clocks
  */
  RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK|RCC_CLOCKTYPE_SYSCLK
                              |RCC_CLOCKTYPE_PCLK1|RCC_CLOCKTYPE_PCLK2;
  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
  RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV2;
  RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV1;

  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_2) != HAL_OK)
  {
    Error_Handler();
  }
}

/**
  * @brief USART2 Initialization Function
  * @param None
  * @retval None
  */
static void MX_USART2_UART_Init(void)
{

  /* USER CODE BEGIN USART2_Init 0 */

  /* USER CODE END USART2_Init 0 */

  /* USER CODE BEGIN USART2_Init 1 */

  /* USER CODE END USART2_Init 1 */
  huart2.Instance = USART2;
  huart2.Init.BaudRate = 115200;
  huart2.Init.WordLength = UART_WORDLENGTH_8B;
  huart2.Init.StopBits = UART_STOPBITS_1;
  huart2.Init.Parity = UART_PARITY_NONE;
  huart2.Init.Mode = UART_MODE_TX_RX;
  huart2.Init.HwFlowCtl = UART_HWCONTROL_NONE;
  huart2.Init.OverSampling = UART_OVERSAMPLING_16;
  if (HAL_UART_Init(&huart2) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN USART2_Init 2 */

  /* USER CODE END USART2_Init 2 */

}

/**
  * @brief GPIO Initialization Function
  * @param None
  * @retval None
  */
static void MX_GPIO_Init(void)
{
  GPIO_InitTypeDef GPIO_InitStruct = {0};

  /* GPIO Ports Clock Enable */
  __HAL_RCC_GPIOC_CLK_ENABLE();
  __HAL_RCC_GPIOH_CLK_ENABLE();
  __HAL_RCC_GPIOA_CLK_ENABLE();
  __HAL_RCC_GPIOB_CLK_ENABLE();

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(LD2_GPIO_Port, LD2_Pin, GPIO_PIN_RESET);

  /*Configure GPIO pin : B1_Pin */
  GPIO_InitStruct.Pin = B1_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_IT_FALLING;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  HAL_GPIO_Init(B1_GPIO_Port, &GPIO_InitStruct);

  /*Configure GPIO pin : LD2_Pin */
  GPIO_InitStruct.Pin = LD2_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(LD2_GPIO_Port, &GPIO_InitStruct);

}

/* USER CODE BEGIN 4 */

/* USER CODE END 4 */

/**
  * @brief  This function is executed in case of error occurrence.
  * @retval None
  */
void Error_Handler(void)
{
  /* USER CODE BEGIN Error_Handler_Debug */
  /* User can add his own implementation to report the HAL error return state */
  __disable_irq();
  while (1)
  {
  }
  /* USER CODE END Error_Handler_Debug */
}

#ifdef  USE_FULL_ASSERT
/**
  * @brief  Reports the name of the source file and the source line number
  *         where the assert_param error has occurred.
  * @param  file: pointer to the source file name
  * @param  line: assert_param error line source number
  * @retval None
  */
void assert_failed(uint8_t *file, uint32_t line)
{
  /* USER CODE BEGIN 6 */
  /* User can add his own implementation to report the file name and line number,
     ex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */
  /* USER CODE END 6 */
}
#endif /* USE_FULL_ASSERT */

