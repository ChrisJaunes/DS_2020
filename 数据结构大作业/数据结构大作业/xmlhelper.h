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

	// 从文件的某个position解析单个对象, 这个不需要根节点, 但是需要提前拿到position
	Info ParseSingle(LPCWSTR filename,size_t position);

	// 解析一个dblp.xml文件的所有对象, 这个文档需要有一个根节点
	OPRESULT ParseFile(LPCWSTR filename, ISolver *pSolver);
protected:
	OPRESULT OpenFile(LPCWSTR);
	OPRESULT ParseAll(ISolver *);
};

// 序列化info对象为STR类型, 写入文件操作可能需要自己实现一个wrapper
// 这个不受xml文档定义限制, 不一定必须要一个根节点
/*
	XMLParser a;
	Info b=a.ParseSingle(L"C:\\Users\\Q4n\\Desktop\\ds\\sb.xml", 580);
	STR t1 = XMLMarshal::Marshal(b);
	wchar_t* tmp= t1.GetBSTR();
	_tprintf(L"%s\n",tmp);
	Info d = XMLMarshal::Unmarshal(t1);
*/
class XMLMarshal {
public:
	static STR Marshal(Info);
	static Info Unmarshal(STR, DWORD=article);
};
