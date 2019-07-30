/* CS s165 A1
 * Embedded Key Value Store Client
 * Starter Code by Wilson Qin <wilsonqin@seas.harvard.edu>
 * Implementation by Andrew Pham anp6338@g.harvard.edu
 */

#include <stdio.h> 
#include <stdlib.h>
#include <unistd.h> 
#include <string.h>
#include "data_types.h"
#include "btree.h"
//#include "storage_engine.h"

//parses a query command (one line), and routes it to the corresponding storage engine methods
int parseRouteQuery(char queryLine[], node* root){
    if(strlen(queryLine) <= 0){
       perror("parseQuery: queryLine length is empty or malspecified.");
       return -1; 
    }else if(strlen(queryLine) < 2){
        perror("parseQuery: queryLine may be missing additional arguments.");
        return -1;
    }
    KEY_t key, lowKey, highKey;
    VAL_t val;

    // finish loadPath
    // read a binary file
    // parse it into an array of keys and array of values, and a total length
    // pass those as args to a load function
    char *loadPath = NULL;
    (void) loadPath;	

    if ( sscanf(queryLine, PUT_PATTERN, &key, &val) >= 1) {  
        // route a point query
        // TODO: hook this into your storage engine's put. b+tree's insert.
        //printf("find %i\n",find(root, key));
        if(find(root, key) == 0){
        	//printf("Main has %p\n", root);
    		insert(root, key, val);
    		
    		//trigger rebuild
    		node* current=first(root);
			Array keys;
			initArray(&keys, 5);
			while(true){
				//printf("insertarray %i\n", current->a);
				insertArray(&keys, current->a);
				//printf("insertarray %i\n", current->b);
				insertArray(&keys, current->b);
				current=current->p3;
				if(current->a==0 || current->b==0 || current->p3==0)
					break;
			}
			printf("We need %li spots\n", keys.used);	

			newtree(root, &keys, keys.used);
    	}
	    else
    		update(root, key, val);
    		
    	//print
 		//printf("checker: \n");
 		//printf("root: %p\n", root);
		node* current=first(root);
		while(current!=0){
			//printf("(%p)",current);
			printf("%i,", current->a);
			printf("%i|", current->b);
			current=current->p3;
		}
		printf("\n");
		
		    		
        printf(PUT_PATTERN, key, val); // Stubbed print for now
    }else if( sscanf(queryLine, GET_PATTERN, &key) >= 1 ) {
        // route a get query
        // TODO: hook this into your storage engine's get. b+tree's find.
        key=find(root, key);
        printf(GET_PATTERN, key); // Stubbed print for now
    }else if( sscanf(queryLine, RANGE_PATTERN, &lowKey, &highKey) >= 1 ) {
        // route a range query
        // NOTE: implement this for graduate credit 
        printf(RANGE_PATTERN, lowKey, highKey); // Stubbed print for now
    }else {
        // query not parsed. handle the query as unknown
        return -1;
    }

    // fflush(stdin);

    return 0;
}

int main(int argc, char *argv[]) { 
	int opt; 
    int queriesSourcedFromFile = 0;
    char fileReadBuffer[1023];

	// parse any filepath option for queries input file
	while((opt = getopt(argc, argv, ":if:lrx")) != -1){ 
		switch(opt) { 
			case 'f': 
				printf("filepath: %s\n", optarg); 
				queriesSourcedFromFile = 1;

                FILE *fp = fopen(optarg, "r");
                
                //initialize tree
                node * root=newnode();
                
                //initial data
                node* current=root;
                Array initdata;
				initArray(&initdata, 6);
				for (int i=1;i<7;){
					insertArray(&initdata, i*2);
					i++;
					insertArray(&initdata, i*2);
					i++;
					current=current->p3;
				}
				
                newtree(root, &initdata, 6);
				
				//print
				current=root->p1;
					printf("%i,", current->a);
					printf("%i|", current->b);
					current=current->p3;
					printf("%i,", current->a);
					printf("%i|", current->b);
					current=current->p3;
					printf("%i,", current->a);
					printf("%i|", current->b);
					current=current->p3;
				printf("Tree Made!\n");
				
                while(fgets(fileReadBuffer, 1023, fp)){
                    parseRouteQuery(fileReadBuffer, root);
                }

                fclose(fp);		

                break;
            case 't':
            	//TODO INSERT TESTS HERE
            	break;
		} 
	}

    // should there be any remaining arguments not parsed by the client, 
    //  then these cases will handle control flow:
	
    if(optind < argc){
        perror("Warning: extra arguments were not parsed by s165 client.");
    }

	for(; optind < argc; optind++){	 
		printf("\textra arguments: %s\n", argv[optind]); 
	} 

    (void) queriesSourcedFromFile;
	
	return 0; 
} 
