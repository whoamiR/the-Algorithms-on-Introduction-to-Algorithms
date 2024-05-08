/* Create time:2024.4.21 by whoamiZ. */
/* Finished time:2024.4.25 by whoamiZ. */
#include <stdio.h>
#include <stdlib.h>
#define BLACK 0
#define RED  1
typedef struct RB_Tree
{
	int key,color;
	struct RB_Tree *p,*left,*right;
}RB_Tree;
/* The root of the tree. */
typedef struct Tree_root
{
	/* We use a sentinel to replace the NULL. */
	RB_Tree *root,nil;
}Tree_root;
/* Two rotate operation(will be used in insert_fixup and delete_fixup operation). 
 *       |                                         |
 *      (x)        ----left_rotate->              (y) 
 *     /   \	                                 /   \
 *    a    (y)					(x)   c	 
 *        /   \    <-right_rotate---           /   \
 *       b     c                              a     b          */
void left_rotate(Tree_root *H,RB_Tree *x)
{
	RB_Tree *y=x->right,*z=x->p,*q=&H->nil;
	x->right=y->left;
	if(y->left!=q) y->left->p=x;
	y->p=z;
	if(z==q) H->root=y;
	else if(x==z->left) z->left=y;
	else z->right=y;
	y->left=x;
	x->p=y;
}
void right_rotate(Tree_root *H,RB_Tree *x)
{
	RB_Tree *y=x->left,*z=x->p,*q=&H->nil;
	x->left=y->right;
	if(y->right!=q) y->right->p=x;
	y->p=z;
	if(z==q) H->root=y;
	else if(x==z->left) z->left=y;
	else z->right=y;
	y->right=x;
	x->p=y;
}
/* The insert opration may broke the rules. So we need to fix-up.
*  Red/Black  Property: Every node is colored, either red or black.
*	Root  Property: The Tree_root is black.
*	Leaf  Property: Every leaf (NIL) is black.
*	Red   Property: If a red node has rootren then, the children are always black.
*	Depth Property: For each node, any simple path from this node to any of its descendant 
*			leaf has the same black-depth (the number of black nodes).
*				from: https://www.programiz.com/dsa/red-black-tree */
void insert_fixup(Tree_root *H,RB_Tree *x)
{
	while(x->p->color==RED){
		/* We divided the situation into two conditon.
		 * the x's parent is left or right node(we just need to change right and left). 
		 * More details see https://oi-wiki.org/ds/rbtree/ */
		if(x->p==x->p->p->left){
			RB_Tree *y=x->p->p->right;
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
			RB_Tree *y=x->p->p->left;
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
	/* To make sure Tree_root's color is BLACK. */
	H->root->color=BLACK;
}
/* the insert operation. */
void insert(Tree_root *H,RB_Tree *x)
{
	RB_Tree *y,*z=H->root,*q=&H->nil;
	/* Locate x's position. 
	 * y is x's parent node. */
	while(z!=q){
		y=z;
		if(x->key>z->key) z=z->right;
		else z=z->left;
	}
	x->p=y;
	if(x->key>y->key) y->right=x;
	else y->left=x;
	x->color=RED;
	insert_fixup(H,x);
}
void build(Tree_root *H)
{
	RB_Tree *q,*tmp=&H->nil;
	for(;;){
		q=(RB_Tree *)malloc(sizeof(RB_Tree));
		q->left=q->right=q->p=&H->nil;
		/* When we want to finish insert,we should type a char to stop.
		 * e.g. 1 2 3 4 5 q */
		if(!scanf("%d",&q->key)) break;
		if(H->root==tmp) H->root=q;
		else insert(H,q); 
	}
}
/* Find a tree that based on x's maxinum. */
RB_Tree *find_max(Tree_root *H,RB_Tree *x)
{
	RB_Tree *y=&H->nil;
	while(x->right!=y) x=x->right;
	return x;
}
RB_Tree *find_min(Tree_root *H,RB_Tree *x)
{
	RB_Tree *y=&H->nil;
	while(x->left!=y) x=x->left;
	return x;
}
/* Delete cases are also complex.
 * See also on https://oi-wiki.org/ds/rbtree/ */
void delete_fixup(Tree_root *H,RB_Tree *x)
{
	RB_Tree *y=H->root;
	while(x!=y && x->color==BLACK){
		/* Same as insert operation. */
		if(x==x->p->left){
			/* x's bro node. */
			RB_Tree *z=x->p->right;
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
				/* finish the loop. */
				x=y;
			}
		}else{	
			RB_Tree *z=x->p->left;
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
/* We use y to replace the x. */
void transplant(Tree_root *H,RB_Tree *x,RB_Tree *y)
{
	/* if x is the Tree_root node. */
	RB_Tree *z=&H->nil;
	if(x->p==z) H->root=y;
	else{
		if(x==x->p->left) x->p->left=y;
		else x->p->right=y;
	}
	y->p=x->p;
}
/*----------------------------------------------------------------------------
 *        |                       |					     |
 *       (x)			 (r)                                         |
 *      /   \        --->       /   \                                        |
 *     NIL  (r)                ..   .. 					     |
 *----------------------------------------------------------------------------
 *        |        		  |                                          |
 *       (x) 			 (z)	                                     |
 *      /   \                   /   \					     |
 *     (y)  (z)    ---->      (y)   (q)					     |
 *         /   \             /  \   /  \				     |
 *        NIL  (q)         ... ... ... ...				     |
 *----------------------------------------------------------------------------
 *        |                     |                                   |        |
 *       (x) 		       (x)                                 (q)	     |
 *      /   \		      /   \     (q)                       /   \      |
 *    (y)   (z)             (y) ...    /   \                    (y)   (z)    |
 *   /   \ /   \                     NIL   (z)                 /     /   \   |
 *   ............ ------>                 /   \ ------>       ...   ...  ... |
 *   	  (q)                           ...  ...		    /        |
 *   	 /   \			         /			   (p)       |
 *      NIL  (p)             		(p)				     |
 *----------------------------------------------------------------------------*/
void delete(Tree_root *H,RB_Tree *x)
{
	RB_Tree *y=x,*z=&H->nil,*q;
	/* Rememberize the delete node (or the node we insert in)'s node. */
	int orginal_color=y->color;
	/* x's left node is NULL. */
	if(x->left==z){
		q=x->right;
		transplant(H,x,q);
	}else{
		if(x->right==z){
			q=x->left;
			transplant(H,x,q);
		}else{
			/* We need to find the mininum on x's right. */
			y=find_min(H,x->right);
			orginal_color=y->color;
			q=y->right;
			/* if y is q's root node. 
			 * To be honest, I think it's useless. */
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
	/* we break the R&B rule. */
	if(orginal_color==BLACK) delete_fixup(H,q);
}
void print(Tree_root *H,RB_Tree *tmp)
{
	RB_Tree *q=&H->nil;
	if(tmp->left!=q) print(H,tmp->left);
	printf("%d",tmp->key);
	if(tmp->color==1) printf("RED,");
	else printf("BLACK,");
	if(tmp->right!=q) print(H,tmp->right);
}
