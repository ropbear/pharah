#!/bin/bash
#Direwolf Parser
#Purpose: Locate and parse the telemetry output from the Direwolf SDR
#program in order to properly position the antenna. The packet will be
#in AX.25 UI frame format, utilizing the APRS protocol.

LINE=`grep "N "`

LONG=`echo $LINE | cut -d " " -f 2`
LAT=`echo $LINE | cut -d " " -f 4`
ALT=`echo $LINE | cut -d " " -f 6`
TEL=`echo $LINE | cut -d " " -f 8`

echo "${LONG} ${LAT} ${ALT} ${TEL}"


