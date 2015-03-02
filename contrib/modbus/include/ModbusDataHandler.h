#ifndef MODBUS_IO_H_
#define MODBUS_IO_H_

#include "ModbusDefinitions.h"
#include "ModbusSettings.h"
#include MB_DATA_MAP

typedef struct ModbusDataHandler ModbusDataHandler;
typedef struct ModbusSlave ModbusSlave;

struct ModbusDataHandler {
	void (*readDigitalData)(ModbusSlave *slave, ModbusFunctionCode funcCode);
	void (*readAnalogData)(ModbusSlave *slave, ModbusFunctionCode funcCode);

	void (*presetSingleRegister)(ModbusSlave *slave);
	void (*forceSingleCoil)(ModbusSlave *slave);
	void (*presetMultipleRegisters)(ModbusSlave *slave);
	void (*forceMultipleCoils)(ModbusSlave *slave);

	bool (*checkException)(ModbusSlave *slave, Uint16 firstAddr, Uint16 totalData, Uint16 sizeOfMap);
	void (*exception)(ModbusSlave *slave, ModbusError exceptionCode);
};

inline void datahandler_readDigitalData(ModbusSlave *slave, ModbusFunctionCode funcCode);
inline void datahandler_readAnalogData(ModbusSlave *slave, ModbusFunctionCode funcCode);
inline void datahandler_presetSingleRegister(ModbusSlave *slave);
inline void datahandler_forceSingleCoil(ModbusSlave *slave);
inline void datahandler_presetMultipleRegisters(ModbusSlave *slave);
inline void datahandler_forceMultipleCoils(ModbusSlave *slave);
inline bool datahandler_checkException(ModbusSlave *slave, Uint16 firstAddr, Uint16 totalData, Uint16 sizeOfMap);
inline void datahandler_exception(ModbusSlave *slave, ModbusError exceptionCode);
ModbusDataHandler construct_ModbusDataHandler();

#endif
