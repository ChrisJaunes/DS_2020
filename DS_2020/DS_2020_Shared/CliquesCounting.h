#pragma once
#include "pch.h"
#include "Author.h"

#define STR bstr_t
#define W int
/*
Need:函数Author getAuthorByName(bstr_t);

F5.聚团分析
按名称大小遍历一遍
对于每个作者只找名称大于它的合作者（以免重复统计

MaxSize最大阶数
CliquesCount各阶数的团的数量

GetResult()返回结果map

用法：
把每个Author*丢进StartCount里
GetResult()可获得结果map<阶数，个数>
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
