#include "ModbusSlave.h"
#include "stdio.h"
ModbusSlave mb;

int main(){
	InitSysCtrl();

	mb = construct_ModbusSlave();

	mb.coils.dummy1 = 1;
	mb.coils.dummy5 = 1;

	mb.registers.dummy1 = 20.2;
	mb.registers.dummy3 = 20.2;

	while(1) {
		mb.loopStates(&mb);
	}
}
