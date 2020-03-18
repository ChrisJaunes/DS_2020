#pragma once
#include "pch.h"
#include "Author.h"

/*
F5.聚团分析
按名称大小遍历一遍
对于每个作者只找名称大于它的合作者（以免重复统计

需求！
GetCollaborator()的返回值是一个Author类型的vector
(按name的大小来sort就是搞个定义<那种)
(再加个getname()这种东西
*/
class CliquesCounting {
public:
	CliquesCounting() {
		MaxSize = 0;
		CliquesCount.clear();
	};
	OPRESULT StartCount(Author);
	//以该作者为起点(即团中名字最小的)
	OPRESULT Counting(std::vector<Author>, uint64_t, std::vector<Author>, uint64_t);
	uint64_t GetSize();
	uint64_t GetResult(uint64_t);

private:
	uint64_t MaxSize;
	std::map<uint64_t, uint64_t> CliquesCount;
	//map<size,number>
};
