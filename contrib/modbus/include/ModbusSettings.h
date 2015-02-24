#ifndef MODBUSSETTINGS_H_
#define MODBUSSETTINGS_H_

#include "DataTypes.h"
#include "ModbusDefinitions.h"
#include "Serial.h"

// Modbus settings ========================================
#define MB_SLAVE_ID				0x01
#define MB_CHECKS				true			// ID check and CRC check (enabled by default)
#define MB_BUFFER_SIZE			96				// Size of content buffer on ModbusData - Depends on the size of your memory map
#define MB_BROADCAST_EXTRA_ID	255		// Broadcast exclusive ID (it will work with Broadcast ID 0 too)
#define MB_DATA_MAP				"ModbusDataMap.h"

// Memory map settings ====================================
#define MB_COILS_ENABLED				1
#define MB_INPUTS_ENABLED				0
#define	MB_HOLDING_REGISTERS_ENABLED	1
#define MB_INPUT_REGISTERS_ENABLED		0

// Serial settings ========================================
#define SERIAL_BAUDRATE 	38400
#define SERIAL_PARITY 		SERIAL_PARITY_NONE
#define SERIAL_BITS_NUMBER 	8

#define SERIAL_START_STOP_NUMBER_BITS		2
#if SERIAL_PARITY == SERIAL_PARITY_NONE
#define SERIAL_PARITY_NUMBER_BITS	0
#else
#define SERIAL_PARITY_NUMBER_BITS	1
#endif

// Debug settings =========================================
#define DEBUG_UTILS_PROFILING				0		// Include the necessary files to the linker!!
#define DEBUG_LOG_ENABLED					0

// Will be used at Log.h
#define MB_DATA_REQUEST_DEBUG_ENABLED		0
#define MB_DATA_RESPONSE_DEBUG_ENABLED		0
#define MB_DATA_HANDLER_DEBUG_ENABLED		0
#define MB_SLAVE_DEBUG_ENABLED				0
#define SERIAL_DEBUG_ENABLED				0
#define TIMER_DEBUG_ENABLED					0

#endif /* MODBUSSETTINGS_H_ */
