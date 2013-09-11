/*
 * mb_timer.c
 *
 *  Created on: 03/09/2013
 *      Author: Bruno Luiz
 */

#include "mb_timer.h"

interrupt void timer_interrupt()
{
	CpuTimer0.InterruptCount++;

	// Clear interrupt flag
    CpuTimer0Regs.TCR.bit.TIF = 1;

    // Check actual state and do the appropriate action
    modbus_chk_states();

    timer_reset();
    PieCtrlRegs.PIEACK.all = PIEACK_GROUP1;
}

void timer_enable()
{
	timer_reset();
    CpuTimer0Regs.TCR.bit.TIE = 1;	//Enable interrupt
	CpuTimer0Regs.TCR.bit.TSS = 0;	//Start counter
}
void timer_reset()
{
	CpuTimer0Regs.TCR.bit.TRB = 1;	//Copy values to the counter
}

void timer_disable()
{
	CpuTimer0Regs.TCR.bit.TIE = 0;	//Disable interrupt
	CpuTimer0Regs.TCR.bit.TSS = 1;	//Stop counter
}

void timer_init(Uint32 usTim1Timerout50us)
{
	Uint64 ulReloadValue = 0;
	// Interrupts that are used in this example are re-mapped to
	// ISR functions found within this file.
	EALLOW;  // This is needed to write to EALLOW protected registers
	PieVectTable.TINT0 = &timer_interrupt;
	EDIS;    // This is needed to disable write to EALLOW protected registers

	// Initialize the Device Peripheral. This function can be found in DSP2833x_CpuTimers.c
	InitCpuTimers();
	ulReloadValue = ( usTim1Timerout50us * 1000000UL) / 20000UL;
	//ulReloadValue = 1000000;

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

	// To ensure precise timing, use write-only instructions to write to the entire register. Therefore, if any
	// of the configuration bits are changed in ConfigCpuTimer and InitCpuTimers (in DSP2833x_CpuTimers.h), the
	// below settings must also be updated.
	//CpuTimer0Regs.TCR.all = 0x4009;	// Use write-only instruction to set TSS bit = 1
	//usar CpuTimer0Regs.TCR.all = 0xC011;

	// Enable CPU int1 which is connected to CPU-Timer 0
	IER |= M_INT1;
	//IER |= M_INT13;
	//IER |= M_INT14;

	// Enable TINT0 in the PIE: Group 1 interrupt 7
	PieCtrlRegs.PIEIER1.bit.INTx7 = 1;

	// Enable global Interrupts and higher priority real-time debug events:
	EINT;   // Enable Global interrupt INTM
	ERTM;   // Enable Global realtime interrupt DBGM
	timer_enable();
}
