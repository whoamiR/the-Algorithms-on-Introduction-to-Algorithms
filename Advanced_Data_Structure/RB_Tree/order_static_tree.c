#include <stdio.h>
#include <stdlib.h>
#define BLACK 0
#define RED  1
typedef struct Order_Tree
{
	int key,color,size;
	struct Order_Tree *p,*left,*right;
}Order_Tree;
typedef struct Tree_root
{
	Order_Tree *root,nil;
}Tree_root;
void left_rotate(Tree_root *H,Order_Tree *x)
{
	Order_Tree *y=x->right,*z=x->p,*q=&H->nil;
	x->right=y->left;
	if(y->left!=q) y->left->p=x;
	y->p=z;
	if(z==q) H->root=y;
	else if(x==z->left) z->left=y;
	else z->right=y;
	y->left=x;
	x->p=y;
	y->size=x->size;
	x->size=x->left->size+x->right->size+1;
}
void right_rotate(Tree_root *H,Order_Tree *x)
{
	Order_Tree *y=x->left,*z=x->p,*q=&H->nil;
	x->left=y->right;
	if(y->right!=q) y->right->p=x;
	y->p=z;
	if(z==q) H->root=y;
	else if(x==z->left) z->left=y;
	else z->right=y;
	y->right=x;
	x->p=y;
	y->size=x->size;
	x->size=x->left->size+x->right->size+1;

}
void insert_fixup(Tree_root *H,Order_Tree *x)
{
	while(x->p->color==RED){
		if(x->p==x->p->p->left){
			Order_Tree *y=x->p->p->right;
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
			Order_Tree *y=x->p->p->left;
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
void insert(Tree_root *H,Order_Tree *x)
{
	Order_Tree *y,*z=H->root,*q=&H->nil;
	x->size=1;
	if(H->root==q){
		H->root=x;
		return;
	}
	while(z!=q){
		y=z;
		y->size+=1;
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
	Order_Tree *q,*tmp=&H->nil;
	for(;;){
		q=(Order_Tree *)malloc(sizeof(Order_Tree));
		q->left=q->right=q->p=&H->nil;
		if(!scanf("%d",&q->key)) break;
		insert(H,q); 
	}
}
Order_Tree *find_max(Tree_root *H,Order_Tree *x)
{
	Order_Tree *y=&H->nil;
	while(x->right!=y) x=x->right;
	return x;
}
Order_Tree *find_min(Tree_root *H,Order_Tree *x)
{
	Order_Tree *y=&H->nil;
	while(x->left!=y) x=x->left;
	return x;
}
void delete_fixup(Tree_root *H,Order_Tree *x)
{
	Order_Tree *y=H->root;
	while(x!=y && x->color==BLACK){
		if(x==x->p->left){
			Order_Tree *z=x->p->right;
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
			Order_Tree *z=x->p->left;
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
void transplant(Tree_root *H,Order_Tree *x,Order_Tree *y)
{
	Order_Tree *z=&H->nil;
	if(x->p==z) H->root=y;
	else{
		if(x==x->p->left) x->p->left=y;
		else x->p->right=y;
	}
	y->p=x->p;
}
void decrease_size(Tree_root *H,Order_Tree *x)
{
	Order_Tree *y=x->p,*z=&H->nil;
	while(y!=z){
		y->size--;
		y=y->p;
	}
}
void delete(Tree_root *H,Order_Tree *x)
{
	decrease_size(H,x);
	Order_Tree *y=x,*z=&H->nil,*q;
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
void print(Tree_root *H,Order_Tree *tmp)
{
	Order_Tree *q=&H->nil;
	if(tmp->left!=q) print(H,tmp->left);
	printf("%d",tmp->key);
	if(tmp->color==1) printf("RED,");
	else printf("BLACK,");
	if(tmp->right!=q) print(H,tmp->right);
}
int query(Tree_root *H,Order_Tree *x)
{
	int ans=x->left->size+1;
	Order_Tree *y=x;
	while(y!=H->root){
		if(y==y->p->right) ans+=y->p->left->size+1;
		y=y->p;
	}
	return ans;
}
Order_Tree *find(Order_Tree *y,Order_Tree *z,int x)
{
	if(z==y) return y;
	if(z->key==x) return z;
	if(z->key>x) return find(y,z->left,x);
	else return find(y,z->right,x);
}
void init()
{
	printf("Order Static Tree v1.0 based on Inrtroduction to Algorithm:\n\
	Allowed Operation:\n\
		Insert Delete Query Find.\n\
		type \"H\" for help.\n\
							By whoami.\n");
}
void help()
{
	printf("type \"I\" for insert\n\
	type \"D\" for delete\n\
	type \"Q\" for query\n\
	type \"F\" for find.\n");
}
void end()
{
	printf("Thanks for using.\n\
		Yours sincerely,\n\
			whoami.\n");
}
int main()
{
	init();
	Tree_root H;
	H.root=&H.nil;
	char dec;
	printf("> ");
	for(;;){
		scanf("%c",&dec);
		if(dec=='q') break;
		if(dec=='\n') continue;
		if(dec=='H'){
			help();
			printf("> ");
		}else if(dec=='I'){
		       	build(&H);
			printf("> ");
		}else if(dec=='D' || dec=='Q' || dec=='F'){
			int key;
			printf("type the key of the node: ");
			scanf("%d",&key);
			Order_Tree *x=find(&H.nil,H.root,key),*y=&H.nil;
			if(x==y){
				printf("Error: no such node.\n");
				printf("> ");
				continue;
			}
			if(dec=='D'){
				delete(&H,x);
				printf("Delete node suscess!\n");
				printf("> ");
				continue;
			}else if(dec=='Q'){
				int z=query(&H,x);
				printf("%d's order is %d\n",key,z);
				printf("> ");
				continue;
			}
			printf("node exists.\n");
			printf("> ");
			continue;
		}
	}
	end();
	return 0;
}
