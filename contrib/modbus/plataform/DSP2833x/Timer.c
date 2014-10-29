#include "DSP2833x_Device.h"     // DSP2833x Headerfile Include File
#include "DSP2833x_CpuTimers.h"
#include "Timer.h"
#include "Log.h"

// For CPU Frequency = 150 Mhz
#define CPU_FREQ	150
// For CPU Frequency = 100 Mhz
//#define CPU_FREQ	100

void timer_resetTimer(){
	CpuTimer0Regs.TCR.bit.TRB = 1;
	TIMER_DEBUG();
}

bool timer_expiredTimer(Timer *self){
	Uint32 timerZeroed = CpuTimer0Regs.TCR.bit.TIF;
	TIMER_DEBUG();

	if (timerZeroed == true) {
		return true;
	}
	else {
		return false;
	}
}

void timer_setTimerReloadPeriod(Timer *self, Uint32 time){
	TIMER_DEBUG();

	self->stop();
	self->reloadTime = time;

	ConfigCpuTimer(&CpuTimer0, CPU_FREQ, time);
}


void timer_init(Timer *self, Uint32 time){
	// START: GOT FROM TEXAS FILES //////////////////////////////////////
	// CPU Timer 0
    // Initialize address pointers to respective timer registers:
    CpuTimer0.RegsAddr = &CpuTimer0Regs;
    // Initialize timer period to maximum:
    CpuTimer0Regs.PRD.all  = 0xFFFFFFFF;
    // Initialize pre-scale counter to divide by 1 (SYSCLKOUT):
    CpuTimer0Regs.TPR.all  = 0;
    CpuTimer0Regs.TPRH.all = 0;
    // Make sure timer is stopped:
    CpuTimer0Regs.TCR.bit.TSS = 1;
    // Reload all counter register with period value:
    CpuTimer0Regs.TCR.bit.TRB = 1;
    // Reset interrupt counters:
    CpuTimer0.InterruptCount = 0;
    // END: GOT FROM TEXAS FILES ////////////////////////////////////////

	// Config the timer reload period
	self->reloadTime = time;
	ConfigCpuTimer(&CpuTimer0, CPU_FREQ, time);

	// If needed, you can set interruptions and other things here

	TIMER_DEBUG();
}

void timer_stop(){
	CpuTimer0Regs.TCR.bit.TSS = 1;
	TIMER_DEBUG();
}

void timer_start(){
	CpuTimer0Regs.TCR.bit.TSS = 0;
	TIMER_DEBUG();
}

Timer construct_Timer(){
	Timer timer;

	timer.timerEnabled = false;
	timer.reloadTime = 0;

	timer.resetTimer = timer_resetTimer;
	timer.expiredTimer = timer_expiredTimer;
	timer.setTimerReloadPeriod = timer_setTimerReloadPeriod;
	timer.init = timer_init;
	timer.stop = timer_stop;
	timer.start = timer_start;

	TIMER_DEBUG();

	return timer;
}
