#include "ModbusDataRequest.h"
#include "Log.h"
#include "stdlib.h"

void request_clear(ModbusDataRequest *self){
	MB_DATA_REQUEST_DEBUG();
	self->slaveAddress = 0;
	self->functionCode = 0;
	self->firstDataAddress = 0;
	self->totalDataRequested = 0;
	self->crc = 0;
}

Uint16 request_size(ModbusDataRequest *self){
	// 1 byte:  Slave Address
	// 1 byte:  Function code
	// 2 bytes: First Data Address
	// 2 bytes:  Total Data Quantity
	// 2 bytes: CRC
	MB_DATA_REQUEST_DEBUG();
	return 8;
}

Uint16 * request_getReceivedString(ModbusDataRequest *self) {
	static Uint16 * string;
	string = (Uint16 *) calloc(self->size(self), sizeof(Uint16));

	string[0] = self->slaveAddress;
	
	string[1] = self->functionCode;
	
	string[2] = (self->firstDataAddress & 0xFF00) >> 8;
	string[3] = self->firstDataAddress & 0x00FF;

	string[4] = (self->totalDataRequested & 0xFF00) >> 8;
	string[5] = self->totalDataRequested & 0x00FF;
	
	string[6] = (self->crc & 0xFF00) >> 8;
	string[7] = self->crc & 0x00FF;

	MB_DATA_REQUEST_DEBUG();
	return string;
}

Uint16 * request_getReceivedStringWithoutCRC(ModbusDataRequest *self) {
	static Uint16 * string;
	string = (Uint16 *) calloc(self->size(self), sizeof(Uint16));

	string[0] = self->slaveAddress;
	
	string[1] = self->functionCode;
	
	string[2] = (self->firstDataAddress & 0xFF00) >> 8;
	string[3] = self->firstDataAddress & 0x00FF;

	string[4] = (self->totalDataRequested & 0xFF00) >> 8;
	string[5] = self->totalDataRequested & 0x00FF;

	MB_DATA_REQUEST_DEBUG();
	return string;
}

ModbusDataRequest construct_ModbusDataRequest(){
	ModbusDataRequest modbusDataRequest;

	modbusDataRequest.slaveAddress = 0;
	modbusDataRequest.functionCode = 0;
	modbusDataRequest.firstDataAddress = 0;
	modbusDataRequest.totalDataRequested = 0;
	modbusDataRequest.crc = 0;

	modbusDataRequest.clear = request_clear;
	modbusDataRequest.size = request_size;
	modbusDataRequest.getReceivedString = request_getReceivedString;
	modbusDataRequest.getReceivedStringWithoutCRC = request_getReceivedStringWithoutCRC;

	MB_DATA_REQUEST_DEBUG();

	return modbusDataRequest;
}

