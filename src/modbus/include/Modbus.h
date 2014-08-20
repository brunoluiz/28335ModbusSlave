#ifndef MODBUS_H_
#define MODBUS_H_

#include "PlataformSettings.h"

// Settings ==========================
// Modify this settings according to your project
#define MODBUS_SLAVE_ID				0x11			// Device ID
#define MODBUS_BAUDRATE 			9600			// Baud rate
#define MODBUS_PARITY				MB_PARITY_NONE	// Bits parity
#define MODBUS_BITS_QNT				8				// Bits quantity
#define MODBUS_FIFO_ENABLED			1				// FIFO mode
#define MODBUS_MODE 				MB_MODE_SLAVE
#define MODBUS_T35 					( 7UL * 220000UL ) / ( 2UL * MODBUS_BAUDRATE ) // T35char time

// Log settings =======================
// Will be used at Log.h
#define DEBUG_LOG_ENABLED	0

#define MB_DATA_REQUEST_DEBUG_ENABLED		0
#define MB_DATA_RESPONSE_DEBUG_ENABLED		0
#define MB_DATA_HANDLER_DEBUG_ENABLED		0
#define MB_SLAVE_DEBUG_ENABLED				0
#define SERIAL_DEBUG_ENABLED				1
#define TIMER_DEBUG_ENABLED					1

// DON'T MODIFY THE CODE BELOW!

// Modbus Mode Constants
 typedef enum {
	MB_MODE_MASTER,
	MB_MODE_SLAVE
} ModbusMode;

// Read codes
 typedef enum {
	MB_FUNC_READ_COIL = 1,
	MB_FUNC_READ_INPUT,
	MB_FUNC_READ_HOLDINGREGISTERS,
	MB_FUNC_READ_INPUTREGISTERS,
	MB_FUNC_FORCE_COIL,
	MB_FUNC_WRITE_HOLDINGREGISTER,
	MB_FUNC_FORCE_NCOILS = 15,
	MB_FUNC_WRITE_NREGISTERS = 16
} ModbusFunctionCode;

// State machine constants
typedef enum {
	MB_CREATE,
	MB_START,
	MB_TIMER_T35_WAIT,
	MB_IDLE,
	MB_RECEIVE,
	MB_PROCESS,
	MB_TRANSMIT,
	MB_DESTROY
} ModbusState;

// Error codes
typedef enum {
	MB_ERROR_ILLEGALFUNC = 1,
	MB_ERROR_ILLEGALADDR,
	MB_ERROR_ILLEGALDATA,
	MB_ERROR_SLVFAILURE,
	MB_ERROR_ACKNOWLEDGE,
	MB_ERROR_SLVBUSY,
	MB_ERROR_NEGACKNOWLEDGE,
	MB_ERROR_MEMPARITY,
	MB_ERROR_GATEWAYPATHUN
} ModbusError;

#endif /* MODBUS_H_ */
