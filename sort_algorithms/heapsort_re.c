/* Create time 2024.3.11 by whoamiZ
 * Finished time 2024.3.11 by whoamiZ. */
#include <stdio.h>
void swap(int *a,int *b)
{
	int tmp=*a;
	*a=*b;
	*b=tmp;
}
void min_heapify(int *a,int i,int len)
{
	int l=i<<1,r=l+1,min=i;
	if(a[l]<a[min] && l<=len) min=l;
	if(a[r]<a[min] && r<=len) min=r;
	if(min!=i){
		swap(&a[min],&a[i]);
		min_heapify(a,min,len);
	}
}
void build(int *a,int len)
{
	int i;
	for(i=len/2;i>=1;i--) min_heapify(a,i,len);
}
void heapsort(int *a,int len)
{	
	int i;
	for(i=len;i>1;i--){
		swap(&a[1],&a[i]);
		min_heapify(a,1,--len);
		printf("%d ",a[len+1]);
	}
	printf("%d ",a[1]);
}
