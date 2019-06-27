/* 
 * CS s165 A1
 * Embedded Key Value Store Client
 * Starter Code
 * 
 * By Wilson Qin <wilsonqin@seas.harvard.edu>
 */

#include <stdio.h> 
#include <stdlib.h>
#include <unistd.h> 

#include "data_types.h"
#include "storage_engine.h"

int executeQuery(STORAGECXT_t store, QUERYTYPE_t qt, KEY_t key, VAL_t val){
    (void) store;
    (void) qt;
    (void) key;
    (void) val;

    switch(qt){
        case PUT:
            break;
        case GET:
            break; 
        case RANGE:
            break;
        default:
            break;
    }

    return 0;
}

int main(int argc, char *argv[]) 
{ 
	int opt; 

    // initial command line argument parsing

    int queriesSourcedFromFile = 0;
	

    char fileReadBuffer[1023];

	// parse any filepath option for queries input file
	while((opt = getopt(argc, argv, ":if:lrx")) != -1) 
	{ 
		switch(opt) 
		{ 
			case 'f': 
				printf("filepath: %s\n", optarg); 
				queriesSourcedFromFile = 1;

                FILE *fp = fopen(optarg, "r");

                while(fgets(fileReadBuffer, 1023, fp)){
                    printf("%s\n", fileReadBuffer);
                }

                fclose(fp);


                break;
		} 
	}

    // should there be any remaining arguments not parsed by the client
	
    if(optind < argc){
        perror("Warning: extra arguments were not parsed by s165 client.");
    }

	for(; optind < argc; optind++){	 
		printf("\textra arguments: %s\n", argv[optind]); 
	} 

    (void) queriesSourcedFromFile;
	
	return 0; 
} 
