#pragma once
#include "pch.h"
#include "error.h"
#include "Info.h"

#include <xmllite.h>
#include <shlwapi.h>
#pragma comment (lib, "xmllite.lib") 
#pragma comment(lib,"Shlwapi.lib")

// 解决具体问题需要的文件头
#include <FrequencyRanking.h>


/*
打开文件

调用xmllite进行dtd解析

Example: 
	DWORD start_time = GetTickCount();
	XMLParser parser;
	parser.OpenFile(L"D:\\Code\\ds_hw\\dblp.xml");
	parser.ParseArticlesToVector();
	std::cout << "count: " << parser.members.size() << std::endl;
	DWORD end_time = GetTickCount();
	std::cout << "The run time is:" << (end_time - start_time)*1.00 / 1000 << "s!" << std::endl;//输出运行时间
*/

class XMLParser {
protected:
	CComPtr<IStream> pFileStream;
	CComPtr<IXmlReader> pReader;
public:
	OPRESULT OpenFile(LPCWSTR);
	OPRESULT ParseAll();

};


/* 这个struct存放各种数据结构相关的全局变量, 全部都放在这个位置
	比如说所有从xml分析完毕后建立的b+树呀, vector呀, 之类的东西, 全部都放到这个位置.

直接在所需要用到的文件下
	#include<xmlhelper.h>
	ImportData.isDone = 1;
可以类似这样直接访问变量

*/
#ifndef IMPORTDATA
#define IMPORTDATA
struct _ImportData {
	// 标识是否完成了解析, 这个在GUI中使用功能前具体判断
	bool isDone;

	// F3获取top10的
	FrequencyRanking* f3_pFrequencyRanking;

};
#endif
extern _ImportData ImportData;


DWORD WINAPI ImportDataWrapper(LPCWSTR filename);
wchar_t* charToWChar(const char* text);