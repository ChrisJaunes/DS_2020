#pragma once
#include "pch.h"
#include "error.h"
#include <xmllite.h>
#include <shlwapi.h>
#pragma comment (lib, "xmllite.lib") 
#pragma comment(lib,"Shlwapi.lib")

#include "F0Solver.h"
#include "Info.h"

/*
���ļ�
����xmllite����dtd����
*/

class XMLParser {
protected:
	CComPtr<IStream> pFileStream;
	CComPtr<IXmlReader> pReader;
	std::vector<MYSTR>* parseInfo;
public:
	// Ĭ��ֻ���� article
	XMLParser();
	// ���flagͨ��|��������
	XMLParser(DWORD flag);
	~XMLParser();

	// ���ļ���ĳ��position������������, �������Ҫ���ڵ�, ������Ҫ��ǰ�õ�position
	//Info ParseSingle(LPCWSTR filename,size_t position);

	// ����һ��dblp.xml�ļ������ж���, ����ĵ���Ҫ��һ�����ڵ�
	OPRESULT ParseFile(LPCWSTR filename, F0Solver *pSolver);
protected:
	OPRESULT OpenFile(LPCWSTR);
	OPRESULT ParseAll(F0Solver *);
};
