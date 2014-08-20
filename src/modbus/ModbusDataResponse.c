#include "ModbusDataResponse.h"
#include "stdlib.h"
#include "Log.h"

void response_clear(ModbusDataResponse *self){
	MB_DATA_RESPONSE_DEBUG();
	self->slaveAddress = 0;
	self->functionCode = 0;
	self->numberOfBytes = 0;
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

Uint16 response_sizeWithoutCRC(ModbusDataResponse *self){
	MB_DATA_RESPONSE_DEBUG();
	return (3 + self->numberOfBytes);
}

 Uint16 * response_getTransmitString(ModbusDataResponse *self){
	static Uint16 * string;
	string = (Uint16 *) calloc(self->size(self), sizeof(Uint16));

	string[0] = self->slaveAddress;
	
	string[1] = self->functionCode;
	
	string[2] = self->numberOfBytes;
	
	string[3] = (self->content & 0xFF00) >> 8;
	string[4] = self->content & 0x00FF;
	
	string[5] = (self->crc & 0xFF00) >> 8;
	string[6] = self->crc & 0x00FF;

	MB_DATA_RESPONSE_DEBUG();
	return string;
}

ModbusDataResponse construct_ModbusDataResponse(){
	ModbusDataResponse modbusDataResponse;

	modbusDataResponse.slaveAddress = 0;
	modbusDataResponse.functionCode = 0;
	modbusDataResponse.numberOfBytes = 0;
	modbusDataResponse.content = 0;
	modbusDataResponse.crc = 0;

	modbusDataResponse.clear = response_clear;
	modbusDataResponse.size = response_size;
	modbusDataResponse.sizeWithoutCRC = response_sizeWithoutCRC;
	modbusDataResponse.getTransmitString = response_getTransmitString;

	MB_DATA_RESPONSE_DEBUG();

	return modbusDataResponse;
}
