/*
cmdlib.c
PURPOSE: This file builds the mapping of necessary commands to control
the rotator. It takes as input the mapping for methods
to model-specific commands, and assigns each of these to a
standard, callable function.

REFERENCES:
https://www.cprogramming.com/tutorial/cfileio.html
*/

//IMPORTS
#include <stdio.h>

//FUNCTIONS
int rotate(double deg);
int stop();
int status();

//MAIN
int main (int argc, char * argv) 
{
	if ( argc < 2) {
		printf("Usage: %s <rotate_cmd> <stop_cmd> <status_cmd>\n", argv[0]);	
	}

}
