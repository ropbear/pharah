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
//Doubles are safe in this case, as precision is only necessary in the tenth's place
double deg (double rad);
double rad (double deg);
double altcalc (double altA, double altB);
double latcalc (double latA, double latB);
double haversine (double latA, double longA, double latB, double longB);
double diffx (double dxz, double dz);
double diffy (double dxz, double dy);
struct APRS_tuple angcalc(double latA, double longA, double altA, double latB, double longB, double altB);

//FUNCTIONS
//Convert from radians to degrees
double deg (double rad)
{
	return rad * (180.0 / M_PI);
}

//Convert from degrees to radians
double rad (double deg)
{
	return deg * (M_PI / 180.0);
}

//Calculate the difference in altitude
double altcalc (double altA, double altB)
{
	return abs(altA-altB);
}

//Calculate the difference in latitude
double latcalc (double latA, double latB)
{
	double deg = latB - latA;
	return LATDIST * deg;
}

//Calculate the haversine distance, the straight-line distance over the curvature of the Earth
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

//Calculate the change in the X
double diffx (double dxz, double dz)
{
	return asin(dz/dxz);	
}

//Calculate the change in the Y
double diffy (double dxz, double dy)
{
	return atan(dy/dxz);
}

//Calculate the necessary angular adjustments
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

