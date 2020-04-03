#pragma once
#include "pch.h"
#include "Author.h"

#define STR bstr_t
/*
F5.聚团分析
按名称大小遍历一遍
对于每个作者只找名称大于它的合作者（以免重复统计

MaxSize最大阶数
CliquesCount各阶数的团的数量

GetResult()返回结果map

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
