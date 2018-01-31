/* 
APRS Parser
Purpose: Built to take packets recieved by a station and
output the different data fields in a CSV format for use
in other parts of the Pharah program.

References:
https://www.tapr.org/pdf/AX25.2.2.pdf
	> Provided information about bit sequence in the frame.

*/

#include <stdio.h>
#include <sys/socket.h>
#include <string.h>
#include <stdlib.h>

#define MAXSIZE 500

//subtract 1 from each field for actual size. Spacing for null byte.
#define HDFLG 2
#define DEST 8
#define SRC 8
#define DGPT 57
#define CTLFLD 2
#define PROTOID 2
#define INFOFLD 257
#define FCS 3
#define TLFLG 2
#define FIELDS 9

void error_handle (char * error_string);
void print_packet(void);

int main (int argc, char * argv[]) 
{
	char head_flag[HDFLG]; //bit sequence that separates each frame by 0x7e
	char dest[DEST]; //APRS destination callsign
	char src[SRC]; //APRS source callsign
	char digipeater[DGPT]; //zero to 8 digipeater callsigns
	char ctl_fld[CTLFLD]; //set to 0x03 for UI frame in ax25 protocol
	char proto_ID[PROTOID]; //set to 0xf0 (no layer 3 protocol)
	char info_fld[INFOFLD]; //APRS data, first byte identifies data type
	char fcs[FCS]; //sequence of 16 bits for checking integrity
	char tail_flag[TLFLG]; //same as the head_flag (0x7e)
	int i,c,offset1,offset2,offset3,offset4,offset5,offset6,offset7,offset8,field_num;

//Build content from buffer.
	i = 0;
	offset1 = HDFLG+DEST;
	offset2 = offset1 + SRC;
	offset3 = offset2 + DGPT;
	offset4 = offset3 + CTLFLD;
	offset5 = offset4 + PROTOID;
	offset6 = offset5 + INFOFLD;
	offset7 = offset6 + FCS;
	offset8 = offset7 + TLFLG;
	field_num = 0;

	while ((c = getchar()) != EOF) {
		if (i <= HDFLG)
			(i == HDFLG) ? (head_flag[i++] = '\0') : (head_flag[i++] = c);
		else if (i <= offset1)
			(i == offset1) ? (dest[i++] = '\0') : (dest[i++] = c);
		else if (i <= offset2)
			(i == offset2) ? (src[i++] = '\0') : (src[i++] = c);
		else if (i <= offset3)
			(i == offset3) ? (digipeater[i++] = '\0') : (digipeater[i++] = c);
		else if (i <= offset4)
			(i == offset4) ? (ctl_fld[i++] = '\0') : (ctl_fld[i++] = c);
		else if (i <= offset5)	
			(i == offset5) ? (proto_ID[i++] = '\0') : (proto_ID[i++] = c);
		else if (i <= offset6)	
			(i == offset6) ? (info_fld[i++] = '\0') : (info_fld[i++] = c);
		else if (i <= offset7)	
			(i == offset7) ? (fcs[i++] = '\0') : (fcs[i++] = c);
		else if (i <= offset8)	
			(i == offset8) ? (tail_flag[i++] = '\0') : (tail_flag[i++] = c);
		else
			break;
	}

	if (i >= MAXSIZE)
		error_handle("[!!] Invalid packet size.\n");

//Output packet.
	printf("###PACKET###\nFLAG: %s\nDest: %s\nSrc: %s\nDigipeaters: %s\nCTL: %s\nProtoID: %s\nDATA: %s\nFCS: %s\nFLAG: %s\n",
		head_flag,dest,src,digipeater,
		ctl_fld,proto_ID,
		info_fld,fcs,tail_flag
	);
	return 0;
}

void error_handle (char * error_string)
{
	printf("%s",error_string);
	exit(1);
}

