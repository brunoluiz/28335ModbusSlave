#ifndef MODBUS_DATA_RESPONSE_H_
#define MODBUS_DATA_RESPONSE_H_

#include "PlataformSettings.h"

typedef struct ModbusData ModbusData;

struct ModbusData {
	Uint16 slaveAddress;
	Uint16 functionCode;
	Uint16 contentIdx;
	Uint16 content[16];
	Uint16 size;
	Uint16 crc;

	void (*clear)(ModbusData *self);
	Uint16 * (*getTransmitString)(ModbusData *self);
	Uint16 * (*getTransmitStringWithoutCRC)(ModbusData *self);
};

inline void data_clear(ModbusData *self);
inline Uint16 * data_getTransmitString(ModbusData *self);
inline Uint16 * data_getTransmitStringWithoutCRC(ModbusData *self);
ModbusData construct_ModbusData();

#endif
