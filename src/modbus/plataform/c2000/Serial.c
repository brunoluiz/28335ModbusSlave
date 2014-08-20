#include "Serial.h"
// #include "Modbus.h"
#include "Log.h"

// Clear flags of overflow
void serial_clear(){
	SERIAL_DEBUG();

	// Clear overflow flag
	SciaRegs.SCIFFRX.bit.RXFFOVRCLR = 1;

	// Reset FIFO
	// TODO: check if it is needed
//	SciaRegs.SCIFFRX.bit.RXFIFORESET=1;

	// Clear interruptions
//	SciaRegs.SCIFFRX.bit.RXFFINTCLR=1;
//	SciaRegs.SCIFFTX.bit.TXFFINTCLR=1;

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

	InitSciaGpio();

	self->setSerialRxEnabled(self, false);
	self->setSerialTxEnabled(self, false);

	// Number of bytes
	switch(self->bitsNumber){
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

	// Baud rate settings - Automatic depending on ulBaudrate
	#if (CPU_FRQ_150MHZ)
	//@LSPCLK = 37.5MHz.
	baudrate = ((Uint32) 37500000 / (self->baudrate*8) - 1);
	#endif
	#if (CPU_FRQ_100MHZ)
	baudrate = ((Uint32) 20000000 / (self->baudrate*8) - 1);
	#endif

	// Configure the High and Low baud rate registers
	SciaRegs.SCIHBAUD = (baudrate & 0xFF00) >> 8;
	SciaRegs.SCILBAUD = (baudrate & 0x00FF);

	// TODO: FIFO configuration in one single register
	SciaRegs.SCIFFTX.bit.SCIRST 	= 0;
	SciaRegs.SCIFFTX.bit.SCIRST 	= 1;
	SciaRegs.SCIFFTX.bit.SCIFFENA 	= 1; // Enable FIFO
	SciaRegs.SCIFFRX.bit.RXFFIENA 	= 0; // Interruption disabled
	SciaRegs.SCIFFTX.bit.TXFFIENA 	= 0; // Interruption disabled
//	SciaRegs.SCIFFTX.bit.TXFFIL 	= 7; // Buffer have 7 words before overflow (1 byte only)
//	SciaRegs.SCIFFRX.bit.RXFFIL 	= 8; // Buffer have 8 words before overflow

	// TODO: check if is needed (low priority)
	SciaRegs.SCIFFCT.all=0x00;

	// Relinquish SCI from Reset state
//	SciaRegs.SCICTL1.all =0x0023;
//	SciaRegs.SCICTL1.bit.RXENA 		= 0;
//	SciaRegs.SCICTL1.bit.TXENA 		= 0;
	SciaRegs.SCICTL1.bit.SWRESET 	= 1;

	// Reset FIFO
	SciaRegs.SCIFFTX.bit.TXFIFOXRESET = 0;
	SciaRegs.SCIFFRX.bit.RXFIFORESET  = 0;
	SciaRegs.SCIFFTX.bit.TXFIFOXRESET = 1;
	SciaRegs.SCIFFRX.bit.RXFIFORESET  = 1;

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
