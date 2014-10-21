/*
 * ModbusVarMap.c
 *
 *  Created on: 08/10/2014
 *      Author: bds
 */

#include "ModbusDataMap.h"
#include "Log.h"

ModbusInputsMap construct_ModbusInputsMap(){
	ModbusInputsMap inputsMap = {0};
	return inputsMap;
}

ModbusCoilsMap construct_ModbusCoilsMap(){
	ModbusCoilsMap coilsMap = {0};
	return coilsMap;
}

ModbusHoldingRegistersMap construct_ModbusHoldingRegistersMap(){
	ModbusHoldingRegistersMap registersMap = {0};
	return registersMap;
}

ModbusInputRegistersMap construct_ModbusInputRegistersMap(){
	ModbusInputRegistersMap registersMap = {0};
	return registersMap;
}
