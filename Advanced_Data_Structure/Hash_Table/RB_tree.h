#ifndef _RB_TREE_H
#define _RB_TREE_H
#include <stdio.h>
#include <stdlib.h>
#define BLACK 0
#define RED  1
typedef struct RB_TREE
{
	int color,data;
	struct RB_TREE *p,*left,*right;
}RB_TREE;
typedef struct Hash
{
	RB_TREE *root,nil;
}Hash;
void left_rotate(Hash *H,RB_TREE *x)
{
	RB_TREE *y=x->right,*z=x->p,*q=&H->nil;
	x->right=y->left;
	if(y->left!=q) y->left->p=x;
	y->p=z;
	if(z==q) H->root=y;
	else if(x==z->left) z->left=y;
	else z->right=y;
	y->left=x;
	x->p=y;
}
void right_rotate(Hash *H,RB_TREE *x)
{
	RB_TREE *y=x->left,*z=x->p,*q=&H->nil;
	x->left=y->right;
	if(y->right!=q) y->right->p=x;
	y->p=z;
	if(z==q) H->root=y;
	else if(x==z->left) z->left=y;
	else z->right=y;
	y->right=x;
	x->p=y;
}
void insert_fixup(Hash *H,RB_TREE *x)
{
	while(x->p->color==RED){
		if(x->p==x->p->p->left){
			RB_TREE *y=x->p->p->right;
			if(y->color==RED){
				x->p->color=y->color=BLACK;
				x->p->p->color=RED;
				x=x->p->p;
			}else{
				if(x==x->p->right){
					x=x->p;
					left_rotate(H,x);
				}else{
					x->p->color=BLACK;
					x->p->p->color=RED;
					right_rotate(H,x->p->p);
				}
			}
		}else{
			RB_TREE *y=x->p->p->left;
			if(y->color==RED){
				x->p->color=y->color=BLACK;
				x->p->p->color=RED;
				x=x->p->p;
			}else{
				if(x==x->p->left){
					x=x->p;
					right_rotate(H,x);
				}else{
					x->p->color=BLACK;
					x->p->p->color=RED;
					left_rotate(H,x->p->p);
				}
			}
		}
	}
	H->root->color=BLACK;
}
void RB_TREE_insert(Hash *H,RB_TREE *x)
{
	RB_TREE *y,*z=H->root,*q=&H->nil;
	if(H->root==q){
		H->root=x;
		return;
	}
	while(z!=q){
		y=z;
		if(x->data>z->data) z=z->right;
		else z=z->left;
	}
	x->p=y;
	if(x->data>y->data) y->right=x;
	else y->left=x;
	x->color=RED;
	insert_fixup(H,x);
}
RB_TREE *find_min(Hash *H,RB_TREE *x)
{
	RB_TREE *y=&H->nil;
	while(x->left!=y) x=x->left;
	return x;
}
void delete_fixup(Hash *H,RB_TREE *x)
{
	RB_TREE *y=H->root;
	while(x!=y && x->color==BLACK){
		if(x==x->p->left){
			RB_TREE *z=x->p->right;
			if(z->color==RED){
				z->color=BLACK;
				x->p->color=RED;
				left_rotate(H,x->p);
				z=x->p->right;
			}
			if(z->left->color==BLACK && z->right->color==BLACK){
				z->color=RED;
				x=x->p;
			}else{
				if(z->right->color==BLACK){
					z->color=RED;
					z->left->color=BLACK;
					right_rotate(H,z);
					z=x->p->right;
				}
				z->color=x->p->color;
				x->p->color=BLACK;
				z->right->color=BLACK;
				left_rotate(H,x->p);
				x=y;
			}
		}else{	
			RB_TREE *z=x->p->left;
			if(z->color==RED){
				z->color=BLACK;
				x->p->color=RED;
				right_rotate(H,x->p);
				z=x->p->left;
			}
			if(z->left->color==BLACK && z->right->color==BLACK){
				z->color=RED;
				x=x->p;
			}else{
				if(z->left->color==BLACK){
					z->color=RED;
					z->right->color=BLACK;
					left_rotate(H,z);
					z=x->p->left;
				}
				z->color=x->p->color;
				x->p->color=BLACK;
				z->left->color=BLACK;
				right_rotate(H,x->p);
				x=y;
			}
		}	
	}
	x->color=BLACK;
}
void transplant(Hash *H,RB_TREE *x,RB_TREE *y)
{
	RB_TREE *z=&H->nil;
	if(x->p==z) H->root=y;
	else{
		if(x==x->p->left) x->p->left=y;
		else x->p->right=y;
	}
	y->p=x->p;
}
void RB_TREE_delete(Hash *H,RB_TREE *x)
{
	RB_TREE *y=x,*z=&H->nil,*q;
	int orginal_color=y->color;
	if(x->left==z){
		q=x->right;
		transplant(H,x,q);
	}else{
		if(x->right==z){
			q=x->left;
			transplant(H,x,q);
		}else{
			y=find_min(H,x->right);
			orginal_color=y->color;
			q=y->right;
			if(y->p==x) q->p=y;
			else{
				transplant(H,y,y->right);
				y->right=x->right;
				y->right->p=y;
			}
			transplant(H,x,y);
			y->left=x->left;
			y->left->p=y;
			y->color=x->color;
		}
	}
	if(orginal_color==BLACK) delete_fixup(H,q);
}
#endif
