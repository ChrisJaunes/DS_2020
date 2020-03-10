#pragma once

#define GDEBUG 0

/*
�ܶ���error code���߼��ڴ�����û��ʵ��
*/

// ����������, С��0Ϊerror
typedef int OPRESULT;

// ���error code
#define CHK_OR(stmt)      if(GDEBUG){  \
		do { OPRESULT hr=(stmt); \
		if (FAILED(hr)) { printf("err: 0x%X\n",stmt); exit(-1);} \
		} while(0);} \
		else { puts("error(no DEBUG option)");exit(-1);}

// ����error code
#define E_InvalidParameter 0x80000000

