/* Create time: 2024.4.30 by whoami. */
#include <stdio.h>
#include <stdlib.h>
typedef struct avl_node
{
	/* NEW ATTRIBUTE: height. */
	int key,height;
	struct avl_node *left,*right,*p;
}avl_node;
typedef struct Tree_root
{
	avl_node *root,nil;
}Tree_root;
int max(int x,int y){return x>y?x:y;}
void update(avl_node *x){x->height=max(x->left->height,x->right->height)+1;}
/* Same left and right rotate as R&B Tree */
avl_node *left_rotate(Tree_root *H,avl_node *x)
{
	avl_node *y=x->right,*z=&H->nil;
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
avl_node *right_rotate(Tree_root *H,avl_node *x)
{
	avl_node *y=x->left,*z=&H->nil;
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
void balance(Tree_root *H,avl_node *x)
{
	avl_node *y=x,*z=&H->nil;
	while(y!=z){
		int bf=y->left->height-y->right->height;
		printf("bf=%d\n",bf);
		/* condition one and three. */
		if(bf > 1){
			int child_bf=y->left->left->height-y->left->right->height;
			if(child_bf>=0) y=right_rotate(H,y);
			else{
				y->left=left_rotate(H,y->left);
				y=right_rotate(H,y);
			}
		}else if(bf < -1){
			int child_bf=y->right->left->height-y->right->right->height;
			if(child_bf<=0) y=left_rotate(H,y);
			else{
				y->right=right_rotate(H,y->left);
				y=left_rotate(H,y);
			}
		}else y=y->p;
	}
}
void insert(Tree_root *H,avl_node *x)
{
	avl_node *y=H->root,*z=&H->nil,*q;
	while(y!=z){
		q=y;
		if(x->key>y->key) y=y->right;
		else y=y->left;
	}
	x->p=q;
	if(x->key>q->key) q->right=x;
	else q->left=x;
	for(avl_node *n=q;n!=z;n=n->p) update(n);
	balance(H,q);
}
avl_node *find_min(Tree_root *H,avl_node *x)
{
	avl_node *y=&H->nil;
	while(x->left!=y) x=x->left;
	return x;
}
void transplant(Tree_root *H,avl_node *x,avl_node *y)
{
	avl_node *q=&H->nil;
	if(x->p==q) H->root=y;
	else if(x==x->p->left) x->p->left=y;
	else x->p->right=y;
	y->p=x->p;
	return;
}
void delete(Tree_root *H,avl_node *x)
{
	avl_node *y=H->root,*z=&H->nil,*q;
	if(x->left==z)
		transplant(H,x,x->right);
	else if(x->right==z)
		transplant(H,x,x->left);
	else{
		q=find_min(H,x->right);
		if(q->p=x){
			transplant(H,q,q->right);
			q->right=x->right;
			x->right->p=q;
		}
		transplant(H,x,q);
		q->left=x->left;
		x->left->p=q;
	}
	for(avl_node *n=q->p;n!=z;n=n->p) update(n);
	balance(H,q->p);
}
void build(Tree_root *H)
{
	avl_node *x=&H->nil,*y;
	for(;;){
		y=(avl_node *)malloc(sizeof(avl_node));
		if(!scanf("%d",&y->key)) break;
		y->left=y->right=y->p=&H->nil;
		y->height=1;
		if(H->root==x) H->root=y;
		else insert(H,y);	
	}
}
int main()
{
	Tree_root H;
	H.root=&H.nil;
	build(&H);
	printf("%d\n",H.root->key);
}
