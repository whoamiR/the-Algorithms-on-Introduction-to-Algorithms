/* Create time 2024.3.11 by whoamiZ
 * Finished time 2024.3.11 by whoamiZ. */
#include <stdio.h>
void swap(int *a,int *b)
{
	int tmp=*a;
	*a=*b;
	*b=tmp;
}
void max_heapify(int *a,int i,int len)
{
	int l=i<<1,r=l+1,max=i;
	if(a[l]>a[max] && l<=len) max=l;
	if(a[r]>a[max] && r<=len) max=r;
	if(max!=i){
		swap(&a[max],&a[i]);
		max_heapify(a,max,len);
	}
}
void build(int *a,int len)
{
	int i;
	for(i=len/2;i>=1;i--) max_heapify(a,i,len);
}
void heapsort(int *a,int len)
{	
	int i;
	for(i=len;i>1;i--){
		swap(&a[1],&a[i]);
		max_heapify(a,1,--len);
		printf("%d ",a[len+1]);
	}
	printf("%d ",a[1]);
}
