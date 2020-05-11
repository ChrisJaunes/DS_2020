#pragma once
#include "Info.h"
#include "config.h"
#include "Solver.h"
#include "xmlhelper.h"
#include "f4.h"

class F4Solver : public ISolver {
	XMLParser* pParser;
	// F3问题的解决类
	f4func* pF4;
public:
	// 构造函数, 这个不要变动 按照F3Solver的构造函数来实现(copy一下)
	F4Solver(const TCHAR* xmlfile, DWORD parseInfo = XMLPARSETYPE_article);
	~F4Solver();

	// 接口的声明, 这两个接口一定要实现, 主要是对要插入Info位置的初始化
	// 还有对对象插入Info结构体的操作
	void InitMemory();
	bool SearchTitles(std::string&keywords, std::vector<std::string>&titles);
	std::vector<Info> SearchInfo(std::vector<std::string>&titles);

	// 这个是自己拓展的
	//void ExportToFile(const TCHAR*);
	//static std::map < MYSTR, std::map<MYSTR, ULONG64> > ImportFromFile(const TCHAR*);
};
