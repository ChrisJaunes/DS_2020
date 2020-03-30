#pragma once
#include "FrequencyRanking.h"
#include "BPTrees.h"

#define STR bstr_t

/* ���struct��Ÿ������ݽṹ��ص�ȫ�ֱ���, ȫ�����������λ��
	����˵���д�xml������Ϻ�����b+��ѽ, vectorѽ, ֮��Ķ���, ȫ�����ŵ����λ��.

ֱ��������Ҫ�õ����ļ���
	#include<CommUtils.h>
	ImportData.isDone = 1;
������������ֱ�ӷ��ʱ���
*/

#ifndef IMPORTDATA
#define IMPORTDATA
struct _ImportData {
	// ��ʶ�Ƿ�����˽���, �����GUI��ʹ�ù���ǰ�����ж�
	bool isDone;

	// F1 ��������
	BPTree<Info, STR> *titleTree;
	BPTree<Info, STR> *authorTree;

	// F3��ȡ��Ƶǰʮ��
	FrequencyRanking* f3_pFrequencyRanking;

};
#endif
extern _ImportData ImportData;

wchar_t* charToWChar(const char* text);


class CalcTime {
	ULONG64 start_time;
public:
	CalcTime();
	~CalcTime();
};


