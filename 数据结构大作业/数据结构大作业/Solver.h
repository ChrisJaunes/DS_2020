#pragma once
#include "Info.h"

/*
ISolver�ӿ�, ʵ�ֶԾ�������Ľ������

InitMemory ����� ȫ�ֱ���ImportData���ڴ��ʼ������,
InsertObject�ǵ��õ���һ��Info�ṹ��ʱ��InitMemory��ʼ���Ľṹ����в���Ĳ���

	ImportDataȫ�ֱ����Ķ�����Ե� CommUtils.h �е� struct _ImportData �н������
	Ϊһ��ָ������, �������

����ʾ�����������F3Solver
*/
class ISolver {
public:
	virtual void InitMemory()=0;
	virtual void InsertObject(Info)=0;
};

// ����ѡ��Ľ�������, Ҳ���ǲ���parseInfo��ֵ, ����article��...�������
#ifndef ENUMPARSEINFO
#define ENUMPARSEINFO
#define article 1
#define book 2
#define incollection 4
#define inproceedings 8
#define mastersthesis 16
#define phdthesis 32
#define proceedings 64
#define www 128
#endif // !ENUMPARSEINFO

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
class F3Solver : public ISolver{
public:
	// ���캯��, �����Ҫ�䶯 ����F3Solver�Ĺ��캯����ʵ��(copyһ��)
	F3Solver(const TCHAR* xmlfile);
	F3Solver(const TCHAR* xmlfile, DWORD parseInfo);

	// �ӿڵ�����, �������ӿ�һ��Ҫʵ��, ��Ҫ�Ƕ�ȫ�ֱ���ImportData�ĳ�ʼ��
	// ���жԶ������Info�ṹ��Ĳ���
	void InitMemory();
	void InsertObject(Info temp);

	// ������Լ���չ��
	void ExportToFile(const TCHAR*);
};
