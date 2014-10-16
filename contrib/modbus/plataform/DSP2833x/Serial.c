#include "PlataformSettings.h"
#include "DSP2833x_Sci.h"
#include "Serial.h"
#include "Log.h"

// Clear flags of overflow
void serial_clear(){
	SERIAL_DEBUG();

	// Reset Serial in case of error
	if(SciaRegs.SCIRXST.bit.RXERROR == true){
		Uint16 i, destroyFIFO;
		SciaRegs.SCICTL1.bit.SWRESET=0;

		// TODO: Check if is necessary
		// Clears FIFO buffer
		for (i = SciaRegs.SCIFFRX.bit.RXFFST; i >= 0; i--)
			destroyFIFO = SciaRegs.SCIRXBUF.all;
	}

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

	// FIFO TX configurations
	// 0:4 	Interruption level: 0
	// 6	Clear interruption flag
	// 13	FIFO Reset
	// 14	FIFO Enabled
	// 15	SCI Reset tx/rx
	SciaRegs.SCIFFTX.all=0xE040;

	// FIFO RX configurations
	// 0:4 	Interruption level: 16 bits
	// 6	Clear interruption flag
	// 13	FIFO Reset
	SciaRegs.SCIFFRX.all=0x204f;

	// FIFO Control configurations
	SciaRegs.SCIFFCT.all=0x00;

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
	baudrate = (Uint32) (LSPCLK / (self->baudrate*8) - 1);

	// Configure the High and Low baud rate registers
	SciaRegs.SCIHBAUD = (baudrate & 0xFF00) >> 8;
	SciaRegs.SCILBAUD = (baudrate & 0x00FF);

	SciaRegs.SCIFFRX.bit.RXFIFORESET=1;
	SciaRegs.SCIFFTX.bit.TXFIFOXRESET=1;

	// Reset the serial
	 SciaRegs.SCICTL1.bit.RXENA		= 1;
	 SciaRegs.SCICTL1.bit.TXENA		= 1;
	// SciaRegs.SCICTL1.bit.SWRESET 	= 1;
	SciaRegs.SCICTL1.all = 0x23;

	SERIAL_DEBUG();
}

// Transmit variable data based on passed size
void serial_transmitData(Uint16 * data, Uint16 size){
	int i = 0;
	SERIAL_DEBUG();

	for (i = 0; i < size; i++){
		SciaRegs.SCITXBUF= data[i];
	}

	// If you want to wait until the TX buffer is empty, uncomment line below
	//while (SciaRegs.SCICTL2.bit.TXEMPTY != true) ;
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

	SERIAL_DEBUG();

	return serial;
}
