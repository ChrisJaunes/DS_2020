#pragma once
#include "error.h"
#include <xmllite.h>
#include <shlwapi.h>
#pragma comment (lib, "xmllite.lib") 
#pragma comment(lib,"Shlwapi.lib")

#include "Solver.h"
#include "Info.h"

/*
���ļ�
����xmllite����dtd����
*/


// ����ѡ��Ľ�������, ����article��...�������
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
	// Ĭ��ֻ���� article
	XMLParser();
	// ���flagͨ��|��������
	XMLParser(DWORD flag);
	~XMLParser();

	// ���ļ���ĳ��position������������, �������Ҫ���ڵ�, ������Ҫ��ǰ�õ�position
	Info ParseSingle(LPCWSTR filename,size_t position);

	// ����һ��dblp.xml�ļ������ж���, ����ĵ���Ҫ��һ�����ڵ�
	OPRESULT ParseFile(LPCWSTR filename, ISolver *pSolver);
protected:
	OPRESULT OpenFile(LPCWSTR);
	OPRESULT ParseAll(ISolver *);
};

// ���л�info����ΪSTR����, д���ļ�����������Ҫ�Լ�ʵ��һ��wrapper
// �������xml�ĵ���������, ��һ������Ҫһ�����ڵ�
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
