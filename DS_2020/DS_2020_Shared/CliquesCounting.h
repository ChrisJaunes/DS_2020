#pragma once
#include "pch.h"
#include "config.h"
#include "error.h"
#include "Info.h"
#include "DblpBptMs.h"
#include "Author.h"

#define STR bstr_t
#define W ULONG64
/*
Need:����Author getAuthorByName(bstr_t);

F5.���ŷ���
�����ƴ�С����һ��
����ÿ������ֻ�����ƴ������ĺ����ߣ������ظ�ͳ��

MaxSize������
CliquesCount���������ŵ�����

GetResult()���ؽ��map

�÷���
��ÿ��Author*����StartCount��
GetResult()�ɻ�ý��map<����������>
*/
class CliquesCounting{
public:
	CliquesCounting() {
		CliquesCount.clear();
		ms = nullptr;
		ins_cnt = 0;
	};
	CliquesCounting(DblpBptMs* Dp) {
		CliquesCount.clear();
		ms = Dp;
		ins_cnt = 0;
	};
	~CliquesCounting() {
		//delete ms;
	}
	void initial();
	void writefile_f5result();
	void readfile_f5result();	
	void writefile_f5result(int);
	int readfile_f5result(int);

	void set_ms(DblpBptMs* Dp);
	void InsertObject();
	OPRESULT Insert(Author*);
	bool Check(std::vector<STR>*, STR);
	OPRESULT Counting(std::vector<Author*>, W, W);
	//W GetSize();
	std::map<W, W> GetResult();

protected:
	DblpBptMs* ms;
	std::map<W, W> CliquesCount;
	//map<size,number>

#ifdef TEST_DEBUG
	int ins_cnt;
#endif
};
