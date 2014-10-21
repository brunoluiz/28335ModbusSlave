#include "ModbusSlave.h"
ModbusSlave mb;

int main(){
	InitSysCtrl();

	mb = construct_ModbusSlave();

	mb.coils.dummy1 = 1;
	mb.coils.dummy5 = 1;
	mb.coils.dummy10 = 1;
	mb.coils.dummy15 = 1;

	mb.holdingRegisters.dummy1 = 20.2;
	mb.holdingRegisters.dummy3 = 20.2;

	while(1) {
		mb.loopStates(&mb);
	}
}
