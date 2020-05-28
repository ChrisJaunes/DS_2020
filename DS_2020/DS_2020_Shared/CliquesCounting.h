#pragma once
#include "pch.h"
#include "config.h"
#include "error.h"
#include "Info.h"
#include "DblpBptMs.h"
#include "Author.h"
#include <iomanip>

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
struct BigNumber {
	W a[110], len;
	const W mod = 100;
	BigNumber() {
		memset(a, 0, sizeof(a)); len = 1;
	}
	friend void fz(BigNumber &x, int value)
	{
		x.len = 1;
		x.a[1] = value;
		x.a[2] = x.a[1] / x.mod;
		x.a[1] %= x.mod;
		while (x.a[x.len + 1] != 0)
		{
			x.len++;
			x.a[x.len + 1] += x.a[x.len] / x.mod;
			x.a[x.len] %= x.mod;
		}
	}
	BigNumber operator +(const W &b)  const
	{
		BigNumber ret;
		ret.len = len;
		for (int i = 1; i <= ret.len; i++)
		{
			if (i == 1) ret.a[i] += a[i] + b;
			else ret.a[i] += a[i];
			ret.a[i + 1] += ret.a[i] / mod;
			ret.a[i] %= mod;
		}
		while (ret.a[ret.len + 1] != 0)
		{
			ret.len++;
			ret.a[ret.len + 1] += ret.a[ret.len] / mod;
			ret.a[ret.len] %= mod;
		}
		return ret;
	}
	BigNumber operator +(const BigNumber &b)  const
	{
		BigNumber ret;
		ret.len = (len>b.len) ? len : b.len;
		for (int i = 1; i <= ret.len; i++)
		{
			ret.a[i] += a[i] + b.a[i];
			ret.a[i + 1] += ret.a[i] / mod;
			ret.a[i] %= mod;
		}
		while (ret.a[ret.len + 1] != 0)
		{
			ret.len++;
			ret.a[ret.len + 1] += ret.a[ret.len] / mod;
			ret.a[ret.len] %= mod;
		}
		return ret;
	}
	BigNumber& operator = (const BigNumber& value)
	{
		len = value.len;
		for (int i = 1; i <= len; i++) a[i] = value.a[i];
		return *this;
	}
	bool operator == (const W &b) const
	{
		W t = b;
		for (int i = 1; i <= len; i++)
		{
			if (a[i] != t % mod) return false;
			t /= mod;
		}
		if (t != 0) return false;
		return true;
	}
	friend std::ostream & operator << (std::ostream &out, BigNumber &t)
	{
		out << t.a[t.len];
		for (int i = t.len - 1; i >= 1; i--) out << std::setw(2) << std::setfill('0') << t.a[i];
		return out;
	}
};

class CliquesCounting {
public:
	CliquesCounting();
	CliquesCounting(DblpBptMs*);
	~CliquesCounting() {
		//delete ms;
	}
	void pre_C(int);

	void initial();
	void writefile_f5result();
	void readfile_f5result();
	void writefile_f5result(int);
	int readfile_f5result(int);

	//������ֱ�Ӵ���
	//void CountDir();//ͨ��info_bpt
	void insert(Info&);//ͨ��xmlhelper

	void set_ms(DblpBptMs* Dp);
	void InsertObject();
	OPRESULT Insert(Author*);
	bool Check(std::vector<STR>*, STR);
	OPRESULT Counting(std::vector<Author*>, W, W);
	//W GetSize();
	std::map<W, W> GetResult();
	std::map<W, MYSTR> GetResult_2();
	int getcnt();

protected:
	DblpBptMs* ms;
	std::map<W, W> CliquesCount;
	BigNumber CC_test[300];
	//map<size,number>
	BigNumber C[300][300];
#ifdef TEST_DEBUG
	int ins_cnt, max_size;
#endif
};
