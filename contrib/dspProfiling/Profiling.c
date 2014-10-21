/*
 * Profiling.c
 *
 *  Created on: 24/09/2014
 *      Author: bds
 */
#include "Profiling.h"
#include "DSP2833x_Device.h"
#include "DSP2833x_CpuTimers.h"

unsigned long long profiling_getElapsedCycles(ProfilingTool * self) {
	unsigned long endTimeLow, endTimeHigh;
	long long result;

	endTimeHigh = CpuTimer2Regs.TIM.half.LSW;
	endTimeLow = CpuTimer2Regs.TIM.half.MSW;

	result = ( (self->startTimeHigh - endTimeHigh) + self->startTimeHigh*(self->startTimeLow - endTimeLow) );

	return result;
}

void profiling_start(ProfilingTool * self) {
	// Save the start values from TIM register
	// It will be used to calculate the elapsed cycles at getElapsedCycles()
	self->startTimeHigh = CpuTimer2Regs.TIM.half.LSW;
	self->startTimeLow = CpuTimer2Regs.TIM.half.MSW;

	// Start Debug Timer
	CpuTimer2Regs.TCR.bit.TSS = 0;
}

void profiling_registerStep(ProfilingTool * self, unsigned short stepIdx) {
	// Stop timer
	CpuTimer2Regs.TCR.bit.TSS = 1;

	// Regiser cycle count
	self->cycleMeasuresSteps[self->timeMeasuresIndex][stepIdx] = self->getElapsedCycles(self);

	// Reset timer
	CpuTimer2Regs.TCR.bit.TRB = 1;

	// Restart timer
	CpuTimer2Regs.TCR.bit.TSS = 0;
}

void profiling_stop(ProfilingTool * self) {
	unsigned short idx, stepIdx;

	// Stop Debug Timer
	CpuTimer2Regs.TCR.bit.TSS = 1;

	// Reset timer
	CpuTimer2Regs.TCR.bit.TRB = 1;

	// Convert STEP cycles to uSeconds and register total utilized time & cycles
#if PROFILING_EXTRA_CALCS
	for(idx = 0; idx < PROFILING_SIZE_TIME_MEASURES; idx++){
		for(stepIdx = 0; stepIdx < PROFILING_SIZE_MAX_STEPS; stepIdx++){
			self->timeMeasuresSteps[idx][stepIdx] = (self->cycleMeasuresSteps[idx][stepIdx] / PROFILING_DSPCLOCK);
			self->cycleMeasures[idx] += self->cycleMeasuresSteps[idx][stepIdx];
		}
		self->timeMeasures[idx] = self->cycleMeasures[idx] / PROFILING_DSPCLOCK;
	}
#endif

	// Reset timeMeasuresIndex
	if (self->timeMeasuresIndex >= PROFILING_SIZE_TIME_MEASURES){
		self->timeMeasuresIndex = 0;

		// Break the code to show the results (if PROFILING_BREAK_MAX_MEASURES is set)
		if(PROFILING_BREAK_MAX_MEASURES) ESTOP0;
	} else {
		self->timeMeasuresIndex++;
	}
}

ProfilingTool construct_ProfilingTool(){
	ProfilingTool profiling;
	unsigned short idx, stepIdx;

	profiling.startTimeHigh = 0;
	profiling.startTimeLow = 0;

	profiling.timeMeasuresIndex = 0;

	profiling.start = profiling_start;
	profiling.registerStep = profiling_registerStep;
	profiling.stop = profiling_stop;
	profiling.getElapsedCycles = profiling_getElapsedCycles;

	// Reset timeMeasures arrays
	for(idx = 0; idx < PROFILING_SIZE_TIME_MEASURES; idx++){
		profiling.timeMeasures[idx] = 0;
		for(stepIdx = 0; stepIdx < PROFILING_SIZE_MAX_STEPS; stepIdx++) {
			profiling.timeMeasuresSteps[idx][stepIdx] = 0;
			profiling.cycleMeasuresSteps[idx][stepIdx] = 0;
		}
	}

    // Init CPU Timer 2
    // Initialize address pointers to respective timer registers:
    CpuTimer2.RegsAddr = &CpuTimer2Regs;
    // Initialize timer period to maximum:
    CpuTimer2Regs.PRD.all  = 0xFFFFFFFF;
    // Initialize pre-scale counter to divide by 1 (SYSCLKOUT):
    CpuTimer2Regs.TPR.all  = 0;
    CpuTimer2Regs.TPRH.all = 0;
    // Make sure timer is stopped:
    CpuTimer2Regs.TCR.bit.TSS = 1;
    // Reload all counter register with period value:
    CpuTimer2Regs.TCR.bit.TRB = 1;
    // Reset interrupt counters:
    CpuTimer2.InterruptCount = 0;

	ConfigCpuTimer(&CpuTimer2, PROFILING_DSPCLOCK, 1000000);
	CpuTimer2Regs.TCR.bit.TRB = 1;

	return profiling;
}
