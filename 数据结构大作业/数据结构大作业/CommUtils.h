#pragma once
#include "FrequencyRanking.h"
#include "BPTrees.h"

#define STR bstr_t

/* 这个struct存放各种数据结构相关的全局变量, 全部都放在这个位置
	比如说所有从xml分析完毕后建立的b+树呀, vector呀, 之类的东西, 全部都放到这个位置.

直接在所需要用到的文件下
	#include<CommUtils.h>
	ImportData.isDone = 1;
可以类似这样直接访问变量
*/

#ifndef IMPORTDATA
#define IMPORTDATA
struct _ImportData {
	// 标识是否完成了解析, 这个在GUI中使用功能前具体判断
	bool isDone;

	// F1 的两颗树
	BPTree<Info, STR> *titleTree;
	BPTree<Info, STR> *authorTree;

	// F3获取词频前十的
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


