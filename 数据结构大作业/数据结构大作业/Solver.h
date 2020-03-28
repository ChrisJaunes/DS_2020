#pragma once
#include "Info.h"

/*
ISolver接口, 实现对具体问题的解决方案

InitMemory 负责对 全局变量ImportData的内存初始化操作,
InsertObject是当拿到了一个Info结构体时对InitMemory初始化的结构体进行插入的操作

	ImportData全局变量的定义可以到 CommUtils.h 中的 struct _ImportData 中进行添加
	为一个指针类型, 方便访问

具体示例参照下面的F3Solver
*/
class ISolver {
public:
	virtual void InitMemory()=0;
	virtual void InsertObject(Info)=0;
};

// 可以选择的解析对象, 也就是参数parseInfo的值, 其中article和...数量最多
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
	// 构造函数, 这个不要变动 按照F3Solver的构造函数来实现(copy一下)
	F3Solver(const TCHAR* xmlfile);
	F3Solver(const TCHAR* xmlfile, DWORD parseInfo);

	// 接口的声明, 这两个接口一定要实现, 主要是对全局变量ImportData的初始化
	// 还有对对象插入Info结构体的操作
	void InitMemory();
	void InsertObject(Info temp);

	// 这个是自己拓展的
	void ExportToFile(const TCHAR*);
};
