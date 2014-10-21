#ifndef MODBUSSETTINGS_H_
#define MODBUSSETTINGS_H_

#include "ModbusDefinitions.h"
#include "Serial.h"

// Modbus settings ========================================
#define MB_SLAVE_ID			0x01
#define MB_RTU_TCP			false			// Disable ID check and CRC check (TCP converter will do that)

// Serial settings ========================================

#define SERIAL_BAUDRATE 	38400
#define SERIAL_PARITY 		SERIAL_PARITY_NONE
#define SERIAL_BITS_NUMBER 	8

#define SERIAL_START_STOP_NUMBER_BITS		2
#if SERIAL_PARITY != SERIAL_PARITY_NONE
	#define SERIAL_PARITY_NUMBER_BITS	1
#else
	#define SERIAL_PARITY_NUMBER_BITS	0
#endif


// Log settings ===========================================
#define DEBUG_UTILS_PROFILING				true

// Will be used at Log.h
#define DEBUG_LOG_ENABLED					false

#define MB_DATA_REQUEST_DEBUG_ENABLED		true
#define MB_DATA_RESPONSE_DEBUG_ENABLED		true
#define MB_DATA_HANDLER_DEBUG_ENABLED		true
#define MB_SLAVE_DEBUG_ENABLED				true
#define SERIAL_DEBUG_ENABLED				true
#define TIMER_DEBUG_ENABLED					true

#endif /* MODBUSSETTINGS_H_ */
