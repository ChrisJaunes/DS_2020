#pragma once
#include "error.h"
#include <xmllite.h>
#include <shlwapi.h>
#pragma comment (lib, "xmllite.lib") 
#pragma comment(lib,"Shlwapi.lib")

#include "Solver.h"

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
	OPRESULT OpenFile(LPCWSTR);
	OPRESULT ParseAll(ISolver *);

};


DWORD WINAPI ImportDataWrapper(LPCWSTR filename,ISolver *);
DWORD WINAPI ImportDataWrapperEx(LPCWSTR filename, ISolver* psolver, DWORD parseInfo);
