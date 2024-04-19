/* Create time 2024.3.11 by whoamiZ
 * Finished time 2024.3.11 by whoamiZ. */
#include <stdio.h>
int a[100000],b[100000];
void swap(int *a,int *b)
{
	int tmp=*a;
	*a=*b;
	*b=tmp;
}
int partition(int l,int r)
{
	int i=l-1,j;
	for(j=l;j<r;j++){
		if(a[j]<=a[r])
			swap(&a[++i],&a[j]);
	}
	swap(&a[i+1],&a[r]);
	return i+1;
}
void quicksort(int l,int r)
{
	if(r<=l) return;
	int q=partition(l,r);
	quicksort(l,q-1);
	quicksort(q+1,r);
}
