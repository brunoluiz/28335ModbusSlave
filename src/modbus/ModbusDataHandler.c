#include "ModbusDataHandler.h"
#include "ModbusSlave.h"
#include "Log.h"

void datahandler_readInputRegisters(ModbusSlave *slave){
	// Each requested register has two bytes inside of it, so the code has to multiply by 2
	Uint16 contentIndex = 0;
	Uint16 firstDataAddress = (slave->dataRequest.content[MB_READ_ADDRESS_HIGH] << 8) |
			slave->dataRequest.content[MB_READ_ADDRESS_LOW];
	Uint16 totalDataRequested = (slave->dataRequest.content[MB_READ_TOTALDATA_HIGH] << 8) |
			slave->dataRequest.content[MB_READ_TOTALDATA_LOW];
	Uint16 sizeWithoutCRC;
	Uint16 * transmitString;
	Uint16 addressIterator;

	slave->dataResponse.slaveAddress = MODBUS_SLAVE_ID;
	slave->dataResponse.functionCode = MB_FUNC_READ_HOLDINGREGISTERS;

	// Number of Bytes
	slave->dataResponse.content[contentIndex++] = totalDataRequested * 2;

	// TODO: Implement the data map here
	for (addressIterator = 0; addressIterator < totalDataRequested; addressIterator++){
		// Points to the start data address + iterator
		Uint16 * addr = (Uint16 *)(firstDataAddress + addressIterator + 0x8000);
		// Get the value from this data address
		Uint16 addr_val = *addr;
		slave->dataResponse.content[contentIndex++] = (addr_val & 0xFF00) >> 8;
		slave->dataResponse.content[contentIndex++] = addr_val & 0x00FF;
	}

	slave->dataResponse.size = MB_SIZE_COMMON_RESP_READ + totalDataRequested * 2;

	sizeWithoutCRC = slave->dataResponse.size - 2;
	transmitString = slave->dataResponse.getTransmitStringWithoutCRC(&slave->dataResponse);
	slave->dataResponse.crc = generateCrc( transmitString, sizeWithoutCRC );

	MB_DATA_HANDLER_DEBUG();
}

void datahandler_presetSingleRegister(ModbusSlave *slave){
	Uint16 * memAddr;
	Uint16 contentIndex = 0;
	Uint16 dataAddress = (slave->dataRequest.content[MB_WRITE_ADDRESS_HIGH] << 8) |
				slave->dataRequest.content[MB_WRITE_ADDRESS_LOW];
	Uint16 sizeWithoutCRC;
	Uint16 * transmitString;

	slave->dataResponse.slaveAddress = MODBUS_SLAVE_ID;
	slave->dataResponse.functionCode = MB_FUNC_WRITE_HOLDINGREGISTER;

	slave->dataResponse.content[contentIndex++] = slave->dataRequest.content[MB_WRITE_ADDRESS_HIGH];
	slave->dataResponse.content[contentIndex++] = slave->dataRequest.content[MB_WRITE_ADDRESS_LOW];

	// TODO: Implement the data map below
	memAddr = (Uint16 *) dataAddress + 0x8000;
	*(memAddr) = (slave->dataRequest.content[MB_WRITE_VALUE_HIGH] << 8) |
			slave->dataRequest.content[MB_WRITE_VALUE_LOW];

	slave->dataResponse.content[contentIndex++] = *(memAddr) && 0x00FF;
	slave->dataResponse.content[contentIndex++] = (*(memAddr) && 0xFF00) >> 8;

	slave->dataResponse.size = MB_SIZE_RESP_WRITE;

	sizeWithoutCRC = slave->dataResponse.size - 2;
	transmitString = slave->dataResponse.getTransmitStringWithoutCRC(&slave->dataResponse);
	slave->dataResponse.crc = generateCrc( transmitString, sizeWithoutCRC );

	MB_DATA_HANDLER_DEBUG();
}

void datahandler_exception(ModbusSlave *slave, ModbusError exceptionCode){
	slave->dataResponse.slaveAddress = MODBUS_SLAVE_ID;
	slave->dataResponse.functionCode = slave->dataRequest.functionCode;
	slave->dataResponse.content[0] 	 = exceptionCode;

	slave->dataResponse.size = MB_SIZE_EXCEPTION;

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
