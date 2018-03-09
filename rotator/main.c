/*
main.c
PURPOSE:
Parse incoming APRS telemetry, and based on the calculate distance,
move the rotator to the proper position by sending commands through
the specificied COM port.
*/

#include <stdio.h>
#include <fcntl.h>
#include "angcalc.h"
#include <stdlib.h>
#include <string.h>

#define ROTATE "APn(-)xxx(.y)\r" /* where 'n' is any integer, 'xxx' is the degrees, and 'y' is the decimal value to the tenth.
																	Anything in the () is optional.*/
#define STOP "Stn ;" /* where 'n' is any integer */
#define STATUS "Bin ;" /* where 'n' is any integer */
#define COMFILE "/dev/tty0" /* File in /dev to which the console is connected */
#define APRS_BUFFSIZE 100 /* Buffer size for incoming APRS data */
#define STAT_LAT 41.390565 /* station's latitude */
#define STAT_LONG -73.955569 /* station's longtitude */
#define STAT_ALT 400.43 /* station's altitude */

//int rotate(struct APRS_tuple deg); /* function to rotate 'deg' degrees */
//int stop(); /* function to stop rotator movement */
//int status(); /* function to request status of rotator */

int main (int argc, char * argv[]) 
{
	int fd, exit_code, c, recvDATA, param, i, enabled;
	double aprsLAT,aprsLONG,aprsALT;
	char databuff[APRS_BUFFSIZE];
	char *ptr;
	struct APRS_tuple mov;

	enabled = 1;
	exit_code = 0;
	fd = open(COMFILE, O_RDWR);
	i = recvDATA = param = 0;

	while (enabled && (c = getchar()) != EOF) /* Loop until APRS breaks pipe or err */
	{
		if (recvDATA) {
			databuff[i++] = c;
		}
		else {
				memset(databuff,0,APRS_BUFFSIZE*sizeof(char));
		}
		switch (c) {
			case ':' : /* start recording input */
				recvDATA = 1;
				getchar(); /* grab ' ' character */
				i = 0;
			case '\n' : /* stop recording input */
				printf("Newline %s\n", databuff);
				switch (param) {
					case 0 :
						aprsLAT = strtod(databuff,&ptr);
					case 1 :
						aprsLONG = strtod(databuff,&ptr);
					case 2 :
						aprsALT = strtod(databuff,&ptr);
					default :
						continue;
				}
				param++; /* increment here to avoid switch statement errors */
				recvDATA = 0;
				i = 0;
			case '#' :
				printf("%f,%f,%f\n",aprsLAT,aprsLONG,aprsALT);
				mov = angcalc(STAT_LAT,STAT_LONG,STAT_ALT,aprsLAT,aprsLONG,aprsALT);
				printf("%f,%f\n",mov.degx,mov.degy);
				//rotate(mov); //TODO: Error handling
				//TODO: Status query
				enabled = 0;
			default :
				continue;
		}	
	} //TODO: Error handling
	return exit_code;
}

