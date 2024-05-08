/*Fibnacci Heap version:1.0*/
/* Description: it has a better time complexity than traditional binary heap. */
/* Allowed option: insert,search. */
/* Create time:2024.4.11 by whoamiZ.
 * Finished time:2024.4.12 by whoamiZ. */
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
typedef struct Fib_Heap {
	/* the mark tag means if the node had lost his child.
	 * the degree tag means the number of child of the node .*/
	int key,mark,degree;
	struct Fib_Heap *p,*child,*left,*right;
} Fib_Heap;
typedef struct root {
	Fib_Heap *min;
	int n;
} root;
void insert(root *H,Fib_Heap *x) {
	if(!H->min) {
		/* The root list is empty. */
		H->min=x;
		x->left=x->right=x;
		H->n=1;
	} else {
		/* If the root list is not empty. We choose to create a double linked list.
		 * (It's convenient for consilidate operation.) */
		if(H->min->left==H->min) { /* If the min has no left bro. */
			x->left=H->min->right;
			H->min->right=x;
		} else {
			/* Otherwise,the min has a left bro. We just need to link the x. */
			x->left=H->min->left;
			H->min->left->right=x;
		}
		H->min->left=x;
		x->right=H->min;
		if(x->key<H->min->key) H->min=x;
		H->n++;
	}
	return;
}
void build(root *H) {
	Fib_Heap *tmp;
	tmp=(Fib_Heap *)malloc(sizeof(Fib_Heap));
	while(scanf("%d",&tmp->key)) {
		insert(H,tmp);
		tmp=(Fib_Heap *)malloc(sizeof(Fib_Heap));
	}
	return;
}
/* Here we need to make the parent's child pointer points at the leftest child. */
void Fib_Heap_Link(Fib_Heap *x,Fib_Heap *y) {
	/* Node y doesn't has a child node. */
	if(!y->child) {
		x->p=y;
		y->child=x;
	} else {
		/* node y has (a) child node(s).
		 * We just link the x to the left of tmp. */
		Fib_Heap *tmp=y->child;
		x->left=NULL;
		x->right=tmp;
		tmp->left=x;
		y->child=x;
		x->p=y;
	}
	y->degree++;
	y->mark=0;
	return;
}
void consolidate(root *H) {
	/* the max_degree is less than lgn. */
	double max_degree=log2((double)H->n);
	int n=H->n;
	/* We just create a array to help us reconstruct heaps. */
	Fib_Heap *a[(int)max_degree|1];
	for(int i=0; i<=(int)max_degree; i++) a[i]=NULL;
	/*First, we make the ring come into the list. */
	H->min->left->right=NULL;
	for(Fib_Heap *tmp=H->min; tmp;) {
		/* tmp may be seperated from root list. So we need a x.
		 * The last operation link will make sure x always in root list. */
		Fib_Heap *x=tmp;
		int d=x->degree;
		while(a[d]) {
			/* Another node with the same degree of x. */
			Fib_Heap *y=a[d];
			if(x->key>y->key) {
				Fib_Heap *temp=x;
				x=y;
				y=temp;
			}
			Fib_Heap_Link(y,x);
			a[d]=NULL;
			d++;
		}
		a[d]=x;
		Fib_Heap *tmp2=tmp;
		tmp=tmp->right;
		/* We need to get the tmp off the root list. */
		tmp2->left=tmp2->right=NULL;
	}
	H->min=NULL;
	/* Reconstruct root list. */
	for(int i=0; i<=max_degree; i++) {
		if(!a[i]) continue;
		insert(H,a[i]);
	}
	H->n=n;
	return;
}
/* Here is the most complex operation.
 * Also, it's the most incredible operation. */
Fib_Heap *extract_min(root *H) {
	Fib_Heap *tmp=H->min;
	/* The heap is empty. */
	if(!tmp) return tmp;
	/* if the node we want to delete has child nodes.
	 * We need to bring them to the root list. */
	if(tmp->child) {
		for(Fib_Heap *x=tmp->child;x;) {
			x->p=NULL;
			x->left=tmp->left;
			tmp->left->right=x;
			Fib_Heap *temp=x->right;
			x->right=tmp;
			tmp->left=x;
			x=temp;
		}
	}
	/* Then we need to delete H.min */
	tmp->left->right=tmp->right;
	tmp->right->left=tmp->left;
	/* Let's see if the root list has been cleared. */
	if(tmp==tmp->right) H->min=NULL;
	else {
		/* Anywhere is ok. */
		H->min=tmp->left;
		consolidate(H);
	}
	H->n--;
	return tmp;
}
void init() {
	printf("Fibnacci Heap(v1.0):\n\
	Allowed options:\n\
		insert\n\
		decrease key\n\
				Author: whoami.\n");
}
void help() {
	printf("Help Information:\n\
		press \"I\" for insert a number into heap.\n\
		      \"S\" for get the minnum in the heap.\n\
		      \"Q\" for quit.\n");
}
void end() {
	printf("Qutting.....\n\
		Thanks for using.\n\
			Any bugs please RTFSC.\n\
			Yours sincerely,\n\
				whoami\n");
}
int main() {
	init();
	char x;
	root H;
	while(scanf("%c",&x)) {
		if(x=='\n') continue;
		if(x=='Q') break;
		if(x=='H') help();
		if(x=='I') {
			printf("Please input the number: ");
			build(&H);
		}
		if(x=='S') {
			Fib_Heap *tmp=extract_min(&H);
			if(!tmp) printf("There is no number in heap now.\n");
			else printf("The min number is %d\n",tmp->key);
		}
	}
	end();
	return 0;
}
