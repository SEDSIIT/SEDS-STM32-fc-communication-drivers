/*
 * INFO:
 * This is a header file defining the structure for data packets 
 * that will be transmitted between the Nova flight computer and
 * the ground station software via USB and telemetry communication
 *
 * UPDATED:
 * 04/15/22
 * 
 * AUTHOR:
 * Michael Gromski
 * 
 * EDITED:
 * Austin Roddy
 *
*/

#ifndef _PACKET_DRIVER_H
#define _PACKET_DRIVER_H

#ifdef __cplusplus
extern "C"
{
#endif

#include <stdint.h>

// Message_packet structure
typedef struct MESSAGE_PACKET_s
{
	float validityState;
	float transmissionMode;
	float systemSelector;
	float variableSelector;
	float data;
}MESSAGE_PACKET_s;

// header ID to identify packet type
typedef enum
{
    packet_id_storage = 0,
    packet_id_telemetry,
    packet_id_fc_config,
    packet_id_sense_calibration,
    packet_id_data_request,
    packet_id_sys_testing
}PACKET_TYPE_e;

typedef enum
{
	var_id_storage_time = 0,
	var_id_storage_event_flags,
	var_id_storage_system_flags,
	var_id_storage_temperature,
	var_id_storage_pressure,
	var_id_storage_altitude,
	var_id_storage_speed,
	var_id_storage_accel_x,
	var_id_storage_accel_y,
	var_id_storage_accel_z,
	var_id_storage_gyro_x,
	var_id_storage_gyro_y,
	var_id_storage_gyro_z,
	var_id_storage_mag_x,
	var_id_storage_mag_y,
	var_id_storage_mag_z,
	var_id_storage_latitude,
	var_id_storage_longitude,
	var_id_storage_gps_altitude,
	var_id_storage_satellites
}VARIABLE_TYPE_STORAGE_e;

typedef enum
{
	var_id_telemetry_time,
	var_id_telemetry_event_flags,
	var_id_telemetry_system_flags,
	var_id_telemetry_system_voltage,
	var_id_telemetry_pressure,
	var_id_telemetry_altitude,
	var_id_telemetry_speed,
	var_id_telemetry_latitude,
	var_id_telemetry_longitude,
	var_id_telemetry_gps_altitude,
	var_id_telemetry_satellites
}VARIABLE_TYPE_TELEMETRY_e;

typedef enum
{
	var_id_fc_config_recovery_drogue_delay,
	var_id_fc_config_recovery_main_deploy_alt,
	var_id_fc_config_recovery_pyro_igintion_time,
	var_id_fc_config_aux_pyro_c_enable,
	var_id_fc_config_aux_pyro_c_trigger,
	var_id_fc_config_aux_pyro_c_delay,
	var_id_fc_config_aux_pyro_d_enable,
	var_id_fc_config_aux_pyro_d_trigger,
	var_id_fc_config_aux_pyro_d_delay,
	var_id_fc_config_aux_pyro_e_enable,
	Var_id_fc_config_aux_pyro_e_trigger,
	var_id_fc_config_aux_pyro_e_delay,
	var_id_fc_config_aux_pyro_f_enable,
	var_id_fc_config_aux_pyro_f_trigger,
	var_id_fc_config_aux_pyro_f_delay,
	var_id_fc_config_telemetry_transmit_power,
	var_id_fc_config_telemetry_baud_rate,
	var_id_fc_config_telemetry_channel,
	var_id_fc_config_telemetry_transmit_rate,
	var_id_fc_config_telemtery_landed_rate,
	var_id_fc_config_telemetry_callsign,
	var_id_fc_config_aux_sys_enable_pwm,
	var_id_fc_config_aux_sys_enable_buzzer,
	var_id_fc_config_aux_sys_buzzer_freq,
	var_id_fc_config_aux_sys_buzzer_pattern
}VARIABLE_TYPE_FC_CONFIG_e;

typedef enum
{
	var_id_sens_calibration_cal_requesting,
	var_id_sens_calibration_cal_accel_select,
	var_id_sens_calibration_cal_accel_status,
	var_id_sens_calibration_cal_mag_select,
	var_id_sens_calibration_cal_mag_status
}VARIABLE_TYPE_SENS_CALIBRATION_e;

typedef enum
{
	var_id_sys_testing_test_requesting,
	var_id_sys_testing_test_pyro_A,
	var_id_sys_testing_test_pyro_A_status,
	var_id_sys_testing_test_pyro_B,
	var_id_sys_testing_test_pyro_B_status,
	var_id_sys_testing_test_pyro_C,
	var_id_sys_testing_test_pyro_C_status,
	var_id_sys_testing_test_pyro_D,
	var_id_sys_testing_test_pyro_D_status,
	var_id_sys_testing_test_pyro_E,
	var_id_sys_testing_test_pyro_E_status,
	var_id_sys_testing_test_pyro_F,
	var_id_sys_testing_test_pyro_F_status,
	var_id_sys_testing_test_telemetry,
	var_id_sys_testing_test_telemetry_status,
	var_id_sys_testing_test_gps,
	var_id_sys_testing_test_gps_status,
	var_id_sys_testing_test_accelerometer_1,
	var_id_sys_testing_test_accelerometer_1_status,
	var_id_sys_testing_test_accelerometer_2,
	var_id_sys_testing_test_accelerometer_2_status
}VARIABLE_TYPE_SYS_TESTING_e;


// data packet structures
typedef struct 
{
 // System (7 Bytes)
	float time; // Milliseconds (note: Overflow in ~50 days)
    float event_flags; // launch, coast_1, sustainer_iginition, coast_2, apogee, drogue_deploy, main_deploy, land
    float system_flags; //
    float temperature; // Celsius (specially formatted)

    // Barometer (10 Bytes)
    float pressure; // Pascal
    float altitude; // Meters
    float speed; // meters per second

    // Accelerometers (12 Bytes)
    float accel_x; // Acceleration in direction of flight (m/s^2) (will swap accelerometers when sensor is saturated)
    float accel_y;
    float accel_z;

    // Gyroscope (12 Bytes)
    float gyro_x; // degrees per second
    float gyro_y;
    float gyro_z;

    // Magnetometer (12 Bytes)
    float mag_x; // Gauss
    float mag_y;
    float mag_z;

    // GPS (11 bytes)
    float latitude; // latitude in degrees
    float longitude; // longitude in degrees
    float gps_altitude; // meters (from GPS)
    float satellites; // number of satellites for GPS lock
} PACKET_STORAGE_s;

typedef struct 
{
    // system (7 bytes)
    float time; // Milliseconds
    float event_flags; // Launch phase
    float system_flags; // Status of system
    float system_voltage; // encoded voltage

    // performance metrics (12 bytes)
    float pressure; // Pascal
    float altitude; // Meters
    float speed; // meters per second

    // location data (13 bytes)
    float latitude; // latitude in deg
    float longitude; // longitude in degrees
    float gps_altitude; // meters (from GPS)
    float satellites; // number of satellites for GPS lock
    
} PACKET_TELEMETRY_s; // 32 bytes

typedef struct 
{
    // Recovery
    float recovery_drogue_delay; // seconds after flag trigger
    float recovery_main_deploy_alt; // meters AGL for main deploy
    float recovery_pyro_igintion_time; // time pyro is active (encoded)

    // Auxiliary Pyro
    float aux_pyro_c_enable; // enable pyro c
    float aux_pyro_c_trigger; // encoded flag trigger
    float aux_pyro_c_delay; // encoded delay after flag trigger
    float aux_pyro_d_enable; // enable pyro d
    float aux_pyro_d_trigger; // encoded flag trigger
    float aux_pyro_d_delay; // encoded delay after flag trigger
    float aux_pyro_e_enable; // enable pyro e
    float aux_pyro_e_trigger; // encoded flag trigger
    float aux_pyro_e_delay; // encoded delay after flag trigger
    float aux_pyro_f_enable; // enable pyro f
    float aux_pyro_f_trigger; // encoded flag trigger
    float aux_pyro_f_delay; // encoded delay after flag trigger

    // Telemetry
    float telemetry_transmit_power; // encoded transmit power
    float telemetry_baud_rate; // encoded serial baud rate
    float telemetry_channel; // channels (1-100)
    float telemetry_transmit_rate; // rate in Hz of telemetry transmition during flight
    float telemtery_landed_rate; // rate in secs of telemetry transmition after landing
    float telemetry_callsign; // telemetry HAM callsign (required by FCC rules)

    // Auxiliary systems
    float aux_sys_enable_pwm; // enable PWM for external servo control (feature not implemented yet!)
    float aux_sys_enable_buzzer; // enable buzzer
    float aux_sys_buzzer_freq; // set at what frequency buzzer operates at (default: 2048 Hz)
    float aux_sys_buzzer_pattern; // encoded select of buzzing pattern

} PACKET_FC_CONFIG_s;

typedef struct
{
    float cal_requesting; // 1 when GSapp is sending out request, 0 when receiving

    float cal_accel_select; // request accelerometers to be calibrated
    float cal_accel_status; // encoded status of accelerometers calibration
    
    float cal_mag_select; // request magnetometers to be calibrated
    float cal_mag_status; // encoded status of magnetometer calibration

} PACKET_SENS_CALBRATION_s;


typedef struct 
{
    // Testing of subsystems
    float test_requesting; // 1 when GSapp is sending out request, 0 when receiving

    // pyro systems
    float test_pyro_A;
    float test_pyro_A_status;

    float test_pyro_B;
    float test_pyro_B_status;

    float test_pyro_C;
    float test_pyro_C_status;

    float test_pyro_D;
    float test_pyro_D_status;

    float test_pyro_E;
    float test_pyro_E_status;

    float test_pyro_F;
    float test_pyro_F_status;

    // telemetry subsystem
    float test_telemetry;
    float test_telemetry_status;

    // GPS subsystem
    float test_gps;
    float test_gps_status;

    // acceleroemter 1 subsystem
    float test_accelerometer_1;
    float test_accelerometer_1_status;

    // accelerometer 2  subsystem
    float test_accelerometer_2;
    float test_accelerometer_2_status;
    
} PACKET_SYS_TESTING_s;


#ifdef __cplusplus
}
#endif

#endif
