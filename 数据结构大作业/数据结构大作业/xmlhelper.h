#pragma once
#include "error.h"
#include <xmllite.h>
#include <shlwapi.h>
#pragma comment (lib, "xmllite.lib") 
#pragma comment(lib,"Shlwapi.lib")

#include "Solver.h"
#include "Info.h"

/*
打开文件
调用xmllite进行dtd解析
*/


// 可以选择的解析对象, 其中article和...数量最多
#ifndef ENUMPARSEINFO
#define ENUMPARSEINFO
#define article 1
#define book 2
#define incollection 4
#define inproceedings 8
#define mastersthesis 16
#define phdthesis 32
#define proceedings 64
#define www 128
#define alltypes (article|book|incollection|inproceedings|mastersthesis|phdthesis|proceedings|www)
#endif // !ENUMPARSEINFO



class XMLParser {
protected:
	CComPtr<IStream> pFileStream;
	CComPtr<IXmlReader> pReader;
	std::vector<STR>* parseInfo;
public:
	// 默认只分析 article
	XMLParser();
	// 多个flag通过|进行连接
	XMLParser(DWORD flag);
	~XMLParser();

	// 从文件的某个position解析单个对象
	// TODO:
	Info ParseSingle(size_t position);

	// 解析一个dblp.xml文件的所有对象
	OPRESULT ParseFile(LPCWSTR filename, ISolver *pSolver);
protected:
	OPRESULT OpenFile(LPCWSTR);
	OPRESULT ParseAll(ISolver *);
};
