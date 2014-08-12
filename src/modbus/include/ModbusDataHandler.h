#ifndef MODBUS_IO_H_
#define MODBUS_IO_H_

#include "Modbus.h"

typedef struct ModbusDataHandler ModbusDataHandler;
typedef struct ModbusSlave ModbusSlave;

struct ModbusDataHandler {
	Uint16 test;

	void (*loopStates)();
	void (*readInputRegisters)(ModbusSlave *slave);
	void (*presetSingleRegister)(ModbusSlave *slave);
	void (*exception)(ModbusSlave *slave, ModbusError exceptionCode);
};

void datahandler_readInputRegisters(ModbusSlave *slave);
void datahandler_presetSingleRegister(ModbusSlave *slave);
void datahandler_exception(ModbusSlave *slave, ModbusError exceptionCode);
ModbusDataHandler construct_ModbusDataHandler();

#endif
