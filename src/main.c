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

	while(1) {
		mb.loopStates(&mb);
	}
}
