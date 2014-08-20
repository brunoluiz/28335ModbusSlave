#ifndef MODBUS_SLAVE_H_
#define MODBUS_SLAVE_H_

#include "Modbus.h"
#include "ModbusDataRequest.h"
#include "ModbusDataResponse.h"
#include "ModbusDataHandler.h"
#include "Serial.h"
#include "Timer.h"

// typedef struct ModbusSlave ModbusSlave;

struct ModbusSlave {
	ModbusState state;

	ModbusDataRequest dataRequest;
	ModbusDataResponse dataResponse;

	Serial serial;
	Timer timer;

	ModbusDataHandler dataHandler;

	void (*loopStates)(ModbusSlave *self);
	void (*create)(ModbusSlave *self);
	void (*start)(ModbusSlave *self);
	void (*timerT35Wait)(ModbusSlave *self);
	void (*idle)(ModbusSlave *self);
	void (*receive)(ModbusSlave *self);
	void (*process)(ModbusSlave *self);
	void (*transmit)(ModbusSlave *self);
	void (*destroy)(ModbusSlave *self);
};

void slave_loopStates(ModbusSlave *self);
void slave_create(ModbusSlave *self);
void slave_start(ModbusSlave *self);
void slave_timerT35Wait(ModbusSlave *self);
void slave_idle(ModbusSlave *self);
void slave_receive(ModbusSlave *self);
void slave_process(ModbusSlave *self);
void slave_transmit(ModbusSlave *self);
void slave_destroy(ModbusSlave *self);
ModbusSlave construct_ModbusSlave();

// extern ModbusSlave modbus;

#endif
