#include "ModbusSerial.h"
#include "Modbus.h"
#include "Log.h"

void serial_setupInterruptions(){
	SERIAL_DEBUG();
}

void serial_clear(){
	SERIAL_DEBUG();
}

bool serial_rxOverflow(){
	return false;
}

void serial_setSerialRxEnabled(Serial *self, Uint16 status){
	SERIAL_DEBUG();
}

void serial_setSerialTxEnabled(Serial *self, Uint16 status){
	SERIAL_DEBUG();
}

void serial_init(Serial *self){
	SERIAL_DEBUG();
}

void serial_transmitData( Uint16 * data, Uint16 size){
	Uint16 i = 0;

	for (i=0; i < size; i++){
		printf("%x ",data[i]);
	}
	printf("\n");
	SERIAL_DEBUG();
}

Uint16 serial_getRxBufferedWord(){
	SERIAL_DEBUG();
	return 0;
}

Serial construct_Serial(){
	Serial serial;

	serial.bitsNumber = 8;
	serial.parityType = MB_PARITY_NONE;
	serial.baudrate = 9600;

	serial.fifoEnabled = true;
	serial.rxEnabled = false;
	serial.txEnabled = false;
	serial.interruptionsEnabled = false;

	serial.setupInterruptions = serial_setupInterruptions;
	serial.clear = serial_clear;
	serial.rxOverflow = serial_rxOverflow;
	serial.setSerialRxEnabled = serial_setSerialRxEnabled;
	serial.setSerialTxEnabled = serial_setSerialTxEnabled;
	serial.init = serial_init;
	serial.transmitData = serial_transmitData;
	serial.getRxBufferedWord = serial_getRxBufferedWord;

	SERIAL_DEBUG();

	return serial;
}