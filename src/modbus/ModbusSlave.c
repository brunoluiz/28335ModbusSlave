#include "ModbusSlave.h"
#include "ModbusData.h"
#include "ModbusData.h"
#include "Modbus.h"
#include "Log.h"

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
	case MB_PRERECEIVE:
		MB_SLAVE_DEBUG("State: MB_PRERECEIVE\n");
		self->preReceive(self);
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

	self->serial.init(&self->serial);
	self->timer.init(&self->timer, MB_T35);

	self->state = MB_START;
}

void slave_start(ModbusSlave *self){
	MB_SLAVE_DEBUG();

	self->timer.resetTimer();
	
	self->dataRequest.clear(&self->dataRequest);
	self->dataResponse.clear(&self->dataResponse);

	self->serial.clear();
	self->serial.setSerialRxEnabled(false);
	self->serial.setSerialTxEnabled(false);

	self->timer.start();
	self->state = MB_TIMER_T35_WAIT;
}

void slave_timerT35Wait(ModbusSlave *self){
	MB_SLAVE_DEBUG();

	if ( self->timer.expiredTimer(&self->timer) ) {
		self->serial.setSerialRxEnabled(true);
		self->timer.stop();
		self->state = MB_PRERECEIVE;
	}
}

void slave_preReceive(ModbusSlave *self)
{
	MB_SLAVE_DEBUG();

	// Wait to receive Slave ID and Function Code
	while ( self->serial.rxBufferStatus() != 2 ){ }

	self->dataRequest.slaveAddress = self->serial.getRxBufferedWord();
	self->dataRequest.functionCode = self->serial.getRxBufferedWord();

	// Check which function code it is to adjust the size of the RX FIFO buffer
	// In the case of function code 0x0016, it has a variable size
	if (self->dataRequest.functionCode == MB_FUNC_WRITE_NREGISTERS) {
		// Wait to receive the first address of registers and the quantity
		while ( self->serial.rxBufferStatus() != 5 ){ }
		self->dataRequest.content[MB_WRITE_N_ADDRESS_HIGH] = self->serial.getRxBufferedWord();
		self->dataRequest.content[MB_WRITE_N_ADDRESS_LOW] = self->serial.getRxBufferedWord();
		self->dataRequest.content[MB_WRITE_N_QUANTITY_HIGH] = self->serial.getRxBufferedWord();
		self->dataRequest.content[MB_WRITE_N_QUANTITY_LOW] = self->serial.getRxBufferedWord();
		self->dataRequest.content[MB_WRITE_N_BYTES] = self->serial.getRxBufferedWord();

		self->dataRequest.contentIdx = 5;
		self->serial.fifoWaitBuffer = self->dataRequest.content[MB_WRITE_N_BYTES] + 2;
	}
	else {
		self->serial.fifoWaitBuffer = 6;
	}

	self->state = MB_IDLE;
}

void slave_idle(ModbusSlave *self){
	MB_SLAVE_DEBUG();

	while ( self->serial.rxBufferStatus() != self->serial.fifoWaitBuffer ){ }

	self->state = MB_RECEIVE;
}

void slave_receive(ModbusSlave *self){
	// Treat received serial data, pushing out of buffer
	Uint16 interator = 0;

	MB_SLAVE_DEBUG();

	if (self->dataRequest.functionCode == 0x0010) {
		while(interator < self->dataRequest.content[MB_WRITE_N_BYTES]){
			self->dataRequest.content[self->dataRequest.contentIdx++] = self->serial.getRxBufferedWord();
			interator++;
		}
	}
	else {
		while(interator < MB_SIZE_CONTENT_NORMAL){
			self->dataRequest.content[self->dataRequest.contentIdx++] = self->serial.getRxBufferedWord();
			interator++;
		}
	}

	self->dataRequest.crc = (self->serial.getRxBufferedWord() << 8) |
			self->serial.getRxBufferedWord();

	// Clears interruptions or overflow flags from serial
	// self->serial.clear();
	// Disable serial receiving
	self->serial.setSerialRxEnabled(false);

	if (self->dataRequest.functionCode == MB_FUNC_READ_HOLDINGREGISTERS){
		self->dataRequest.size = MB_SIZE_REQ_READ;
	}
	else if(self->dataRequest.functionCode == MB_FUNC_WRITE_NREGISTERS) {
		self->dataRequest.size = MB_SIZE_REQ_WRITE_N_MINIMUM + self->dataRequest.content[MB_WRITE_N_BYTES];
	}
	else if (self->dataRequest.functionCode == MB_FUNC_WRITE_HOLDINGREGISTER){
		self->dataRequest.size = MB_SIZE_REQ_WRITE;
	}

	self->state = MB_PROCESS;
}

