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
#define RADIUS 6371000 //meters
#define LATDIST 111133 //meters

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
double latcalc (double latA, double latB)
{
	double deg = latB - latA;
	return LATDIST * deg;
}

//Calculate the haversine distance, the straight-line distance over the curvature of the Earth
double haversine (double latA, double longA, double latB, double longB) //RADIANS
{
	double phi1, phi2, dphi, dlambda, a, c;
	
	phi1 = degtorad(latA);
	phi2 = degtorad(latB);
	dphi = degtorad(latB-latA);
	dlambda = degtorad(longB-longA);

	a = (sin(dphi/2) * sin(dphi/2)) + (cos(phi1) * cos(phi2) * sin(dlambda/2) * sin(dlambda/2));
	c = 2 * atan2(sqrt(a),sqrt(1-a));
	return RADIUS * c;
}

//Calculate the change in the X
double diffx (double dxz, double dz) //RADIANS
{
	return asin(dz/dxz);	
}

//Calculate the change in the Y
double diffy (double dxz, double dy) //RADIANS
{
	return atan(dy/dxz);
}

//Calculate the necessary angular adjustments
struct APRS_tuple angcalc(double latA, double longA, double altA, double latB, double longB, double altB)
{	
	struct APRS_tuple tup;
	double dxz = haversine(latA,longA,latB,longB);
	double dy = abs(altA-altB);
	double dz = latcalc(latA, latB);

	tup.degx = radtodeg(diffx(dxz,dz));
	tup.degy = radtodeg(diffy(dxz,dy));
	return tup; //DEGREES
}

