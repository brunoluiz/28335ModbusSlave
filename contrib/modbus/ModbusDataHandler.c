#include "ModbusDefinitions.h"
#include "ModbusDataHandler.h"
#include "ModbusSlave.h"
#include "Log.h"
#include "Crc.h"
#include "math.h"

void datahandler_readDigitalData(ModbusSlave *slave, ModbusFunctionCode funcCode) {
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
	char * dataPtr;

	// dataPtr points to the right struct (depending on passed funcCode)
	if (funcCode == MB_FUNC_READ_COIL){
		dataPtr = (char *)&(slave->coils);
	}
	else if(funcCode == MB_FUNC_READ_INPUT){
		dataPtr = (char *)&(slave->inputs);
	}

	slave->dataResponse.slaveAddress = MB_SLAVE_ID;
	slave->dataResponse.functionCode = funcCode;

	// Number of Bytes
	slave->dataResponse.content[slave->dataResponse.contentIdx++] = numberOfBytes;

	// Reads specified address values
	for (idx = 0; idx < numberOfBytes; idx++) {
		Uint16 coilsNum = totalDataRequested - idx*8;
		Uint16 padding = idx + firstDataAddress - reg16ReturnIdx;
		Uint16 content = 0;
		content = *(dataPtr + padding) >> (reg16ReturnIdx*8);

		if(coilsNum >= 8) {
			slave->dataResponse.content[slave->dataResponse.contentIdx++] = content & 0x00FF;
		} else {
			Uint16 mask;
			mask = ~(0xFFFF << coilsNum);
			slave->dataResponse.content[slave->dataResponse.contentIdx++] = content & mask;
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

void datahandler_readAnalogData(ModbusSlave *slave, ModbusFunctionCode funcCode) {
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
	if (funcCode == MB_FUNC_READ_HOLDINGREGISTERS){
		registersPtr = (char *)&(slave->holdingRegisters);
	}
	else if(funcCode == MB_FUNC_READ_INPUTREGISTERS){
		registersPtr = (char *)&(slave->inputRegisters);
	}

	slave->dataResponse.slaveAddress = MB_SLAVE_ID;
	slave->dataResponse.functionCode = funcCode;

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
	Uint16 dataAddress = (slave->dataRequest.content[MB_WRITE_ADDRESS_HIGH] << 8) |
				slave->dataRequest.content[MB_WRITE_ADDRESS_LOW];
	Uint16 * transmitString;

	// Reference to MODBUS Data Map
	char * registersPtr;
	registersPtr = (char *)&(slave->holdingRegisters);

	slave->dataResponse.slaveAddress = MB_SLAVE_ID;
	slave->dataResponse.functionCode = MB_FUNC_WRITE_HOLDINGREGISTER;

	slave->dataResponse.content[slave->dataResponse.contentIdx++] = slave->dataRequest.content[MB_WRITE_ADDRESS_HIGH];
	slave->dataResponse.content[slave->dataResponse.contentIdx++] = slave->dataRequest.content[MB_WRITE_ADDRESS_LOW];

	// TODO: Implement the data map below
	*(registersPtr + dataAddress) = (slave->dataRequest.content[MB_WRITE_VALUE_HIGH] << 8) |
			slave->dataRequest.content[MB_WRITE_VALUE_LOW];

	slave->dataResponse.content[slave->dataResponse.contentIdx++] = *(registersPtr + dataAddress) && 0x00FF;
	slave->dataResponse.content[slave->dataResponse.contentIdx++] = (*(registersPtr + dataAddress) && 0xFF00) >> 8;

	// Data response size
	// This size will be used at serial.transmitData internal loop
	slave->dataResponse.size = MB_SIZE_RESP_WRITE;

	// Generate CRC
	transmitString = slave->dataResponse.getTransmitStringWithoutCRC(&slave->dataResponse);
	slave->dataResponse.crc = generateCrc( transmitString, MB_SIZE_RESP_WRITE_WITHOUTCRC, true);

	MB_DATA_HANDLER_DEBUG();
}

void datahandler_forceSingleCoil(ModbusSlave *slave){
	Uint16 dataAddress = (slave->dataRequest.content[MB_WRITE_ADDRESS_HIGH] << 8) |
				slave->dataRequest.content[MB_WRITE_ADDRESS_LOW];
	Uint16 coilAddress = floor(dataAddress/16);
	Uint16 padding = dataAddress - floor(dataAddress/16)*16;
	Uint32 tempValue;
	Uint16 coilValue = (slave->dataRequest.content[MB_WRITE_VALUE_HIGH] << 8) |
			slave->dataRequest.content[MB_WRITE_VALUE_LOW];
	Uint16 coilValueResponse;
	Uint16 * transmitString;

	// Reference to MODBUS Data Map
	char * registersPtr;
	registersPtr = (char *)&(slave->coils);
	tempValue = *(registersPtr + coilAddress);

	slave->dataResponse.slaveAddress = MB_SLAVE_ID;
	slave->dataResponse.functionCode = MB_FUNC_FORCE_COIL;

	slave->dataResponse.content[slave->dataResponse.contentIdx++] = slave->dataRequest.content[MB_WRITE_ADDRESS_HIGH];
	slave->dataResponse.content[slave->dataResponse.contentIdx++] = slave->dataRequest.content[MB_WRITE_ADDRESS_LOW];

	// Setting value
	if (coilValue == 0xFF00) coilValue = 1;
	coilValue = coilValue << padding;
	tempValue &= ~(1 << padding);
	*(registersPtr + coilAddress) = tempValue | coilValue;

	if( (*(registersPtr + coilAddress) >> padding ) & 0x0001) coilValueResponse = 0xFF;
	else coilValueResponse = 0x00;

	slave->dataResponse.content[slave->dataResponse.contentIdx++] = coilValueResponse;
	slave->dataResponse.content[slave->dataResponse.contentIdx++] = 0x00;

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
	Uint16 totalData = slave->dataRequest.content[MB_WRITE_N_BYTES] / 2;

	// Reference to MODBUS Data Map
	char * registersPtr;
	registersPtr = (char *)&(slave->holdingRegisters);

	// Prepare response (it is the same thing of dataRequest, but you can do some checks at writen registers)
	slave->dataResponse.slaveAddress = MB_SLAVE_ID;
	slave->dataResponse.functionCode = MB_FUNC_WRITE_NREGISTERS;
	slave->dataResponse.content[slave->dataResponse.contentIdx++] = slave->dataRequest.content[MB_WRITE_N_ADDRESS_HIGH];
	slave->dataResponse.content[slave->dataResponse.contentIdx++] = slave->dataRequest.content[MB_WRITE_N_ADDRESS_LOW];
	slave->dataResponse.content[slave->dataResponse.contentIdx++] = slave->dataRequest.content[MB_WRITE_N_QUANTITY_HIGH];
	slave->dataResponse.content[slave->dataResponse.contentIdx++] = slave->dataRequest.content[MB_WRITE_N_QUANTITY_LOW];

	// Writes values at specified address values
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

void datahandler_forceMultipleCoils(ModbusSlave *slave){
	Uint16 idx;
	Uint16 * transmitString;
	Uint16 coilAddress = (slave->dataRequest.content[MB_WRITE_N_ADDRESS_HIGH] << 8) |
				slave->dataRequest.content[MB_WRITE_N_ADDRESS_LOW];
//	Uint16 totalBytes = slave->dataRequest.content[MB_WRITE_N_BYTES];
	Uint16 totalCoils = (slave->dataRequest.content[MB_WRITE_N_QUANTITY_HIGH] << 8) |
			slave->dataRequest.content[MB_WRITE_N_QUANTITY_LOW];
	Uint16 addrPadding;
	Uint16 totalPadding;
	Uint16 ptrPadding;
	char tempAddrContent, tempCoilContent;
	char mask;

	// Reference to MODBUS Data Map
	char * coilsPtr;
	coilsPtr = (char *)&(slave->coils);

	// Prepare response (it is the same thing of dataRequest, but you can do some checks at writen registers)
	slave->dataResponse.slaveAddress = MB_SLAVE_ID;
	slave->dataResponse.functionCode = MB_FUNC_FORCE_NCOILS;
	slave->dataResponse.content[slave->dataResponse.contentIdx++] = slave->dataRequest.content[MB_WRITE_N_ADDRESS_HIGH];
	slave->dataResponse.content[slave->dataResponse.contentIdx++] = slave->dataRequest.content[MB_WRITE_N_ADDRESS_LOW];
	slave->dataResponse.content[slave->dataResponse.contentIdx++] = slave->dataRequest.content[MB_WRITE_N_QUANTITY_HIGH];
	slave->dataResponse.content[slave->dataResponse.contentIdx++] = slave->dataRequest.content[MB_WRITE_N_QUANTITY_LOW];

	// Writes values at specified address values
	// Not fully implemented: only work with 8 coils

	addrPadding = coilAddress % 16;
	for (idx = 0; idx < totalCoils; idx++) {
		ptrPadding = coilAddress/16 + idx/16;
		totalPadding = addrPadding + idx%16;

		tempAddrContent = *( coilsPtr );

		mask = (1 << totalPadding );

		tempCoilContent = slave->dataRequest.content[MB_WRITE_N_VALUES_START_HIGH + idx/8] >> idx%8;
		tempCoilContent = ( tempCoilContent << totalPadding ) & mask;
		tempAddrContent = ( tempAddrContent & ~mask );
		*(coilsPtr + ptrPadding) = tempAddrContent | tempCoilContent;
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

	modbusDataHandler.readDigitalData = datahandler_readDigitalData;
	modbusDataHandler.readAnalogData = datahandler_readAnalogData;

	modbusDataHandler.presetSingleRegister = datahandler_presetSingleRegister;
	modbusDataHandler.forceSingleCoil = datahandler_forceSingleCoil;
	modbusDataHandler.presetMultipleRegisters = datahandler_presetMultipleRegisters;
	modbusDataHandler.forceMultipleCoils = datahandler_forceMultipleCoils;

	modbusDataHandler.exception = datahandler_exception;

	MB_DATA_HANDLER_DEBUG();

	return modbusDataHandler;
}
