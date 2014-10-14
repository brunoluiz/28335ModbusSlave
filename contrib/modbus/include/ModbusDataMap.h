/*
 * ModbusVarMap.h
 *
 *  Created on: 08/10/2014
 *      Author: bds
 */

#ifndef MODBUSVARMAP_H_
#define MODBUSVARMAP_H_

#include "PlataformSettings.h"

typedef struct ModbusCoilsMap ModbusCoilsMap;
typedef struct ModbusHoldingRegistersMap ModbusHoldingRegistersMap;

struct ModbusCoilsMap{
	bool dummy1:1;
	bool dummy2:1;
	bool dummy3:1;
	bool dummy4:1;
	bool dummy5:1;
	bool dummy6:1;
	bool dummy7:1;
};

struct ModbusHoldingRegistersMap {
	float32 dummy1;
	float32 dummy2;
	float32 dummy3;
	float32 dummy4;
};

ModbusCoilsMap construct_ModbusCoilsMap();
ModbusHoldingRegistersMap construct_ModbusHoldingRegistersMap();

#endif /* MODBUSVARMAP_H_ */
