//B+tree header
//andrew pham anp6338@g.harvard.edu

#ifndef BTREE_H
#define BTREE_H

#include "data_types.h"
#include "query.h"
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

//NODE STRUCT
typedef struct node node;
struct node{
	int a,b;
	int vala,valb;
	node *p1, *p2, *p3;
	bool leaf;
};
node* newnode(){
    node * n=(node*)malloc(sizeof(node));
    return n;
}

//first and last follows pointers till end
node* first(node* n){
	if(n->p1!=0)
		return first(n->p1);
	else
		return n;
}
node* last(node* n){
	if(n->p3!=0)
		return last(n->p3);
	else
		return n;
}

//addchildren adds a full set of children to a node
void addchildren(node* n){
	node* n1=newnode();
	node* n2=newnode();
	node* n3=newnode();
	//initialize all keys to zero
	n1->a=0;
	n1->b=0;
	n2->a=0;
	n2->b=0;
	n3->a=0;
	n3->b=0;
	//point parent to children
	n->p1=n1;
	n->p2=n2;
	n->p3=n3;
	//in order children points to eachother
	n1->p1=NULL;
	n1->p3=n2;
	n2->p1=n1;
	n2->p3=n3;
	n3->p1=n2;
	n3->p3=NULL;
	//set isleaf
	n->leaf=false;
	n1->leaf=true;
	n2->leaf=true;
	n3->leaf=true;
}

//add level adds one depth to the tree
void addlevel(node* n){
	node* next=n->p3;
	addchildren(n);
	//printf("Children created for %p\n", n);
	if(next!=0){
		addlevel(next);
		//link in order traversals
		n->p3->p3=next->p1;
		next->p1->p1=n->p3;
	}
}

//count data nodes
int countdnodes(node* n){
	int count=1;
	if(n->p3!=0)
		count+=countdnodes(n->p3);
	return count;
}

//buildtree connects pointers of tree
void buildtree(node* n){
	if(!n->leaf){
		buildtree(n->p1);
		buildtree(n->p2);
		buildtree(n->p3);
		n->a = n->p1->b;
		n->b = n->p3->a;
	}
}

//FIND
int find(node* n, int a){
	if(n->leaf && n->a==a)
		return n->vala;
	if(n->leaf && n->b==a)
		return n->valb;
	if(n->leaf)
		return 0;
	if(!n->leaf){
		if(a <= n->a)
			return find(n->p1,a);
		if(a > n->a && a < n->b)
			return find(n->p2,a);
		if(a >= n->b)
			return find(n->p3,a);
	}
	return 0;
}

//shift moves data during an insert
void shift(node* n, int a, int v){
	int temp=a;
	int tempv=v;
	if(n->a==0){
		n->a=a;
		n->vala=v;
	}
	//insert into a
	else if(a <= n->a){
		temp=n->b;
		n->b=n->a;
		n->a=a;
		tempv=n->valb;
		n->valb=n->vala;
		n->vala=v;
	}
	//insert into b
	if(a > n->a && a < n->b){
		temp=n->b;
		n->b=a;
		tempv=n->valb;
		n->valb=n->vala;
	}
	
	//shift into next node
	if(n->p3 != 0)
		shift(n->p3,temp,tempv);
	
	//create new node at end if necessary
	if(n->p3 == 0){
		if(a==12)printf("its in nn\n");
		node* nn= newnode();
		nn->a=temp;
		nn->vala=v;
		nn->b=0;
		nn->p3=0;
		nn->p1=n;
		//printf("%p's p3 set as %p\n",n,nn);
		n->p3=nn;
		//printf("%p created with %i\n",nn, nn->a);
	}
}

//dynamic array struct
typedef struct {
	int *array;
    size_t used;
    size_t size;
}Array;
void initArray(Array *a, size_t initialSize) {
	a->array = (int *)malloc(initialSize * sizeof(int));
	a->used = 0;
	a->size = initialSize;
}
void insertArray(Array *a, int element) {
    if (a->used == a->size) {
        a->size *= 2;
        a->array = (int *)realloc(a->array, a->size * sizeof(int));
    }
    a->array[a->used++] = element;
}

//tree initializer
void newtree(node * root, Array * data ,int size){
	//printf("Initializing tree...\n");
	root->a=0;
	root->b=0;
	root->p1=0;
	root->p3=0;
	//printf("Newtree called with %p\n", root);
	node* f=root;
	int dataspots=2;
	while(dataspots<size){
		addlevel(f);
		//printf("Add a level\n");
		f=f->p1;
		dataspots=countdnodes(f)*2;
		printf("We now have %i dataspots\n", dataspots);
	}
	
	//assign the data to the leafs
	first(f);
	for(int i=0;i<size;){
		//printf("Assigning %i to %p\n", data->array[i], f);
		f->a=data->array[i];
		i++;
		//printf("Assigning %i to %p\n", data->array[i], f);
		f->b=data->array[i];
		i++;
		f=f->p3;
	}
	
	printf("Data assigned to leafs\n");
	//now give values to the tree...
	buildtree(root);
}

//INSERT
void insert(node* n, int a, int v){
	//printf("insert called with %i\n", a);
	if(n->leaf && n->b==0){
		n->b=a;
		//printf("%i inserted into empty b\n",a);
	}
	else if(n->leaf && n->b!=0){
		shift(n, a, v);
		//printf("%i inserted into b, shift called\n",a);
	}
	if(!n->leaf){
			//printf("not leaf, digging\n");
			if(a <= n->a || (n->a==0 && n->b==0)){
				printf("%i < %i going left\n", a, n->a);
				insert(n->p1,a,v);
			}
			else if((a > n->a && a < n->b) || (a > n->a && n->b == 0)){
				printf("%i between %i and %i going middle\n", a,n->a,n->b);
				insert(n->p2,a,v);
			}
			else if(a >= n->b){
				printf("%i > %i going right %i\n", a,n->b,n->a);
				insert(n->p3,a,v);
			}
	}
}

//UPDATE value
void update(node* n, int a, int v){
	if(n->leaf && n->a==a)
		n->vala=v;
	else if(n->leaf && n->b==a)
		n->valb=v;
	if(!n->leaf){
		if(a < n->a)
			update(n->p1,a,v);
		if(a >= n->a && a <= n->b)
			update(n->p2,a,v);
		if(a>n->b)
			update(n->p3,a,v);
	}
}

/*RANGE (GRADUATE CREDIT)
Scans are range searches for entries whose keys fall between a low key and high key.
Consider how many nodes need to be accessed during a range search for keys, within the B+Tree?
Can you describe two different methods to return the qualifying keys for a range search? 
(Hint: how does the algorithm of a range search compare to an equality search? What are their similarities, what is different?)
Can you describe a generic cost expression for Scan, measured in number of random accesses, with respect to the depth of the tree?
*/

// TODO GRADUATE: here you will need to define RANGE for finding qualifying keys and values that fall in a key range.


#endif
