/* Create time 2024.3.11 by whoamiZ
 * Finished time 2024.3.11 by whoamiZ. */
#include <stdio.h>
int b[1000000],a[10000000];
void msort(int l,int r)
{
	if(l==r) return;
	int mid=(l+r)/2;
	msort(l,mid);
	msort(mid+1,r);
	int i=l,j=mid+1,k=l;
	while(i<=mid && j<=r){
		if(a[i]<a[j]) b[k++]=a[i++];
	       	else b[k++]=a[j++];	
	}
	while(i<=mid) b[k++]=a[i++];
	while(j<=r) b[k++]=a[j++];
	for(i=l;i<=r;i++) a[i]=b[i];
}
