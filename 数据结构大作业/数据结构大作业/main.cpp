#include "pch.h"
#include "Author.h"
#include "BPTrees.h"
#include "CommUtils.h"
#include "F1Solver.h"

int _tmain()
{
    CalcTime timer;
	//F1Solver f1solver(L"E:\\数据结构课设\\dblp.xml\\dblp.xml", article);
	XMLParser a;
	Info b=a.ParseSingle(L"C:\\Users\\Q4n\\Desktop\\ds\\sb.xml", 580);
	XMLMarshal c;
	_tprintf(L"%s\n",c.Marshal(b).GetBSTR());
	return 0;
}