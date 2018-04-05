/*
main.c
PURPOSE:
Parse incoming APRS telemetry, and based on the calculate distance,
move the rotator to the proper position by sending commands through
the specificied COM port.
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <fcntl.h>
#include <unistd.h>
#include "angcalc.h"
#include "rotator.h"


#define ACOM "COM0" /* fd for COM port associated with azimuth control */
#define ECOM "COM1" /* fd for COM port associated with eleveation control */

/* Elevation and azimuth delta calculation globals */
#define APRS_BUFFSIZE 100 /* Buffer size for incoming APRS data */
#define STAT_LAT 41.390565 /* station's latitude */
#define STAT_LONG -73.955569 /* station's longtitude */
#define STAT_ALT 400.43 /* station's altitude */

int rotate(int afd, int efd, struct APRS_tuple deg); //function to rotate 'deg' degrees
int stop(int afd, int efd); //function to stop rotator movement
int status(int afd, int efd); //function to request status of rotator
int reset(int afd, int efd); //function to reset the rotator to its default position
//TODO make azimuth fd and elevation fd globals

int main (int argc, char * argv[]) 
{
	int exit_code, c, param, i, enabled;
	double aprsLAT,aprsLONG,aprsALT;
	char databuff[APRS_BUFFSIZE];
	char *ptr;
	struct APRS_tuple mov;

	int azimuth = open(ACOM, O_RDWR);
	int elevation = open(ECOM, O_RDWR);

	enabled = 1;
	i = exit_code = param = 0;

	memset(&databuff,0,APRS_BUFFSIZE*sizeof(char));

	while (enabled && (c = getchar()) != EOF) /* Loop until APRS breaks pipe or err */
	{
		databuff[i++] = c;
		
		switch (c) {
			case ',': /* stop recording input */
				switch (param) {
					case 0:
						aprsLAT = strtod(databuff,&ptr);
						break;
					case 1:
						aprsLONG = strtod(databuff,&ptr);
						break;
					case 2:
						aprsALT = strtod(databuff,&ptr);
						break;
					default :
						continue;
				}
				param++; /* increment here to avoid switch statement errors */
				i = 0; /* reset the index of databuff */
				memset(&databuff,0,APRS_BUFFSIZE*sizeof(char)); /* clear the buffer */
				break;

			case '\n':
				mov = angcalc(STAT_LAT,STAT_LONG,STAT_ALT,aprsLAT,aprsLONG,aprsALT);
				if(0 != rotate(azimuth,elevation,mov)) goto error;
				enabled = 0;

			default :
				continue;
		}	
	}
	return 0;

//TODO Implement error messages
error:
	return 1;
}

/*
The functions below must change if the model of the antenna control unit changes.
Please reference rotator.h for more information.
*/

//TODO Implement status loops before returning

int rotate(int azimuth, int elevation, struct APRS_tuple deg)
{
	/* Tuple is in form (azimuth,elevation) */
	int chars = 5; //these aren't magic numbers, they are the number of chars in the cmd
	int rotsize = 5+chars;
	char a[chars]; 
	char e[chars];
	memset(&a,0,chars*sizeof(char));
	memset(&e,0,chars*sizeof(char));

	//commands can be sent in parallel, they are two different systems
	if(-1 == write(azimuth,RC28_ROT(ftoa(deg->degx,a,RC28_PRECISION)),rotsize)) goto error;
	if(-1 == write(elevation,RC28_ROT(ftoa(deg->degy,e,RC28_PRECISION)),rotsize)) goto error;
	
	return 0;
error:
	return 1;
}

int stop(int azimuth, int elevation)
{
	if(-1 == write(azimuth,RC28_STOP,sizeof(RC28_STOP))) goto error;
	if(-1 == write(elevation,RC28_STOP,sizeof(RC28_STOP))) goto error;

	return 0;
error:
	return 1;
}
int status(int azimuth, int elevation)
{
	if(-1 == write(azimuth,RC28_STAT,sizeof(RC28_STAT))) goto error;
	if(-1 == write(elevation,RC28_STAT,sizeof(RC28_STAT))) goto error;

	return 0;
error:
	return 1;
}

int reset(int azimuth, int elevation)
{
	if(-1 == write(azimuth,RC28_RST,sizeof(RC28_RST))) goto error;
	if(-1 == write(elevation,RC28_RST,sizeof(RC28_RST))) goto error;

	return 0;
error:
	return 1;
}

