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
	int fd, exit_code, c, param, i, enabled;
	double aprsLAT,aprsLONG,aprsALT;
	char databuff[APRS_BUFFSIZE];
	char *ptr;
	struct APRS_tuple mov;

	enabled = 1;
	fd = open(COMFILE, O_RDWR); //TODO implement rotator communication
	i = exit_code = param = 0;

	memset(&databuff,0,APRS_BUFFSIZE*sizeof(char));

	while (enabled && (c = getchar()) != EOF) /* Loop until APRS breaks pipe or err */
	{
		databuff[i++] = c;
		
		switch (c) {
			case ',' : /* stop recording input */
				switch (param) {
					case 0 :
						aprsLAT = strtod(databuff,&ptr);
						break;
					case 1 :
						aprsLONG = strtod(databuff,&ptr);
						break;
					case 2 :
						aprsALT = strtod(databuff,&ptr);
						break;
					default :
						continue;
				}
				param++; /* increment here to avoid switch statement errors */
				i = 0; /* reset the index of databuff */
				memset(&databuff,0,APRS_BUFFSIZE*sizeof(char)); /* clear the buffer */
				break;

			case '\n' :
				mov = angcalc(STAT_LAT,STAT_LONG,STAT_ALT,aprsLAT,aprsLONG,aprsALT);
				printf("%f,%f\n",mov.degx,mov.degy); //print for testing purposes
				//rotate(mov); //TODO: Error handling
				enabled = 0;

			default :
				continue;
		}	
	}
	return exit_code;
}

