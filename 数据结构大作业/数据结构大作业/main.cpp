#include "pch.h"
#include "Author.h"
#include "BPTrees.h"
#include "CommUtils.h"
#include "Solver.h"

int _tmain()
{
    CalcTime timer;
	F3Solver f3solver(L"D:\\Code\\ds_hw\\dblp.xml", inproceedings);
	f3solver.ExportToFile(L"D:\\Code\\ds_hw\\new\\new1\\DS_2020\\Database\\F3_inproceedings.txt");
	//std::map < STR, std::map<STR, ULONG64> > result=F3Solver::ImportFromFile(L"D:\\Code\\ds_hw\\new\\new1\\DS_2020\\Database\\F3_articles.txt");
	return 0;
}