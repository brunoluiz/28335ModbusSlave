#include "ModbusSlave.h"
#include "Modbus.h"

int main(){
	ModbusSlave mb = construct_ModbusSlave();

	InitSysCtrl();

	while(1) {
		mb.loopStates(&mb);
	}
}
