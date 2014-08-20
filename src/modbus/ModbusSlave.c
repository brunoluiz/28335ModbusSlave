#include "ModbusSlave.h"
#include "ModbusDataRequest.h"
#include "ModbusDataResponse.h"
#include "Modbus.h"
#include "Log.h"
#include "crc.h"
#include "stdlib.h"

void slave_loopStates(ModbusSlave *self){
	MB_SLAVE_DEBUG();
	switch (self->state) {
	case MB_CREATE:
		MB_SLAVE_DEBUG("State: MB_CREATE");
		self->create(self);
		break;
	case MB_START:
		MB_SLAVE_DEBUG("State: MB_START");
		self->start(self);
		break;
	case MB_TIMER_T35_WAIT:
		MB_SLAVE_DEBUG("State: MB_TIMER_T35_WAIT");
		self->timerT35Wait(self);
		break;
	case MB_IDLE:
		MB_SLAVE_DEBUG("State: MB_IDLE\n");
		self->idle(self);
		break;
	case MB_RECEIVE:
		MB_SLAVE_DEBUG("State: MB_RECEIVE\n");
		self->receive(self);
		break;
	case MB_PROCESS:
		MB_SLAVE_DEBUG("State: MB_PROCESS\n");
		self->process(self);
		break;
	case MB_TRANSMIT:
		MB_SLAVE_DEBUG("State: MB_TRANSMIT\n");
		self->transmit(self);
		break;
	case MB_DESTROY:
		MB_SLAVE_DEBUG("State: MB_DESTROY\n");
		self->destroy(self);
		break;
	}
}

void slave_create(ModbusSlave *self){
	MB_SLAVE_DEBUG();

	self->serial.baudrate		= MODBUS_BAUDRATE;
	self->serial.bitsNumber		= MODBUS_BITS_QNT;
	self->serial.fifoEnabled	= MODBUS_FIFO_ENABLED;
	self->serial.parityType		= SERIAL_PARITY_NONE;
	self->serial.init(&self->serial);

	self->timer.init(&self->timer, MODBUS_T35);

	self->state = MB_START;
}

void slave_start(ModbusSlave *self){
	MB_SLAVE_DEBUG();

	self->timer.resetTimer();
	
	self->serial.setSerialRxEnabled(&self->serial, false);
	self->serial.setSerialTxEnabled(&self->serial, false);

	self->timer.start();
	self->state = MB_TIMER_T35_WAIT;
}

void slave_timerT35Wait(ModbusSlave *self){
	MB_SLAVE_DEBUG();

	if ( self->timer.expiredTimer(&self->timer) ) {
		self->serial.setSerialRxEnabled(&self->serial, true);
		self->timer.stop();
		self->state = MB_IDLE;
	}
}

void slave_idle(ModbusSlave *self){
	MB_SLAVE_DEBUG();

	while ( self->serial.rxBufferStatus() != self->dataRequest.size(&self->dataRequest) ){ }

	self->state = MB_RECEIVE;
}

void slave_receive(ModbusSlave *self){
	// Treat received serial data, pushing out of buffer
	Uint16 request_firstDataAddress;
	Uint16 request_totalDataRequested;
	Uint16 request_crcRequest;

	MB_SLAVE_DEBUG();

	self->dataRequest.slaveAddress = self->serial.getRxBufferedWord();
	self->dataRequest.functionCode = self->serial.getRxBufferedWord();

	request_firstDataAddress = self->serial.getRxBufferedWord() << 8;
	request_firstDataAddress = request_firstDataAddress | self->serial.getRxBufferedWord();
	self->dataRequest.firstDataAddress = request_firstDataAddress;

	request_totalDataRequested = self->serial.getRxBufferedWord() << 8;
	request_totalDataRequested = request_totalDataRequested | self->serial.getRxBufferedWord();
	self->dataRequest.totalDataRequested = request_totalDataRequested;
	
	request_crcRequest = self->serial.getRxBufferedWord() << 8;
	request_crcRequest = request_crcRequest | self->serial.getRxBufferedWord();
	self->dataRequest.crc = request_crcRequest;

	// Clears interruptions or overflow flags from serial
	self->serial.clear();
	// Disable serial receiving
	self->serial.setSerialRxEnabled(&self->serial, false);

	self->state = MB_PROCESS;
}

void slave_process(ModbusSlave *self){
	// First things first: check if the CRC is OK
	// If it is not, throw an error!
	Uint16 receivedCrc = self->dataRequest.crc;
	Uint16 generatedCrc = 0x0;
	Uint16 sizeWithoutCrc = self->dataRequest.size(&self->dataRequest) - 2;
	generatedCrc = generateCrc(self->dataRequest.getReceivedStringWithoutCRC(&self->dataRequest),
		sizeWithoutCrc);

	// Check if the received CRC is equal to CRC locally generated
	if (generatedCrc != receivedCrc) {
		MB_SLAVE_DEBUG("Error on CRC!");
		self->dataHandler.exception(self, MB_ERROR_ILLEGALDATA);
	}

	// Requested slave address must be equal of pre-defined ID
	if (self->dataRequest.slaveAddress != MODBUS_SLAVE_ID){
		MB_SLAVE_DEBUG("Request is not for this device!");
		self->state = MB_START;
		return ;
	}

	// Check the function code and do some action
	if (self->dataRequest.functionCode == MB_FUNC_READ_HOLDINGREGISTERS){
		MB_SLAVE_DEBUG("Reading holding registers");
		self->dataHandler.readInputRegisters(self);
	}
	else if (self->dataRequest.functionCode == MB_FUNC_WRITE_HOLDINGREGISTER){
		MB_SLAVE_DEBUG("Presetting holding registers");
		self->dataHandler.presetSingleRegister(self);
	}
	else {
		MB_SLAVE_DEBUG("Exception: ILLEGALFUNC");
		self->dataHandler.exception(self, MB_ERROR_ILLEGALFUNC);
	}

	self->state = MB_TRANSMIT;
}

void slave_transmit(ModbusSlave *self){
	Uint16 transmitStringSize = self->dataResponse.size(&self->dataResponse);
	Uint16 * transmitString = (Uint16 *) calloc(transmitStringSize, sizeof(Uint16));

	MB_SLAVE_DEBUG();

	transmitString = self->dataResponse.getTransmitString(&self->dataResponse);
	
	self->serial.setSerialTxEnabled(&self->serial, true);
	self->serial.transmitData(transmitString, transmitStringSize);

	self->state = MB_START;
}

void slave_destroy(ModbusSlave *self){
	MB_SLAVE_DEBUG();
}

ModbusSlave construct_ModbusSlave(){
	ModbusSlave modbusSlave;

	MB_SLAVE_DEBUG();

	modbusSlave.state = MB_CREATE;
	modbusSlave.dataRequest = construct_ModbusDataRequest();
	modbusSlave.dataResponse = construct_ModbusDataResponse();
	modbusSlave.serial = construct_Serial();
	modbusSlave.timer = construct_Timer();

	modbusSlave.dataHandler = construct_ModbusDataHandler();

	modbusSlave.loopStates = slave_loopStates;
	modbusSlave.create = slave_create;
	modbusSlave.start = slave_start;
	modbusSlave.timerT35Wait = slave_timerT35Wait;
	modbusSlave.idle = slave_idle;
	modbusSlave.receive = slave_receive;
	modbusSlave.process = slave_process;
	modbusSlave.transmit = slave_transmit;
	modbusSlave.destroy = slave_destroy;

	return modbusSlave;
}
