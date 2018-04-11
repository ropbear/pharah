/*
angcalc.c
PURPOSE: Take telemetry data, and based on the currently defined
postition of the ground station, calculate what angle in the x-y
plane and the y-z plane the rotator must move to.
*/

//INCLUDES
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include "angcalc.h"

//GLOBALS
#define TYPE "deg"
/*
RADIUS: The raidus of the earth
LATCONST: The rough estimate, in the form of a constant, necessary for converting degrees of latitude to meters
*/
#define RADIUS 6371000 //meters

//FUNCTIONS
double radtodeg (double rad)
{
	return rad * (180.0 / M_PI);
}

double degtorad (double deg)
{
	return deg * (M_PI / 180.0);
}

//Calculate the change in latitude
double azimuth (double latA, double longA, double latB, double longB)
{
	/* TODO: Explanation
	Lubin (https://math.stackexchange.com/users/17760/lubin), Equation for calculating azimuth between two points, URL (version: 2014-03-20): https://math.stackexchange.com/q/719747
	*/
	double a = degtorad(fabs(90-latA));
	double b = degtorad(fabs(90-latB));
	double C = degtorad(fabs(longA-longB));
	double c = acos(cos(a)*cos(b) + sin(a)*sin(b)*cos(C));
	double A = asin(sin(a)*(sin(C)/sin(c)));
	printf("%.2f, %.2f, %.2f, %.2f, %.2f\n",a,b,C,c,A);

	/*
	Verified with https://www.fcc.gov/media/radio/distance-and-azimuths
	It was determined that if latB > latA, the value A should be subtracted from 360.
	Otherwise, it should be added from 180.
	*/

	if (latB > latA) {
		return 360-radtodeg(A);
	}
	else {
		return radtodeg(A) + 180;
	}
}

//Calculate the haversine distance, the straight-line distance over the curvature of the Earth
double haversine (double latA, double longA, double latB, double longB) //RADIANS
{
	double phi1, phi2, dphi, dlambda, a, c = 0;
	
	phi1 = degtorad(latA);
	phi2 = degtorad(latB);
	dphi = degtorad(latB-latA);
	dlambda = degtorad(longB-longA);

	a = (sin(dphi/2) * sin(dphi/2)) + (cos(phi1) * cos(phi2) * sin(dlambda/2) * sin(dlambda/2));
	c = 2 * atan2(sqrt(a),sqrt(1-a));
	return RADIUS * c;
}

//Calculate the necessary angular adjustments
struct APRS_tuple calc_location (double latA, double longA, double altA, double latB, double longB, double altB)
{	
	struct APRS_tuple tup;
	double hvr = haversine(latA,longA,latB,longB);
	double alt = abs(altA-altB);

	tup.degx = azimuth(latA,longA,latB,longB);
	tup.degy = radtodeg(atan(alt/hvr));
	return tup; //DEGREES
}

