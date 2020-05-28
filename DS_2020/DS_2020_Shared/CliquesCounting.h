#pragma once
#include "pch.h"
#include "config.h"
#include "error.h"
#include "Info.h"
#include "DblpBptMs.h"
#include "Author.h"

#define STR bstr_t
#define W ULONG64
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
		CliquesCount.clear();
		ms = nullptr;
		ins_cnt = 0;
	};
	CliquesCounting(DblpBptMs* Dp) {
		CliquesCount.clear();
		ms = Dp;
		ins_cnt = 0;
	};
	~CliquesCounting() {
		//delete ms;
	}
	void initial();
	void writefile_f5result();
	void readfile_f5result();	
	void writefile_f5result(int);
	int readfile_f5result(int);

	void set_ms(DblpBptMs* Dp);
	void InsertObject();
	OPRESULT Insert(Author*);
	bool Check(std::vector<STR>*, STR);
	OPRESULT Counting(std::vector<Author*>, W, W);
	//W GetSize();
	std::map<W, W> GetResult();

protected:
	DblpBptMs* ms;
	std::map<W, W> CliquesCount;
	//map<size,number>

#ifdef TEST_DEBUG
	int ins_cnt;
#endif
};
