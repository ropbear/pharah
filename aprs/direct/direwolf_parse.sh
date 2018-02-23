#!/bin/bash
#Direwolf Parser
#Purpose: Locate and parse the telemetry output from the Direwolf SDR
#program in order to properly position the antenna. The packet will be
#in AX.25 UI frame format, utilizing the APRS protocol.

LINE=`grep "N "`

LONG=`echo $LINE | cut -d "," -f 1`
LAT=`echo $LINE | cut -d "," -f 2`
ALT=`echo $LINE | cut -d "," -f 5 | cut -d " " -f 3`
TEL=`echo $LINE | cut -d "," -f 3`
COURSE=`echo $LINE | cut -d "," -f 4 | cut -d " " -f 3`

echo -e "Latitude: ${LONG}\nLongtitude:${LAT}\nAltitude: ${ALT} ft\nSpeed:${TEL}\nCourse: ${COURSE} Degrees"

