/*
 * mb_serial.h
 *
 *  Created on: 03/09/2013
 *      Author: Bruno Luiz
 */

#ifndef MB_SERIAL_H_
#define MB_SERIAL_H_
#include "mb_main.h"

void serial_init(Uint32 ulBaudRate, Uint16 ucDataBits, Uint16 eParity );	// Init the serial
void serial_interrupt_switch( Uint16 xRxEnable, Uint16 xTxEnable );			// Enable or disable the receiver and transmitter
void serial_send_data();					// Send data
interrupt void serial_interrupt_rx();		// Manage receiver interrupt
interrupt void serial_interrupt_tx();		// Manage transmitter interrupt

/*void clear_rx_frame();		// Clear
void clear_tx_frame();*/

#endif /* MB_SERIAL_H_ */
