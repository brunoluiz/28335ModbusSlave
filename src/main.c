#include "PlataformSettings.h"
#include "Modbus.h"
#include "ModbusSlave.h"

ModbusSlave mb;

int main(){
	InitSysCtrl();

	mb = construct_ModbusSlave();

	while(1) {
		mb.loopStates(&mb);
	}
}
