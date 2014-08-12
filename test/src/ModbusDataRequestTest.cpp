#include "gtest/gtest.h"
extern "C" {
	#include <ModbusDataRequest.h>
}

namespace{

class DataRequestTest : public ::testing::Test {
protected:
	ModbusDataRequest mbDataRequest;
	DataRequestTest(){
		mbDataRequest = construct_ModbusDataRequest();
	}

	virtual ~DataRequestTest(){

	}

	virtual void SetUp(){
		mbDataRequest.slaveAddress = 0x11;
		mbDataRequest.functionCode = 0x03;
		mbDataRequest.firstDataAddress = 0x006B;
		mbDataRequest.totalDataRequested = 0x0003;
		mbDataRequest.crc = 0x7687;
	}

	virtual void TearDown(){
	}
};

TEST_F(DataRequestTest, clear){
	mbDataRequest.clear(&mbDataRequest);

	ASSERT_EQ(0, mbDataRequest.slaveAddress);
	ASSERT_EQ(0, mbDataRequest.functionCode);
	ASSERT_EQ(0, mbDataRequest.firstDataAddress);
	ASSERT_EQ(0, mbDataRequest.totalDataRequested);
	ASSERT_EQ(0, mbDataRequest.crc);
}


TEST_F(DataRequestTest, sizes){
	Uint16 requestNormalSize = 8;
	Uint16 requestWithoutCrcSize = requestNormalSize - 2;

	ASSERT_EQ(requestNormalSize, mbDataRequest.size(&mbDataRequest));
	ASSERT_EQ(requestWithoutCrcSize, mbDataRequest.sizeWithoutCRC(&mbDataRequest));
}

TEST_F(DataRequestTest, getReceivedString){
	Uint16 * string = mbDataRequest.getReceivedString(&mbDataRequest);

	ASSERT_EQ(string[0], mbDataRequest.slaveAddress);
	ASSERT_EQ(string[1], mbDataRequest.functionCode);

	Uint16 firstDataAddress = 0;
	firstDataAddress = string[2] << 8;
	firstDataAddress = firstDataAddress | string[3];
	ASSERT_EQ(firstDataAddress, mbDataRequest.firstDataAddress);

	Uint16 totalNumber = 0;
	totalNumber = string[4] << 8;
	totalNumber = totalNumber | string[5];
	ASSERT_EQ(totalNumber, mbDataRequest.totalDataRequested);

	Uint16 crc = 0;
	crc = string[6] << 8;
	crc = crc | string[7];
	ASSERT_EQ(crc, mbDataRequest.crc);
}

}
