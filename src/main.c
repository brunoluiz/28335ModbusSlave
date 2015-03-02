#include "ModbusSlave.h"
#include "DSP2833x_Device.h"
#include "DSP2833x_GlobalPrototypes.h"

ModbusSlave mb;

int main(){
	InitSysCtrl();

	mb = construct_ModbusSlave();

//	mb.coils.dummy1 = 1;
//	mb.coils.dummy3 = 1;
//
//	mb.holdingRegisters.AmplitudeDeTensaoCa = 25.73;

#if MB_LIBMODBUS_TESTS
	mb.inputRegisters.dummy25 = 0x000A;

	mb.holdingRegisters.dummy2 = 0x01;
	mb.holdingRegisters.dummy3 = 0x01;
	mb.holdingRegisters.dummy4 = 0x01;
	mb.holdingRegisters.dummy5 = 0x01;
	mb.holdingRegisters.dummy6 = 0x01;
#endif

	while(1) {
		mb.loopStates(&mb);
	}
}
