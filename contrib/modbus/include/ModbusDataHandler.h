#ifndef MODBUS_IO_H_
#define MODBUS_IO_H_

#include "ModbusDefinitions.h"
#include "PlataformSettings.h"
#include "ModbusDataMap.h"

typedef struct ModbusDataHandler ModbusDataHandler;
typedef struct ModbusSlave ModbusSlave;

struct ModbusDataHandler {
	void (*loopStates)();
	void (*readCoils)(ModbusSlave *slave);
	void (*readHoldingRegisters)(ModbusSlave *slave);
	void (*presetSingleRegister)(ModbusSlave *slave);
	void (*forceSingleCoil)(ModbusSlave *slave);
	void (*presetMultipleRegisters)(ModbusSlave *slave);
	void (*forceMultipleCoils)(ModbusSlave *slave);
	void (*exception)(ModbusSlave *slave, ModbusError exceptionCode);
};

inline void datahandler_readHoldingRegisters(ModbusSlave *slave);
inline void datahandler_readCoils(ModbusSlave *slave);
inline void datahandler_presetSingleRegister(ModbusSlave *slave);
inline void datahandler_forceSingleCoil(ModbusSlave *slave);
inline void datahandler_presetMultipleRegisters(ModbusSlave *slave);
inline void datahandler_forceMultipleCoils(ModbusSlave *slave);
inline void datahandler_exception(ModbusSlave *slave, ModbusError exceptionCode);
ModbusDataHandler construct_ModbusDataHandler();

#endif
