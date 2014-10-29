#ifndef MODBUS_SERIAL_H_
#define MODBUS_SERIAL_H_

#include "PlataformSettings.h"

typedef struct Serial Serial;

// Parity constants
typedef enum {
	SERIAL_PARITY_NONE,
	SERIAL_PARITY_EVEN,
	SERIAL_PARITY_ODD
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
	bool (*getRxError)();
};

void serial_clear();
inline Uint16 serial_rxBufferStatus();
inline void serial_setSerialRxEnabled(bool status);
inline void serial_setSerialTxEnabled(bool status);
inline void serial_init(Serial *self);
inline void serial_transmitData(Uint16 * data, Uint16 size);
inline Uint16 serial_getRxBufferedWord();
inline bool serial_getRxError();
Serial construct_Serial();

#endif
