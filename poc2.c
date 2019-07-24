/*b+tree proof of concept
andrew pham, anp6338@g.harvard.edu
this file is not required for program function
it is a proof of concept I used as scratch paper*/

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#define INITIAL_CAPACITY 2

typedef struct {
  int *array;
  size_t used;
  size_t size;
} Array;

void initArray(Array *a, size_t initialSize) {
  a->array = (int *)malloc(initialSize * sizeof(int));
  a->used = 0;
  a->size = initialSize;
}

void insertArray(Array *a, int element) {
  // a->used is the number of used entries, because a->array[a->used++] updates a->used only *after* the array has been accessed.
  // Therefore a->used can go up to a->size 
  if (a->used == a->size) {
    a->size *= 2;
    a->array = (int *)realloc(a->array, a->size * sizeof(int));
  }
  a->array[a->used++] = element;
}

typedef struct node node;
struct node{
	int a,b;
	node *p1, *p2, *p3;
	bool isleaf;
};
node* newnode(){
    node * n=(node*)malloc(sizeof(node));
    return n;
}

node* first(node* n){
	if(n->p1!=0)
		first(n->p1);
	else
		return n;
}
node* last(node* n){
	if(n->p3!=0)
		last(n->p3);
	else
		return n;
}

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
	n->isleaf=false;
	n1->isleaf=true;
	n2->isleaf=true;
	n3->isleaf=true;
}

void addlevel(node* n){
	node* next=n->p3;
	addchildren(n);
	printf("Children created for %p\n", n);
	if(next!=0){
		addlevel(next);
		//link in order traversals
		n->p3->p3=next->p1;
		next->p1->p1=n->p3;
	}
}

int countdnodes(node* n){
	int count=1;
	if(n->p3!=0)
		count+=countdnodes(n->p3);
	return count;
}

void treebuilder(node* n){
	if(!n->isleaf){
		treebuilder(n->p1);
		treebuilder(n->p2);
		treebuilder(n->p3);
	}
	n->a=
	
}

int main(){
	//initial data dynamic array
	Array a;
	int i;
	initArray(&a, 5);  // initially 5 elements
	for (i = 0; i < 100; i++)
    	insertArray(&a, i); 
	printf("We have %d pieces of data\n", a.used);
	
	//initialize the tree
	printf("Now we make a tree...\n");
	node root;
	root.a=0;
	root.b=0;
	root.p1=0;
	root.p3=0;
	int dataspots=2;
	node* f=&root;
	while(dataspots<a.used){
		addlevel(f);
		printf("Add a level\n");
		f=first(f->p1);
		dataspots=countdnodes(f)*2;
		printf("We now have %i dataspots\n", dataspots);
	}
	
	//assign the data to the leafs
	node* current=f;
	for(int i=0;i<a.used;){
		current->a=a.array[i];
		i++;
		current->b=a.array[i];
		i++;
		current=current->p3;
	}
	printf("Data assigned to leafs\n");
	
	//now give values to the tree...
	//treebuilder(&root);
}
