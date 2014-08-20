#include "ModbusSlave.h"
#include "Modbus.h"
#include "PlataformSettings.h"

int main(){
	ModbusSlave mb = construct_ModbusSlave();

	InitSysCtrl();
	InitSciaGpio();

	while(1) {
		mb.loopStates(&mb);
	}
}
