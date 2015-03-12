@echo off
SET CCSv3OutputDir=CCSv3
SET CopySerialTimerHeader=0

echo ## Entering at 28335ModbusSlave directory...
cd %~dp0
echo.

if not exist %CCSv3OutputDir% (
	echo ## Creating %CCSv3OutputDir% directory...
	mkdir %CCSv3OutputDir%
	mkdir %CCSv3OutputDir%\Include
	mkdir %CCSv3OutputDir%\Source
	echo.
)


echo ## Copying .c files to %CCSv3OutputDir%\Source...
copy contrib\modbus\*.c %CCSv3OutputDir%\Source
copy contrib\modbus\plataform\DSP2833x\*.c %CCSv3OutputDir%\Source
echo.

echo ## Copying .h files to %CCSv3OutputDir%\Include...
copy contrib\modbus\include\*.h %CCSv3OutputDir%\Include
if %CopySerialTimerHeader%==1 (
	copy contrib\modbus\plataform\DSP2833x\include\*.h %CCSv3OutputDir%\Include
)
echo.

echo ## Done!
pause