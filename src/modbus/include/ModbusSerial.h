#ifndef SERIAL_H_
#define SERIAL_H_

#include "Modbus.h"

typedef struct Serial Serial;

struct Serial {
	Uint16 bitsNumber;
	Uint16 parityType;
	Uint16 baudrate;
	bool fifoEnabled;
	bool rxEnabled;
	bool txEnabled;
	bool interruptionsEnabled;

	void (*setupInterruptions)();
	void (*clear)();
	bool (*rxOverflow)();
	void (*setSerialRxEnabled)(Serial *self, bool status);
	void (*setSerialTxEnabled)(Serial *self, bool status);
	void (*init)(Serial *self);
	void (*transmitData)(Uint16 * data, Uint16 size);
	Uint16 (*getRxBufferedWord)();
};

void serial_setupInterruptions();
void serial_clear();
bool serial_rxOverflow();
void serial_setSerialRxEnabled(Serial *self, bool status);
void serial_setSerialTxEnabled(Serial *self, bool status);
void serial_init(Serial *self);
void serial_transmitData(Uint16 * data, Uint16 size);
Uint16 serial_getRxBufferedWord();
Serial construct_Serial();

#endif
