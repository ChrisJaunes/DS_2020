#pragma once
#include "Info.h"

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
class InfoMarshal {
public:
	static wchar_t* Marshal(Info);
	static Info Unmarshal(MYSTR, DWORD flag = XMLPARSETYPE_alltypes);
};

