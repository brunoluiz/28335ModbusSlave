#ifndef PLATAFORM_SETTINGS_H_
#define PLATAFORM_SETTINGS_

#include "DSP2833x_Device.h"     // DSP2833x Headerfile Include File

// CPU Frequency (and LSPCLK) =============================
// The below values are the most common

// For CPU Frequency = 150 Mhz
#define CPU_FREQ	150
#define LSPCLK 	37500000
// For CPU Frequency = 100 Mhz
//#define CPU_FREQ	100
//#define LSPCLK 		20000000

//#define CPU_FREQ	150
//#define LSPCLK 		15000000

// Data types definitions =================================

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

#ifndef bool
	#define false   0
	#define true    1
	#define bool 	Uint16
#endif

extern void InitSci(void);
extern void InitSciGpio(void);
extern void InitSciaGpio(void);
#if DSP28_SCIB
extern void InitScibGpio(void);
#endif // endif DSP28_SCIB
#if DSP28_SCIC
extern void InitScicGpio(void);
#endif

#endif
