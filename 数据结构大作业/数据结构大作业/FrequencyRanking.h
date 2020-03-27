#pragma once
#include "pch.h"
#include "error.h"
#include "Info.h"
typedef std::pair<bstr_t, ULONG64> PAIR;

/*
F3: 热点分析功能。分析每一年发表的文章中，题目所包含的单词中，出现频率排名前10的关键词。

Example:
	Article a,b,c;
	a.Setyear(L"1988");
	a.Settitle(L"fuck you mother you");
	b.Setyear(L"1988");
	b.Settitle(L"fuck you");
	c.Setyear(L"1989");
	c.Settitle(L"fuck");
	FrequencyRanking ac(L"fuck");
	ac.Insert(&a);
	ac.Insert(&c);
	ac.Insert(&b);
	std::vector<bstr_t> ad=ac.Get(L"1988");
	for (int i = 0; i < ad.size(); i++) {
		wprintf(L"%s\n", ad[i].GetBSTR());
	}



	// The run time is : 468.563s! version2
	// The run time is : 1297.31s! version1
*/

class FrequencyRanking {
public:
	FrequencyRanking(std::vector<bstr_t> ignore) {
		ignores.clear();
		for (int i = 0; i < ignore.size(); i++) {
			ignores.push_back(ignore.at(i).copy());
		}
	}
	// 构造方法考虑了存在忽略某些单词的情况
	FrequencyRanking(bstr_t ignore) {
		ignores.clear();
		if (!ignore) { return; }
		ignores.push_back(ignore.copy());
	}
	FrequencyRanking() {
		ignores.clear();
	}
	// 插入一个对象, 对其进行分析
	OPRESULT Insert(Info);
	// 返回某一年单词top10的复制, 避免uaf, 这里没有计算个数了
	std::vector<PAIR> Get(bstr_t);

	std::vector<bstr_t> GetYears();

private:
	std::vector<PAIR> _SortTop10(bstr_t);
	bool CheckIfIgnore(bstr_t);

	// map<year,<word,count>>
	std::map<bstr_t, std::map<bstr_t,uint64_t>> all;
	// map<year,<word,count>[max_is_10]>
	std::map<bstr_t, std::map<bstr_t, uint64_t>> top10;
	// map<year,min_in_top10>
	std::map<bstr_t, uint64_t> minCountInTop10;
	std::vector<bstr_t> ignores;
};

/// 重构代码

class FrequencyRanking2Data {
private:
	bstr_t word;
	ULONG64 cb;
public:
	FrequencyRanking2Data();
	FrequencyRanking2Data(bstr_t,ULONG64);
	FrequencyRanking2Data(const FrequencyRanking2Data&);
	ULONG64& size();
	bstr_t& content();
};
bool CompFrequencyRanking2Data(FrequencyRanking2Data&, FrequencyRanking2Data&);

class FrequencyRanking2 {
public:
	FrequencyRanking2();
	FrequencyRanking2(std::vector<bstr_t> ignore);
	FrequencyRanking2(const FrequencyRanking2&);
	~FrequencyRanking2();

	OPRESULT Insert(Info);
	std::vector<FrequencyRanking2Data> Get(bstr_t year);
protected:
	// stl指针为浅复制, 假如有复制构造函数, 则为深复制
	// 对象为深复制,字符串对象bstr_t是浅复制, 但是有引用计数增加

	// insert后插入这里, 不进行排序, 只分词
	std::map<bstr_t,std::map<bstr_t,ULONG64> >* bakup;

	//忽略的单词
	std::vector<bstr_t> *ignores;

	bool CheckIfIgnore(bstr_t);
};

std::vector<STR> ignoresWords();
