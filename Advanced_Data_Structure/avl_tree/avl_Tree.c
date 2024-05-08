/* Create time: 2024.4.30 by whoami. */
#include <stdio.h>
#include <stdlib.h>
typedef struct avl_tree
{
	/* NEW ATTRIBUTE: height. */
	int key,height;
	struct avl_tree *left,*right,*p;
}avl_tree;
typedef struct Tree_root
{
	avl_tree *root,nil;
}Tree_root;
int max(int x,int y){return x>y?x:y;}
void update(avl_tree *x){x->height=max(x->left->height,x->right->height)+1;}
/* Same left and right rotate as R&B Tree */
avl_tree *left_rotate(Tree_root *H,avl_tree *x)
{
	avl_tree *y=x->right,*z=&H->nil;
	x->right=y->left;
	if(y->left!=z) y->left->p=x;
	y->p=x->p;
	if(x->p==z) H->root=y;
	else if(x==x->p->left) x->p->left=y;
	else x->p->right=y;
	y->left=x;
	x->p=y;
	update(x);
	update(y);
	return y;
}
avl_tree *right_rotate(Tree_root *H,avl_tree *x)
{
	avl_tree *y=x->left,*z=&H->nil;
	x->left=y->right;
	if(y->right!=z) y->right->p=x;
	y->p=x->p;
	if(x->p==z) H->root=y;
	else if(x==x->p->right) x->p->right=y;
	else x->p->left=y;
	y->right=x;
	x->p=y;
	update(x);
	update(y);
	return y;
}
/* We use the balanced factor to recgonize the conditions. 
 * --------------------------------------------------------
 * condition one: LL       				  |
 *         |	                                     |    |
 * 	  (x) 					    (x)	  |			
 *  	 /   \                                     /   \  |
 *	(y)  (z)     single right rotate         (q)   (z)|      
 *     /             ------------------->       /   \     |
 *    (q)                                      (p)  (y)   |     
 *    /							  |	
 *   (p)                                                  | 
 *________________________________________________________|
 *---------------------------------------------------------
 * condition two: RR       				  |
 *         |	                                   |      |
 * 	  (x) 					  (x)	  |			
 *  	 /   \                                   /   \    |
 *	(y)  (z)     single left rotate        (q)   (z)  |    
 *              \    ------------------->           /   \ |
 *              (q)                               (p)  (q)|     
 *     	          \				          |		   	
 *                (p)                                     |       
 *________________________________________________________|
 *---------------------------------------------------------
 * condition three: LR       				  |
 *        |                                        |      |
 *       (x)                                      (x)     |
 *      /   \                                    /   \    |
 *     (y) (z)  left rotate into LL            (y)   (z)  |
 *    /         ------------------->           /          |
 *   (q)                                     (p)          |
 *      \                                    /            | 
 *      (p)                                 (q)           | 
 *________________________________________________________|
 *---------------------------------------------------------
 * condition four: RL       				  |
 *        |                                       |       |
 *       (x)                			 (x)      |
 *      /   \					/   \     | 
 *    (y)   (z)    right rotate into RR       (y)   (z)   |
 *            \    -------------------->              \   |
 *           (q)    				      (p) |	
 *          /                                          \  |
 *         (p)                                        (q) |
 *________________________________________________________| */
avl_tree *balance(Tree_root *H,avl_tree *x)
{
	int bf=x->left->height-x->right->height;
	printf("bf=%d\n",bf);
	/* condition one and three. */
	if(bf > 1){
		int child_bf=x->left->left->height-x->left->right->height;
		if(child_bf>=0) x=right_rotate(H,x);
		else{
			x->left=left_rotate(H,x->left);
			x=right_rotate(H,x);
		}
	}else if(bf < -1){
		int child_bf=x->right->left->height-x->right->right->height;
		if(child_bf<=0) x=left_rotate(H,x);
		else{
			x->right=right_rotate(H,x->left);
			x=left_rotate(H,x);
		}
	}
	return x;
}
avl_tree *insert(Tree_root *H,avl_tree *x,int y)
{
	avl_tree *z=&H->nil;
	/* insert new node. */
	if(x==z){
		x=(avl_tree *)malloc(sizeof(avl_tree));
		x->p=x->left=x->right=z;
		x->key=y;
		x->height=1;		
	}else{
		if(x->key<y){
			x->right=insert(H,x->right,y);
			x->right->p=x;
		}else{
			x->left=insert(H,x->left,y);
			x->left->p=x;
		}
		update(x);
		x=balance(H,x);
	}
	return x;
}
void build(Tree_root *H)
{
	int key;
	avl_tree *x=&H->nil;
	for(;;){
		if(!scanf("%d",&key)) break;
		H->root=insert(H,H->root,key);
	}
}
avl_tree *find_min(Tree_root *H,avl_tree *x)
{
	avl_tree *y=&H->nil;
	while(x->left!=y) x=x->left;
	return x;
}
void transplant(Tree_root *H,avl_tree *x,avl_tree *y)
{
	avl_tree *q=&H->nil;
	if(x->p==q) H->root=y;
	else if(x==x->p->left) x->p->left=y;
	else x->p->right=y;
	y->p=x->p;
	return;
}
avl_tree *delete(Tree_root *H,avl_tree *x,avl_tree *y)
{
	avl_tree *z=&H->nil,*q=x;
	if(x->key>y->key){
		x->left=delete(H,x->left,y);
		x->left->p=x;
	}else if(x->key<y->key){
		x->right=delete(H,x->right,y);
		x->right->p=x;
	}else{
		/* Find it! */
		if(x->left==z){
			transplant(H,x,x->right);
			q=x->right;
		}else if(x->right==z){
			transplant(H,x,x->left);
			q=x->left;
		}else{
			q=find_min(H,x->right);
			if(q->p!=x){
				transplant(H,q,q->right);
				q->right=x->right;
				x->right->p=q;
			}
			transplant(H,x,q);
			q->left=x->left;
			x->left->p=q;
		}
	}
	q=balance(H,q);
	return q;	
}
