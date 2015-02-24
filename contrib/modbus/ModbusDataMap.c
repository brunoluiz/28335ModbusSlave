/*
 * ModbusVarMap.c
 *
 *  Created on: 08/10/2014
 *      Author: bds
 */

#include "ModbusSettings.h"
#include MB_DATA_MAP
#include "Log.h"

#if MB_COILS_ENABLED
ModbusCoilsMap construct_ModbusCoilsMap(){
	ModbusCoilsMap coilsMap = {0};
	return coilsMap;
}
#endif

#if MB_INPUTS_ENABLED
ModbusInputsMap construct_ModbusInputsMap(){
	ModbusInputsMap inputsMap = {0};
	return inputsMap;
}
#endif

#if MB_INPUT_REGISTERS_ENABLED
ModbusInputRegistersMap construct_ModbusInputRegistersMap(){
	ModbusInputRegistersMap registersMap = {0};
	return registersMap;
}
#endif

#if MB_HOLDING_REGISTERS_ENABLED
ModbusHoldingRegistersMap construct_ModbusHoldingRegistersMap(){
	ModbusHoldingRegistersMap registersMap = {0};
	return registersMap;
}
#endif
