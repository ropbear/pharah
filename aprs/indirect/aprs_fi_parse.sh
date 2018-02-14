#!/bin/bash
#https://aprs.fi raw packet parser
#Purpose: Locate and parse the telemetry output from 
#https://aprs.fi/?c=raw
#in order to properly position the antenna. This allows
#for robust locating capabilities when combined with
#the direct direwolf parser.

callsign="SQ1FYB-15"
limit=1000
view="decoded"
page="raw"
url="https://aprs.fi/?c=$page&call=$callsign&limit=$limit&view=$view"

curl -s -o current_packets.html "$url"

LONG=`cat current_packets.html | grep "longitude" | head -n 1 | cut -d " " -f 2`
LAT=`cat current_packets.html | grep "latitude" | head -n 1 | cut -d " " -f 2`
ALT=`cat current_packets.html | grep "altitude" | head -n 1 | cut -d " " -f 2`
SPEED=`cat current_packets.html | grep "speed" | head -n 1 | cut -d " " -f 2`
COURSE=`cat current_packets.html | grep "course" | head -n 1 | cut -d " " -f 2`
echo -e "Latitude: ${LONG}\nLongtitude: ${LAT}\nAltitude: ${ALT}\nSpeed: ${SPEED}\nCourse: ${COURSE}"

