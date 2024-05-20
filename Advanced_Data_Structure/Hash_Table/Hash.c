#include "hash.h"
#include "RB_tree.h"
#include <stdio.h>
#include <stdlib.h>
#define INF 0
#define u 200
void build(Hash *a)
{
	for(;;){
		RB_TREE *tmp=(RB_TREE *)malloc(sizeof(RB_TREE));
		scanf("%d",&tmp->data);
		if(tmp->data==INF) break;
		hash_function(a,tmp,u);
	}
}
void help()
{
	printf("Help Infomation:\n\
		press <h> to see this text.\n\
		press <i> to insert.\n\
		press <m> to modify a cost for data.\n\
		press <s> to get the data you input.\n\
		press <c> to clear.\n\
		ptree <d> to delete a edge.\n");
}
void tree_show(RB_TREE *a,RB_TREE *H)
{
	if(H->left!=a) tree_show(a,H->left);
	printf("%d\n",H->data);
	if(H->right!=a) tree_show(a,H->right);
}
void show(Hash *a)
{
	for(int i=0;i<u;i++){
		RB_TREE *tmp=&a[i].nil;
		if(a[i].root!=tmp){
			tree_show(&a[i].nil,a[i].root);
			printf("\n");
		}
	}
}
void init()
{
	printf("Hash Table(Based on R&B Tree):\n\
		press <h> for help.\n\n");
}
int main()
{
	init();
	Hash *a=(Hash *)malloc(sizeof(Hash)*u);
	for(int i=0;i<u;i++) a[i].root=&a[i].nil;
	printf("MST> ");
	for(char dec;;){
		scanf("%c",&dec);
		if(dec=='\n') continue;
		if(dec=='h' || dec=='H'){
			help();
			printf("MST> ");
			continue;
		}
		if(dec=='i' || dec=='I'){
			printf("Please input the data:\n");
			build(a);
			printf("MST> ");
			continue;
		}
		if(dec=='m' || dec=='M' || dec=='d' || dec=='D'){
			printf("Please input the data's info(same as new oparation): \n");
			int data1;
			scanf("%d",&data1);
			RB_TREE *tmp=query(a,data1,u);
			if(!tmp)
				printf("Sorry,no such edge.\n");
			else{
				RB_TREE_delete(&a[tmp->data],tmp);
				if(dec=='m' || dec=='M'){
					printf("New cost: ");
					int data2;
					scanf("%d",&data2);
					tmp->data=data2;
					tmp->p=tmp->left=tmp->right=&a[data2].nil;
					RB_TREE_insert(a+(data2%u),tmp);
				}
			}
			printf("MST> ");
			continue;
		}
		if(dec=='s' || dec=='S'){
			show(a);
			printf("MSF> ");
			continue;
		}
		if(dec=='q' || dec=='Q') break;
		if(dec=='c' || dec=='C'){
			system("clear");
			printf("MST> ");
		}else{
			printf("Unknown Option.\n");
			printf("MSF> ");
		}
	}
	printf("Bye!\n");
}
