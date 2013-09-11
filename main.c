//###########################################################################
//
// FILE:    Example_2833xSci_Echoback.c
//
// TITLE:   DSP2833x Device SCI Echoback.
//
// ASSUMPTIONS:
//
//    This program requires the DSP2833x header files.
//    As supplied, this project is configured for "boot to SARAM" operation.
//
//    Connect the SCI-A port to a PC via a transciever and cable.
//    The PC application 'hypterterminal' can be used to view the data
//    from the SCI and to send information to the SCI.  Characters recieved
//    by the SCI port are sent back to the host.
//
//    As supplied, this project is configured for "boot to SARAM"
//    operation.  The 2833x Boot Mode table is shown below.
//    For information on configuring the boot mode of an eZdsp,
//    please refer to the documentation included with the eZdsp,
//
//       $Boot_Table:
//
//         GPIO87   GPIO86     GPIO85   GPIO84
//          XA15     XA14       XA13     XA12
//           PU       PU         PU       PU
//        ==========================================
//            1        1          1        1    Jump to Flash
//            1        1          1        0    SCI-A boot
//            1        1          0        1    SPI-A boot
//            1        1          0        0    I2C-A boot
//            1        0          1        1    eCAN-A boot
//            1        0          1        0    McBSP-A boot
//            1        0          0        1    Jump to XINTF x16
//            1        0          0        0    Jump to XINTF x32
//            0        1          1        1    Jump to OTP
//            0        1          1        0    Parallel GPIO I/O boot
//            0        1          0        1    Parallel XINTF boot
//            0        1          0        0    Jump to SARAM	    <- "boot to SARAM"
//            0        0          1        1    Branch to check boot mode
//            0        0          1        0    Boot to flash, bypass ADC cal
//            0        0          0        1    Boot to SARAM, bypass ADC cal
//            0        0          0        0    Boot to SCI-A, bypass ADC cal
//                                              Boot_Table_End$
//
// DESCRIPTION:
//
//
//    This test recieves and echo-backs data through the SCI-A port.
//
//    1) Configure hyperterminal:
//       Use the included hyperterminal configuration file SCI_96.ht.
//       To load this configuration in hyperterminal: file->open
//       and then select the SCI_96.ht file.
//    2) Check the COM port.
//       The configuration file is currently setup for COM1.
//       If this is not correct, disconnect Call->Disconnect
//       Open the File-Properties dialog and select the correct COM port.
//    3) Connect hyperterminal Call->Call
//       and then start the 2833x SCI echoback program execution.
//    4) The program will print out a greeting and then ask you to
//       enter a character which it will echo back to hyperterminal.
//
//    As is, the program configures SCI-A for 9600 baud with
//    SYSCLKOUT = 150MHz and LSPCLK = 37.5 MHz
//    SYSCLKOUT = 100MHz and LSPCLK = 25.0 Mhz
//
//
//    Watch Variables:
//       LoopCount for the number of characters sent
//       ErrorCount
//
//
//###########################################################################
// $TI Release: 2833x/2823x Header Files and Peripheral Examples V133 $
// $Release Date: June 8, 2012 $
//###########################################################################

#include "modbus/mb_main.h"
#include "mb_timer.h"
#include "mb_serial.h"

void main(void)
{
	EALLOW;
		GpioCtrlRegs.GPADIR.all = 0xFFFF;
	   // Enable GPIO outputs on GPIO8 - GPIO11, set it high
	   GpioCtrlRegs.GPAPUD.bit.GPIO8 = 0;   // Enable pullup on GPIO8
	   GpioDataRegs.GPATOGGLE.bit.GPIO8 = 1;   // Load output latch
	   GpioCtrlRegs.GPAMUX1.bit.GPIO8 = 0;  // GPIO8 = GPIO8
	   GpioCtrlRegs.GPADIR.bit.GPIO8 = 1;   // GPIO8 = output

	   GpioCtrlRegs.GPAPUD.bit.GPIO9 = 0;   // Enable pullup on GPIO9
	   GpioDataRegs.GPATOGGLE.bit.GPIO9 = 1;   // Load output latch
	   GpioCtrlRegs.GPAMUX1.bit.GPIO9 = 0;  // GPIO9 = GPIO9
	   GpioCtrlRegs.GPADIR.bit.GPIO9 = 1;   // GPIO9 = output

	   GpioCtrlRegs.GPAPUD.bit.GPIO15 = 0;  // Enable pullup on GPIO10
	   GpioDataRegs.GPATOGGLE.bit.GPIO15 = 0;  // Load output latch
	   GpioCtrlRegs.GPAMUX1.bit.GPIO15 = 0; // GPIO10 = GPIO10
	   GpioCtrlRegs.GPADIR.bit.GPIO15 = 1;   // GPIO10 = output

	   GpioCtrlRegs.GPAPUD.bit.GPIO31 = 0;  // Enable pullup on GPIO11
	   GpioCtrlRegs.GPAMUX2.bit.GPIO31 = 0; // GPIO11 = GPIO11
	   GpioCtrlRegs.GPADIR.bit.GPIO31 = 1;  // GPIO11 = output
	   EDIS;
// Step 1. Initialize System Control:
// PLL, WatchDog, enable Peripheral Clocks
// This example function is found in the DSP2833x_SysCtrl.c file.
   InitSysCtrl();

// Step 2. Initalize GPIO:
// This example function is found in the DSP2833x_Gpio.c file and
// illustrates how to set the GPIO to it's default state.
   // InitGpio(); Skipped for this example

// For this example, only init the pins for the SCI-A port.
// This function is found in the DSP2833x_Sci.c file.
   InitSciaGpio();

// Step 3. Clear all interrupts and initialize PIE vector table:
// Disable CPU interrupts
   DINT;

// Initialize PIE control registers to their default state.
// The default state is all PIE interrupts disabled and flags
// are cleared.
// This function is found in the DSP2833x_PieCtrl.c file.
   InitPieCtrl();

// Disable CPU interrupts and clear all CPU interrupt flags:
   IER = 0x0000;
   IFR = 0x0000;

// Initialize the PIE vector table with pointers to the shell Interrupt
// Service Routines (ISR).
// This will populate the entire table, even if the interrupt
// is not used in this example.  This is useful for debug purposes.
// The shell ISR routines are found in DSP2833x_DefaultIsr.c.
// This function is found in DSP2833x_PieVect.c.
   InitPieVectTable();

// Step 4. Initialize all the Device Peripherals:
// This function is found in DSP2833x_InitPeripherals.c
// InitPeripherals(); // Not required for this example

    modbus_init(9600,8,MB_PAR_NONE);
	for(;;)
    {

    }

}

