#ifndef MODBUS_DATA_RESPONSE_H_
#define MODBUS_DATA_RESPONSE_H_

#include "Modbus.h"

typedef struct ModbusDataResponse ModbusDataResponse;

struct ModbusDataResponse {
	Uint16 slaveAddress;
	Uint16 functionCode;
	Uint16 numberOfBytes;
	Uint16 content;
	Uint16 crc;

	void (*clear)(ModbusDataResponse *self);
	Uint16 (*size)(ModbusDataResponse *self);
	Uint16 (*sizeWithoutCRC)(ModbusDataResponse *self);
	Uint16 * (*getTransmitString)(ModbusDataResponse *self);
	Uint16 * (*getTransmitStringWithoutCRC)(ModbusDataResponse *self);
};

void response_clear(ModbusDataResponse *self);
Uint16 response_size(ModbusDataResponse *self);
Uint16 response_sizeWithoutCRC(ModbusDataResponse *self);
Uint16 * response_getTransmitString(ModbusDataResponse *self);
Uint16 * response_getTransmitStringWithoutCRC(ModbusDataResponse *self);
ModbusDataResponse construct_ModbusDataResponse();

#endif
