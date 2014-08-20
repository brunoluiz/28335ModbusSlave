#ifndef MODBUS_DATA_REQUEST_H_
#define MODBUS_DATA_REQUEST_H_

#include "Modbus.h"

typedef struct ModbusDataRequest ModbusDataRequest;

struct ModbusDataRequest {
	Uint16 slaveAddress;
	Uint16 functionCode;
	Uint16 firstDataAddress;
	Uint16 totalDataRequested;
	Uint16 crc;

	void (*clear)(ModbusDataRequest *self);
	Uint16 (*size)(ModbusDataRequest *self);
	Uint16 * (*getReceivedString)(ModbusDataRequest *self);
	Uint16 * (*getReceivedStringWithoutCRC)(ModbusDataRequest *self);
};

void request_clear(ModbusDataRequest *self);
Uint16 request_size(ModbusDataRequest *self);
Uint16 * request_getReceivedString(ModbusDataRequest *self);
Uint16 * request_getReceivedStringWithoutCRC(ModbusDataRequest *self);
ModbusDataRequest construct_ModbusDataRequest();

#endif
