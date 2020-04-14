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

	// ���ļ���ĳ��position������������
	// TODO:
	Info ParseSingle(size_t position);

	// ����һ��dblp.xml�ļ������ж���
	OPRESULT ParseFile(LPCWSTR filename, ISolver *pSolver);
protected:
	OPRESULT OpenFile(LPCWSTR);
	OPRESULT ParseAll(ISolver *);
};
