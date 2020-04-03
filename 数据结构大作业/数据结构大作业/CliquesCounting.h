#pragma once
#include "pch.h"
#include "Author.h"

#define STR bstr_t
/*
F5.���ŷ���
�����ƴ�С����һ��
����ÿ������ֻ�����ƴ������ĺ����ߣ������ظ�ͳ��

MaxSize������
CliquesCount���������ŵ�����

GetResult()���ؽ��map

*/
class CliquesCounting{
public:
	CliquesCounting() {
		MaxSize = 0;
		CliquesCount.clear();
	};
	OPRESULT StartCount(Author*);
	bool Check(std::vector<STR>*, STR);
	OPRESULT Counting(std::vector<Author*>, uint64_t, uint64_t);
	uint64_t GetSize();
	std::map<uint64_t, uint64_t> GetResult();

protected:
	uint64_t MaxSize;
	std::map<uint64_t, uint64_t> CliquesCount;
	//map<size,number>
};
