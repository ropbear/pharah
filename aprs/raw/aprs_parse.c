/* 
APRS Parser
Purpose: Recieve packets from stdin and parse out the relevant
data, printing it to stdout in the CSV form "dest,src,dgpt,data" on
each line for each packet.

References:
https://www.tapr.org/pdf/AX25.2.2.pdf (Excerpt included in pharah/ref/)
	> Provided information about bit sequence in the frame,
	> as well as the Frame Check Sequence.

*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

//Maximum sizes of frame fields.
#define ADDR_MAX 7
#define DGPT_MAX 56
#define DATA_MAX 258

#define FLAG 0x7e //The flag-byte the frame begins and ends with.
#define CTLFLAG 0x03 //The Control field and Protocol ID will always be 0x03...
#define PIDFLAG 0xf0 // ...and 0xf0

void error_handle (char * error_string);
void print_packet (char * dest,char * src, char * dgpt, char * data);
int recieve(void);

int main (int argc, char * argv[]) 
{
	return recieve();
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

int recieve (void)
{
	//Allocate relevant fields
	char dest[ADDR_MAX];
	char src[ADDR_MAX];
	char dgpt[DGPT_MAX];
	char data[DATA_MAX];

	//Zero out the buffers to clean up any junk
	memset(dest, 0x00, ADDR_MAX);
	memset(src, 0x00, ADDR_MAX);
	memset(dgpt, 0x00, DGPT_MAX);
	memset(data, 0x00, DATA_MAX);

	int c,findex,i,throughput;
	int recv,addr,mid,info;

	findex = throughput = 0;
	addr = 1; // addr mode will be the first to be exec once a flag is found
	recv = mid = info = 0; /* RECV is high when packet data is coming in,
														MID is high when the CTLFLAG has been detected.
														INFO is high when the PIDFLAG has been detected
															and the data section can be parsed.
												 */ 

	while ((c = getchar()) != EOF) {
		if (1 == recv) {
			if (1 == addr) {
				if (c == CTLFLAG) {
					mid = 1;
				}
				else if (c == PIDFLAG && mid == 1) {
					addr = 0;
					info = 1;
					findex = 0;
				}
				else if (findex < DGPT_MAX + (2*ADDR_MAX)) {
					switch ((findex)/ADDR_MAX) {
						case 0 : 
							dest[findex++] = c; //parse the first 7 bytes to the DEST buffer
							break;
						case 1 : 
							src[((findex++)%ADDR_MAX)] = c; //next 7 go to source
							break;
						default :
							dgpt[((findex++)-(ADDR_MAX*2))] = c; //and the final 0-56 go to dgpt
							break;
					}
					mid = 0; //make sure there wasn't a random CTLFLAG byte
				}
				else {
					//reached maximum addr field index
					addr = 0;
					findex = 0;
					info = 1;
				}
			}
			else if (1 == info && findex < DATA_MAX) {
				if (c == FLAG) { //tail flag found, print packet.
					print_packet(dest,src,dgpt,data);
					recv = 0;
					break;
				}
				data[findex++] = c;
			}
			else {
				recv = 0; //reached packet size limit, not a packet. RST.
				error_handle("[!!] Error during listen.\n");
			}
		}
		else if (c == FLAG) {
			recv = 1;
		}
		throughput++;
	}
	//function returns the number of bytes recieved while running.
	return throughput;
}

