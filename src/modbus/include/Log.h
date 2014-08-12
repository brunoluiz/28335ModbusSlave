#ifndef CRC_H_
#define CRC_H_

#define MB_DATA_REQUEST_DEBUG_ENABLED		0
#define MB_DATA_RESPONSE_DEBUG_ENABLED		0
#define MB_DATA_HANDLER_DEBUG_ENABLED		0
#define MB_SLAVE_DEBUG_ENABLED				1
#define SERIAL_DEBUG_ENABLED				1
#define TIMER_DEBUG_ENABLED					0

#define X86_DEBUG	1

#if X86_DEBUG
	#include "stdio.h"
    #define _DEBUG(fmt, ...) \
	do { fprintf(stderr, "%s():%d called! " fmt "\n", __FUNCTION__, \
		__LINE__); } \
		while (0)
#else
    #define _DEBUG(fmt, ...)
#endif

#if MB_DATA_REQUEST_DEBUG_ENABLED
	#define MB_DATA_REQUEST_DEBUG(fmt, ...) _DEBUG(fmt, __VA_ARGS__)	
#else
	#define MB_DATA_REQUEST_DEBUG(fmt, ...)
#endif

#if MB_DATA_RESPONSE_DEBUG_ENABLED
	#define MB_DATA_RESPONSE_DEBUG(fmt, ...) _DEBUG(fmt, __VA_ARGS__)	
#else
	#define MB_DATA_RESPONSE_DEBUG(fmt, ...)
#endif

#if MB_DATA_HANDLER_DEBUG_ENABLED
	#define MB_DATA_HANDLER_DEBUG(fmt, ...) _DEBUG(fmt, __VA_ARGS__)	
#else
	#define MB_DATA_HANDLER_DEBUG(fmt, ...)
#endif

#if SERIAL_DEBUG_ENABLED
	#define SERIAL_DEBUG(fmt, ...) _DEBUG(fmt, __VA_ARGS__)	
#else
	#define SERIAL_DEBUG(fmt, ...)
#endif

#if MB_SLAVE_DEBUG_ENABLED
	#define MB_SLAVE_DEBUG(fmt, ...) _DEBUG(fmt, __VA_ARGS__)	
#else
	#define MB_SLAVE_DEBUG(fmt, ...)
#endif

#if TIMER_DEBUG_ENABLED
	#define TIMER_DEBUG(fmt, ...) _DEBUG(fmt, __VA_ARGS__)	
#else
	#define TIMER_DEBUG(fmt, ...)
#endif

#endif /* CRC_H_ */
