/*
 * mb_serial.c
 *
 *  Created on: 03/09/2013
 *      Author: Bruno Luiz
 */

#include "mb_serial.h"

/*
 * Manage the serial receive interrupt
 * Clear the Pie/interrupt registers
 * Reset the timer
 * Change the state (for the FSM)
 * Get the received data and put at rx_frame
 */
interrupt void serial_interrupt_rx()
{
	timer_reset();

	// Change the actual state for "reading"
	modbus_state = MB_STATE_READING;

	// Put the receive data at rx_frame and increase the location pointer
	rx_frame[rx_frame_pointer] = SciaRegs.SCIRXBUF.all;
	rx_frame_pointer++;

	//Clears the interrupt
	PieCtrlRegs.PIEACK.all|=0x100;
}

/*
 * Manage the serial transmission interrupt
 * Clean the Pie/interrupt registers
 */
interrupt void serial_interrupt_tx()
{
	//Clears the interrupt
	PieCtrlRegs.PIEACK.all|=0x100;
}

/*
 * Enable or disable receiver and transmitter
 * xRxEnable - state of receiver: 		0 = disable | 1 = enable
 * xTxEnable - state of transmitter: 	0 = disable | 1 = enable
 */
void serial_interrupt_switch( Uint16 xRxEnable, Uint16 xTxEnable )
{
	//Enable or disable RX (receiver) interrupt
	if(xRxEnable)
		SciaRegs.SCICTL2.bit.RXBKINTENA =1;
	else
		SciaRegs.SCICTL2.bit.RXBKINTENA =0;

	//Enable or disable TX (transmiter) interrupt
	if(xTxEnable)
		SciaRegs.SCICTL2.bit.TXINTENA =1;
	else
		SciaRegs.SCICTL2.bit.TXINTENA =0;
}

/*
 * Send the data back to the server
 * Disable timer (it will not receive any other command)
 * Prepare the response
 * Send the data from tx_frame (it was previously prepared)
 */
void serial_send_data(){
	// Disable timer
	timer_disable();
	// If the received ID is the same of the device (specified at mb_main.h)
	if(rx_frame[0] == MB_SLAVE_ID)
	{
		int i = 0;
		// Get the length of the tx_frame
		Uint16 frame_lenght = modbus_prep_response();

		// Loop the tx_frame and send bit by bit to the buffer
		for(i = 0; i < frame_lenght; i++)
		{
			// Send tx_fram to TX Buffer
			SciaRegs.SCITXBUF= tx_frame[i];
			// Wait until it was fully sent
			while (!SciaRegs.SCICTL2.bit.TXEMPTY) {}
		}

	}
	// Clear the tx_frame[] and rx_frame[]
	clear_tx_frame();
	clear_rx_frame();
	// Re-enable the timer, allowing the DSP get new data
	timer_enable();
}

/*
 * Initialize the serial, without FIFO and with the given arguments
 * ulBaudRate: baud rate of the system
 * ucDataBits: number of bits which will be transmitted - the most common is 8
 * eParity: the type of parity which will be used - the most common is none
 */
void serial_init(Uint32 ulBaudRate, Uint16 ucDataBits, Uint16 eParity  )
{
	Uint32 baud;

	// Map interrupts to the correct functions
	PieVectTable.SCIRXINTA = &serial_interrupt_rx;
	PieVectTable.SCITXINTA = &serial_interrupt_tx;

	// 1 stop bit,  No loopback
	// No parity,8 char bits,
	// async mode, idle-line protocol
	SciaRegs.SCICCR.all =0x0007;

	// Enable TX, RX, internal SCICLK,
	// Disable RX ERR, SLEEP, TXWAKE
	SciaRegs.SCICTL1.all =0x0003;
	SciaRegs.SCIFFTX.bit.SCIFFENA = 0;

	// Number of bits to send
	switch(ucDataBits){
		case 8:
			SciaRegs.SCICCR.bit.SCICHAR = 0x7;
			break;
		case 7:
			SciaRegs.SCICCR.bit.SCICHAR = 0x6;
			break;
		default:
			SciaRegs.SCICCR.bit.SCICHAR = 0x7;
	}

	// Parity settings
	switch(eParity){
		case MB_PAR_EVEN:
			SciaRegs.SCICCR.bit.PARITYENA = 1;
			SciaRegs.SCICCR.bit.PARITY = 1;
			break;
		case MB_PAR_ODD:
			SciaRegs.SCICCR.bit.PARITYENA = 1;
			SciaRegs.SCICCR.bit.PARITY = 0;
			break;
		case MB_PAR_NONE:
			SciaRegs.SCICCR.bit.PARITYENA = 0;
			break;
		default:
			SciaRegs.SCICCR.bit.PARITYENA = 0;
	}

	// Baud rate settings - Automatic depending on ulBaudrate
	#if (CPU_FRQ_150MHZ)
	//@LSPCLK = 37.5MHz.
	baud = ((Uint32) 37500000 / (ulBaudRate*8) - 1);
	#endif
	#if (CPU_FRQ_100MHZ)
	baud = ((Uint32) 20000000 / (ulBaudRate*8) - 1);
	#endif

	// Configure the High and Low baud rate registers
	SciaRegs.SCIHBAUD = (baud & 0xFF00) >> 8;
	SciaRegs.SCILBAUD = (baud & 0x00FF);

	// Enable interrupts
	PieCtrlRegs.PIECTRL.bit.ENPIE = 1;   // Enable the PIE block
	PieCtrlRegs.PIEIER9.bit.INTx1=1;     // PIE Group 9, int1
	PieCtrlRegs.PIEIER9.bit.INTx2=1;     // PIE Group 9, INT2
	IER |= M_INT9;	// Enable CPU INT

	// Relinquish SCI from Reset state
	SciaRegs.SCICTL1.all =0x0023;
}
