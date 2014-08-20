#include "ModbusDataHandler.h"
#include "ModbusSlave.h"
#include "Log.h"

void datahandler_readInputRegisters(ModbusSlave *slave){
	// Each requested register has two bytes inside of it, so the code has to multiply by 2
	static Uint16 counter = 0;
	Uint16 totalDataRequested = slave->dataRequest.totalDataRequested;
	Uint16 numberOfBytes = totalDataRequested * 2;
	Uint16 sizeWithoutCRC;
	Uint16 addressIterator;

	slave->dataResponse.slaveAddress = MODBUS_SLAVE_ID;
	slave->dataResponse.functionCode = MB_FUNC_READ_HOLDINGREGISTERS;
	slave->dataResponse.numberOfBytes = numberOfBytes;

	for (addressIterator = 0; addressIterator < totalDataRequested; addressIterator++){
		slave->dataResponse.content[addressIterator] = 0xCAFE + (addressIterator);
	}

	sizeWithoutCRC = slave->dataResponse.size(&slave->dataResponse) - 2;
	slave->dataResponse.crc = generateCrc( slave->dataResponse.getTransmitStringWithoutCRC(&slave->dataResponse),
			sizeWithoutCRC);

	counter++;
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
