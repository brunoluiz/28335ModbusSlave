#include "ModbusDefinitions.h"
#include "ModbusDataHandler.h"
#include "ModbusSlave.h"
#include "Log.h"
#include "Crc.h"
#include "math.h"

void datahandler_readCoils(ModbusSlave *slave) {
	// Each requested register has two bytes inside of it, so the code has to multiply by 2
	Uint16 firstDataAddress = (slave->dataRequest.content[MB_READ_ADDRESS_HIGH] << 8) |
			slave->dataRequest.content[MB_READ_ADDRESS_LOW];
	Uint16 totalDataRequested = (slave->dataRequest.content[MB_READ_TOTALDATA_HIGH] << 8) |
			slave->dataRequest.content[MB_READ_TOTALDATA_LOW];
	Uint16 numberOfBytes = ceil(totalDataRequested/8 + 0.5);

	Uint16 sizeWithoutCRC;
	Uint16 * transmitString;
	Uint16 idx;
	Uint16 reg16ReturnIdx = 0;

	// Reference to MODBUS Data Map
	char * coilsPtr;
	coilsPtr = (char *)&(slave->coils);

	slave->dataResponse.slaveAddress = MB_SLAVE_ID;
	slave->dataResponse.functionCode = MB_FUNC_READ_COIL;

	// Number of Bytes
	slave->dataResponse.content[slave->dataResponse.contentIdx++] = numberOfBytes;

	// Reads specified address values
	for (idx = 0; idx < numberOfBytes; idx++) {
		Uint16 coilsNum = totalDataRequested - idx*8;
		Uint16 padding = idx + firstDataAddress - reg16ReturnIdx;
		Uint16 content = 0;
		content = *(coilsPtr + padding) >> (reg16ReturnIdx*8);

		if(coilsNum >= 8) {
			slave->dataResponse.content[slave->dataResponse.contentIdx++] = content & 0x00FF;
		} else {
			slave->dataResponse.content[slave->dataResponse.contentIdx++] = content & ~(0xFF << coilsNum);
		}

		if(reg16ReturnIdx == 0) reg16ReturnIdx = 1;
		else reg16ReturnIdx = 0;
	}

	// Data response size based on total data requested
	// This size will be used at serial.transmitData internal loop
	slave->dataResponse.size = MB_SIZE_RESP_READ_MINIMUM + numberOfBytes;

	// Generate CRC
	sizeWithoutCRC = slave->dataResponse.size - 2;
	transmitString = slave->dataResponse.getTransmitStringWithoutCRC(&slave->dataResponse);
	slave->dataResponse.crc = generateCrc( transmitString, sizeWithoutCRC, true);

	MB_DATA_HANDLER_DEBUG();
}

void datahandler_readHoldingRegisters(ModbusSlave *slave) {
	// Each requested register has two bytes inside of it, so the code has to multiply by 2
	Uint16 firstDataAddress = (slave->dataRequest.content[MB_READ_ADDRESS_HIGH] << 8) |
			slave->dataRequest.content[MB_READ_ADDRESS_LOW];
	Uint16 totalDataRequested = (slave->dataRequest.content[MB_READ_TOTALDATA_HIGH] << 8) |
			slave->dataRequest.content[MB_READ_TOTALDATA_LOW];

	Uint16 sizeWithoutCRC;
	Uint16 * transmitString;
	Uint16 idx;

	// Reference to MODBUS Data Map
	char * registersPtr;
	registersPtr = (char *)&(slave->registers);

	slave->dataResponse.slaveAddress = MB_SLAVE_ID;
	slave->dataResponse.functionCode = MB_FUNC_READ_HOLDINGREGISTERS;

	// Number of Bytes
	slave->dataResponse.content[slave->dataResponse.contentIdx++] = totalDataRequested * 2;

	// TODO: Implement the data map here
	// Reads specified address values
	// Works well with 32 bits registers
	for (idx = 0; idx < totalDataRequested; idx++) {
		Uint16 padding = idx + firstDataAddress;
		slave->dataResponse.content[slave->dataResponse.contentIdx++] = (*(registersPtr + padding + 1) & 0xFF00) >> 8;
		slave->dataResponse.content[slave->dataResponse.contentIdx++] = *(registersPtr + padding + 1) & 0x00FF;
		idx++;
		slave->dataResponse.content[slave->dataResponse.contentIdx++] = (*(registersPtr + padding) & 0xFF00) >> 8;
		slave->dataResponse.content[slave->dataResponse.contentIdx++] = *(registersPtr + padding) & 0x00FF;
	}

	// Data response size based on total data requested
	// This size will be used at serial.transmitData internal loop
	slave->dataResponse.size = MB_SIZE_RESP_READ_MINIMUM + totalDataRequested * 2;

	// Generate CRC
	sizeWithoutCRC = slave->dataResponse.size - 2;
	transmitString = slave->dataResponse.getTransmitStringWithoutCRC(&slave->dataResponse);
	slave->dataResponse.crc = generateCrc( transmitString, sizeWithoutCRC, true);

	MB_DATA_HANDLER_DEBUG();
}

