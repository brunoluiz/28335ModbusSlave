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
};

struct ModbusHoldingRegistersMap {
	float32 dummy0;
	float32 dummy1;
	float32 dummy2;
	float32 dummy3;
	float32 dummy4;
};

ModbusCoilsMap construct_ModbusCoilsMap();
ModbusHoldingRegistersMap construct_ModbusHoldingRegistersMap();

#endif /* MODBUSVARMAP_H_ */
