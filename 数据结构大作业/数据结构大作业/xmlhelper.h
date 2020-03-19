#pragma once
#include "pch.h"
#include "error.h"
#include "article.h"

#include <xmllite.h>
#include <shlwapi.h>
#pragma comment (lib, "xmllite.lib") 
#pragma comment(lib,"Shlwapi.lib")

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
	IXmlReader* pReader;
	XmlNodeType nodeType;
public:
	std::vector<Article*> members;
	OPRESULT OpenFile(LPCWSTR);
	OPRESULT ParseArticlesToVector();
	OPRESULT ParseAll();

};

/* 这个struct存放各种数据结构相关的全局变量, 全部都放在这个位置
	比如说所有从xml分析完毕后建立的b+树呀, vector呀, 之类的东西, 全部都放到这个位置.

直接在所需要用到的文件下
	#include<xmlhelper.h>
	_ImportData ImportData;

	ImportData.isDone = 1;
可以类似这样直接访问变量

*/
#ifndef IMPORTDATA
#define IMPORTDATA
struct _ImportData {
	// 标识是否完成了解析, 这个在GUI中使用功能前具体判断
	bool isDone;

	// 添加数据结构... 对了, 不要忘记加注释哦

};
#endif
extern _ImportData ImportData;


DWORD WINAPI ImportDataWrapper(LPCWSTR filename);
wchar_t* charToWChar(const char* text);