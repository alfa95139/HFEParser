/*
Mirage Disk 
tracks  | Mem for sectors 0 to 4 |Mem for Sector 5 | Total memory
80	|	1024 * 5 	 | 512	           |    450560    == 440K

HFE Format
More than 2M
*/

#include <stdio.h>
#include <stdlib.h>
#include "./hfe.h"


int main () {
short int i,j,k,m;
int blocksPerTrack;
int padding;
FILE *fptr;
struct picfileformatheader_ hfeHeader;
struct pictrack_ hfeTrackTable[80];
unsigned char LASTGARBAGE[192];

struct trackdatablock_ {
	unsigned char side0[256];
	unsigned char side1[256];	
	} trackdatablock;

unsigned char EVENMOREGARBAGE[80];

printf("Hello World \n");

fptr = fopen("./Classical 10.hfe","r");

if(fptr == NULL)
   {
      printf("No such file");   
      exit(1);             
   }

fread( &hfeHeader, sizeof(hfeHeader), 1, fptr);

printf("%s\n",hfeHeader.HEADERSIGNATURE);
printf("Format Revision:  %d\n", (unsigned char) hfeHeader.formatrevision);
printf("Number of tracks: %d\n", (unsigned char) hfeHeader.number_of_track);          // Number of track in the file
printf("Number of sides:  %d\n", (unsigned char) hfeHeader.number_of_side);           // Number of valid side (Not used by the emulator)
printf("Track Encoding:   %02x\n",(unsigned char) hfeHeader.track_encoding);           // Track Encoding mode
printf("Bitrate in kbit/sec: %d\n",(unsigned short) hfeHeader.bitRate);                 // Bitrate in Kbit/s. Ex : 250=250000bits/s
printf("Rotation per minute: %d\n", (unsigned short) hfeHeader.floppyRPM);               // Rotation per minute  (Not used by the emulator)
printf("Floppy Interface mode: %02X\n", (unsigned char) hfeHeader.floppyinterfacemode);      // Floppy interface mode. (Please see the list above.)
printf("Dunno: %02x\n", (unsigned char) hfeHeader.dnu);                      // Free
printf("Offset of track list LUT (blocks of 512): %d\n", (unsigned short) hfeHeader.track_list_offset);       // Offset of the track list LUT in block of 512 bytes
printf("Write Protection: %02x\n",(unsigned char)hfeHeader.write_allowed);            // The Floppy image is write protected ?
printf(" 0xFF : Single Step – 0x00 Double Step mode %x\n", (unsigned char) hfeHeader.single_step);              // 0xFF : Single Step – 0x00 Double Step mode
printf("(0x00 is alternate track encoding for track 0 side 0): %d\n", (unsigned char) hfeHeader.track0s0_altencoding);     // 0x00 : Use an alternate track_encoding for track 0 Side 0
printf("%d\n", (unsigned char) hfeHeader.track0s0_encoding);        // alternate track_encoding for track 0 Side 0
printf("(0x00 is alternate track encoding for track 0 side 1): %d\n" , (unsigned char) hfeHeader.track0s1_altencoding);     // 0x00 : Use an alternate track_encoding for track 0 Side 0
printf("%d\n",(unsigned char) hfeHeader.track0s1_encoding);        // alternate track_encoding for track 0 Side 0

for (i=0; i<486; i++) printf("%02x ", hfeHeader.FILLTHISGARBAGE[i]);
printf("\n");

fread(&hfeTrackTable, sizeof(hfeTrackTable), 1, fptr);
for(i=0; i< hfeHeader.number_of_track; i++) {

	printf("Track %d, Offset (in blocks of 512bytes):          %d\n", i, 	(unsigned short)hfeTrackTable[i].offset);
	printf("          Length (bytes):  %d\n", 				(unsigned short)hfeTrackTable[i].track_len);
	}


printf("\n");
fread(&LASTGARBAGE, sizeof(LASTGARBAGE), 1, fptr);
for (i=0; i<sizeof(LASTGARBAGE); i++) printf("%02x ", LASTGARBAGE[i]);
printf("\n");

for(i=0; i< hfeHeader.number_of_track; i++) {
	printf("/-===========-\\\n");
	printf("Track #: %d of %d\n", i, hfeHeader.number_of_track-1);


	padding = hfeTrackTable[i].track_len % 512;
	blocksPerTrack = (hfeTrackTable[i].track_len - padding) / 512;
	printf("blocksPerTrack = %d, padding = %d\n", blocksPerTrack, padding);
	printf("/-===========-\\\n");

	for(m=0; m < blocksPerTrack; m++){
		fread(&trackdatablock,  sizeof(trackdatablock), 1, fptr); 
		k=0;
		printf("Track %d of %d\nSide Zero , block %d\n",i,hfeHeader.number_of_track-1 , m);
		for(j = 0; j<= 255; j++){ 
			k++;
			printf(" %02X", trackdatablock.side0[j]);
			if(k == 32) { printf("\n") ; k=0;}
			}
		printf("\n");
		k=0;
		printf("Side One\n");
        	for(j = 0; j<= 255; j++) {
			k++;		
                	printf(" %02X", trackdatablock.side1[j]);
			if(k == 32) { printf("\n") ; k=0;}
			}	
		printf("\n");
		}
	printf("\\-===========-/\n");
	}

 	for(i=0; i<80; i++) {
		fread(&EVENMOREGARBAGE, sizeof(EVENMOREGARBAGE), 1, fptr);
		printf("PADDING (AGAIN!!!): \n");
		k=0;
		for (m=0; m<sizeof(EVENMOREGARBAGE); m++) {
			k++;
			printf("%02x ", EVENMOREGARBAGE[m]);
			if(k == 32) { printf("\n") ; k=0;}
			}
		printf("\n");
	}

fclose(fptr);
return(0);
}
