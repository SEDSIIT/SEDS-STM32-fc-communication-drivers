/*
 * INFO:
 * This is a header file defining the structure for data packets 
 * that will be transmitted between the Nova flight computer and
 * the ground station software via USB and telemetry communication
 *
 * UPDATED:
 * 03/28/22
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
	// Transmission mode value, 1 = receiving/ 0 = sending
	int transmissionMode;
	// Packet type is determined from the PACKET_TYPE_e enum.
	int packetType;
	// Variable type is determined from the # that the specific variable is listed in the struct.
	// I.e, in system, time is considered number 0.
	int variableNumber;
	// data is (probably) an undetermined data type that we will cast into what we need I made it uint32 for now because its big. :-)
	uint32_t data;
	char ender;
}MESSAGE_PACKET_s;

// use union to convert between data types easier
typedef union data_4byte
{
    uint8_t uint8[4]; // 1 Byte form ([3]<<24|[2]<<16|[1]<<8|[0])
    uint32_t uint32;
    int32_t int32;
    float f;
} data_4byte;

typedef union data_2byte
{
    uint8_t uint8[2]; // 1 Byte form ([1]<<8|[0])
    uint16_t uint16;
    int16_t int16;
} data_2byte;

// header ID to identify packet type
typedef enum
{
    packet_id_storage = 0,
    packet_id_telemetry = 1,
    packet_id_fc_config = 2,
    packet_id_sense_calibration = 3,
    packet_id_data_request = 4,
    packet_id_sys_testing = 5
} PACKET_TYPE_e;

// data packet structures
typedef struct 
{
 // System (7 Bytes)
    union data_4byte time; // Milliseconds (note: Overflow in ~50 days)
    uint8_t event_flags; // launch, coast_1, sustainer_iginition, coast_2, apogee, drogue_deploy, main_deploy, land
    uint8_t system_flags; //
    uint8_t temperature; // Celsius (specially formatted)

    // Barometer (10 Bytes)
    union data_4byte pressure; // Pascal
    union data_4byte altitude; // Meters
    union data_2byte speed; // meters per second

    // Accelerometers (12 Bytes)
    union data_4byte accel_x; // Acceleration in direction of flight (m/s^2) (will swap accelerometers when sensor is saturated)
    union data_4byte accel_y;
    union data_4byte accel_z;

    // Gyroscope (12 Bytes)
    union data_4byte gyro_x; // degrees per second
    union data_4byte gyro_y;
    union data_4byte gyro_z;

    // Magnetometer (12 Bytes)
    union data_4byte mag_x; // Gauss
    union data_4byte mag_y;
    union data_4byte mag_z;

    // GPS (11 bytes)
    union data_4byte latitude; // latitude in degrees
    union data_4byte longitude; // longitude in degrees
    union data_2byte gps_altitude; // meters (from GPS)
    uint8_t satellites; // number of satellites for GPS lock
   
} PACKET_STORAGE_s;

typedef struct 
{
    // system (7 bytes)
    union data_4byte time; // Milliseconds
    uint8_t event_flags; // Launch phase
    uint8_t system_flags; // Status of system
    uint8_t system_voltage; // encoded voltage

    // performance metrics (12 bytes)
    union data_4byte pressure; // Pascal
    union data_4byte altitude; // Meters
    union data_4byte speed; // meters per second

    // location data (13 bytes)
    union data_4byte latitude; // latitude in deg
    union data_4byte longitude; // longitude in degrees
    union data_4byte gps_altitude; // meters (from GPS)
    uint8_t satellites; // number of satellites for GPS lock
    
} PACKET_TELEMETRY_s; // 32 bytes

typedef struct 
{
    // Recovery
    uint8_t recovery_drogue_delay; // seconds after flag trigger
    union data_2byte recovery_main_deploy_alt; // meters AGL for main deploy
    uint8_t recovery_pyro_igintion_time; // time pyro is active (encoded)

    // Auxiliary Pyro
    uint8_t aux_pyro_c_enable; // enable pyro c
    uint8_t aux_pyro_c_trigger; // encoded flag trigger
    uint8_t aux_pyro_c_delay; // encoded delay after flag trigger
    uint8_t aux_pyro_d_enable; // enable pyro d
    uint8_t aux_pyro_d_trigger; // encoded flag trigger
    uint8_t aux_pyro_d_delay; // encoded delay after flag trigger
    uint8_t aux_pyro_e_enable; // enable pyro e
    uint8_t aux_pyro_e_trigger; // encoded flag trigger
    uint8_t aux_pyro_e_delay; // encoded delay after flag trigger
    uint8_t aux_pyro_f_enable; // enable pyro f
    uint8_t aux_pyro_f_trigger; // encoded flag trigger
    uint8_t aux_pyro_f_delay; // encoded delay after flag trigger

    // Telemetry
    uint8_t telemetry_transmit_power; // encoded transmit power
    uint8_t telemetry_baud_rate; // encoded serial baud rate
    uint8_t telemetry_channel; // channels (1-100)
    uint8_t telemetry_transmit_rate; // rate in Hz of telemetry transmition during flight
    uint8_t telemtery_landed_rate; // rate in secs of telemetry transmition after landing
    char telemetry_callsign[6]; // telemetry HAM callsign (required by FCC rules)

    // Auxiliary systems
    uint8_t aux_sys_enable_pwm; // enable PWM for external servo control (feature not implemented yet!)
    uint8_t aux_sys_enable_buzzer; // enable buzzer
    union data_2byte aux_sys_buzzer_freq; // set at what frequency buzzer operates at (default: 2048 Hz)
    uint8_t aux_sys_buzzer_pattern; // encoded select of buzzing pattern

} PACKET_FC_CONFIG_s;

typedef struct
{
    uint8_t cal_requesting; // 1 when GSapp is sending out request, 0 when receiving

    uint8_t cal_accel_select; // request accelerometers to be calibrated
    uint8_t cal_accel_status; // encoded status of accelerometers calibration
    
    uint8_t cal_mag_select; // request magnetometers to be calibrated
    uint8_t cal_mag_status; // encoded status of magnetometer calibration

} PACKET_SENS_CALBRATION_s;

typedef struct 
{
    uint8_t data_requesting; // 1 when GSapp is sending out request, 0 when receiving
    PACKET_STORAGE_s data_packet; // send data stored on FC flash IC
} PACKET_DATA_REQUEST_s;

typedef struct 
{
    // Testing of subsystems
    uint8_t test_requesting; // 1 when GSapp is sending out request, 0 when receiving

    // pyro systems
    uint8_t test_pyro_A; 
    uint8_t test_pyro_A_status;

    uint8_t test_pyro_B;
    uint8_t test_pyro_B_status;

    uint8_t test_pyro_C;
    uint8_t test_pyro_C_status;

    uint8_t test_pyro_D;
    uint8_t test_pyro_D_status;

    uint8_t test_pyro_E;
    uint8_t test_pyro_E_status;

    uint8_t test_pyro_F;
    uint8_t test_pyro_F_status;

    // telemetry subsystem
    uint8_t test_telemetry;
    uint8_t test_telemetry_status;

    // GPS subsystem
    uint8_t test_gps;
    uint8_t test_gps_status;

    // acceleroemter 1 subsystem
    uint8_t test_accelerometer_1;
    uint8_t test_accelerometer_1_status;

    // accelerometer 2  subsystem
    uint8_t test_accelerometer_2;
    uint8_t test_accelerometer_2_status;
    
} PACKET_SYS_TESTING_s;


#ifdef __cplusplus
}
#endif

#endif
