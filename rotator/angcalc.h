/*
angcalc.h
PURPOSE:
This header file exists to provide the APRS_tuple to the other files.
*/

#ifndef ANGCALC_H_
#define ANGCALC_H_

//FUNC DECLARATIONS
//Doubles are safe in this case, as precision is only necessary in the tenth's place
extern double radtodeg (double rad);
extern double degtorad (double deg);
extern double latcalc (double latA, double latB);
extern double haversine (double latA, double longA, double latB, double longB);
extern double diffx (double dxz, double dz);
extern double diffy (double dxz, double dy);
extern struct APRS_tuple angcalc(double latA, double longA, double altA, double latB, double longB, double altB);

struct APRS_tuple {
	double degx;
	double degy;
};

#endif

