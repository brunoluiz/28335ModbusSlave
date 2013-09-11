/*
 * mb_serial.h
 *
 *  Created on: 03/09/2013
 *      Author: Bruno Luiz
 */

#ifndef MB_SERIAL_H_
#define MB_SERIAL_H_
#include "mb_main.h"

void serial_init(Uint32 ulBaudRate, Uint16 ucDataBits, Uint16 eParity );
void serial_interrupt_switch( Uint16 xRxEnable, Uint16 xTxEnable );
void serial_send_data();
interrupt void serial_interrupt_rx();
interrupt void serial_interrupt_tx();

void clear_rx_frame();
void clear_tx_frame();

#endif /* MB_SERIAL_H_ */
