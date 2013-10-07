/*
 * mb_timer.c
 *
 *  Created on: 03/09/2013
 *      Author: Bruno Luiz
 */

#include "mb_timer.h"

/*
 * Manage the timer interrupt
 * Clear the Pie/interrupt registers and flags
 * Check the actual state - which will take new actions if the state have changed
 * (for more info about the states, check the modbus_chk_states() at mb_main.c)
 * Resets the timer (putting it again to 0) and leaves it running
 */
interrupt void timer_interrupt()
{
	CpuTimer0.InterruptCount++;

	// Clear interrupt flag
    CpuTimer0Regs.TCR.bit.TIF = 1;

    // Check actual state and do the appropriate action
    modbus_chk_states();

    // Reset timer (set it to 0)
    timer_reset();

    // Clean interrupt
    PieCtrlRegs.PIEACK.all = PIEACK_GROUP1;
}

/*
 * Enable timer by:
 * Resetting it, enabling the interrupt and starting the counter
 */
void timer_enable()
{
	timer_reset();
    CpuTimer0Regs.TCR.bit.TIE = 1;	//Enable interrupt
	CpuTimer0Regs.TCR.bit.TSS = 0;	//Start counter
}
/*
 * Reset the timer
 */
void timer_reset()
{
	CpuTimer0Regs.TCR.bit.TRB = 1;	//Copy values to the counter
}
/*
 * Disable timer by:
 * Disabling interrupt and stopping the counter
 */
void timer_disable()
{
	CpuTimer0Regs.TCR.bit.TIE = 0;	//Disable interrupt
	CpuTimer0Regs.TCR.bit.TSS = 1;	//Stop counter
}

/*
 * Initialize the timer and starts it
 * usTim1Timerout50us: the time of 3.5 char for this system (depends on baud rate)
 */
void timer_init(Uint32 usTim1Timerout50us)
{
	Uint64 ulReloadValue = 0;

	EALLOW;  // This is needed to write to EALLOW protected registers
	// Maps the interrupt function to Pie Table
	PieVectTable.TINT0 = &timer_interrupt;
	EDIS;    // This is needed to disable write to EALLOW protected registers

	// Initialize the Device Peripheral. This function can be found in DSP2833x_CpuTimers.c
	InitCpuTimers();
	// Configure the reload time, based on given parameter
	ulReloadValue = ( usTim1Timerout50us * 1000000UL) / 20000UL;

	#if (CPU_FRQ_150MHZ)
	// Configure CPU-Timer 0 to interrupt every second:
	// 150MHz CPU Freq, 1 second Period (in uSeconds)
	ConfigCpuTimer(&CpuTimer0, 150, ulReloadValue);
	#endif

	#if (CPU_FRQ_100MHZ)
	// Configure CPU-Timer 0 to interrupt every second:
	// 100MHz CPU Freq, 1 second Period (in uSeconds)
	ConfigCpuTimer(&CpuTimer0, 100, usTim1Timerout50us);
	#endif

	// Enable CPU int1 which is connected to CPU-Timer 0
	IER |= M_INT1;

	// Enable TINT0 in the PIE: Group 1 interrupt 7
	PieCtrlRegs.PIEIER1.bit.INTx7 = 1;

	// Enable global Interrupts and higher priority real-time debug events:
	EINT;   // Enable Global interrupt INTM
	ERTM;   // Enable Global realtime interrupt DBGM

	// Enables the timer by starting the counter
	timer_enable();
}
