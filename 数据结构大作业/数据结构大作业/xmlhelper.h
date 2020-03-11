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