#ifndef MODBUS_DATA_RESPONSE_H_
#define MODBUS_DATA_RESPONSE_H_

#include "Modbus.h"

typedef struct ModbusData ModbusData;

struct ModbusData {
	Uint16 slaveAddress;
	Uint16 functionCode;
	Uint16 content[16];
	Uint16 size;
	Uint16 crc;

	void (*clear)(ModbusData *self);
	Uint16 * (*getTransmitString)(ModbusData *self);
	Uint16 * (*getTransmitStringWithoutCRC)(ModbusData *self);
};

void response_clear(ModbusData *self);
Uint16 * response_getTransmitString(ModbusData *self);
Uint16 * response_getTransmitStringWithoutCRC(ModbusData *self);
ModbusData construct_ModbusData();

#endif
