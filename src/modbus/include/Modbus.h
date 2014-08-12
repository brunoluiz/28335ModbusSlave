#ifndef MODBUS_H_
#define MODBUS_H_

// #include "DSP28x_Project.h"

// Settings
// Modify this settings according to your project
#define MODBUS_SLAVE_ID				0x11			// Device ID
#define MODBUS_BAUDRATE 			9600			// Baud rate
#define MODBUS_PARITY				MB_PARITY_NONE	// Bits parity
#define MODBUS_BITS_QNT				8				// Bits quantity
#define MODBUS_FRAME_CHAR_TOTALS	512				// Quantity of chars at a frame
#define MODBUS_FIFO_ENABLED			1				// FIFO mode
#define MODBUS_MODE 				MB_MODE_SLAVE
#define X86_DEBUG					1
//do { fprintf(stderr, "%s:%d:%s() ## " fmt "\n", __FILE__, \
//	__LINE__, __FUNCTION__, __VA_ARGS__); } while (0)

// DON´T MODIFY THE CODE BELOW!

// Parity constants
 typedef enum {
	MB_MODE_MASTER,
	MB_MODE_SLAVE,
} ModbusMode;

// Parity constants
 typedef enum {
	MB_PARITY_EVEN,
	MB_PARITY_ODD,
	MB_PARITY_NONE
} ModbusParity;

// Read codes
 typedef enum {
	MB_FUNC_READ_COIL = 1,
	MB_FUNC_READ_INPUT,
	MB_FUNC_READ_HOLDINGREGISTERS,
	MB_FUNC_READ_INPUTREGISTERS,
	MB_FUNC_FORCE_COIL,
	MB_FUNC_WRITE_HOLDINGREGISTER,
	MB_FUNC_FORCE_NCOILS = 15,
	MB_FUNC_WRITE_NREGISTERS = 16,
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

typedef int Uint16;

#define false   0
#define true    1
#define bool int

#endif /* MODBUS_H_ */
