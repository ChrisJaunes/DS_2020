#include "pch.h"
#include "Author.h"
#include "BPTrees.h"
#include "CommUtils.h"
#include "Solver.h"

int _tmain()
{
    CalcTime timer;
	F3Solver f3solver(L"D:\\Code\\ds_hw\\dblp.xml");
	f3solver.ExportToFile(L"f3_out.txt");
	return 0;
}