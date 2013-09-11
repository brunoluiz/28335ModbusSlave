/*
 * mb_timer.h
 *
 *  Created on: 03/09/2013
 *      Author: Bruno Luiz
 */

#ifndef MB_TIMER_H_
#define MB_TIMER_H_
#include "mb_main.h"

void timer_init(Uint32 usTim1Timerout50us);
void timer_enable();
void timer_disable();
void timer_reset();
interrupt void timer_interrupt();



#endif /* MB_TIMER_H_ */
