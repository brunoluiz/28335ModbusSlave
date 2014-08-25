#include "ModbusData.h"
#include "Log.h"

void response_clear(ModbusData *self){
	MB_DATA_RESPONSE_DEBUG();
	self->slaveAddress = 0;
	self->functionCode = 0;
	self->crc = 0;
}

 Uint16 * response_getTransmitString(ModbusData *self){
	static Uint16 string[16] = {0};
	Uint16 stringIndex = 0;
	Uint16 contentIterator;

	string[stringIndex++] = self->slaveAddress;
	string[stringIndex++] = self->functionCode;
	
	for(contentIterator = 0; contentIterator < (self->size - MB_SIZE_COMMON_DATA); contentIterator++){
		string[stringIndex++] = (self->content[contentIterator]) & 0x00FF;
	}
	
	string[stringIndex++] = (self->crc & 0xFF00) >> 8;
	string[stringIndex++] = self->crc & 0x00FF;

	MB_DATA_RESPONSE_DEBUG();
	return string;
}


 Uint16 * response_getTransmitStringWithoutCRC(ModbusData *self){
	static Uint16 string[16] = {0};
	Uint16 stringIndex = 0;
	Uint16 contentIterator;

	string[stringIndex++] = self->slaveAddress;
	string[stringIndex++] = self->functionCode;

	for(contentIterator = 0; contentIterator < (self->size - MB_SIZE_COMMON_DATA_wo_CRC); contentIterator++){
		string[stringIndex++] = (self->content[contentIterator]);
	}

	MB_DATA_RESPONSE_DEBUG();
	return string;
}

ModbusData construct_ModbusData(){
	ModbusData modbusData;

	modbusData.slaveAddress = 0;
	modbusData.functionCode = 0;
	modbusData.crc = 0;

	modbusData.size = 0;

	modbusData.clear = response_clear;
	modbusData.getTransmitString = response_getTransmitString;
	modbusData.getTransmitStringWithoutCRC = response_getTransmitStringWithoutCRC;

	MB_DATA_RESPONSE_DEBUG();

	return modbusData;
}
