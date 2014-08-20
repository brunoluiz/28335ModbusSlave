#ifndef MODBUS_SERIAL_H_
#define MODBUS_SERIAL_H_

#include "PlataformSettings.h"

typedef struct Serial Serial;

#ifndef SERIAL_BAUDRATE
#define SERIAL_BAUDRATE 9600
#endif

#ifndef SERIAL_BITS_NUMBER
#define SERIAL_BITS_NUMBER 8
#endif

// Parity constants
 typedef enum {
	SERIAL_PARITY_EVEN,
	SERIAL_PARITY_ODD,
	SERIAL_PARITY_NONE
} SerialParity;

struct Serial {
	Uint16 bitsNumber;
	Uint16 parityType;
	Uint16 baudrate;
	bool fifoEnabled;
	bool rxEnabled;
	bool txEnabled;

	void (*clear)();
	Uint16 (*rxBufferStatus)();
	void (*setSerialRxEnabled)(Serial *self, bool status);
	void (*setSerialTxEnabled)(Serial *self, bool status);
	void (*init)(Serial *self);
	void (*transmitData)(Uint16 * data, Uint16 size);
	Uint16 (*getRxBufferedWord)();
};

void serial_clear();
Uint16 serial_rxBufferStatus();
void serial_setSerialRxEnabled(Serial *self, bool status);
void serial_setSerialTxEnabled(Serial *self, bool status);
void serial_init(Serial *self);
void serial_transmitData(Uint16 * data, Uint16 size);
Uint16 serial_getRxBufferedWord();
Serial construct_Serial();

#endif
