#pragma once
#include "pch.h"
#include "Info.h"
#include "config.h"
#include "Solver.h"
#include "xmlhelper.h"
#include "DblpBptMs.h"
#include "CliquesCounting.h"

class F5Solver// : public ISolver 
{
	// F5����Ľ����
	DblpBptMs *ms;
	CliquesCounting* pF5;
public:
	// ���캯��, �����Ҫ�䶯 ����F3Solver�Ĺ��캯����ʵ��(copyһ��)
	F5Solver(const wchar_t* info_bpt_file, const wchar_t* author_bpt_file, FILE_Status exist);
	~F5Solver();

	// �ӿڵ�����, �������ӿ�һ��Ҫʵ��, ��Ҫ�Ƕ�Ҫ����Infoλ�õĳ�ʼ��
	// ���жԶ������Info�ṹ��Ĳ���
	//void InsertObject();

	std::map<ULONG64, ULONG64> GetResult();

	// ������Լ���չ��
	void ExportToFile();
	std::map<W,W> ImportFromFile();
	//void ExportToFile(const TCHAR*);
	//static std::map < MYSTR, std::map<MYSTR, ULONG64> > ImportFromFile(const TCHAR*);

};
