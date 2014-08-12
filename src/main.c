#include "ModbusSlave.h"
#include "Modbus.h"
#include "stdlib.h"
#include "stdio.h"

int main(){
	ModbusSlave mb = construct_ModbusSlave();
	int i = 0;
	int size = 0;

	/*_DEBUG("dataRequest size: %d", mb.dataRequest.size(&mb.dataRequest));
	mb.dataRequest.slaveAddress = 10;
	_DEBUG("dataRequest.dataAddress: %d", mb.dataRequest.slaveAddress);
	mb.loopStates(&mb);*/
	
	/*mb.dataResponse.slaveAddress = 0x11;
	mb.dataResponse.functionCode = 0x03;
	mb.dataResponse.numberOfBytes = 0x0002;
	mb.dataResponse.content = 0x0003;
	mb.dataResponse.crc = 0x7687;*/

	mb.dataRequest.slaveAddress = 0x11;
	mb.dataRequest.functionCode = 0x03;
	mb.dataRequest.firstDataAddress = 0x006B;
	mb.dataRequest.totalDataRequested = 0x0003;
	mb.dataRequest.crc = 0x7687;

	mb.state = MB_PROCESS;
	mb.loopStates(&mb);
	
	/*size = mb.dataRequest.size(&mb.dataRequest);
	for (i=0; i < size; i++){
		Uint16 * string = mb.dataRequest.getReceivedString(&mb.dataRequest);
		printf("%x ",*(string + i));
	}*/

	getchar();
	return 0;
}