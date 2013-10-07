/*
 * mb_main.h
 *
 *  Created on: 03/09/2013
 *      Author: Bruno Luiz
 */

#ifndef MB_MAIN_H_
#define MB_MAIN_H_

#include "DSP28x_Project.h"
#include <cmath>

#include "mb_timer.h"
#include "mb_serial.h"

// Settings
// Modify this settings according to your project
#define MB_SLAVE_ID				0x01			// Device ID
#define MB_BAUDRATE 			9600			// Baud rate
#define MB_PARITY				MB_PAR_NONE		// Bits parity
#define MB_BITS_QNT				8				// Bits quantity
#define MB_FRAME_CHAR_TOTALS	512				// Quantity of chars at a frame



// DON´T MODIFY THE CODE BELOW!

// Parity constants
#define MB_PAR_EVEN			1
#define MB_PAR_ODD 			2
#define MB_PAR_NONE			0

// Read codes
#define MB_FUNC_READ_COIL 					1
#define MB_FUNC_READ_INPUT					2
#define MB_FUNC_READ_HOLDINGREGISTERS		3
#define MB_FUNC_READ_INPUTREGISTERS			4

//Write codes
#define MB_FUNC_FORCE_COIL 					5
#define MB_FUNC_WRITE_REGISTER				6
#define MB_FUNC_FORCE_NCOILS				15
#define MB_FUNC_WRITE_NREGISTERS			16

// Error codes
#define MB_ERROR_ILLEGALFUNC	0x01
#define MB_ERROR_ILLEGALADDR	0x02
#define MB_ERROR_ILLEGALDATA	0x03
#define MB_ERROR_SLVFAILURE		0x04
#define MB_ERROR_ACKNOWLEDGE	0x05
#define MB_ERROR_SLVBUSY		0x06
#define MB_ERROR_NEGACKNOWLEDGE	0x07
#define MB_ERROR_MEMPARITY		0x08
#define MB_ERROR_GATEWAYPATHUN	0x0A

// State machine
#define MB_STATE_T35	 		0
#define MB_STATE_WAITING 		1
#define MB_STATE_READING		2
#define MB_STATE_T35_SEND 		3
#define MB_STATE_SENDING		4

// Modbus core functions
void modbus_init(Uint32 ulBaudRate, Uint16 ucDataBits, Uint16 eParity );	// Initialize MODBUS
void modbus_chk_states();			// "FSM" for checking the actual state and take the right action
Uint16 modbus_prep_response();		// Prepare a response to send to the server
Uint16 modbus_read_func();			// Function to read data from DSP
Uint16 modbus_write_func();			// Function to write data to DSP
Uint16 modbus_error(Uint16 type);	// Prepare a frame to send info about an error to the server

// Modbus auxiliary
Uint16 generate_crc(Uint16 buf[], int len);		// Generate CRC code
Uint32 memory_map(Uint16 type);					// Give the memory map (the allowed start address)
void reset_data_pointers();						// Resets the rx_frame and tx_frame data pointers

// Utilities
void swap_values(Uint16 *val1, Uint16 *val2);	// Swap the val1 to val2 and val2 to val1
void clear_rx_frame();							// Clear the rx_frame (zeroing all the frame)
void clear_tx_frame();							// Clear the tx_frame (zeroing all the frame)

// Extern  / Global variables
// The statement "extern" permits the use of the variable at any file which includes this header
extern Uint16 rx_frame[MB_FRAME_CHAR_TOTALS];
extern Uint16 rx_frame_pointer;
extern Uint16 tx_frame[MB_FRAME_CHAR_TOTALS];
extern Uint16 tx_frame_pointer;
extern Uint32 usTimerT35;
extern Uint16 modbus_state;

#endif /* MB_MAIN_H_ */
