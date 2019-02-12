#!/bin/bash

#give program permision initially "chmod +x LoomUI_Setup.sh"

checkLISTofSensors(){

while [ test $ = $(cat -e $SensorList | tail -n 1) ]#check if last line in file in newline
do
sed '$d' $SensorList
done


}

#adding sensor function:
add_sensor(){

while :
do
echo
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
	if[ test $ = $(cat -e $SensorList | tail -n 1) ]
	;;
	2)
	echo Adding Hall Effect Sensor
	if[ test $ = $(cat -e $SensorList | tail -n 1) ]
	;;
	3)
	echo Adding Flow Meter
	if[ test $ = $(cat -e $SensorList | tail -n 1) ]	
	;;
	q)
	echo Quitting
	exit 0
	;;
	*)
	echo invalid input
	;;
esac
done

}


#removing sensor function
remove_sensor(){

echo returning sensor
}

SensorList=ListOfSensors.txt


while :
do
echo 
echo Tasks:
echo 0-add sensor
echo 1-remove sensor
echo 2-quit

read -p 'Enter the number for Task: ' userin
echo ${#userin} #number of argument entered

if [ "$userin" = "0" ]
then
	echo add sensor
	add_sensor
elif [ "$userin" = "1" ]
then
	echo remove sensor
	remove_sensor
elif [ "$userin" = "2" ]
then
	echo quit
	exit 0
else
	echo invalid input
fi

done

