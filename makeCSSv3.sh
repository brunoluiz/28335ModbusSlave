#!/bin/bash
script=$(readlink -f "$0")
scriptPath=$(dirname "$script")
CCSv3OutputDir="CCSv3"
CopySerialTimerHeader=1

echo "## Entering at 28335ModbusSlave directory..."
cd $scriptPath
echo 

if [ -d $CCSv3OutputDir ]; then
	echo "## Destroying old $CCSv3OutputDir directory..."
	echo 
	rm -rf $CCSv3OutputDir;
fi

if [ ! -d $CCSv3OutputDir ]; then
	echo "## Creating $CCSv3OutputDir directory..."
	mkdir $CCSv3OutputDir
	mkdir $CCSv3OutputDir/Include
	mkdir $CCSv3OutputDir/Source
	echo 
fi

echo "## Copying .c files to $CCSv3OutputDir/Source..."
cp contrib/modbus/*.c $CCSv3OutputDir/Source
cp contrib/modbus/plataform/DSP2833x/*.c $CCSv3OutputDir/Source
echo

echo "## Copying .h files to $CCSv3OutputDir/Include..."
cp contrib/modbus/include/*.h $CCSv3OutputDir/Include
if [ $CopySerialTimerHeader == 1 ]; then
	cp contrib/modbus/plataform/DSP2833x/include/*.h $CCSv3OutputDir/Include
fi
echo

echo "## Done!"