void slave_process(ModbusSlave *self){
#if (MB_RTU_TCP == false)
	Uint16 receivedCrc = self->dataRequest.crc;
	Uint16 sizeWithoutCrc = self->dataRequest.size - 2;
	Uint16 generatedCrc = 0; generateCrc(self->dataRequest.getTransmitStringWithoutCRC(&self->dataRequest),
		sizeWithoutCrc);

	// Check if the received CRC is equal to CRC locally generated
	if (generatedCrc != receivedCrc) {
		MB_SLAVE_DEBUG("Error on CRC!");
		self->dataHandler.exception(self, MB_ERROR_ILLEGALDATA);
	}

	// Requested slave address must be equal of pre-defined ID
	if (self->dataRequest.slaveAddress != MB_SLAVE_ID, 0){
		MB_SLAVE_DEBUG("Request is not for this device!");
		self->state = MB_START;
		return ;
	}
#endif

	// Check the function code and do some action
	if (self->dataRequest.functionCode == MB_FUNC_READ_HOLDINGREGISTERS){
		MB_SLAVE_DEBUG("Reading holding registers");
		self->dataHandler.readInputRegisters(self);
	}
	else if (self->dataRequest.functionCode == MB_FUNC_WRITE_HOLDINGREGISTER){
		MB_SLAVE_DEBUG("Presetting holding registers");
		self->dataHandler.presetSingleRegister(self);
	}
	else if (self->dataRequest.functionCode == MB_FUNC_WRITE_NREGISTERS){
		MB_SLAVE_DEBUG("Presetting holding registers");
		self->dataHandler.presetMultipleRegisters(self);
	}
	else {
		MB_SLAVE_DEBUG("Exception: ILLEGALFUNC");
		self->dataHandler.exception(self, MB_ERROR_ILLEGALFUNC);
	}

	self->state = MB_TRANSMIT;
}

void slave_transmit(ModbusSlave *self){
	MB_SLAVE_DEBUG();
	
	self->serial.setSerialTxEnabled(true);
	self->serial.transmitData(self->dataResponse.getTransmitString(&self->dataResponse),
			self->dataResponse.size);

	self->state = MB_START;
}

void slave_destroy(ModbusSlave *self){
	MB_SLAVE_DEBUG();
}

ModbusSlave construct_ModbusSlave(){
	ModbusSlave modbusSlave;

	MB_SLAVE_DEBUG();

	modbusSlave.state = MB_CREATE;
	modbusSlave.dataRequest = construct_ModbusData();
	modbusSlave.dataResponse = construct_ModbusData();
	modbusSlave.serial = construct_Serial();
	modbusSlave.timer = construct_Timer();

	modbusSlave.dataHandler = construct_ModbusDataHandler();

	modbusSlave.loopStates = slave_loopStates;
	modbusSlave.create = slave_create;
	modbusSlave.start = slave_start;
	modbusSlave.timerT35Wait = slave_timerT35Wait;
	modbusSlave.preReceive = slave_preReceive;
	modbusSlave.idle = slave_idle;
	modbusSlave.receive = slave_receive;
	modbusSlave.process = slave_process;
	modbusSlave.transmit = slave_transmit;
	modbusSlave.destroy = slave_destroy;

	return modbusSlave;
}
