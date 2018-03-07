/*
angcalc.c
PURPOSE: Take telemetry data, and based on the currently defined
postition of the ground station, calculate what angle in the x-y
plane and the y-z plane the rotator must move to.
*/
//INCLUDES
#include <stdio.h>
#include <math.h>
#include <stdlib.h>
#include "angcalc.h"

//GLOBALS
#define TYPE "deg"
#define RADIUS 6371000 //meters
#define LATDIST 111133 //meters

//FUNC DECLARATIONS
double deg (double rad);
double rad (double deg);
double altcalc (double altA, double altB);
double latcalc (double latA, double latB);
double haversine (double latA, double longA, double latB, double longB);
double diffx (double dxz, double dz);
double diffy (double dxz, double dy);
struct APRS_tuple angcalc(double latA, double longA, double altA, double latB, double longB, double altB);

//FUNCTIONS
double deg (double rad)
{
	return rad * (180.0 / M_PI);
}

double rad (double deg)
{
	return deg * (M_PI / 180.0);
}

double altcalc (double altA, double altB)
{
	return abs(altA-altB);
}

double latcalc (double latA, double latB)
{
	double deg = latB - latA;
	return LATDIST * deg;
}

double haversine (double latA, double longA, double latB, double longB)
{
	double phi1, phi2, dphi, dlambda, a, c;
	
	phi1 = rad(latA);
	phi2 = rad(latB);
	dphi = rad((latB-latA));
	dlambda = rad((longB-longA));

	a = (sin(dphi/2) * sin(dphi/2)) + (cos(phi1) * cos(phi2) * sin(dlambda/2) * sin(dlambda/2));
	c = 2 * atan2(sqrt(a),sqrt(1-a));
	return RADIUS * c;
}

double diffx (double dxz, double dz)
{
	return asin(dz/dxz);	
}

double diffy (double dxz, double dy)
{
	return atan(dy/dxz);
}

struct APRS_tuple angcalc(double latA, double longA, double altA, double latB, double longB, double altB)
{	
	struct APRS_tuple tup;
	double dxz = haversine(latA,longA,latB,longB);
	double dy = altcalc(altA, altB);
	double dz = latcalc(latA, latB);

	tup.degx = diffx(dxz,dz);
	tup.degy = diffy(dxz,dy);
	return tup;
}

