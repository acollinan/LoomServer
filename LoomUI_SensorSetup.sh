#!/bin/bash

#give program permision initially "chmod +x LoomUI_Setup.sh"

echo Tasks:
echo 0-add sensor
echo 1-remove sensor
echo 2-quit

read -p 'Enter the number for Task: ' userin
echo ${#userin}
if [ $userin -eq 0 ]
then
	echo add sensor
	add_sensor()

elif [ $userin -eq 1 ]
then
	echo remove sensor
	remove_sensor()

elif [ $userin -eq 2 ]
then
	echo quit
	exit 0
else
	echo invalid input
fi

add_sensor(){

echo Avaliable sensors:
echo 0-Salinity sensor
echo 1-PH Sensor
echo 2-Hall Effect Sensor
echo 3-Flow Meter
echo Enter q to quit.
	
read -p 'Enter the sensor number: ' sensornum

case $sensornum in 
	0)
	echo Adding Salinity sensor
	;;
	1)
	echo Adding PH Sensor
	;;
	2)
	echo Adding Hall Effect Sensor
	;;
	3)
	echo Adding Flow Meter
	;;
	q)
	echo Quitting
	exit 0
	*)
	echo invalid input
	;;
esac



}

remove_sensor(){


}
