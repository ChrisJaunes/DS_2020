#pragma once
#include "Solver.h"

class F1Solver : public ISolver {
public:
	// ���캯��, �����Ҫ�䶯 ����F3Solver�Ĺ��캯����ʵ��(copyһ��)
	F1Solver(const TCHAR* xmlfile);
	F1Solver(const TCHAR* xmlfile, DWORD parseInfo);

	// �ӿڵ�����, �������ӿ�һ��Ҫʵ��, ��Ҫ�Ƕ�ȫ�ֱ���ImportData�ĳ�ʼ��
	// ���жԶ������Info�ṹ��Ĳ���
	void InitMemory();
	void InsertObject(Info temp);

	// ������Լ���չ��
};