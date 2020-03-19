#pragma once
#include "pch.h"
#include "error.h"
#include "article.h"
typedef std::pair<bstr_t, uint64_t> PAIR;

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
	OPRESULT Insert(Article *);
	// 返回某一年单词top10的复制, 避免uaf, 这里没有计算个数了
	std::vector<bstr_t> Get(bstr_t);
private:
	std::vector<PAIR>* _SortTop10(bstr_t);
	bool CheckIfIgnore(bstr_t);

	// map<year,<word,count>>
	std::map<bstr_t, std::map<bstr_t,uint64_t>> all;
	// map<year,<word,count>[max_is_10]>
	std::map<bstr_t, std::map<bstr_t, uint64_t>> top10;
	// map<year,min_in_top10>
	std::map<bstr_t, uint64_t> minCountInTop10;
	std::vector<bstr_t> ignores;
};