#pragma once
#include <tchar.h>
#include <windows.h>
#include <vector>
#include <string>
#include <queue>
#include <ctime>
#include <iostream>
#include <winerror.h>
#include <comdef.h>
#include <algorithm>
#include <map>

#define STR_TITLE L"title"
#define STR_AUTHOR L"author"

#define input "test" //输入文件名
#define problem "data" //输出文件名
#define output "test"//标准输出文件名 
#define std "std" //待测程序名 

char ak[1100]; 
int main()
{
	int cnt = 0;
	for (int T = 1; T <= 10; T++) {
		
		sprintf(ak, std ".exe < " input "%d.in >" problem ".ans", T);
		double st = clock();
		system(ak);
		double ed = clock();
		
		sprintf(ak, "fc "problem".ans "output"%d.ans", T);
		if (system(ak)) {
			printf("Wrong Answer!!\n");
		}
		else {
			printf("Accepted\n");
			printf("Time:%.0lfms.\n", ed - st);
			cnt++;
		}
	}
	printf("正确率: %.2lf%%\n", cnt*1.0 / 10.0);
	system("pause");
}
