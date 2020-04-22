#pragma once
#include "config.h"
#include "Solver.h"
#include "FrequencyRanking.h"
#include "xmlhelper.h"
#include "Info.h"

/*
eg:
	#include "CommUtils.h"
	#include "Solver.h"
	int _tmain()
	{
		CalcTime timer;
		F3Solver f3solver(L"D:\\Code\\ds_hw\\dblp.xml");
		// F3Solver f3solver(L"D:\\Code\\ds_hw\\dblp.xml", article | book );
		f3solver.ExportToFile(L"f3_out.txt");
		return 0;
	}
*/
class F3Solver : public ISolver {
	XMLParser* pParser;
	// F3����Ľ����
	FrequencyRanking* pF3;
public:
	// ���캯��, �����Ҫ�䶯 ����F3Solver�Ĺ��캯����ʵ��(copyһ��)
	F3Solver(const TCHAR* xmlfile, DWORD parseInfo=article);
	~F3Solver();

	// �ӿڵ�����, �������ӿ�һ��Ҫʵ��, ��Ҫ�Ƕ�Ҫ����Infoλ�õĳ�ʼ��
	// ���жԶ������Info�ṹ��Ĳ���
	void InitMemory();
	void InsertObject(Info temp);

	// ������Լ���չ��
	void ExportToFile(const TCHAR*);
	static std::map < MYSTR, std::map<MYSTR, ULONG64> > ImportFromFile(const TCHAR*);
};