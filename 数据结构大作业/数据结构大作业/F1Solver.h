#pragma once
#include "Solver.h"
#include "BPTrees.h"
#include "Info.h"
#include "xmlhelper.h"

class F1Solver : public ISolver {
	BPTree<Info, STR>* pF1;
	XMLParser* pParser;
public:
	// ���캯��, �����Ҫ�䶯 ����F3Solver�Ĺ��캯����ʵ��(copyһ��)
	F1Solver(const TCHAR* xmlfile, DWORD parseInfo=article);
	~F1Solver();

	// �ӿڵ�����, �������ӿ�һ��Ҫʵ��, ��Ҫ�Ƕ�ȫ�ֱ���ImportData�ĳ�ʼ��
	// ���жԶ������Info�ṹ��Ĳ���
	void InitMemory();
	void InsertObject(Info temp);

	// ������Լ���չ��
};