void datahandler_presetSingleRegister(ModbusSlave *slave){
	Uint16 * memAddr;
	Uint16 dataAddress = (slave->dataRequest.content[MB_WRITE_ADDRESS_HIGH] << 8) |
				slave->dataRequest.content[MB_WRITE_ADDRESS_LOW];
	Uint16 * transmitString;

	slave->dataResponse.slaveAddress = MB_SLAVE_ID;
	slave->dataResponse.functionCode = MB_FUNC_WRITE_HOLDINGREGISTER;

	slave->dataResponse.content[slave->dataResponse.contentIdx++] = slave->dataRequest.content[MB_WRITE_ADDRESS_HIGH];
	slave->dataResponse.content[slave->dataResponse.contentIdx++] = slave->dataRequest.content[MB_WRITE_ADDRESS_LOW];

	// TODO: Implement the data map below
	memAddr = (Uint16 *) (dataAddress + 0x8000);
	*(memAddr) = (slave->dataRequest.content[MB_WRITE_VALUE_HIGH] << 8) |
			slave->dataRequest.content[MB_WRITE_VALUE_LOW];

	slave->dataResponse.content[slave->dataResponse.contentIdx++] = *(memAddr) && 0x00FF;
	slave->dataResponse.content[slave->dataResponse.contentIdx++] = (*(memAddr) && 0xFF00) >> 8;

	// Data response size
	// This size will be used at serial.transmitData internal loop
	slave->dataResponse.size = MB_SIZE_RESP_WRITE;

	// Generate CRC
	transmitString = slave->dataResponse.getTransmitStringWithoutCRC(&slave->dataResponse);
	slave->dataResponse.crc = generateCrc( transmitString, MB_SIZE_RESP_WRITE_WITHOUTCRC, true);

	MB_DATA_HANDLER_DEBUG();
}

void datahandler_presetMultipleRegisters(ModbusSlave *slave){
	Uint16 * memAddr;
	Uint16 idx;
	Uint16 dataAddress = (slave->dataRequest.content[MB_WRITE_N_ADDRESS_HIGH] << 8) |
				slave->dataRequest.content[MB_WRITE_N_ADDRESS_LOW];
	Uint16 * transmitString;
	Uint16 totalData = slave->dataRequest.content[MB_WRITE_N_BYTES];

	// Reference to MODBUS Data Map
	char * registersPtr;
	registersPtr = (char *)&(slave->registers);

	// Prepare response (it is the same thing of dataRequest, but you can do some checks at writen registers)
	slave->dataResponse.slaveAddress = MB_SLAVE_ID;
	slave->dataResponse.functionCode = MB_FUNC_WRITE_NREGISTERS;
	slave->dataResponse.content[slave->dataResponse.contentIdx++] = slave->dataRequest.content[MB_WRITE_N_ADDRESS_HIGH];
	slave->dataResponse.content[slave->dataResponse.contentIdx++] = slave->dataRequest.content[MB_WRITE_N_ADDRESS_LOW];
	slave->dataResponse.content[slave->dataResponse.contentIdx++] = slave->dataRequest.content[MB_WRITE_N_QUANTITY_HIGH];
	slave->dataResponse.content[slave->dataResponse.contentIdx++] = slave->dataRequest.content[MB_WRITE_N_QUANTITY_LOW];

	// Writes values at specified address values
//	for(addressIterator = 0; addressIterator < slave->dataRequest.content[MB_WRITE_N_BYTES]; addressIterator++) {
//		memAddr = (Uint16 *) (dataAddress + 0x8000 + addressIterator);
//		*(memAddr) = (slave->dataRequest.content[MB_WRITE_N_VALUES_START_HIGH + addressIterator*2] << 8) |
//				slave->dataRequest.content[MB_WRITE_N_VALUES_START_LOW + addressIterator*2];
//	}
	for (idx = 0; idx < totalData; idx++) {
		Uint16 padding = idx + dataAddress;
		memAddr = (Uint16 *) (registersPtr + padding);
		*(memAddr) = (slave->dataRequest.content[MB_WRITE_N_VALUES_START_HIGH + idx*2] << 8) |
				slave->dataRequest.content[MB_WRITE_N_VALUES_START_LOW + idx*2];
	}

	// Data response size
	// This size will be used at serial.transmitData internal loop
	slave->dataResponse.size = MB_SIZE_RESP_WRITE_N;

	// Generate CRC
	transmitString = slave->dataResponse.getTransmitStringWithoutCRC(&slave->dataResponse);
	slave->dataResponse.crc = generateCrc( transmitString, MB_SIZE_RESP_WRITE_N_WITHOUTCRC, true);

	MB_DATA_HANDLER_DEBUG();
}


void datahandler_exception(ModbusSlave *slave, ModbusError exceptionCode){
	Uint16 * transmitString;

	slave->dataResponse.slaveAddress = MB_SLAVE_ID;
	slave->dataResponse.functionCode = slave->dataRequest.functionCode | 0x80;
	slave->dataResponse.content[slave->dataResponse.contentIdx++] = exceptionCode;

	slave->dataResponse.size = MB_SIZE_RESP_EXCEPTION;

	transmitString = slave->dataResponse.getTransmitStringWithoutCRC(&slave->dataResponse);
	slave->dataResponse.crc = generateCrc(transmitString, MB_SIZE_RESP_EXCEPTION_WITHOUTCRC, true);

	MB_DATA_HANDLER_DEBUG();
}

ModbusDataHandler construct_ModbusDataHandler(){
	ModbusDataHandler modbusDataHandler;

	modbusDataHandler.readCoils = datahandler_readCoils;
	modbusDataHandler.readHoldingRegisters = datahandler_readHoldingRegisters;
	modbusDataHandler.presetSingleRegister = datahandler_presetSingleRegister;
	modbusDataHandler.presetMultipleRegisters = datahandler_presetMultipleRegisters;
	modbusDataHandler.exception = datahandler_exception;

	MB_DATA_HANDLER_DEBUG();

	return modbusDataHandler;
}
