#ifndef TIMER_H_
#define TIMER_H_

#include "Modbus.h"

typedef struct Timer Timer;

struct Timer {
	Uint16 reloadTime;
	bool timerEnabled;

	void (*setupInterruptions)();
	void (*resetTimer)();
	bool (*expiredTimer)();
	void (*setTimerEnabled)(Timer *self, bool enabled);
	void (*setTimerReload)(Timer *self, Uint16 time);
	void (*init)(Timer *self);
	void (*stop)();
	void (*start)();
};

void timer_setupInterruptions();
void timer_resetTimer();
bool timer_expiredTimer();
void timer_setTimerEnabled(Timer *self, bool enabled);
void timer_setTimerReload(Timer *self, Uint16 time);
void timer_init(Timer *self);
void timer_stop();
void timer_start();

Timer construct_Timer();

#endif