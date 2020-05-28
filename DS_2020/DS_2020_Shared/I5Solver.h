#pragma once
#include "pch.h"
#include "config.h"
#include "Info.h"
#include "Author.h"
#include "Solver.h"
#include "xmlhelper.h"
#include "CliquesCounting.h"
/*
ISolver�ӿ�, ʵ�ֶԾ�������Ľ������

InitMemory ����� ȫ�ֱ���ImportData���ڴ��ʼ������,
InsertObject�ǵ��õ���һ��Info�ṹ��ʱ��InitMemory��ʼ���Ľṹ����в���Ĳ���

ImportDataȫ�ֱ����Ķ�����Ե� CommUtils.h �е� struct _ImportData �н������
Ϊһ��ָ������, �������

����ʾ�����������F3Solver
*/
class I5Solver :public ISolver{
	CliquesCounting* pF5;
	XMLParser* pParser;
public:
	I5Solver() {
		pF5 = new CliquesCounting();
	}
	I5Solver(const TCHAR* xmlfile, DWORD parseInfo = XMLPARSETYPE_article);
	~I5Solver();
	void InitMemory();
	void InsertObject(Info&);

	void WriteToFile();
};
