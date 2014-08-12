/*#include "gtest/gtest.h"
extern "C" {
	#include <Modbus.h>
	#include <ModbusSlave.h>
	#include <ModbusDataHandler.h>
}

namespace {

class DataHandlerTest : public ::testing::Test {
protected:

	DataHandlerTest(){
		mb = construct_ModbusSlave();
		mb.dataRequest.slaveAddress = 0x11;
		mb.dataRequest.functionCode = 0x03;
	}

	virtual ~DataHandlerTest(){

	}

	virtual void SetUp(){
	}

	virtual void TearDown(){
	}

	ModbusSlave mb;
};

TEST_F(DataHandlerTest, readRegisters){
	int * addr = 0;
	addr = (int *)0x31150000;
	*(addr) = 20;

	//mb.dataRequest.firstDataAddress = 0x1000;
	//mb.dataRequest.totalDataRequested = 0x0001;

	// mb.dataHandler.readInputRegisters(&mb);
}

}
*/
