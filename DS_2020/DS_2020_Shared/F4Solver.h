#pragma once
#include "Info.h"
#include "config.h"
#include "Solver.h"
#include "xmlhelper.h"
#include "f4.h"

class F4Solver : public ISolver {
	XMLParser* pParser;
	// F3����Ľ����
	f4func* pF4;
public:
	// ���캯��, �����Ҫ�䶯 ����F3Solver�Ĺ��캯����ʵ��(copyһ��)
	F4Solver(const TCHAR* xmlfile, DWORD parseInfo = XMLPARSETYPE_article);
	~F4Solver();

	// �ӿڵ�����, �������ӿ�һ��Ҫʵ��, ��Ҫ�Ƕ�Ҫ����Infoλ�õĳ�ʼ��
	// ���жԶ������Info�ṹ��Ĳ���
	void InitMemory();
	bool SearchTitles(std::string&keywords, std::vector<std::string>&titles);
	std::vector<Info> SearchInfo(std::vector<std::string>&titles);

	// ������Լ���չ��
	//void ExportToFile(const TCHAR*);
	//static std::map < MYSTR, std::map<MYSTR, ULONG64> > ImportFromFile(const TCHAR*);
};
