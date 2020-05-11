#pragma once
#include "Info.h"

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
class InfoMarshal {
public:
	static wchar_t* Marshal(Info);
	static Info Unmarshal(MYSTR, DWORD flag = XMLPARSETYPE_alltypes);
};

