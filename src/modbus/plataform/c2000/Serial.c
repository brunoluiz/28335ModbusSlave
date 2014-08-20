#include "PlataformSettings.h"
#include "Serial.h"
#include "Log.h"

// Clear flags of overflow
void serial_clear(){
	SERIAL_DEBUG();

	// Clear FIFO Rxoverflow flag
	SciaRegs.SCIFFRX.bit.RXFFOVRCLR = 1;

	// Reset FIFO
	SciaRegs.SCIFFRX.bit.RXFIFORESET=1;
	SciaRegs.SCIFFTX.bit.TXFIFOXRESET=1;

	// Clear interruptions
	SciaRegs.SCIFFRX.bit.RXFFINTCLR=1;
	SciaRegs.SCIFFTX.bit.TXFFINTCLR=1;

}

// TODO: check if this works
Uint16 serial_rxBufferStatus(){
	return SciaRegs.SCIFFRX.bit.RXFFST;
}

// Enable or disable RX (receiver)
void serial_setSerialRxEnabled(Serial *self, bool status){
	SERIAL_DEBUG();
	SciaRegs.SCICTL1.bit.RXENA = status;
	self->rxEnabled = status;
}

// Enable or disable TX (trasmiter)
void serial_setSerialTxEnabled(Serial *self, bool status){
	SERIAL_DEBUG();
	SciaRegs.SCICTL1.bit.TXENA = status;
	self->txEnabled = status;

}

// Initialize Serial (actually SCIA)
void serial_init(Serial *self){
	Uint32 baudrate;

	// InitSciaGpio();

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

	// TODO: Check why the baud rate settings is returning a wrong value
	// Baud rate settings - Automatic depending on ulBaudrate
//	#if (CPU_FRQ_150MHZ)
//	//@LSPCLK = 37.5MHz.
//	baudrate = (Uint32) ((Uint32) ((Uint32) 37500000 / (Uint32) (self->baudrate*8) ) - 1);
//	#endif
//	#if (CPU_FRQ_100MHZ)
//	baudrate = (Uint32) ((Uint32) ((Uint32) 20000000 / (self->baudrate*8)) - 1);
//	#endif

	// Configure the High and Low baud rate registers
//	SciaRegs.SCIHBAUD = (baudrate & 0xFF00) >> 8;
//	SciaRegs.SCILBAUD = (baudrate & 0x00FF);

	SciaRegs.SCIHBAUD    =0x0001;  // 9600 baud @LSPCLK = 37.5MHz.
	SciaRegs.SCILBAUD    =0x00E7;

	SciaRegs.SCICTL2.bit.TXINTENA 	= 0;
	SciaRegs.SCICTL2.bit.RXBKINTENA = 0;

	// Disable RX and TX temporarily
	self->setSerialRxEnabled(self, false);
	self->setSerialTxEnabled(self, false);

	// Reset the serial
	SciaRegs.SCICTL1.bit.SWRESET 	= 1;

	SERIAL_DEBUG();
}

// Transmit variable data based on passed size
void serial_transmitData(Uint16 * data, Uint16 size){
	int i = 0;
	SERIAL_DEBUG();

	for (i = 0; i < size; i++){
		// while (!SciaRegs.SCICTL2.bit.TXRDY) ; // TODO: check if it is needed
		SciaRegs.SCITXBUF= data[i];
	}
}

// Get Read data from buffer
Uint16 serial_getRxBufferedWord(){
	SERIAL_DEBUG();

	// TODO: check if it is needed
	// while (SciaRegs.SCIRXST.bit.RXRDY) ;

	return SciaRegs.SCIRXBUF.all;
}

// Construct the Serial Module
Serial construct_Serial(){
	Serial serial;

	serial.bitsNumber	= SERIAL_BITS_NUMBER;
	serial.parityType	= SERIAL_PARITY_NONE;
	serial.baudrate 	= SERIAL_BAUDRATE;

	serial.fifoEnabled = true;
	serial.rxEnabled = false;
	serial.txEnabled = false;

	serial.clear = serial_clear;
	serial.rxBufferStatus = serial_rxBufferStatus;
	serial.setSerialRxEnabled = serial_setSerialRxEnabled;
	serial.setSerialTxEnabled = serial_setSerialTxEnabled;
	serial.init = serial_init;
	serial.transmitData = serial_transmitData;
	serial.getRxBufferedWord = serial_getRxBufferedWord;

	SERIAL_DEBUG();

	return serial;
}
