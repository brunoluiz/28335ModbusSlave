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
	bool dummy0:1;
	bool dummy1:1;
	bool dummy2:1;
	bool dummy3:1;
	bool dummy4:1;
	bool dummy5:1;
	bool dummy6:1;
	bool dummy7:1;
	bool dummy8:1;
	bool dummy9:1;
	bool dummy10:1;
	bool dummy11:1;
	bool dummy12:1;
	bool dummy13:1;
	bool dummy14:1;
	bool dummy15:1;
	bool dummy16:1;
	bool dummy17:1;
	bool dummy18:1;
};

ModbusCoilsMap construct_ModbusCoilsMap();
#endif

#if MB_INPUTS_ENABLED
typedef struct ModbusInputsMap ModbusInputsMap;
struct ModbusInputsMap{
	bool dummy0:1;
	bool dummy1:1;
	bool dummy2:1;
	bool dummy3:1;
	bool dummy4:1;
	bool dummy5:1;
	bool dummy6:1;
	bool dummy7:1;
	bool dummy8:1;
	bool dummy9:1;
	bool dummy10:1;
	bool dummy11:1;
	bool dummy12:1;
	bool dummy13:1;
	bool dummy14:1;
	bool dummy15:1;
	bool dummy16:1;
	bool dummy17:1;
	bool dummy18:1;
};

ModbusInputsMap construct_ModbusInputsMap();
#endif

#if MB_HOLDING_REGISTERS_ENABLED
typedef struct ModbusHoldingRegistersMap ModbusHoldingRegistersMap;
struct ModbusHoldingRegistersMap {
	bool dummy0:1;
	bool dummy1:1;
	bool dummy2:1;
	bool dummy3:1;
	bool dummy4:1;
	bool dummy5:1;
	bool dummy6:1;
	bool dummy7:1;
	bool dummy8:1;
	bool dummy9:1;
	bool dummy10:1;
	bool dummy11:1;
	bool dummy12:1;
	bool dummy13:1;
	bool dummy14:1;
	bool dummy15:1;
	bool dummy16:1;
	bool dummy17:1;
	bool dummy18:1;
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
