#pragma once
#include "Solver.h"
#include "BPTrees.h"
#include "Info.h"
#include "xmlhelper.h"

class F1Solver : public ISolver {
	BPTree<Info, STR>* pF1;
	XMLParser* pParser;
public:
	// 构造函数, 这个不要变动 按照F3Solver的构造函数来实现(copy一下)
	F1Solver(const TCHAR* xmlfile, DWORD parseInfo=article);
	~F1Solver();

	// 接口的声明, 这两个接口一定要实现, 主要是对全局变量ImportData的初始化
	// 还有对对象插入Info结构体的操作
	void InitMemory();
	void InsertObject(Info temp);

	// 这个是自己拓展的
};