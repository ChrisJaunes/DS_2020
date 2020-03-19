#pragma once
#include "pch.h"
#include "error.h"
#include "article.h"

#include <xmllite.h>
#include <shlwapi.h>
#pragma comment (lib, "xmllite.lib") 
#pragma comment(lib,"Shlwapi.lib")

/*
���ļ�

����xmllite����dtd����

Example: 
	DWORD start_time = GetTickCount();
	XMLParser parser;
	parser.OpenFile(L"D:\\Code\\ds_hw\\dblp.xml");
	parser.ParseArticlesToVector();
	std::cout << "count: " << parser.members.size() << std::endl;
	DWORD end_time = GetTickCount();
	std::cout << "The run time is:" << (end_time - start_time)*1.00 / 1000 << "s!" << std::endl;//�������ʱ��
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

/* ���struct��Ÿ������ݽṹ��ص�ȫ�ֱ���, ȫ�����������λ��
	����˵���д�xml������Ϻ�����b+��ѽ, vectorѽ, ֮��Ķ���, ȫ�����ŵ����λ��.

ֱ��������Ҫ�õ����ļ���
	#include<xmlhelper.h>
	_ImportData ImportData;

	ImportData.isDone = 1;
������������ֱ�ӷ��ʱ���

*/
#ifndef IMPORTDATA
#define IMPORTDATA
struct _ImportData {
	// ��ʶ�Ƿ�����˽���, �����GUI��ʹ�ù���ǰ�����ж�
	bool isDone;

	// ������ݽṹ... ����, ��Ҫ���Ǽ�ע��Ŷ

};
#endif
extern _ImportData ImportData;


DWORD WINAPI ImportDataWrapper(LPCWSTR filename);
wchar_t* charToWChar(const char* text);