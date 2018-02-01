/* 
APRS Parser
Purpose: Recieve packets from stdin and parse out the relevant
data, printing it to stdout in the form "dest,src,dgpt,data" on
each line for each packet.

References:
https://www.tapr.org/pdf/AX25.2.2.pdf
	> Provided information about bit sequence in the frame,
	> as well as the Frame Check Sequence.

*/

#include <stdio.h>
#include <sys/socket.h>
#include <string.h>
#include <stdlib.h>

#define ADDR_MAX 7
#define DGPT_MAX 56
#define DATA_MAX 258

#define FLAG 0x7e //The flag the frame begins and ends with.
#define CTLFLAG 0x03 //The Control field and Protocol ID will always be 0x03 and 0xf0
#define PIDFLAG 0xf0

void error_handle (char * error_string);
void print_packet (char * dest,char * src, char * dgpt, char * data);

int main (int argc, char * argv[]) 
{
	unsigned int c,findex;
	unsigned long throughput;
	int recv,addr,mid,info;
	char dest[ADDR_MAX];
	char src[ADDR_MAX];
	char dgpt[DGPT_MAX];
	char data[DATA_MAX];

	findex = throughput = 0;
	addr = 1;
	recv = mid = info = 0;

	while ((c = getchar()) != EOF) {
		if (1 == recv) {
			if (1 == addr) {
				if (c == CTLFLAG)
					mid = 1;
				else if (c == PIDFLAG && mid == 1) {
					addr = 0;
					info = 1;
					findex = 0;
				}
				else if (findex < DGPT_MAX + (2*ADDR_MAX)) {
					switch ((findex)/ADDR_MAX) {
						case 0 : 
							dest[findex++] = c;
							break;
						case 1 : 
							src[((findex++)%ADDR_MAX)] = c;
							break;
						default : 
							dgpt[((findex++)-(ADDR_MAX*2))] = c;
							break;
					}
					mid = 0; //make sure there wasn't a random CTLFLAG byte
				}
			}
			else if (info == 1 && findex < DATA_MAX) {
				if (c == FLAG) {
					print_packet(dest,src,dgpt,data);
					recv = 0;
					break;
				}
				data[findex] = c;
			}
			else
				error_handle("[!!] Error during listen.\n");
		}
		else if (c == FLAG) {
			recv = 1;
		}
		throughput++;
	}
	return 0;
}

void error_handle (char * error_string)
{
	printf("%s",error_string);
	exit(1);
}

void print_packet (char * dest, char * src, char * dgpt, char * data)
{
	printf("%s,%s,%s,%s\n",dest,src,dgpt,data);
}

