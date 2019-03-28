// Author: Naufer Nusran.  Aug 20, 2014
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "spinapi.h"
#define NSTOSEC 1000000000.0
#define CLOCK_FREQUENCY 500.0 
#define BNC0 0x01


char INPUT_FILE[]="turnon.txt";// the location of the file holding the sequence

int getl(char *line,int max,FILE *fp);

int main(void)
{    
    
    double clock_freq=CLOCK_FREQUENCY;
    int nbytes = 2000;
    int out;
    char *line;
    int numlines=0;
    int i;
    int start, loop;
    unsigned int flags;
    
    FILE *fp = fopen(INPUT_FILE, "r");
    if ((fp==NULL)){
         printf("Error opening turnon.txt\n");
         return(1); 
    }
    //read in all the values from the formatted file "INPUT_FILE"
    line = (char *)malloc(nbytes+1);
    // get the number of lines
    while (getl(line,nbytes,fp))
		  numlines++;
	
    // return to the beginning of the file and read in the arrays
   	fseek(fp,0,SEEK_SET);
    for (i=0;i<numlines;i++){
		getl(line,nbytes,fp);
		sscanf(line,"channel:%d \n",&out);
		// loopwidth = loopwidth+ (temp[i+1].time - temp[i].time);
	}
	// flags = BNC0 <<(unsigned int) (out-1);
	flags = (unsigned int) out;
	fclose(fp);
	printf ("Using spinapi library version %s\n", pb_get_version());
	// Check for proper initialization of PulseBlasterESR	    
    if (pb_init() != 0){
			printf("--- Error Initializing PulseBlaster ---\n");
			return(2);
		}

    // Set clock frequency
	pb_set_clock(clock_freq);	

    // program the pulse-blaster with the initial values
  	//Begin pulse program
		pb_start_programming(PULSE_PROGRAM);
		
		// Instruction 0 
		start =	pb_inst_pbonly(ON|flags,CONTINUE,0,500*ms);
        pb_inst_pbonly(0x0,BRANCH,start,100*ns); 
	// Stop the pulseblaster
			pb_inst_pbonly(0x0,STOP ,0,100*ns);		
	// End of programming registers and pulse program
	pb_stop_programming();
    // Start execution of the pulse program
    pb_start();
       
    // when done sweeping, close pulseblaster
    pb_close();    
	 // getchar();//hold windows command prompt so errors can be read
	return(0);
}

int getl(char *line,int max,FILE *fp)
{
 	if (fgets(line,max,fp) == NULL)
 	   return 0;
    else 
  	  return strlen(line);
}
