#ifndef PROFILING_H_
#define PROFILING_H_

#define PROFILING_SIZE_TIME_MEASURES	5
#define PROFILING_SIZE_MAX_STEPS		5
#define PROFILING_DSPCLOCK 				150U // 150 Mhz
#define PROFILING_EXTRA_CALCS			1	// true
#define PROFILING_BREAK_MAX_MEASURES	1	// true

typedef struct ProfilingTool ProfilingTool;

struct ProfilingTool {
	unsigned long long cycleMeasuresSteps[PROFILING_SIZE_TIME_MEASURES][PROFILING_SIZE_MAX_STEPS];
	long double timeMeasuresSteps[PROFILING_SIZE_TIME_MEASURES][PROFILING_SIZE_MAX_STEPS];

	unsigned long startTimeHigh;
	unsigned long startTimeLow;

	unsigned int timeMeasuresIndex;

	void (*start)(ProfilingTool * self);
	void (*registerStep)(ProfilingTool * self, unsigned short idx);
	void (*stop)(ProfilingTool * self);
	unsigned long long (*getElapsedCycles)(ProfilingTool * self);
};

inline unsigned long long profiling_getElapsedCycles(ProfilingTool * self);
inline void profiling_start(ProfilingTool * self);
inline void profiling_stop(ProfilingTool * self);
inline void profiling_registerStep(ProfilingTool * self, unsigned short idx);
ProfilingTool construct_ProfilingTool();

#endif
