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
		first(n->p1);
	else
		return n;
	return n;
}
node* last(node* n){
	if(n->p3!=0)
		last(n->p3);
	else
		return n;
	return n;
}

//addchildren adds a full set of children to a node
void addchildren(node* n){
	node* n1=newnode();
	node* n2=newnode();
	node* n3=newnode();
	//initialize all values to zero
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
		n->a = n->p1->b+1;
		n->b = n->p3->a-1;
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
		if(a < n->a)
			find(n->p1,a);
		if(a>n->a && a<n->b)
			find(n->p2,a);
		if(a>n->b)
			find(n->p3,a);
	}
	return 0;
}

//shift moves data during an insert
void shift(node* n, int a, int v){
	int temp=a;
	//insert into a
	if(a <= n->a){
		temp=n->b;
		n->b=n->a;
		n->a=a;
		temp=n->valb;
		n->valb=n->vala;
		n->vala=v;
	}
	//insert into b
	if(a > n->a && a < n->b){
		temp=n->b;
		n->b=a;
		temp=n->valb;
		n->valb=n->vala;
	}
	
	//shift into next node
	if(n->p3 != 0)
		shift(n->p3,temp,v);
	
	//create new node at end if necessary
	if(n->p3 == 0){
		node* nn= newnode();
		nn->a=temp;
		nn->vala=v;
		nn->b=0;
		nn->p3=0;
		nn->p1=n;
		n->p3=nn;
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

//tree initializer with bulk data
void newtree(node * root, Array * data ,int size){
	printf("Initializing tree...\n");
	root->a=0;
	root->b=0;
	root->p1=0;
	root->p3=0;
	node* f=root;
	int dataspots=2;
	while(dataspots<size){
		addlevel(f);
		printf("Add a level\n");
		f=first(f->p1);
		dataspots=countdnodes(f)*2;
		printf("We now have %i dataspots\n", dataspots);
	}
	
	//assign the data to the leafs
	f=first(root);
	for(int i=0;i<size;){
		f->a=data->array[i];
		i++;
		f->b=data->array[i];
		i++;
		f=f->p3;
	}
	//printf("Data assigned to leafs\n");
	//now give values to the tree...
	buildtree(root);
}

//emptytree creates an empty tree with root and 3 children
void emptytree(node* root){
	//printf("Initializing tree...\n");
	root->a=0;
	root->b=0;
	addlevel(root);
	//printf("Children added!\n");
	
	//assign the data to the leafs
	node* f=root->p1;
	for(int i=0;i<6;){
		f->a=0;
		i++;
		f->b=0;
		i++;
		f=f->p3;
	}
}

//INSERT
void insert(node* n, int a, int v){
	if(n->leaf && n->a==0){
		printf("Inserting %i into a\n", a);
		n->a=a;
		n->vala=v;
	}
	else if(n->leaf && n->b==0){
		printf("Inserting %i into b\n", a);		
		n->b=a;
		n->valb=v;
	}
	if(n->leaf && n->a!=0 && n->b!=0){
		//printf("shift!\n");
		shift(n, a, v);
		//trigger tree rebuild
		node* current=first(n);
		Array b;
		initArray(&b, 1);
		for (int i=0;i<countdnodes(current);i++){
			insertArray(&b, current->a);
			insertArray(&b, current->b);
			current=current->p3;
		}
		newtree(n, &b, b.used);
		//printf("New Tree Built!\n");
	}
	if(!n->leaf){
		//printf("not leaf, digging\n");
		if(a==0)
			insert(n->p1,a,v);
		else if(a < n->a)
			insert(n->p1,a,v);
		if(a >= n->a && a <= n->b)
			insert(n->p2,a,v);
		if(a>n->b)
			insert(n->p3,a,v);
	}
}

//UPDATE value
void update(node* n, int a, int v){
	if(n->leaf && n->a==a)
		n->vala=v;
	else if(n->leaf && n->b==a)
		n->valb=v;
	if(!n->leaf){
		//printf("not leaf, digging\n");
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
