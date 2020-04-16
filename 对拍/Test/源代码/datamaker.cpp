#include<cstdio>
#include<cstdlib>
#include<cstring>
#include<ctime>
#include<iostream>
 
using namespace std;
/*
数据输入格式
插入操作：int type-1, key, value
删除操作:int type-2, key 
查询操作:int type-3

数据输出格式
对每个查询操作，输出按key大小排序的value值
一行，空格间隔，行末换行 

数据保证相同key在树中只会同时存在一个 
*/

#define problem "test" //输出文件名
#define sol "vio" //暴力程序名 
#define std "std" //待测程序名 

char ak[1000];

int cases = 10;//数据组数 

const int MAXN = 1e5 + 10;

bool vis[MAXN];

wchar_t* trans(int x) {
	char fc[20];
	sprintf(fc,"%d",x); 
	
	wchar_t str[110];//=fc; 
	return str;
}
int getrand(int t,int lim) {
	
	int bk=0;
	while (bk<=1000) {
		
		int x=rand()%lim+1;
		
		bk++;
		if (!vis[x] && t==1) {vis[x]=true;return x;}//return trans(x);
		if (vis[x] && t==2) {vis[x]=false;return x;}//return trans(x);
	}
	return -1;

}

void make_data(int test) {

    int N,M; 
	if (test<=4) N=100;
	else if (test<=7) N=1000;
	else N=10000;
	
	memset(vis,false,sizeof(vis));
	
	M=N*2;
	for (int i=1;i<=M;i++)
	{
	 	if (rand()%10<7) //add
		{
			int x=getrand(1,N);
			if (x!=-1)	cout<<1<<" "<<x<<" "<<rand()%M+1<<endl;
			/*wchar_t op[110]; 
			wcscpy ( op, getrand(1,N));
			wprintf(L"%s ", op);
		 	
		 	wcscpy ( op, trans(rand()%M+1));
			wprintf(L"%s\n", op);*/
		 }
		 else //delete
		 {
			int x=getrand(2,N);
			if (x!=-1)	cout<<2<<" "<<x<<endl;
			/*wchar_t op[110]; 
			wcscpy ( op, getrand(2,N));
			wprintf(L"%s\n", op);*/
		 }
		if (i%5==0) cout<<3<<endl;
	}
}
#undef int 
main() {
    srand(time(0));

    for (int t = 1; t <= cases; t++) {
        fprintf(stderr, "%d\n", t);

        sprintf(ak, problem "%d.in", t);
        freopen(ak, "w", stdout);

        make_data(t);
        fclose(stdout);
        
        sprintf(ak, sol ".exe < " problem "%d.in >" problem "%d.ans", t, t);
        system(ak);
    }
    return 0;
}
