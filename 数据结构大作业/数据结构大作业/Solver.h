#pragma once

/*
eg:
	#include "CommUtils.h"
	#include "Solver.h"
	int _tmain()
	{
		CalcTime timer;
		F3Solver f3solver(L"D:\\Code\\ds_hw\\dblp.xml");
		f3solver.ExportToFile(L"f3_out.txt");
		return 0;
	}
*/
class F3Solver {
public:
	F3Solver(const TCHAR* xmlfile);
	void ExportToFile(const TCHAR*);
};