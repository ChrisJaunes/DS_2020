#include<cstdio>
#include<cstdlib>
#include<cstring>
#include<cmath>
#include<iostream>
#include<algorithm>
using namespace std;
typedef long long LL;
#define maxn 101000

char ak[maxn];

struct node
{
	int key,value;
	bool vis;	 
}a[maxn];int len;
bool cmp(node x,node y)
{
	if (x.vis==y.vis) return x.key<y.key;
	if (x.vis) return 1;
	else return 0; 
} 
int main()
{
	int c,i,key,cnt;cnt=len=0;
	//sprintf(ak, "test%d.ans", 1);
    //freopen(ak, "w", stdout);
	
	while (cin>>c)
	{
		if (c==1)
		{
			node now;now.vis=true;
			scanf("%d %d",&now.key,&now.value);
			a[++len]=now;
		}else if (c==2)
		{
			scanf("%d",&key);
			/*int l=1,r=len,ans=-1;
			while (l<=r)
			{
				int mid=(l+r)>>1;
				if (a[mid].key<=key) {ans=mid;l=mid+1;}
				else r=mid-1; 
			}
			if (ans!=-1 && a[ans].key==key) a[ans].vis=false;*/
			for (i=1;i<=len;i++) 
			 if (a[i].vis && a[i].key==key)
			 {
			 	a[i].vis=false;
			 	break;
			 }
			cnt++;
		}else
		{
			sort(a+1,a+1+len,cmp);
			//len-=cnt;cnt=0;
			i=1;while (!a[i].vis) i++;
			printf("%d",a[i++].value);
			for (;i<=len;i++)
			 if (a[i].vis) printf(" %d",a[i].value);
			 else break;
			printf("\n");
		}
	}
	return 0;
}

