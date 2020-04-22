#pragma once
#include "pch.h"
#include "Author.h"

#define STR bstr_t
#define W int
/*
Need:����Author getAuthorByName(bstr_t);

F5.���ŷ���
�����ƴ�С����һ��
����ÿ������ֻ�����ƴ������ĺ����ߣ������ظ�ͳ��

MaxSize������
CliquesCount���������ŵ�����

GetResult()���ؽ��map

�÷���
��ÿ��Author*����StartCount��
GetResult()�ɻ�ý��map<����������>
*/
class CliquesCounting{
public:
	CliquesCounting() {
		MaxSize = 0;
		CliquesCount.clear();
	};
	OPRESULT StartCount(Author*);
	bool Check(std::vector<STR>*, STR);
	OPRESULT Counting(std::vector<Author*>, W, W);
	W GetSize();
	std::map<W, W> GetResult();

protected:
	W MaxSize;
	std::map<W, W> CliquesCount;
	//map<size,number>
};
