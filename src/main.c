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
//	0xAC, 0xDB, 0x35
	mb.inputs.dummy10  = 1;
	mb.inputs.dummy11 = 1;
	mb.inputs.dummy13 = 1;
	mb.inputs.dummy15 = 1;
	mb.inputs.dummy16 = 1;
	mb.inputs.dummy17 = 1;
	mb.inputs.dummy19 = 1;
	mb.inputs.dummy20 = 1;
	mb.inputs.dummy22 = 1;
	mb.inputs.dummy23 = 1;
	mb.inputs.dummy24 = 1;
	mb.inputs.dummy26 = 1;
	mb.inputs.dummy28 = 1;
	mb.inputs.dummy29 = 1;

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
