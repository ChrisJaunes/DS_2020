#pragma once
#include "pch.h"
#include "error.h"
#include <xmllite.h>
#include <shlwapi.h>
#pragma comment (lib, "xmllite.lib") 
#pragma comment(lib,"Shlwapi.lib")

#include "Info.h"
#include "Solver.h"
/*
打开文件
调用xmllite进行dtd解析
*/

class XMLParser {
protected:
	CComPtr<IStream> pFileStream;
	CComPtr<IXmlReader> pReader;
	std::vector<MYSTR>* parseInfo;
public:
	// 默认只分析 article
	XMLParser();
	// 多个flag通过|进行连接
	XMLParser(const DWORD flag);
	~XMLParser();

	// 从文件的某个position解析单个对象, 这个不需要根节点, 但是需要提前拿到position
	//Info ParseSingle(LPCWSTR filename,size_t position);

	// 解析一个dblp.xml文件的所有对象, 这个文档需要有一个根节点
	OPRESULT ParseFile(LPCWSTR filename, ISolver *pSolver);
protected:
	OPRESULT OpenFile(LPCWSTR);
	OPRESULT ParseAll(ISolver*);
};
