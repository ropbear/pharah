#ifndef ANGCALC_H_
#define ANGCALC_H_

extern int angcalc(double latA, double longA, double altA, double latB, double longB, double altB);

struct APRS_tuple {
	double degx;
	double degy;
};

#endif

