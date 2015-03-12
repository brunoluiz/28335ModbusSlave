/*
 * DataTypes.h
 *
 *  Created on: 29/10/2014
 *      Author: bds
 */

#ifndef DATATYPES_H_
#define DATATYPES_H_

#ifndef bool
#define false   0
#define true    1
#define bool 	unsigned int
#endif

#ifndef DSP28_DATA_TYPES
#define DSP28_DATA_TYPES
typedef int                int16;
typedef long               int32;
typedef long long          int64;
typedef unsigned int       Uint16;
typedef unsigned long      Uint32;
typedef unsigned long long Uint64;
typedef float              float32;
typedef long double        float64;
#endif

typedef short     		   int8;
typedef unsigned short     Uint8;

#endif /* DATATYPES_H_ */
