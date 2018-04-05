/*
rotator.h
PURPOSE:
Define commands for different models of antenna control units.
The number of models is limited to the amount the project is used on, I will not
be going out of my way to add models I will not be using. Please follow the format
I used for the M2 RC2800PRKX2SU.
*/

/*
M2 Antenna Systems Inc.
RC2800PRKX2SU
Prefix: RC28
*/

#define RC28_ROT(X) ("AP0" #X "\r;") // 'X' Is a number between -360.0 and 360.0
#define RC28_STOP "St0 ;"
#define RC28_STAT "Bi0 ;"
#define RC28_RST "W00 ;"
#define RC28_PRECISION 1 // How many decimal places the rotate command can interpret

