/*
 * mb_timer.h
 *
 *  Created on: 03/09/2013
 *      Author: Bruno Luiz
 */

#ifndef MB_TIMER_H_
#define MB_TIMER_H_
#include "mb_main.h"

void timer_init(Uint32 usTim1Timerout50us);	// Init the timer
void timer_enable();						// Enable the timer (interrupts and start counter)
void timer_disable();						// Disable the timer (stop interruptions and stop counter)
void timer_reset();							// Reset the timer to 0
interrupt void timer_interrupt();			// Manage the interrupt



#endif /* MB_TIMER_H_ */
