#include <stdio.h>
#include <stdlib.h>
int a[100000],b[100000];
void swap(int *a,int *b)
{
	int tmp=*a;
	*a=*b;
	*b=tmp;
}
int randomly_partition(int l,int r)
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
	int q=randomly_partition(l,r);
	quicksort(l,q-1);
	quicksort(q+1,r);
}
int main()
{
	int n,i;
	scanf("%d",&n);
	for(i=1;i<=n;i++) scanf("%d",a+i);
	quicksort(1,n);
	for(i=1;i<=n;i++) printf("%d ",a[i]);
	printf("\n");
}
