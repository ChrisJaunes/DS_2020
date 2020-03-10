#pragma once
#define DEBUG 0

/*
很多检测error code的逻辑在代码中没有实现

*/

// 操作返回数, 小于0为error
typedef int OPRESULT;

// 检查error code
#define CHK_OR(stmt)      if(DEBUG){  \
		do { OPRESULT hr=(stmt); \
		if (FAILED(hr)) { printf("err: 0x%X\n",stmt); exit(-1);} \
		} while(0);} \
		else { puts("error(no DEBUG option)");}

// 定义error code
#define E_InvalidParameter 0x80000000

