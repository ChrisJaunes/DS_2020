#pragma once
#include "Info.h"
#include "config.h"
#include "Solver.h"
#include "xmlhelper.h"
#include "f4.h"
#include "DblpBptMs.h"

class F4Solver : public ISolver {
	XMLParser* pParser;
	// F4����Ľ����
	f4func* pF4;
public:
	// ���캯��, �����Ҫ�䶯 ����
	//Solver�Ĺ��캯����ʵ��(copyһ��)
	F4Solver() {
		pF4 = new f4func;
	}
	F4Solver(const TCHAR* xmlfile, DWORD parseInfo = XMLPARSETYPE_article);
	~F4Solver();

	// �ӿڵ�����, �������ӿ�һ��Ҫʵ��, ��Ҫ�Ƕ�Ҫ����Infoλ�õĳ�ʼ��
	// ���жԶ������Info�ṹ��Ĳ���
	void InitMemory();
	void InsertObject(Info&temp);
	bool SearchTitles(std::string&keywords, std::vector<std::string>&titles);
	std::pair<OPRESULT, std::vector<Info> > SearchInfo(std::string &title);
	void ImportFromFile();
	// ������Լ���չ��
	//void ExportToFile(const TCHAR*);
	//static std::map < MYSTR, std::map<MYSTR, ULONG64> > ImportFromFile(const TCHAR*);
};
