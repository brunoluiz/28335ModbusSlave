#ifndef CRC_H_
#define CRC_H_

#include "PlataformSettings.h"

// Log tool =============================
// When called it displays a message at console using Variadic feature
// To disable it set DEBUG_LOG_ENABLED = 0 at Modbus.h
#if DEBUG_LOG_ENABLED
	#include "stdio.h"
    #define SMART_DEBUG(fmt, ...) \
	do { fprintf(stderr, "%s():%d called! " fmt "\n", __FUNCTION__, \
		__LINE__); } \
		while (0)
// Original expression (found at web but don´t work in some cases)
//do { fprintf(stderr, "%s:%d:%s() ## " fmt "\n", __FILE__, \
//	__LINE__, __FUNCTION__, __VA_ARGS__); } while (0)
#else
    #define SMART_DEBUG(nonVariadicVar)
#endif


// Different logs =======================
// Each file has it´s specific Log debug. If you want to enable only some
// logs, go to Modbus.h and modify the macros XXXXX_DEBUG_ENABLED

#if MB_DATA_REQUEST_DEBUG_ENABLED && DEBUG_LOG_ENABLED
	#define MB_DATA_REQUEST_DEBUG(fmt, ...) SMART_DEBUG(fmt, __VA_ARGS__)
#else
	#define MB_DATA_REQUEST_DEBUG(nonVariadicVar)
#endif

#if MB_DATA_RESPONSE_DEBUG_ENABLED && DEBUG_LOG_ENABLED
	#define MB_DATA_RESPONSE_DEBUG(fmt, ...) SMART_DEBUG(fmt, __VA_ARGS__)
#else
	#define MB_DATA_RESPONSE_DEBUG(nonVariadicVar)
#endif

#if MB_DATA_HANDLER_DEBUG_ENABLED && DEBUG_LOG_ENABLED
	#define MB_DATA_HANDLER_DEBUG(fmt, ...) SMART_DEBUG(fmt, __VA_ARGS__)
#else
	#define MB_DATA_HANDLER_DEBUG(nonVariadicVar)
#endif

#if SERIAL_DEBUG_ENABLED && DEBUG_LOG_ENABLED
	#define SERIAL_DEBUG(fmt, ...) SMART_DEBUG(fmt, __VA_ARGS__)
#else
	#define SERIAL_DEBUG(nonVariadicVar)
#endif

#if MB_SLAVE_DEBUG_ENABLED && DEBUG_LOG_ENABLED
	#define MB_SLAVE_DEBUG(fmt, ...) SMART_DEBUG(fmt, __VA_ARGS__)
#else
	#define MB_SLAVE_DEBUG(nonVariadicVar)
#endif

#if TIMER_DEBUG_ENABLED && DEBUG_LOG_ENABLED
	#define TIMER_DEBUG(fmt, ...) SMART_DEBUG(fmt, __VA_ARGS__)
#else
	#define TIMER_DEBUG(nonVariadicVar)
#endif

#endif /* CRC_H_ */
