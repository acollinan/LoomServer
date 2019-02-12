#!/bin/bash

#give program permision initially "chmod +x LoomUI_Setup.sh"



CreateSensorData_File(){

echo
echo Creating sensor Data file SensorData$3.csv for $1 $2 Id: $3
#check if file already exists
 echo This is the Data file for the $1 $2 ID : $3  > "./Sensors/$4/SensorData$3.csv"
#push colum headers to file

}

CreateSensorConfig_File(){

echo
echo Creating sensor config file SensorConfig$3.txt for $1 $2 Id: $3
#check if file already exists
 echo This is the configuration file for the $1 $2 ID : $3  > "./Sensors/$4/SensorConfig$3.txt"
#push colum headers to file


}

CreateSensor_directory(){

#check if directory already exists
echo
echo Creating directory Sensor$3 for $1 $2 Id: $3
mkdir "./SensorFiles/Sensor$3"

CreateSensorConfig_File $1 $2 $3 "Sensor$3" 
CreateSensordata_File $1 $2 $3 "Sensor$3" 
}

AddtoLISTofSensors(){

SensorList=./SensorFiles/ListOfSensors.txt
echo $SensorList

echo In list sensors
cat $SensorList
#cat ./SensorFiles/ListOfSensors.txt
cat -e $SensorList | tail -n 1 #check if last line in file in newline

while [ $(test $ = $(cat -e $SensorList | tail -n 1)) ; echo $? ] #check if last line in file in newline
do
sed -i '' -e '$ d' $SensorList
done

MaxSensorNum=$(tail -n 1 $Sensorlist | cut -d ' ' -f 1)
echo MaxSensorNum: $MaxSensorNum

#do check before assignment

NewSensorNum=$($MaxSensorNum+1)
echo NewSensorNum: $MaxSensorNum
echo $NewSensorNum $1 $2 $(date) >> $Sensorlist

CreateSensor_directory $1 $2 $NewSensorNum
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
	AddtoLISTofSensors Salinity sensor
	;;
	1)
	echo Adding PH Sensor
	AddtoLISTofSensors PH sensor
	;;
	2)
	echo Adding Hall Effect Sensor
	AddtoLISTofSensors PH sensor
	;;
	3)
	echo Adding Flow Meter
	AddtoLISTofSensors Flow Sensor
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

