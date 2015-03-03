#include "DSP2833x_Device.h"     // DSP2833x Headerfile Include File
#include "Serial.h"
#include "Log.h"
#include "ModbusSettings.h"

// Clear flags of overflow
void serial_clear(){
	static unsigned short i, destroyFifo;

	SERIAL_DEBUG();

	// Reset Serial in case of error
	if(SciaRegs.SCIRXST.bit.RXERROR == true){
		SciaRegs.SCICTL1.bit.SWRESET=0;
	}

	// Clears FIFO buffer (if there is any data)
	for (i = SciaRegs.SCIFFRX.bit.RXFFST; i > 0; i--)
		destroyFifo = SciaRegs.SCIRXBUF.all;

	// Reset FIFO
	SciaRegs.SCIFFRX.bit.RXFIFORESET=1;
	SciaRegs.SCIFFTX.bit.TXFIFOXRESET=1;

	SciaRegs.SCICTL1.bit.SWRESET=1;

}

// Get how much data is at the RX FIFO Buffer
Uint16 serial_rxBufferStatus(){
	return SciaRegs.SCIFFRX.bit.RXFFST;
}

// Enable or disable RX (receiver)
void serial_setSerialRxEnabled(bool status){
	SERIAL_DEBUG();
	SciaRegs.SCICTL1.bit.RXENA = status;
}

// Enable or disable TX (trasmiter)
void serial_setSerialTxEnabled(bool status){
	SERIAL_DEBUG();
	SciaRegs.SCICTL1.bit.TXENA = status;

}

// Initialize Serial (actually SCIA)
void serial_init(Serial *self){
	Uint32 baudrate;

	// START: GOT FROM InitScia() FUNCTION (TEXAS FILES) ////////////////////////////////////////
	EALLOW;

	/* Enable internal pull-up for the selected pins */
	// Pull-ups can be enabled or disabled disabled by the user.
	// This will enable the pullups for the specified pins.
	GpioCtrlRegs.GPAPUD.bit.GPIO28 = 0;    // Enable pull-up for GPIO28 (SCIRXDA)
	GpioCtrlRegs.GPAPUD.bit.GPIO29 = 0;	   // Enable pull-up for GPIO29 (SCITXDA)

	/* Set qualification for selected pins to asynch only */
	// Inputs are synchronized to SYSCLKOUT by default.
	// This will select asynch (no qualification) for the selected pins.
	GpioCtrlRegs.GPAQSEL2.bit.GPIO28 = 3;  // Asynch input GPIO28 (SCIRXDA)

	/* Configure SCI-A pins using GPIO regs*/
	// This specifies which of the possible GPIO pins will be SCI functional pins.
	GpioCtrlRegs.GPAMUX2.bit.GPIO28 = 1;   // Configure GPIO28 for SCIRXDA operation
	GpioCtrlRegs.GPAMUX2.bit.GPIO29 = 1;   // Configure GPIO29 for SCITXDA operation

	EDIS;
	// END: GOT FROM InitScia() FUNCTION (TEXAS FILES) ////////////////////////////////////////
	// Number of bytes

	switch(self->bitsNumber) {
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
	switch(self->parityType){
		case SERIAL_PARITY_EVEN:
			SciaRegs.SCICCR.bit.PARITYENA = 1;
			SciaRegs.SCICCR.bit.PARITY = 1;
			break;
		case SERIAL_PARITY_ODD:
			SciaRegs.SCICCR.bit.PARITYENA = 1;
			SciaRegs.SCICCR.bit.PARITY = 0;
			break;
		case SERIAL_PARITY_NONE:
			SciaRegs.SCICCR.bit.PARITYENA = 0;
			break;
		default:
			SciaRegs.SCICCR.bit.PARITYENA = 0;
	}

	// Baud rate settings - Automatic depending on self->baudrate
//	baudrate = (Uint32) (SysCtrlRegs.LOSPCP.bit.LSPCLK / (self->baudrate*8) - 1);
	baudrate = (Uint32) (LOW_SPEED_CLOCK / (self->baudrate*8) - 1);

	// Configure the High and Low baud rate registers
	SciaRegs.SCIHBAUD = (baudrate & 0xFF00) >> 8;
	SciaRegs.SCILBAUD = (baudrate & 0x00FF);

	// Enables TX and RX Interrupts
	SciaRegs.SCICTL2.bit.TXINTENA = 0;
	SciaRegs.SCIFFTX.bit.TXFFIENA = 0;
	SciaRegs.SCICTL2.bit.RXBKINTENA = 0;
	SciaRegs.SCIFFRX.bit.RXFFIENA = 0;

	// FIFO TX configurations
	SciaRegs.SCIFFTX.bit.TXFFIL = 1;	// Interrupt level
	SciaRegs.SCIFFTX.bit.SCIFFENA = 1;	// Enables FIFO
	SciaRegs.SCIFFTX.bit.TXFFINTCLR = 1;	// Clear interrupt flag

	// FIFO: RX configurations
	SciaRegs.SCIFFRX.bit.RXFFIL = 1;	// Interrupt level
	SciaRegs.SCIFFRX.bit.RXFFINTCLR = 1;	// Clear interrupt flag
	SciaRegs.SCIFFRX.bit.RXFFOVRCLR = 1;	// Clear overflow flag

	// FIFO: Control configurations
	SciaRegs.SCIFFCT.all=0x00;

	// Enable RX and TX and reset the serial
	SciaRegs.SCICTL1.bit.RXENA	 = 1;
	SciaRegs.SCICTL1.bit.TXENA	 = 1;
	SciaRegs.SCICTL1.bit.SWRESET = 1;

	// FIFO: Reset
	SciaRegs.SCIFFRX.bit.RXFIFORESET = 1;
	SciaRegs.SCIFFTX.bit.TXFIFOXRESET = 1;
	SciaRegs.SCIFFTX.bit.SCIRST = 0;
	SciaRegs.SCIFFTX.bit.SCIRST = 1;

	SERIAL_DEBUG();
}

// Transmit variable data based on passed size
void serial_transmitData(Uint16 * data, Uint16 size){
	static unsigned short i = 0;
	SERIAL_DEBUG();

	for (i = 0; i < size; i++){
		SciaRegs.SCITXBUF= data[i];

		if(i%4 == 0){
			while (SciaRegs.SCICTL2.bit.TXEMPTY != true) ;
		}
	}

	// If you want to wait until the TX buffer is empty, uncomment line below
//	while (SciaRegs.SCICTL2.bit.TXEMPTY != true) ;
}

// Read data from buffer (byte per byte)
Uint16 serial_getRxBufferedWord(){
	SERIAL_DEBUG();

	// TODO: check if it is needed
	while (SciaRegs.SCIRXST.bit.RXRDY) ;

	return SciaRegs.SCIRXBUF.all;
}

bool serial_getRxError(){
	SERIAL_DEBUG();

	return SciaRegs.SCIRXST.bit.RXERROR;
}

// Construct the Serial Module
Serial construct_Serial(){
	Serial serial;

	serial.clear = serial_clear;
	serial.rxBufferStatus = serial_rxBufferStatus;
	serial.setSerialRxEnabled = serial_setSerialRxEnabled;
	serial.setSerialTxEnabled = serial_setSerialTxEnabled;
	serial.init = serial_init;
	serial.transmitData = serial_transmitData;
	serial.getRxBufferedWord = serial_getRxBufferedWord;
	serial.getRxError = serial_getRxError;

	serial.fifoWaitBuffer = 0;

	SERIAL_DEBUG();

	return serial;
}
