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
#define alltypes (article|book|incollection|inproceedings|mastersthesis|phdthesis|proceedings|www)
#endif // !ENUMPARSEINFO


