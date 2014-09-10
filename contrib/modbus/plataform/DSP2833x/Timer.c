#include "PlataformSettings.h"
#include "DSP2833x_CpuTimers.h"
#include "Timer.h"
#include "Log.h"

void timer_resetTimer(){
	ReloadCpuTimer0();
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
	// Initialize the Device Peripheral. This function can be found in DSP2833x_CpuTimers.c
	InitCpuTimers();

	// Config the timer reload period
	self->setTimerReloadPeriod(self, time);

	// If needed, you can set interruptions and other things here

	TIMER_DEBUG();
}

void timer_stop(){
	StopCpuTimer0();
	TIMER_DEBUG();
}

void timer_start(){
	StartCpuTimer0();
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
