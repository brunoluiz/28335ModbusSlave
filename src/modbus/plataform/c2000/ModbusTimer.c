#include "ModbusTimer.h"
#include "Modbus.h"
#include "Log.h"

void timer_setupInterruptions(){
	TIMER_DEBUG();
}

void timer_resetTimer(){
	TIMER_DEBUG();
}

bool timer_expiredTimer(){
	TIMER_DEBUG();
	return true;
};

void timer_setTimerReload(Timer *self, Uint16 time){
	TIMER_DEBUG();
}

void timer_setTimerEnabled(Timer *self, bool enabled){
	TIMER_DEBUG();
}

void timer_init(Timer *self){
	TIMER_DEBUG();
}

void timer_stop(){
	TIMER_DEBUG();
}

void timer_start(){
	TIMER_DEBUG();
}

Timer construct_Timer(){
	Timer timer;

	timer.timerEnabled = false;
	timer.reloadTime = 0;

	timer.setupInterruptions = timer_setupInterruptions;
	timer.resetTimer = timer_resetTimer;
	timer.expiredTimer = timer_expiredTimer;
	timer.setTimerEnabled = timer_setTimerEnabled;
	timer.setTimerReload = timer_setTimerReload;
	timer.init = timer_init;
	timer.stop = timer_stop;
	timer.start = timer_start;

	TIMER_DEBUG();

	return timer;
}
