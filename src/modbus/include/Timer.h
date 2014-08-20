#ifndef MODBUS_TIMER_H_
#define MODBUS_TIMER_H_

#include "Modbus.h"

typedef struct Timer Timer;

struct Timer {
	Uint32 reloadTime;
	bool timerEnabled;

	void (*setupInterruptions)();
	void (*resetTimer)();
	bool (*expiredTimer)(Timer *self);
	void (*setTimerReloadPeriod)(Timer *self, Uint32 time);
	void (*init)(Timer *self, Uint32 time);
	void (*stop)();
	void (*start)();
};

void timer_setupInterruptions();
void timer_resetTimer();
bool timer_expiredTimer(Timer *self);
void timer_setTimerReloadPeriod(Timer *self, Uint32 time);
void timer_init(Timer *self, Uint32 time);
void timer_stop();
void timer_start();

Timer construct_Timer();

#endif
