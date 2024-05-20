#ifndef HASH_H
#define HASH_H
#include <stdio.h>
#include <stdlib.h>
#include "RB_tree.h"
void hash_function(Hash *a,RB_TREE *b,int p)
{
	int pos=b->data%p;
	b->p=b->right=b->left=&a[pos].nil;
	RB_TREE_insert(a+pos,b);
	return;
}
RB_TREE *query(Hash *a,int key,int p)
{
	int pos=key%p;
	if(a[pos].root==NULL) return NULL;
	RB_TREE *tmp=a[pos].root,*x=&a[pos].nil;
	for(;;){
		if(tmp==x) break;
		if(tmp->data==key) break;
		if(tmp->data<=key) tmp=tmp->right;
		else tmp=tmp->left;
	}
	if(tmp==x) return NULL;
	return tmp;
}
#endif
