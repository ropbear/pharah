/*
main.c
PURPOSE:
Parse incoming APRS telemetry, and based on the calculate distance,
move the rotator to the proper position by sending commands through
the specificied COM port.
*/

#include <stdio.h>
#include <fcntl.h>
#include <angcalc.h>
#include <stdlib.h>

#define ROTATE "APn(-)xxx(.y)\r" /* where 'n' is any integer, 'xxx' is the degrees, and 'y' is the decimal value to the tenth.
																	Anything in the () is optional.*/
#define STOP "Stn ;" /* where 'n' is any integer */
#define STATUS "Bin ;" /* where 'n' is any integer */
#define COMFILE "tty0" /* File in /dev to which the console is connected */
#define APRS_BUFFSIZE 100 /* Buffer size for incoming APRS data */
#define STAT_LAT 43.333 /* station's latitude */
#define STAT_LONG 102.222 /* station's longtitude */
#define STAT_ALT 400.43 /* station's altitude */

int rotate(struct APRS_tuple deg); /* function to rotate 'deg' degrees */
int stop(); /* function to stop rotator movement */
int status(); /* function to request status of rotator */

int main (int argc, char * argv) 
{
	int fd, exit_code, c, recvDATA, recvNAME, param, i;
	double aprsLAT,aprsLONG,aprsALT;
	char databuff[APRS_BUFFSIZE];

	exit_code = 0;
	fd = open("/dev/"+COMFILE, O_RDWR);
	i = param = 0;

	while ((c = getchar()) != EOF) /* Loop until APRS breaks pipe or err */
	{
		if (recvDATA) {
			databuff[i++] = c;
		}
		switch (c) {
			case ':' : /* start recording input */
				recvDATA = 1;
				recvNAME = 0;
				getchar(); /* grab ' ' character */
				i = 0;
			case '\n' : /* stop recording input */
				switch (param) {
					case 0 :
						aprsLAT = strtod(databuff);
					case 1 :
						aprsLONG = strtod(databuff);
					case 2 :
						aprsALT = strtod(databuff);
					default : /* add other cases n > 3 for speed, bearing, etc. */
						continue;
				}
				param++; /* increment here to avoid switch statement errors */
				recvNAME = 1;
				recvDATA = 0;
				memset(&databuff,'\0',APRS_BUFFSIZE*sizeof(char));
				i = 0;
			case '#' :
				mov = angcalc(STAT_LAT,STAT_LONG,STAT_ALT,aprsLAT,aprsLONG,aprsALT);
				rotate(mov); //TODO: Error handling
				//TODO: Status query
				param = 0;
			default
				continue;
		}	
	} //TODO: Error handling
	return exit_code;
}

