#ifndef MODBUS_SERIAL_H_
#define MODBUS_SERIAL_H_

#include "PlataformSettings.h"

typedef struct Serial Serial;

#ifndef SERIAL_BAUDRATE
#define SERIAL_BAUDRATE 115200
#endif

#ifndef SERIAL_BITS_NUMBER
#define SERIAL_BITS_NUMBER 8
#endif

#ifndef SERIAL_PARITY
#define SERIAL_PARITY SERIAL_PARITY_NONE
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
	Uint32 baudrate;

	Uint16 fifoWaitBuffer;

	void (*clear)();
	Uint16 (*rxBufferStatus)();
	void (*setSerialRxEnabled)(bool status);
	void (*setSerialTxEnabled)(bool status);
	void (*init)(Serial *self);
	void (*transmitData)(Uint16 * data, Uint16 size);
	Uint16 (*getRxBufferedWord)();
};

void serial_clear();
inline Uint16 serial_rxBufferStatus();
inline void serial_setSerialRxEnabled(bool status);
inline void serial_setSerialTxEnabled(bool status);
void serial_init(Serial *self);
void serial_transmitData(Uint16 * data, Uint16 size);
inline Uint16 serial_getRxBufferedWord();
Serial construct_Serial();

#endif
