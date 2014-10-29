#ifndef CRC_UTIL_H_
#define CRC_UTIL_H_

#ifndef bool
#define false   0
#define true    1
#define bool 	char
#endif

unsigned int generateCrc(unsigned int * buf, int len, bool swap);

#endif /* CRC_UTIL_H_ */

