#ifndef MODBUS_H_
#define MODBUS_H_

#include "PlataformSettings.h"

// Settings ===============================================
// Modify this settings according to your project
#define MB_SLAVE_ID		0x01			// Device ID
#define MB_MODE 		MB_MODE_SLAVE
#define MB_RTU_TCP		true			// Disable ID check and CRC check (TCP converter will do that)

// Log settings ===========================================
// Will be used at Log.h
#define MB_DATA_REQUEST_DEBUG_ENABLED		true
#define MB_DATA_RESPONSE_DEBUG_ENABLED		true
#define MB_DATA_HANDLER_DEBUG_ENABLED		true
#define MB_SLAVE_DEBUG_ENABLED				true
#define SERIAL_DEBUG_ENABLED				true
#define TIMER_DEBUG_ENABLED					true

#ifndef DEBUG_LOG_ENABLED
#define DEBUG_LOG_ENABLED	false
#endif

// DON'T MODIFY THE CODE BELOW!

// T3.55char time
#define MB_T35 			( 7UL * 220000UL ) / ( 2UL * SERIAL_BAUDRATE )

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
	MB_PRERECEIVE,
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

typedef enum {
	MB_READ_ADDRESS_HIGH,
	MB_READ_ADDRESS_LOW,
	MB_READ_TOTALDATA_HIGH,
	MB_READ_TOTALDATA_LOW
} ModbusContentRequestReadIndexes;


typedef enum {
	MB_WRITE_ADDRESS_HIGH,
	MB_WRITE_ADDRESS_LOW,
	MB_WRITE_VALUE_HIGH,
	MB_WRITE_VALUE_LOW
} ModbusContentRequestWriteIndexes;

typedef enum {
	MB_WRITE_N_ADDRESS_HIGH,
	MB_WRITE_N_ADDRESS_LOW,
	MB_WRITE_N_QUANTITY_HIGH,
	MB_WRITE_N_QUANTITY_LOW,
	MB_WRITE_N_BYTES,
	MB_WRITE_N_VALUES_START_HIGH,
	MB_WRITE_N_VALUES_START_LOW
} ModbusContentRequestWriteNIndexes;

// Size constants (with CRC) ==============================

#define MB_SIZE_COMMON_DATA			4 // Slave address (1 byte) + Function Code (1 byte) + CRC (2 bytes)
#define MB_SIZE_CONTENT_NORMAL		4
// Response sizes
#define MB_SIZE_RESP_READ_MINIMUM	MB_SIZE_COMMON_DATA + 1 // MB_SIZE_COMMON_DATA + Number of requested registers (1 byte)
#define MB_SIZE_RESP_WRITE			MB_SIZE_COMMON_DATA + MB_SIZE_CONTENT_NORMAL // MB_SIZE_COMMON_DATA + Data address (2 bytes) + Value writen (2 bytes)
#define MB_SIZE_RESP_WRITE_N		MB_SIZE_COMMON_DATA + MB_SIZE_CONTENT_NORMAL // MB_SIZE_COMMON_DATA + Data address (2 bytes) + Number of writen values (2 bytes)
#define MB_SIZE_RESP_EXCEPTION		MB_SIZE_COMMON_DATA + 1
// Request sizes
#define MB_SIZE_REQ_WRITE_N_MINIMUM	MB_SIZE_COMMON_DATA + 5 // Data addr (2 bytes) + Number of registers (2 bytes) + Following data (1 byte)
#define MB_SIZE_REQ_READ			MB_SIZE_COMMON_DATA + MB_SIZE_CONTENT_NORMAL
#define MB_SIZE_REQ_WRITE			MB_SIZE_COMMON_DATA + MB_SIZE_CONTENT_NORMAL

// Size constants (without CRC) ===========================

#define MB_SIZE_COMMON_DATA_WITHOUTCRC			MB_SIZE_COMMON_DATA - 2
// Response sizes
#define MB_SIZE_RESP_READ_MINIMUM_WITHOUTCRC	MB_SIZE_RESP_READ_MINIMUM - 2
#define MB_SIZE_RESP_WRITE_WITHOUTCRC			MB_SIZE_RESP_WRITE - 2
#define MB_SIZE_RESP_WRITE_N_WITHOUTCRC			MB_SIZE_RESP_WRITE_N - 2
#define MB_SIZE_RESP_EXCEPTION_WITHOUTCRC		MB_SIZE_RESP_EXCEPTION - 2
// Request sizes
#define MB_SIZE_REQ_WRITE_N_MINIMUM_WITHOUTCRC	MB_SIZE_REQ_WRITE_N_MINIMUM - 2
#define MB_SIZE_REQ_READ_WITHOUTCRC				MB_SIZE_REQ_READ - 2
#define MB_SIZE_REQ_WRITE_WITHOUTCRC			MB_SIZE_REQ_WRITE - 2

#endif /* MODBUS_H_ */
