/*
 * mb_serial.c
 *
 *  Created on: 03/09/2013
 *      Author: Bruno Luiz
 */

#include "mb_serial.h"

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

interrupt void serial_interrupt_tx()
{
	//Clears the interrupt
	PieCtrlRegs.PIEACK.all|=0x100;
}

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

void serial_send_data(){
	timer_disable();
	if(rx_frame[0] == MB_SLAVE_ID)
	{
		int i = 0;
		Uint16 frame_lenght = modbus_prep_response();

		for(i = 0; i < frame_lenght; i++)
		{
			SciaRegs.SCITXBUF= tx_frame[i];
			while (!SciaRegs.SCICTL2.bit.TXEMPTY) {}
		}

	}
	clear_tx_frame();
	clear_rx_frame();
	timer_enable();
}

void serial_init(Uint32 ulBaudRate, Uint16 ucDataBits, Uint16 eParity  )
{
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

	// Baud rate settings
	#if (CPU_FRQ_150MHZ)
	SciaRegs.SCIHBAUD    =0x0001;  // 9600 baud @LSPCLK = 37.5MHz.
	SciaRegs.SCILBAUD    =0x00E7;
	#endif
	#if (CPU_FRQ_100MHZ)
	SciaRegs.SCIHBAUD    =0x0001;  // 9600 baud @LSPCLK = 20MHz.
	SciaRegs.SCILBAUD    =0x0044;


	#endif

	// Enable interrupts
	PieCtrlRegs.PIECTRL.bit.ENPIE = 1;   // Enable the PIE block
	PieCtrlRegs.PIEIER9.bit.INTx1=1;     // PIE Group 9, int1
	PieCtrlRegs.PIEIER9.bit.INTx2=1;     // PIE Group 9, INT2
	IER |= M_INT9;	// Enable CPU INT

	// Relinquish SCI from Reset state
	SciaRegs.SCICTL1.all =0x0023;
}
