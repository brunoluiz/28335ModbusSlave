/*
 * ModbusVarMap.h
 *
 *  Created on: 08/10/2014
 *      Author: bds
 */

#ifndef MODBUSVARMAP_H_
#define MODBUSVARMAP_H_

#include "ModbusSettings.h"

#if MB_COILS_ENABLED
typedef struct ModbusCoilsMap ModbusCoilsMap;
struct ModbusCoilsMap{
	Uint8 dummy0;
	Uint8 dummy1;
	Uint8 dummy2;
	Uint8 dummy3;
	Uint8 dummy4;
	Uint8 dummy5;
	Uint8 dummy6;
	Uint8 dummy7;
	Uint8 dummy8;
	Uint8 dummy9;
	Uint8 dummy10;
	Uint8 dummy11;
	Uint8 dummy12;
	Uint8 dummy13;
	Uint8 dummy14;
	Uint8 dummy15;
	Uint8 dummy16;
	Uint8 dummy17;
	Uint8 dummy18;
};

ModbusCoilsMap construct_ModbusCoilsMap();
#endif

#if MB_INPUTS_ENABLED
typedef struct ModbusInputsMap ModbusInputsMap;
struct ModbusInputsMap{
	Uint8 dummy0;
	Uint8 dummy1;
	Uint8 dummy2;
	Uint8 dummy3;
	Uint8 dummy4;
	Uint8 dummy5;
	Uint8 dummy6;
	Uint8 dummy7;
	Uint8 dummy8;
	Uint8 dummy9;
	Uint8 dummy10;
	Uint8 dummy11;
	Uint8 dummy12;
	Uint8 dummy13;
	Uint8 dummy14;
	Uint8 dummy15;
	Uint8 dummy16;
	Uint8 dummy17;
	Uint8 dummy18;
};

ModbusInputsMap construct_ModbusInputsMap();
#endif

#if MB_HOLDING_REGISTERS_ENABLED
typedef struct ModbusHoldingRegistersMap ModbusHoldingRegistersMap;
struct ModbusHoldingRegistersMap {
	float32 dummy0;
	float32 dummy1;
	float32 dummy2;
	float32 dummy3;
	float32 dummy4;
};

ModbusHoldingRegistersMap construct_ModbusHoldingRegistersMap();
#endif

#if MB_INPUT_REGISTERS_ENABLED
typedef struct ModbusInputRegistersMap ModbusInputRegistersMap;
struct ModbusInputRegistersMap {
	float32 dummy0;
	float32 dummy1;
	float32 dummy2;
	float32 dummy3;
	float32 dummy4;
};

ModbusInputRegistersMap construct_ModbusInputRegistersMap();
#endif

#endif /* MODBUSVARMAP_H_ */
