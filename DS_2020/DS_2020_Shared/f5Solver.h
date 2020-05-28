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
	// F5问题的解决类
	DblpBptMs *ms;
	CliquesCounting* pF5;
public:
	// 构造函数, 这个不要变动 按照F3Solver的构造函数来实现(copy一下)
	F5Solver(const wchar_t* info_bpt_file, const wchar_t* author_bpt_file, FILE_Status exist);
	~F5Solver();

	// 接口的声明, 这两个接口一定要实现, 主要是对要插入Info位置的初始化
	// 还有对对象插入Info结构体的操作
	//void InsertObject();

	std::map<ULONG64, ULONG64> GetResult();

	// 这个是自己拓展的
	void ExportToFile();
	std::map<W,W> ImportFromFile();
	//void ExportToFile(const TCHAR*);
	//static std::map < MYSTR, std::map<MYSTR, ULONG64> > ImportFromFile(const TCHAR*);

};
