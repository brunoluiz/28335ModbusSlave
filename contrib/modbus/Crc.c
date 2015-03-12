#include "Crc.h"

unsigned int generateCrc(unsigned int * buf, int len, bool swap)
{
	unsigned int crc = 0xFFFF;
	unsigned int temp = 0;
	int pos = 0;
	int i = 0;

	for (pos = 0; pos < len; pos++) {
		// XOR byte into least sig. byte of crc
		crc ^= (unsigned int) buf[pos];

		// Loop over each bit
		for (i = 8; i != 0; i--) {

			// If the LSB is set
			if ((crc & 0x0001) != 0) {
				crc >>= 1; 		// Shift right and XOR 0xA001
				crc ^= 0xA001;
			}
			else
				crc >>= 1;		// Just shift right
		}
	}

	// Swype bytes
	if(swap == true) {
		temp = crc;
		crc = (crc & 0xFF00) >> 8;
		crc = ( (temp & 0x00FF) << 8 ) | crc;
	}

	return crc;
}
