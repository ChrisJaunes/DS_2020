#pragma once
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
	//virtual void InitMemory()=0;
	//virtual void InsertObject(Info)=0;
};