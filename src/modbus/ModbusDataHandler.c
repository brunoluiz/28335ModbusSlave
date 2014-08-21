#include "ModbusDataHandler.h"
#include "ModbusSlave.h"
#include "Log.h"
#include "stdlib.h"

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

	// TODO: Implement the data map here
	for (addressIterator = 0; addressIterator < totalDataRequested; addressIterator++){
		// Points to the start data address + iterator
		Uint16 * addr = (Uint16 *)(slave->dataRequest.dataAddress + addressIterator + 0x8000);
		// Get the value from this data address
		Uint16 addr_val = *addr;
		slave->dataResponse.content[addressIterator] = addr_val;

		// TODO: free() needed?
		// free(addr);
	}
	counter++;

	sizeWithoutCRC = slave->dataResponse.size(&slave->dataResponse) - 2;
	slave->dataResponse.crc = generateCrc( slave->dataResponse.
			getTransmitStringWithoutCRC(&slave->dataResponse), sizeWithoutCRC);

	MB_DATA_HANDLER_DEBUG();
}

void datahandler_presetSingleRegister(ModbusSlave *slave){
//	Uint16 * memAddr = 0;
//	Uint16 dataAddress = slave->dataRequest.dataAddress;
//	Uint16 sizeWithoutCRC;
//
//	slave->dataResponse.slaveAddress = MODBUS_SLAVE_ID;
//	slave->dataResponse.functionCode = MB_FUNC_WRITE_HOLDINGREGISTER;
//	slave->dataResponse.numberOfBytes = dataAddress;
//
//	// TODO: Implement the data map below
//	memAddr = dataAddress;
//	*(memAddr) = slave->dataRequest.dataToWrite;
//	slave->dataResponse.content[0] = *(memAddr);
//
//	sizeWithoutCRC = slave->dataResponse.size(&slave->dataResponse) - 2;
//	slave->dataResponse.crc = generateCrc( slave->dataResponse.
//			getTransmitStringWithoutCRC(&slave->dataResponse), sizeWithoutCRC);

	MB_DATA_HANDLER_DEBUG();
	MB_DATA_HANDLER_DEBUG();
}

void datahandler_exception(ModbusSlave *slave, ModbusError exceptionCode){
	slave->dataResponse.slaveAddress = MODBUS_SLAVE_ID;
	slave->dataResponse.functionCode = slave->dataRequest.functionCode;
	slave->dataResponse.exceptionCode = exceptionCode;

	slave->dataResponse.crc = generateCrc(slave->dataResponse.
			getTransmitStringWithoutCRC(&slave->dataResponse), 3);

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
