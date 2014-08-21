#include "ModbusSlave.h"
#include "Modbus.h"
#include "PlataformSettings.h"

ModbusSlave mb;

int main(){
	InitSysCtrl();
	InitSciaGpio();

	mb = construct_ModbusSlave();

	while(1) {
		mb.loopStates(&mb);
	}
}
