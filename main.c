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
        printf(PUT_PATTERN, key, val);
        
        if(find(root, key) == 0){
        	//printf("Main has %p\n", root);
    		insert(root, key, val);
    		
    		//trigger treebuild
    		node* current=first(root);
			Array keys;
			Array values;
			initArray(&keys, 1);
			initArray(&values, 1);
			while(true){
				//printf("%i,", current->a);
				insertArray(&keys, current->a);
				insertArray(&values, current->vala);
				//printf("%i,", current->b);
				insertArray(&keys, current->b);
				insertArray(&values, current->valb);
				current=current->p3;
				if(current->a==0 || current->b==0 || current->p3==0){
					//printf("%i\n", current->a);
					insertArray(&keys, current->a);
					insertArray(&values, current->vala);
					//printf("%i\n", current->b);
					insertArray(&keys, current->b);
					insertArray(&values, current->valb);
					break;
				}
			}
			//printf("We need %li spots\n", keys.used);	

			newtree(root, &keys, keys.used, &values);
    	}
	    else
    		update(root, key, val);
    	
    	/*treeprinter	
 		printf("                %i,%i\n", root->a, root->b);
 		printf("    %i,%i ", root->p1->a, root->p1->b);
 		printf("        %i,%i ", root->p2->a, root->p2->b);
 		printf("        %i,%i\n",root->p3->a, root->p3->b);
 		
		node* current=first(root);
		while(current!=0){
			//printf("(%p)",current);
			printf("%i,", current->a);
			printf("%i|", current->b);
			current=current->p3;
		}
        printf("\n");
        */
    }else if( sscanf(queryLine, GET_PATTERN, &key) >= 1 ) {
        // route a get query
        // TODO: hook this into your storage engine's get. b+tree's find.
        //printf("\n");
        printf(GET_PATTERN, key);
        int val=find(root, key);
        printf("Got: %i\n", val);
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

//parse route query 1 prints data nodes
int parseRouteQuery1(char queryLine[], node* root){
    if(strlen(queryLine) <= 0){
       perror("parseQuery: queryLine length is empty or malspecified.");
       return -1; 
    }else if(strlen(queryLine) < 2){
        perror("parseQuery: queryLine may be missing additional arguments.");
        return -1;
    }
    KEY_t key, lowKey, highKey;
    VAL_t val;
    char *loadPath = NULL;
    (void) loadPath;	
    if ( sscanf(queryLine, PUT_PATTERN, &key, &val) >= 1) {  
        printf(PUT_PATTERN, key, val);        
        if(find(root, key) == 0){
    		insert(root, key, val);	
    		//trigger treebuild
    		node* current=first(root);
			Array keys;
			Array values;
			initArray(&keys, 1);
			initArray(&values, 1);
			while(true){
				insertArray(&keys, current->a);
				insertArray(&values, current->vala);
				insertArray(&keys, current->b);
				insertArray(&values, current->valb);
				current=current->p3;
				if(current->a==0 || current->b==0 || current->p3==0){
					insertArray(&keys, current->a);
					insertArray(&values, current->vala);
					insertArray(&keys, current->b);
					insertArray(&values, current->valb);
					break;
				}
			}
			newtree(root, &keys, keys.used, &values);
    	}else{
    		//printf("UPDATE!");
    		update(root, key, val);
    	}
 		
 		//print
		node* current=first(root);
		while(current!=0){
			printf("%i,", current->vala);
			printf("%i|", current->valb);
			current=current->p3;
		}
        printf("\n");
        
    }else if( sscanf(queryLine, GET_PATTERN, &key) >= 1 ) {
        // route a get query
        printf(GET_PATTERN, key);
        int val=find(root, key);
        printf("Got: %i\n", val);
    }else if( sscanf(queryLine, RANGE_PATTERN, &lowKey, &highKey) >= 1 ) {
        // route a range query
		printf(RANGE_PATTERN, lowKey, highKey);
        Array rangear;
		initArray(&rangear,1);
		range(root, highKey, lowKey, &rangear);
		printf("Vals in range: ");
		for(size_t i=0;i<rangear.used;i++)
			printf("%i,", rangear.array[i]);
		printf("\n");
    }else {
        // query not parsed. handle the query as unknown
        return -1;
    }
    return 0;
}

//parse route query 2 prints a depth 3 tree
int parseRouteQuery2(char queryLine[], node* root){
    if(strlen(queryLine) <= 0){
       perror("parseQuery: queryLine length is empty or malspecified.");
       return -1; 
    }else if(strlen(queryLine) < 2){
        perror("parseQuery: queryLine may be missing additional arguments.");
        return -1;
    }
    KEY_t key, lowKey, highKey;
    VAL_t val;
    char *loadPath = NULL;
    (void) loadPath;	
    if ( sscanf(queryLine, PUT_PATTERN, &key, &val) >= 1) {  
        printf("\n");
        printf(PUT_PATTERN, key, val);        
        if(find(root, key) == 0){
    		insert(root, key, val);	
    		//trigger treebuild
    		node* current=first(root);
			Array keys;
			Array values;
			initArray(&keys, 1);
			initArray(&values, 1);
			while(true){
				insertArray(&keys, current->a);
				insertArray(&values, current->vala);
				insertArray(&keys, current->b);
				insertArray(&values, current->valb);
				current=current->p3;
				if(current->a==0 || current->b==0 || current->p3==0){
					insertArray(&keys, current->a);
					insertArray(&values, current->vala);
					insertArray(&keys, current->b);
					insertArray(&values, current->valb);
					break;
				}
			}
			newtree(root, &keys, keys.used, &values);
    	}else{
    		update(root, key, val);
    	}
 		//treeprinter	
 		printf("                             %i,%i\n", root->a, root->b);
 		printf("          %i,%i", root->p1->a, root->p1->b);
 		printf("               %i,%i", root->p2->a, root->p2->b);
 		printf("               %i,%i\n",root->p3->a, root->p3->b);	
 		printf("Data: ");
		node* current=first(root);
		while(current!=0){
			printf("%i,", current->a);
			printf("%i|", current->b);
			current=current->p3;
		}
        printf("\n");
        
    }else if( sscanf(queryLine, GET_PATTERN, &key) >= 1 ) {
        // route a get query
        printf(GET_PATTERN, key);
        int val=find(root, key);
        printf("Got: %i\n", val);
    }else if( sscanf(queryLine, RANGE_PATTERN, &lowKey, &highKey) >= 1 ) {
        // route a range query
        printf(RANGE_PATTERN, lowKey, highKey); // Stubbed print for now
    }else {
        // query not parsed. handle the query as unknown
        return -1;
    }
    return 0;
}

//parse route query 3 prints data nodes
int parseRouteQuery3(char queryLine[], node* root){
    if(strlen(queryLine) <= 0){
       perror("parseQuery: queryLine length is empty or malspecified.");
       return -1; 
    }else if(strlen(queryLine) < 2){
        perror("parseQuery: queryLine may be missing additional arguments.");
        return -1;
    }
    KEY_t key, lowKey, highKey;
    VAL_t val;
    char *loadPath = NULL;
    (void) loadPath;	
    if ( sscanf(queryLine, PUT_PATTERN, &key, &val) >= 1) {  
        printf("\n");
        printf(PUT_PATTERN, key, val);        
        if(find(root, key) == 0){
    		insert(root, key, val);	
    		//trigger treebuild
    		node* current=first(root);
			Array keys;
			Array values;
			initArray(&keys, 1);
			initArray(&values, 1);
			while(true){
				insertArray(&keys, current->a);
				insertArray(&values, current->vala);
				insertArray(&keys, current->b);
				insertArray(&values, current->valb);
				current=current->p3;
				if(current->a==0 || current->b==0 || current->p3==0){
					insertArray(&keys, current->a);
					insertArray(&values, current->vala);
					insertArray(&keys, current->b);
					insertArray(&values, current->valb);
					break;
				}
			}
			newtreeverbose(root, &keys, keys.used, &values);
    	}else{
    		update(root, key, val);
    	}
        printf("\n");
        
    }else if( sscanf(queryLine, GET_PATTERN, &key) >= 1 ) {
        // route a get query
        printf(GET_PATTERN, key);
        int val=find(root, key);
        printf("Got: %i\n", val);
    }else if( sscanf(queryLine, RANGE_PATTERN, &lowKey, &highKey) >= 1 ) {
        // route a range query
        printf(RANGE_PATTERN, lowKey, highKey); // Stubbed print for now
    }else {
        // query not parsed. handle the query as unknown
        return -1;
    }
    return 0;
}

int main(int argc, char *argv[]) { 
	int opt; 
    int queriesSourcedFromFile = 0;
    char fileReadBuffer[1023];
	//initialize tree
    node * root=newnode();
    node* current=root;
    Array initdata, initvals;
	initArray(&initdata, 6);
	initArray(&initvals, 6);
	for (int i=1;i<7;){
		insertArray(&initdata, 0);
		insertArray(&initvals, 0);
		i++;
		insertArray(&initdata, 0);
		insertArray(&initvals, 0);
		i++;
		current=current->p3;
	}				
    newtree(root, &initdata, 6, &initvals);
                
	// parse any filepath option for queries input file
	while((opt = getopt(argc, argv, ":if:lrx1234567890")) != -1){ 
		switch(opt) { 
			case 'f': 
				printf("filepath: %s\n", optarg); 
				queriesSourcedFromFile = 1;
                FILE *fp = fopen(optarg, "r");                
                while(fgets(fileReadBuffer, 1023, fp)){
                    parseRouteQuery(fileReadBuffer, root);
                }
                fclose(fp);		
                break;
                
            case '1':
            	printf("TEST 1: One\n");
            	printf("Description: First insert into the tree.\n");
            	printf("Input: p 5 5\n");
            	printf("Expected Behavior: Prints data with input left most\n");
            	printf("Execution...\n\n");
				queriesSourcedFromFile = 1;
                FILE *f1 = fopen("workload-gen/test1.txt", "r");                
                while(fgets(fileReadBuffer, 1023, f1)){
                    parseRouteQuery1(fileReadBuffer, root);
                }
                fclose(f1);		
                break;

			case '2':
            	printf("TEST 2: Very small\n");
            	printf("Description: First inserts, does not need to grow.\n");
            	printf("Input: 5 inserts, no duplicates\n");
            	printf("Expected Behavior: Prints data with inputs ordered\n");
            	printf("Execution...\n\n");
				queriesSourcedFromFile = 1;
                FILE *f2 = fopen("workload-gen/test2.txt", "r");                
                while(fgets(fileReadBuffer, 1023, f2)){
                    parseRouteQuery1(fileReadBuffer, root);
                }
                fclose(f2);		
                break;
                
            case '3':
            	printf("TEST 3: Average Case\n");
            	printf("Description: 16 inserts, tree needs to grow, not full at end\n");
            	printf("Input: 10 inserts, no duplicates\n");
            	printf("Expected Behavior: Prints tree after every insert\n");
            	printf("Execution...\n\n");
				queriesSourcedFromFile = 1;
                FILE *f3 = fopen("workload-gen/test3.txt", "r");                
                while(fgets(fileReadBuffer, 1023, f3)){
                    parseRouteQuery2(fileReadBuffer, root);
                }
                fclose(f3);		
                break;
                
             case '4':
            	printf("TEST 4: Updates\n");
            	printf("Description: Check if we're updating values\n");
            	printf("Input: 9 inserts with values same as key, 3 updates, 4 gets (3 updated, 1 not)\n");
            	printf("Expected Behavior: Values changed to key*11\n");
            	printf("Execution...\n\n");
				queriesSourcedFromFile = 1;
                FILE *f4 = fopen("workload-gen/test4.txt", "r");                
                while(fgets(fileReadBuffer, 1023, f4)){
                    parseRouteQuery1(fileReadBuffer, root);
                }
                fclose(f4);		
                break;
                
             case '5':
            	printf("TEST 5: Larger Tree\n");
            	printf("Description: Have the tree expand from its initial state twice.\n");
            	printf("Input: 20 inserts\n");
            	printf("Expected Behavior: Tree will grow to 27 data nodes, prints data nodes\n");
            	printf("Execution...\n\n");
				queriesSourcedFromFile = 1;
                FILE *f5 = fopen("workload-gen/test5.txt", "r");
                while(fgets(fileReadBuffer, 1023, f5)){
                    parseRouteQuery1(fileReadBuffer, root);
                }
                fclose(f5);
                break;
                
             case '6':
            	printf("TEST 6: Range\n");
            	printf("Description: Gets values between keys\n");
            	printf("Input: 10 inserts, 1 range request\n");
            	printf("Expected Behavior: prints values between range\n");
            	printf("Execution...\n\n");
				queriesSourcedFromFile = 1;
                FILE *f6 = fopen("workload-gen/test6.txt", "r");
                while(fgets(fileReadBuffer, 1023, f6)){
                    parseRouteQuery1(fileReadBuffer, root);
                }
                fclose(f6);
                break;
                   
             case '7':
            	printf("TEST 7: Negatives\n");
            	printf("Description: Insert negative numbers into the dataset\n");
            	printf("Input: 10 puts with negaties\n");
            	printf("Expected Behavior: Tree will sort negatives to left\n");
            	printf("Known Bug: Inserting a third negative will cause program hang. No printfs trigger, hard to trace.\n");
            	printf("Execution...\n\n");
				queriesSourcedFromFile = 1;
                FILE *f7 = fopen("workload-gen/test7.txt", "r");
                while(fgets(fileReadBuffer, 1023, f7)){
                    parseRouteQuery2(fileReadBuffer, root);
                }
                fclose(f7);
                break;
                
             case '8':
            	printf("TEST 8: Back to Back\n");
            	printf("Description: Insert a set of sequential numbers\n");
            	printf("Input: put 1-10\n");
            	printf("Expected Behavior: Tree will order numbers correctly\n");
            	printf("Execution...\n\n");
				queriesSourcedFromFile = 1;
                FILE *f8 = fopen("workload-gen/test8.txt", "r");
                while(fgets(fileReadBuffer, 1023, f8)){
                    parseRouteQuery1(fileReadBuffer, root);
                }
                fclose(f8);
                break;   
             
             case '9':
            	printf("TEST 9: Edges\n");
            	printf("Description: Insert to the front and back of a dataset\n");
            	printf("Input: put 10 random numbers between 2-98, p 1 1, p 99 99\n");
            	printf("Expected Behavior: Tree will order numbers correctly\n");
            	printf("Execution...\n\n");
				queriesSourcedFromFile = 1;
                FILE *f9 = fopen("workload-gen/test9.txt", "r");
                while(fgets(fileReadBuffer, 1023, f9)){
                    parseRouteQuery1(fileReadBuffer, root);
                }
                fclose(f9);
                break;  
                
             case '0':
            	printf("TEST 10: Very Large\n");
            	printf("Description: Insert a large dataset\n");
            	printf("Input: random puts between 1-999\n");
            	printf("Expected Behavior: Tree will scale and order numbers correctly\n");
            	printf("Known Bug: Segmentation Fault");
            	printf("Execution...\n\n");
				queriesSourcedFromFile = 1;
                FILE *f0 = fopen("workload-gen/test10.txt", "r");
                while(fgets(fileReadBuffer, 1023, f0)){
                    parseRouteQuery2(fileReadBuffer, root);
                }
                fclose(f0);
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
