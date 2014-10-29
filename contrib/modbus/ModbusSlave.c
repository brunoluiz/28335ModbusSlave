#include "ModbusSlave.h"
#include "ModbusData.h"
#include "ModbusDefinitions.h"
#include "ModbusSettings.h"
#include "Log.h"

#if DEBUG_UTILS_PROFILING
#include "Profiling.h"
ProfilingTool profiling;
#endif

void slave_loopStates(ModbusSlave *self){
	MB_SLAVE_DEBUG();
	switch (self->state) {
	case MB_CREATE:
		MB_SLAVE_DEBUG("State: MB_CREATE\n");
		self->create(self);
		break;
	case MB_START:
		MB_SLAVE_DEBUG("State: MB_START\n");
		self->start(self);
		break;
	case MB_TIMER_T35_WAIT:
		MB_SLAVE_DEBUG("State: MB_TIMER_T35_WAIT\n");
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

// STATE: MB_CREATE
// Initialize all needed peripherals (timer and serial mostly)
// Only executed once!
void slave_create(ModbusSlave *self){

	// Configure the 3.5c time for timer
	// For baudrates higher than 19200 is recommended a fixed value
#if SERIAL_BAUDRATE <= 19200
	Uint16 serialFrameSize = SERIAL_START_STOP_NUMBER_BITS
			+ SERIAL_BITS_NUMBER
			+ SERIAL_PARITY_NUMBER_BITS;
	Uint64 mbT35 = (serialFrameSize * 3500000) / SERIAL_BAUDRATE;
#else
	Uint64 mbT35 = 1750;
#endif
	MB_SLAVE_DEBUG();

	// Configure Serial Port A
	self->serial.baudrate = SERIAL_BAUDRATE;
	self->serial.parityType = SERIAL_PARITY_NONE;
	self->serial.bitsNumber = SERIAL_BITS_NUMBER;
	self->serial.init(&self->serial);

	// Configure timer with the 3.5c time for timeout
	self->timer.init(&self->timer, mbT35);

#if DEBUG_UTILS_PROFILING
	profiling = construct_ProfilingTool();
#endif

	self->state = MB_START;
}

// STATE: MB_START
// Reset all data from serial, data frames and timers
// Restart point to the lifecycle
void slave_start(ModbusSlave *self){
	MB_SLAVE_DEBUG();

	// Return the value to 3.5 char
	self->timer.resetTimer();
	
	self->dataRequest.clear(&self->dataRequest);
	self->dataResponse.clear(&self->dataResponse);

	self->serial.clear();

	self->timer.start();
#if DEBUG_UTILS_PROFILING
	profiling.start(&profiling);
#endif

	self->state = MB_TIMER_T35_WAIT;
}

// STATE: MB_TIMER_T35_WAIT
// Waits until the timer expires (when timeout = 3.5c)
void slave_timerT35Wait(ModbusSlave *self){
	MB_SLAVE_DEBUG();

	if ( self->timer.expiredTimer(&self->timer) ) {
		self->timer.stop();
		self->state = MB_IDLE;

		#if DEBUG_UTILS_PROFILING
		profiling.registerStep(&profiling, profiling_MB_TIMER_T35_WAIT);
		#endif
	}
}

// STATE: MB_IDLE
// Will stay idle, receiving serial data
// It has some processes in it to check if the size will be variable
// Example: writing on multiple registers
void slave_idle(ModbusSlave *self){
	MB_SLAVE_DEBUG();

	// Wait to receive Slave ID and Function Code
	while ( ( self->serial.rxBufferStatus() < 2 ) &&
		(self->serial.getRxError() == false ) ){}

	// Check which function code it is to adjust the size of the RX FIFO buffer
	// In the case of function code 0x0010, it has a variable size
	self->dataRequest.slaveAddress = self->serial.getRxBufferedWord();
	self->dataRequest.functionCode = self->serial.getRxBufferedWord();

	// If the function code is for writing on multiple registers, then the FIFO Wait Buffer will not be fixed
	// Else it uses a fixed value (6 bytes)
	if (self->dataRequest.functionCode == MB_FUNC_WRITE_NREGISTERS ||
			self->dataRequest.functionCode == MB_FUNC_FORCE_NCOILS) {
		// Wait to receive the first address of registers and the quantity
		while ( ( self->serial.rxBufferStatus() < 5 ) &&
				(self->serial.getRxError() == false ) ){ }
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

	// Waiting RX data (based on fifoWaitBuffer)
	while ( ( self->serial.rxBufferStatus() < self->serial.fifoWaitBuffer ) &&
			(self->serial.getRxError() == false ) ){}

	// If there is any error on reception, it will go to the START state
	if (self->serial.getRxError() == true){
		self->state = MB_START;
	} else {
		self->state = MB_RECEIVE;
	}

	#if DEBUG_UTILS_PROFILING
	profiling.registerStep(&profiling, profiling_MB_IDLE);
	#endif
}

// STATE: MB_RECEIVE
// Treat the received data, putting the serial data on the dataRequest structure
// The dataRequest.content size can be different for each function code
void slave_receive(ModbusSlave *self){
	// Treat received serial data, pushing out of buffer
	Uint16 interator = 0;

	MB_SLAVE_DEBUG();

	if (self->dataRequest.functionCode == MB_FUNC_WRITE_NREGISTERS ||
			self->dataRequest.functionCode == MB_FUNC_FORCE_NCOILS ) {
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
	self->serial.clear();

	// Configures the size of data request frame
	// It will be used in some loops on ModbusDataHandler
	// By default it will use the READ size, because it is always the same (independent of function code)
	self->dataRequest.size = MB_SIZE_REQ_READ;

	if(self->dataRequest.functionCode == MB_FUNC_WRITE_NREGISTERS ||
			self->dataRequest.functionCode == MB_FUNC_FORCE_NCOILS) {
		self->dataRequest.size = MB_SIZE_REQ_WRITE_N_MINIMUM + self->dataRequest.content[MB_WRITE_N_BYTES];
	}
	else if (self->dataRequest.functionCode == MB_FUNC_WRITE_HOLDINGREGISTER ||
			self->dataRequest.functionCode == MB_FUNC_FORCE_COIL){
		self->dataRequest.size = MB_SIZE_REQ_WRITE;
	}

	self->state = MB_PROCESS;

	#if DEBUG_UTILS_PROFILING
	profiling.registerStep(&profiling, profiling_MB_RECEIVE);
	#endif
}

// STATE: MB_PROCESS
// Do the "magic". Check if the request have the right CRC and is for this device
// After first checks, it will begin the requested funcion code and prepare the dataResponse
void slave_process(ModbusSlave *self){
	bool jumpProcessing = false;

	#if (MB_RTU_TCP == false)
	Uint16 receivedCrc = self->dataRequest.crc;
	Uint16 sizeWithoutCrc = self->dataRequest.size - 2;
	Uint16 generatedCrc = generateCrc(self->dataRequest.getTransmitStringWithoutCRC(&self->dataRequest),
		sizeWithoutCrc, false);

	// Check if the received CRC is equal to locally generated CRC
	if (generatedCrc != receivedCrc) {
		MB_SLAVE_DEBUG("Error on CRC!");
		self->dataHandler.exception(self, MB_ERROR_ILLEGALDATA);
		self->state = MB_TRANSMIT;
		jumpProcessing = true;
	}

	// Requested slave address must be equal of pre-defined ID
	if (self->dataRequest.slaveAddress != MB_SLAVE_ID && self->dataRequest.slaveAddress != 0){
		MB_SLAVE_DEBUG("Request is not for this device!");
		self->state = MB_START;
		jumpProcessing = true;
	}
	#endif

	if (jumpProcessing == false) {
		// Check the function code and do some action using dataHandler
		if (self->dataRequest.functionCode == MB_FUNC_READ_COIL && MB_COILS_ENABLED){
			MB_SLAVE_DEBUG("Reading coils");
			self->dataHandler.readDigitalData(self, MB_FUNC_READ_COIL);
		}
		else if (self->dataRequest.functionCode == MB_FUNC_READ_INPUT && MB_INPUTS_ENABLED){
			MB_SLAVE_DEBUG("Reading coils");
			self->dataHandler.readDigitalData(self, MB_FUNC_READ_INPUT);
		}
		else if (self->dataRequest.functionCode == MB_FUNC_READ_INPUTREGISTERS && MB_INPUT_REGISTERS_ENABLED){
			MB_SLAVE_DEBUG("Reading holding registers");
			self->dataHandler.readAnalogData(self, MB_FUNC_READ_INPUTREGISTERS);
		}
		else if (self->dataRequest.functionCode == MB_FUNC_READ_HOLDINGREGISTERS && MB_HOLDING_REGISTERS_ENABLED){
			MB_SLAVE_DEBUG("Reading holding registers");
			self->dataHandler.readAnalogData(self, MB_FUNC_READ_HOLDINGREGISTERS);
		}
		else if (self->dataRequest.functionCode == MB_FUNC_WRITE_HOLDINGREGISTER && MB_HOLDING_REGISTERS_ENABLED){
			MB_SLAVE_DEBUG("Presetting holding register");
			self->dataHandler.presetSingleRegister(self);
		}
		else if (self->dataRequest.functionCode == MB_FUNC_FORCE_COIL && MB_COILS_ENABLED){
			MB_SLAVE_DEBUG("Forcing coil");
			self->dataHandler.forceSingleCoil(self);
		}
		else if (self->dataRequest.functionCode == MB_FUNC_WRITE_NREGISTERS && MB_HOLDING_REGISTERS_ENABLED){
			MB_SLAVE_DEBUG("Presetting multiple holding registers");
			self->dataHandler.presetMultipleRegisters(self);
		}
		else if (self->dataRequest.functionCode == MB_FUNC_FORCE_NCOILS && MB_COILS_ENABLED){
			MB_SLAVE_DEBUG("Forcing multiple coils");
			self->dataHandler.forceMultipleCoils(self);
		}
		else {
			MB_SLAVE_DEBUG("Exception: ILLEGALFUNC");
			self->dataHandler.exception(self, MB_ERROR_ILLEGALFUNC);
		}
		self->state = MB_TRANSMIT;
	}

	#if DEBUG_UTILS_PROFILING
	profiling.registerStep(&profiling, profiling_MB_PROCESS);
	#endif
}

// STATE: MB_TRANSMIT
// Transmit all data from dataResponse throught TX and then go back to MB_START
void slave_transmit(ModbusSlave *self){
	MB_SLAVE_DEBUG();

	self->serial.transmitData(self->dataResponse.getTransmitString(&self->dataResponse),
			self->dataResponse.size);

	self->state = MB_START;

	#if DEBUG_UTILS_PROFILING
	profiling.registerStep(&profiling, profiling_MB_TRANSMIT);
	profiling.stop(&profiling);
	#endif
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

#if MB_COILS_ENABLED
	modbusSlave.coils = construct_ModbusCoilsMap();
#endif
#if MB_INPUTS_ENABLED
	modbusSlave.inputs = construct_ModbusInputsMap();
#endif
#if MB_HOLDING_REGISTERS_ENABLED
	modbusSlave.holdingRegisters = construct_ModbusHoldingRegistersMap();
#endif
#if MB_INPUT_REGISTERS_ENABLED
	modbusSlave.inputRegisters = construct_ModbusInputRegistersMap();
#endif

	modbusSlave.loopStates = slave_loopStates;
	modbusSlave.create = slave_create;
	modbusSlave.start = slave_start;
	modbusSlave.timerT35Wait = slave_timerT35Wait;
	modbusSlave.idle = slave_idle;
	modbusSlave.receive = slave_receive;
	modbusSlave.process = slave_process;
	modbusSlave.transmit = slave_transmit;
	modbusSlave.destroy = slave_destroy;

	modbusSlave.readInputCounter = 0;

	return modbusSlave;
}
