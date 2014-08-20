#include "ModbusDataResponse.h"
#include "stdlib.h"
#include "Log.h"

void response_clear(ModbusDataResponse *self){
	MB_DATA_RESPONSE_DEBUG();
	self->slaveAddress = 0;
	self->functionCode = 0;
	self->numberOfBytes = 0;
	self->exceptionCode = 0;
	self->content = 0;
	self->crc = 0;
}

Uint16 response_size(ModbusDataResponse *self){
	// 1 byte:  Slave Address
	// 1 byte:  Function code
	// 1 bytes: Number of data to transmit
	// BASED_ON_BEFORE bytes:  Content (1 register = 2 bytes)
	// 2 bytes: CRC
	MB_DATA_RESPONSE_DEBUG();
	return (5 + self->numberOfBytes);
}

 Uint16 * response_getTransmitString(ModbusDataResponse *self){
	static Uint16 * string;
	Uint16 stringIndex = 0;
	Uint16 contentIterator;
	Uint16 contentQuantity = self->numberOfBytes / 2;
	string = (Uint16 *) calloc(self->size(self), sizeof(Uint16));

	if(self->exceptionCode != 0){
		return response_exception(self);
	}

	string[stringIndex++] = self->slaveAddress;
	string[stringIndex++] = self->functionCode;
	string[stringIndex++] = self->numberOfBytes;
	
	for(contentIterator = 0; contentIterator < contentQuantity; contentIterator++){
		string[stringIndex++] = (self->content[contentIterator] & 0xFF00) >> 8;
		string[stringIndex++] = self->content[contentIterator] & 0x00FF;
	}
	
	string[stringIndex++] = (self->crc & 0xFF00) >> 8;
	string[stringIndex++] = self->crc & 0x00FF;

	MB_DATA_RESPONSE_DEBUG();
	return string;
}


 Uint16 * response_getTransmitStringWithoutCRC(ModbusDataResponse *self){
	static Uint16 * string;
	Uint16 stringIndex = 0;
	Uint16 contentIterator;
	Uint16 contentQuantity = self->numberOfBytes / 2;
	string = (Uint16 *) calloc(self->size(self), sizeof(Uint16));

	if(self->exceptionCode != 0){
		return response_exception(self);
	}

	string[stringIndex++] = self->slaveAddress;
	string[stringIndex++] = self->functionCode;
	string[stringIndex++] = self->numberOfBytes;

	for(contentIterator = 0; contentIterator < contentQuantity; contentIterator++){
		string[stringIndex++] = (self->content[contentIterator] & 0xFF00) >> 8;
		string[stringIndex++] = self->content[contentIterator] & 0x00FF;
	}

	MB_DATA_RESPONSE_DEBUG();
	return string;
}

Uint16 * response_exception(ModbusDataResponse *self){
	static Uint16 * string;
	string = (Uint16 *) calloc(self->size(self), sizeof(Uint16));

	string[0] = self->slaveAddress;
	string[1] = self->functionCode;
	string[2] = self->exceptionCode;
	string[3] = (self->crc & 0xFF00) >> 8;
	string[4] = self->crc & 0x00FF;

	MB_DATA_RESPONSE_DEBUG();
	return string;
}

ModbusDataResponse construct_ModbusDataResponse(){
	ModbusDataResponse modbusDataResponse;

	modbusDataResponse.slaveAddress = 0;
	modbusDataResponse.functionCode = 0;
	modbusDataResponse.numberOfBytes = 0;
	modbusDataResponse.content = 0;
	modbusDataResponse.exceptionCode = 0;
	modbusDataResponse.crc = 0;

	modbusDataResponse.clear = response_clear;
	modbusDataResponse.size = response_size;
	modbusDataResponse.getTransmitString = response_getTransmitString;
	modbusDataResponse.getTransmitStringWithoutCRC = response_getTransmitStringWithoutCRC;

	MB_DATA_RESPONSE_DEBUG();

	return modbusDataResponse;
}
