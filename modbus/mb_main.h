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
#define MB_SLAVE_ID	17

// Parity codes
#define MB_PAR_EVEN	1	//Par
#define MB_PAR_ODD 	2	//Ímpar
#define MB_PAR_NONE 0	//Nenhum

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

// Quantity of chars at a frame
#define MB_FRAME_CHAR_TOTALS	512

// Modbus core functions
void modbus_init(Uint32 ulBaudRate, Uint16 ucDataBits, Uint16 eParity );
void modbus_chk_states();
Uint16 modbus_prep_response();
Uint16 modbus_read_func();
Uint16 modbus_write_func();
Uint16 modbus_error(Uint16 type);

// Modbus auxiliary
Uint16 generate_crc(Uint16 buf[], int len);
Uint32 memory_map(Uint16 type);
void reset_data_pointers();

// Utilities
void swap_values(Uint16 *val1, Uint16 *val2);
void clear_rx_frame();
void clear_tx_frame();

// Extern  / Global variables
extern Uint16 rx_frame[MB_FRAME_CHAR_TOTALS];
extern Uint16 rx_frame_pointer;
extern Uint16 tx_frame[MB_FRAME_CHAR_TOTALS];
extern Uint16 tx_frame_pointer;
extern Uint32 usTimerT35;
extern Uint16 modbus_state;

#endif /* MB_MAIN_H_ */
