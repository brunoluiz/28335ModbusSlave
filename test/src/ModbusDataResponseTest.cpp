#include "gtest/gtest.h"
extern "C" {
	#include <ModbusDataResponse.h>
}

namespace{

class DataResponseTest : public ::testing::Test {
protected:
	DataResponseTest(){
		mbDataResponse = construct_ModbusDataResponse();
	}

	virtual ~DataResponseTest(){
	}

	virtual void SetUp(){
		mbDataResponse.slaveAddress = 0x11;
		mbDataResponse.functionCode = 0x03;
		mbDataResponse.numberOfBytes = 0x02;
		mbDataResponse.content = 0xAE41;
		mbDataResponse.crc = 0xD7C5;
	}

	virtual void TearDown(){
	}
	ModbusDataResponse mbDataResponse;
};

TEST_F(DataResponseTest, clear){
	mbDataResponse.clear(&mbDataResponse);

	ASSERT_EQ(0, mbDataResponse.slaveAddress);
	ASSERT_EQ(0, mbDataResponse.functionCode);
	ASSERT_EQ(0, mbDataResponse.numberOfBytes);
	ASSERT_EQ(0, mbDataResponse.content);
	ASSERT_EQ(0, mbDataResponse.crc);
}


TEST_F(DataResponseTest, sizes){
	// 7 Bytes from normal frame + Number of requested bytes (this case 2 bytes)
	Uint16 responseNormalSize = 7;
	// Only 2 bytes less than the normal size
	Uint16 responseWithoutCrcSize = responseNormalSize - 2;

	ASSERT_EQ(responseNormalSize, mbDataResponse.size(&mbDataResponse));
	ASSERT_EQ(responseWithoutCrcSize, mbDataResponse.sizeWithoutCRC(&mbDataResponse));
}

TEST_F(DataResponseTest, getReceivedString){
	Uint16 * string = mbDataResponse.getTransmitString(&mbDataResponse);

	ASSERT_EQ(string[0], mbDataResponse.slaveAddress);
	ASSERT_EQ(string[1], mbDataResponse.functionCode);

	Uint16 numberOfBytes = 0;
	numberOfBytes= string[2];
	ASSERT_EQ(numberOfBytes, mbDataResponse.numberOfBytes);

	Uint16 content = 0;
	content = string[3] << 8;
	content = content | string[4];
	ASSERT_EQ(content, mbDataResponse.content);

	Uint16 crc = 0;
	crc = string[5] << 8;
	crc = crc | string[6];
	ASSERT_EQ(crc, mbDataResponse.crc);
}

}
