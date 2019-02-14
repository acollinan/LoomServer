#!/bin/bash

#give program permision initially "chmod +x LoomUI_Setup.sh"



CreateSensorData_File(){

echo Creating sensor Data file ./$4/SensorData$3.csv for $1 $2 Id: $3
#check if file already exists
echo This is the Data file for the $1 $2 ID : $3  > "./SensorFiles/$4/SensorData$3.csv"
#cat ./SensorFiles/$4/SensorData$3.csv

#push colum headers to file

}

CreateSensorConfig_File(){

echo Creating sensor config file ./$4/SensorConfig$3.txt for $1 $2 Id: $3
#check if file already exists
echo This is the configuration file for the $1 $2 ID : $3  > "./SensorFiles/$4/SensorConfig$3.txt"
#cat ./SensorFiles/$4/SensorConfig$3.txt
#--------push colum headers to file


}

CreateSensor_directory(){

#check if directory already exists
echo Creating directory Sensor$3 for $1 $2 Id: $3
mkdir "./SensorFiles/Sensor$3"


CreateSensorConfig_File $1 $2 $3 "Sensor$3" 
CreateSensorData_File $1 $2 $3 "Sensor$3" 
echo
echo Directory Tree
find SensorFiles -print | sed -e 's;[^/]*/;|____;g;s;____|; |;g'
echo
}

AddtoLISTofSensors(){

SensorList=./SensorFiles/ListOfSensors.txt
#echo $SensorList

#cat $SensorList
#cat ./SensorFiles/ListOfSensors.txt
#cat -e $SensorList | tail -n 1 #check if last line in file in newline
#while [ $(test $ = $(cat -e $SensorList | tail -n 1)) ; echo $? ] #check if last line in file in newline
#do
#sed -i '' -e '$ d' $SensorList
#done

#-----neeed to add logic if 0 sensors in file
#echo $SensorList

MaxSensorNum=$(tail -n 1 $SensorList | cut -d ' ' -f 1)
#echo MaxSensorNum: $MaxSensorNum

#do check before assignment

NewSensorNum=$(($MaxSensorNum + 1))
#echo NewSensorNum: $NewSensorNum

#echo $NewSensorNum $1 $2 $(date) 
echo $NewSensorNum $1 $2 $(date) >> $SensorList


CreateSensor_directory $1 $2 $NewSensorNum
}

#adding sensor function:
add_sensor(){

#To add additional sensors, follow all format. Find commented lines 
#that require new commands that will hell you add a new sensor to the UI

#Edit code below to add new sensor, follow format below
while :
do
echo
echo Avaliable sensors:
echo 0-Salinity sensor
echo 1-PH Sensor
echo 2-Hall-Effect Sensor
echo 3-Flow Sensor
echo Enter q to quit.
	

read -p 'Enter the sensor number: ' sensornum

echo
#Edit code below to add new sensor, follow format below
case $sensornum in 
	0)
	echo Adding Salinity sensor
	AddtoLISTofSensors Salinity Sensor
	break
	;;
	1)
	echo Adding PH Sensor
	AddtoLISTofSensors PH Sensor
	break
	;;
	2)
	echo Adding Hall-Effect Sensor
	AddtoLISTofSensors Hall-Effect Sensor
	break
	;;
	3)
	echo Adding Flow Sensor
	AddtoLISTofSensors Flow Sensor
	break
	;;
	q)
	echo Quitting
	exit 0
	break
	;;
	*)
	echo invalid input
	;;
esac
done

}


#removing sensor function
remove_sensor(){
cat ./SensorFiles/ListOfSensors.txt
echo Enter q to quit.

read -p 'From list of sensors above, enter sensor ID to remove: ' sensorid

if [ "$sensorid" = "q" ]
then
	echo quiting
	clear
	exit 0 
fi
if [ "$sensorid" = "111000" ]
then
	echo This is not a sensor that can be deleted.
elif [ -d "./SensorFiles/Sensor$sensorid" ]
then
	echo Are you sure you want to delete the following files:
	find ./SensorFiles/Sensor$sensorid -print | sed -e 's;[^/]*/;|____;g;s;____|; |;g'
	read -p 'Enter y or n: ' userinys
	if [ "$userinys" = "y" ]
	then
		sed -i '' "/$sensorid/d" ./SensorFiles/ListOfSensors.txt
		rm -fr ./SensorFiles/Sensor$sensorid
		clear
	elif [ "$useinys" = "n" ]
	then
		echo
		clear 	
	else
		echo Error:Invalid input.
	fi
		
else
	echo Error: Invalid Sensor id
fi



}



while :
do
echo 
echo Tasks:
echo 0-add sensor
echo 1-remove sensor
echo Enter q to quit.

read -p 'Enter the number for Task: ' userin
echo ${#userin} #number of argument entered

clear

if [ "$userin" = "0" ]
then
	echo add sensor
	add_sensor
elif [ "$userin" = "1" ]
then
	echo remove sensor
	remove_sensor
elif [ "$userin" = "q" ]
then
	echo quit
	exit 0
else
	echo invalid input
fi

done

