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
	// F3问题的解决类
	FrequencyRanking* pF3;
public:
	// 构造函数, 这个不要变动 按照F3Solver的构造函数来实现(copy一下)
	F3Solver(const TCHAR* xmlfile, DWORD parseInfo=article);
	~F3Solver();

	// 接口的声明, 这两个接口一定要实现, 主要是对要插入Info位置的初始化
	// 还有对对象插入Info结构体的操作
	void InitMemory();
	void InsertObject(Info temp);

	// 这个是自己拓展的
	void ExportToFile(const TCHAR*);
	static std::map < MYSTR, std::map<MYSTR, ULONG64> > ImportFromFile(const TCHAR*);
};