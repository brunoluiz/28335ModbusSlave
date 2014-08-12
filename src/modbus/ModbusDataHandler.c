#include "ModbusDataHandler.h"
#include "ModbusSlave.h"
#include "Log.h"

void datahandler_readInputRegisters(ModbusSlave *slave){
	Uint16 numberOfBytes = slave->dataRequest.totalDataRequested;

	// Points to the start data address + iterator
	Uint16 * startAddress = (Uint16 *)(slave->dataRequest.firstDataAddress);
	// Get the value from this data address
	Uint16 startAddressVal = *startAddress;


	Uint16 content = startAddressVal;

	slave->dataResponse.content = content;

	MB_DATA_HANDLER_DEBUG();
}

void datahandler_presetSingleRegister(ModbusSlave *slave){
	MB_DATA_HANDLER_DEBUG();
}

void datahandler_exception(ModbusSlave *slave, ModbusError exceptionCode){
	MB_DATA_HANDLER_DEBUG();
}

ModbusDataHandler construct_ModbusDataHandler(){
	ModbusDataHandler modbusDataHandler;

	modbusDataHandler.readInputRegisters = datahandler_readInputRegisters;
	modbusDataHandler.presetSingleRegister = datahandler_presetSingleRegister;
	modbusDataHandler.exception = datahandler_exception;

	MB_DATA_HANDLER_DEBUG();

	return modbusDataHandler;
}
