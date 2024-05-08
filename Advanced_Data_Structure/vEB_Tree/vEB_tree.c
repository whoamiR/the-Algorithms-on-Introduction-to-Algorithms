/* Create time: 2024.4.19 by whoamiZ. 
* Finished time: 2024.4.19 by whoamiZ. */
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#define INF -0x3f3f3f3f
#define high(x) x/(int)log2((double)H->u)
#define low(x) x%(int)log2((double)H->u)
#define index(x,y) x*((int)log2((double)H->u))+y
typedef struct vEB_Tree
{
	/* u means the universe,the mininum of u is 2. */
	int u,min,max;
	/* summary is to show the min and max cluster in this vEB_Tree. */
	struct vEB_Tree *summary,*cluster;
}vEB_Tree;
/* A slow build progress. 
 * Cos we use the recursive method. 
 * CAUTION: PLEASE MAKE SURE u is 2^(2^k)*/
void init(vEB_Tree *H,int u)
{
	H->u=u;
	H->min=H->max=INF;
	/* Basic Situation. */ 
	if(u==2) return;
	/* Count the next u. */
	int tmp=(int)log2((double)u);
	H->summary=(vEB_Tree *)malloc(sizeof(vEB_Tree));
	init(H->summary,tmp),
	H->cluster=(vEB_Tree *)malloc(sizeof(vEB_Tree)*tmp);
	vEB_Tree *x=H->cluster;
	for(int i=1;i<=tmp;i++,x++) init(x,tmp);
	return;
}
int vEB_Tree_Min(vEB_Tree *H) { return H->min; }
int vEB_Tree_Max(vEB_Tree *H) { return H->max; }
/* Check if a element is in the tree. */
int vEB_Tree_Member(vEB_Tree *H,int x)
{
	/* if x is max or min, return 1. */
	if(x==H->max || x==H->min) return 1;
	else if(H->u==2) return 0;
	/* if u isn't 2 then we need to recurse to the basic situation. */
	else return vEB_Tree_Member(&H->cluster[high(x)],low(x));
}
void vEB_Tree_Empty_Insert(vEB_Tree *H,int x){ H->min=H->max=x; }
void vEB_Tree_Insert(vEB_Tree *H,int x)
{
	/* There isn't any elements in the tree. */
	if(H->min==INF) vEB_Tree_Empty_Insert(H,x);
       	else{
		if(x<H->min){
			int tmp=x;
			x=H->min;
			H->min=tmp;
		} 
		if(H->u>2){
			/* if there isn't any elements in the cluster.
			 * Then we need to update the summary. */
			if(vEB_Tree_Min(&H->cluster[high(x)])==INF){
				vEB_Tree_Insert(H->summary,high(x));
				vEB_Tree_Insert(&H->cluster[high(x)],low(x));
			}else vEB_Tree_Insert(&H->cluster[high(x)],low(x));
		}
		if(x>H->max) H->max=x;
	}
}
void vEB_Tree_Delete(vEB_Tree *H,int x)
{
	/* there is just one element in the cluster. */
	if(H->min==H->max) H->min=H->max=INF;
	/* then it's the basic situation. */
	else if(H->u==2){
		/* the element we want to delete is the min,the max will be the new min. */
		if(x==0) H->min=1;
		/* Same thing. */
		else H->min=0;
		H->max=H->min;
	}else{ 
		if(x==H->min){
		/* if the element we want to delete is the min,the we need to update the min. */ 
			int first_cluster=vEB_Tree_Min(H->summary);
			x=index(first_cluster,vEB_Tree_Min(&H->cluster[first_cluster]));
			H->min=x;
		}
		vEB_Tree_Delete(&H->cluster[high(x)],low(x));
		/* if there is no element in the cluster. */
		if(vEB_Tree_Min(&H->cluster[high(x)])==INF){
			vEB_Tree_Delete(H->summary,high(x));
			/* if the element we deleted is the max. */
			if(x==H->max){
				int summary_max=vEB_Tree_Max(H->summary);
				/* There is just one element in the summary. */
				if(summary_max==INF) H->max=H->min;
				else H->max=index(summary_max,vEB_Tree_Max(&H->cluster[summary_max]));
			}	
		}else if(x==H->max) H->max=index(high(x),vEB_Tree_Max(&H->cluster[high(x)]));
	}
}
int vEB_Tree_Successor(vEB_Tree *H,int x)
{
	if(H->u==2){
		if(x==0 && H->max==1) return 1;
		else return INF;
	}
	else{
		if(H->min!=INF && x<H->min) return H->min;
		else{
			int max=vEB_Tree_Max(&H->cluster[high(x)]);
			if(max!=INF && low(x)<max){
			       int offset=vEB_Tree_Successor(&H->cluster[high(x)],low(x));
			       return index(high(x),offset);
			 }else{
			 	int succ_cluster=vEB_Tree_Successor(H->summary,high(x));
				if(succ_cluster==INF) return INF;
			        else{
					int offset=vEB_Tree_Min(&H->cluster[succ_cluster]);
					return index(succ_cluster,offset);
				}	
			 }
		}
	}
}
int vEB_Tree_Predecessor(vEB_Tree *H,int x)
{
	if(H->u==2){
		if(x==1 && H->min==0) return 0;
		else return INF;
	}
	else{
		if(H->max!=INF && x>H->max) return H->max;
		else{
			int min=vEB_Tree_Min(&H->cluster[high(x)]);
			if(min!=INF && low(x)>min){
			       int offset=vEB_Tree_Predecessor(&H->cluster[high(x)],low(x));
			       return index(high(x),offset);
			 }else{
			 	int pred_cluster=vEB_Tree_Predecessor(H->summary,high(x));
				if(pred_cluster==INF){
					if(H->min!=INF && x>H->min) return H->min;
					else return INF;
				} 
			        else{
					int offset=vEB_Tree_Min(&H->cluster[pred_cluster]);
					return index(pred_cluster,offset);
				}	
			 }
		}
	}
}
void vEB()
{
	printf("van Emde Boas Tree:\n\
	Allowed operation:\n\
	insert,search,successor,predecessor.\n\
		Author: whoami.\n");
}
void help()
{
	printf("	type \"H\" for help\n\
	type \"I\" for insert\n\
	type \"D\" for delete\n\
	type \"S\" for search\n\
	type \"N\" for successor\n\
	type \"h\" for tips\n\
	type \"P\" for predecessor.\n");
}
void tips()
{
	printf("Don't forget to type q when you want to finish your operation.\n");
}
void end()
{
	printf("Thanks for using.\n\
		Yours sincerely,\n\
			whoamiZ.\n");
}
int main()
{
	vEB_Tree *H=(vEB_Tree *)malloc(sizeof(vEB_Tree));
	char dec;
	int u;
	vEB();
	printf("Now please input the universe: ");
	scanf("%d",&u);
	init(H,u);
	help();
	while(scanf("%c",&dec)){
		if(dec=='\n') continue;
		if(dec=='Q') break;
		if(dec=='h') tips();
		if(dec=='H') help();
		if(dec=='I'){
			int x;
			while(scanf("%d",&x)) vEB_Tree_Insert(H,x);
			printf("Continue? ");
		}
		if(dec=='S'){
			int x;
			while(scanf("%d",&x)){
				if(vEB_Tree_Member(H,x)) printf("%d is in the tree.\n",x);
				else printf("%d is not in the tree.\n",x);
			}
			printf("Continue? ");
		}
		if(dec=='D'){
			int x;
			while(scanf("%d",&x)){
				if(!vEB_Tree_Member(H,x)){
					printf("x is not in the tree.\nAgain: ");
					continue;
				}
				else vEB_Tree_Delete(H,x);
			}
			printf("Continue? ");
		}
		if(dec=='N'){
			int x;
			while(scanf("%d",&x)){
				int tmp=vEB_Tree_Successor(H,x);
				if(tmp==INF) printf("%d has no successor.\n",x);
				else printf("%d's successor is %d",x,tmp);
			}
			printf("Continue? ");
		}
		if(dec=='P'){
			int x;
			while(scanf("%d",&x)){
				int tmp=vEB_Tree_Successor(H,x);
				if(tmp==INF) printf("%d has no predecessor.\n",x);
				else printf("%d's predecessor is %d",x,tmp);
			}
			printf("Continue? ");
		}
	}
	end();
}
